#include "../include/video.h"
#include "../include/stdio.h"

#include "../include/stdio.h"

static colour STD_DISPLAY_background_colour    =    
DEFAULT_STD_DISPLAY_BACKGROUND_COLOUR;
static colour STD_DISPLAY_text_colour          =    
DEFAULT_STD_DISPLAY_TEXT_COLOUR;
static colour REG_DISPLAY_background_colour    =    
DEFAULT_REG_DISPLAY_BACKGROUND_COLOUR;
static colour REG_DISPLAY_text_colour          =   
 DEFAULT_REG_DISPLAY_TEXT_COLOUR;

static VBElem video_buffer[VIDEO_BUFFER_SIZE]={{'\0',-1}};
static int vbindex=0;

static int STD_DISPLAY_offset;
static int REG_DISPLAY_offset;

void __init_graphics(){
    STD_DISPLAY_offset=__getOffsetOfRow(STD_DISPLAY_MIN_ROW);
    vbindex=0;
    REG_DISPLAY_offset=__getOffsetOfRow(REG_DISPLAY_MIN_ROW);
    __paint_area(STD_DISPLAY,STD_DISPLAY_background_colour,
        STD_DISPLAY_text_colour);
    __paint_area(REG_DISPLAY,REG_DISPLAY_background_colour,
        REG_DISPLAY_text_colour);
    __set_cursor_position(0);

}

size_t __print(int disp, const void * buffer, size_t count){
    int ans;
    switch(disp){
        case STD_DISPLAY:
            ans=__bounded_print(STD_DISPLAY_MIN_ROW, STD_DISPLAY_MAX_ROW,
             true, &STD_DISPLAY_offset, buffer, count);
            __set_cursor_position(STD_DISPLAY_offset);
            return ans;
        case REG_DISPLAY:
            return __bounded_print(REG_DISPLAY_MIN_ROW, REG_DISPLAY_MAX_ROW, false, &REG_DISPLAY_offset, buffer, count);
        default:
            return INVALID_DISPLAY;
    }
}

size_t __bounded_print(int minRow, int maxRow, bool usesVB, int * offset, const void* buffer, size_t count){
    char c;
    int row;
    size_t written;
    
    for(written=0; written < count; written++){
        c=((char *)buffer)[written];

        if(usesVB==true){
            __setVBElem(vbindex,c,*offset);
        }

        if (__getRowOf(*offset) > maxRow) {
            __shift_up(minRow,maxRow,offset,1);
        }

        if(usesVB==true){
            __bounded_print_char(minRow,offset,c);
            vbindex++;
            vbindex%=VIDEO_BUFFER_SIZE;
        }else{
            __bounded_print_char_noVB(minRow,offset,c);
        }
    }
    return written;
}

void __bounded_print_char(int minRow, int * offset, char c){
    char *video = (char*)VIDEO_ADDRESS;
    int tab;

    switch(c){
        case '\b':
            if(__getRowOf((*offset)-1) >= minRow){
                switch(__getVBElem(vbindex-1)->c){
                    case '\t':
                    case '\n':
                        __resetOffset(offset,__getVBElem(vbindex-1)->startOffset);
                        break;
                    default:
                        video[(--(*offset))*2]='\0';
                }
                __clearVBElem(vbindex--);
            }
            __clearVBElem(vbindex--);
            break;
        case '\n':
            __setOffset(offset,__getRowOf(*offset)+1,0);
            break;
        case '\t':
            for(tab=0; tab<TAB_LENGTH && (__getColOf(*offset)+tab)<=MAX_COL; tab++){
                video[((*offset)++)*2]=TAB_CHAR;
            }
            break;
        default:
            if(__getColOf((*offset)+1) % MAX_COL == 0){
                (*offset)=__getOffsetOfRow(__getRowOf((*offset)+1));
            }
            video[((*offset)++)*2]=c;
            break;
    }
}


void __bounded_print_char_noVB(int minRow, int * offset, char c){
    char *video = (char*)VIDEO_ADDRESS;
    int tab;
    
    switch(c){
        case '\n':
            __setOffset(offset,__getRowOf(*offset)+1,0);
            return;
        case '\b':
            return;
        case '\t':
            for(tab=0; tab<TAB_LENGTH && 
                (__getRowOf((*offset)+TAB_LENGTH)==__getRowOf(*offset)); 
                tab++){
                video[((*offset)++)*2]='\0';
            }
            return;
        default:
            if(__getColOf((*offset)+1) % MAX_COL == 0){
                (*offset)=__getOffsetOfRow(__getRowOf((*offset)+1));
            }
            video[((*offset)++)*2]=c;
            return;
    }
}


int __paint_area(int disp, colour backgroundColour, colour textColour){
    switch(disp){
        case STD_DISPLAY:
            __bounded_paint_area(STD_DISPLAY_MIN_ROW, 
                STD_DISPLAY_MAX_ROW, MIN_COL, MAX_COL, backgroundColour, 
                textColour);
            break;
        case REG_DISPLAY:
            __bounded_paint_area(REG_DISPLAY_MIN_ROW, 
                REG_DISPLAY_MAX_ROW, MIN_COL, MAX_COL, 
                backgroundColour, textColour);
            break;
        default:
            return INVALID_DISPLAY;
    }
}

void __bounded_paint_area(int minRow, int maxRow, int minCol, int maxCol,
 colour backgroundColour, colour textColour){
    char *video = (char*)VIDEO_ADDRESS;
    int i,j;
    for(i=minRow;i<=maxRow;i++){
        for(j=minCol;j<=maxCol;j++){
            video[2*__getOffsetOf(i,j)+1]=AS_COLOUR_BYTE(backgroundColour,
                textColour);
        }
    }
}

void __shift_up(int minRow, int maxRow, int *offset, int rows){
    char *video = (char*)VIDEO_ADDRESS;
    char c;
    int row, i;
    int killOffset,cloneOffset;

    for(;rows>0;rows--){
        for(row=minRow; row<maxRow; row++){
            killOffset=__getOffsetOfRow(row);
            cloneOffset=__getOffsetOfRow(row+1);
            for(i=0; i<2*WIDTH; i++){
                video[2*killOffset+i]=video[2*cloneOffset+i];
            }
        }
        killOffset=__getOffsetOfRow(maxRow);
        for(i=0; i<WIDTH; i++){
            video[2*(killOffset+i)]='\0';
        }
        *offset=killOffset;
        maxRow--;
    }
}

//Code taken from http://wiki.osdev.org/Text_Mode_Cursor
int __set_cursor_position(int offset){
    int row=__getRowOf(offset);
    int col=__getColOf(offset);
    if(row<0 || row>HEIGHT || col<0 || col>MAX_COL){
        return INVALID_CURSOR;
    }

    unsigned short position=(row*WIDTH) + col;

    // cursor LOW port to vga INDEX register
    outb(BASE_PORT, 0x0F);
    outb(BASE_PORT+1, (unsigned char)(position&0xFF));
    // cursor HIGH port to vga INDEX register
    outb(BASE_PORT, 0x0E);
    outb(BASE_PORT+1, (unsigned char )((position>>8)&0xFF));

    return 0;
}

int __getRowOf(int offset){
    return offset/WIDTH;
}

int __getColOf(int offset){
    return offset-__getOffsetOf(__getRowOf(offset),0);
}

int __getOffsetOf(int row,int col){
    return row*WIDTH + col;
}

void __setOffset(int * offset, int row, int col){
    *offset=__getOffsetOf(row,col);
}

void __resetOffset(int * currOffset, int prevOffset){
    *currOffset=prevOffset;
}

VBElem * __getVBElem(int index){
    return &(video_buffer[index]);
}

void __setVBElem(int index, char c, int currOffset){
    video_buffer[index].c=c;
    video_buffer[index].startOffset=currOffset;
}

void __clearVBElem(int index){
    video_buffer[index].c='\0';
    video_buffer[index].startOffset=-1;
}

