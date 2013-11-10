#include "../include/video.h"
#include "../include/stdio.h"

#include "../include/stdio.h"

colour STD_DISPLAY_background_colour    =    DEFAULT_STD_DISPLAY_BACKGROUND_COLOUR;
colour STD_DISPLAY_text_colour          =    DEFAULT_STD_DISPLAY_TEXT_COLOUR;
colour REG_DISPLAY_background_colour    =    DEFAULT_REG_DISPLAY_BACKGROUND_COLOUR;
colour REG_DISPLAY_text_colour          =    DEFAULT_REG_DISPLAY_TEXT_COLOUR;

//TODO dont forget to shift this buffer up when the screen is shifted upwards
//TODO since it doesn't make much sense to have a video buffer for the reg_disp, maybe this should be implied in the name
char video_buffer[VIDEO_BUFFER_SIZE]={'\0'};

int STD_DISPLAY_offset;
int REG_DISPLAY_offset;

void __init_graphics(){
    STD_DISPLAY_offset=__getOffsetOfRow(STD_DISPLAY_MIN_ROW);
    REG_DISPLAY_offset=__getOffsetOfRow(REG_DISPLAY_MIN_ROW);
    __paint_area(STD_DISPLAY,STD_DISPLAY_background_colour,STD_DISPLAY_text_colour);
    __paint_area(REG_DISPLAY,REG_DISPLAY_background_colour,REG_DISPLAY_text_colour);
    __set_cursor_position(0);
}

size_t __print(int disp, const void * buffer, size_t count){
    int ans;
    switch(disp){
        case STD_DISPLAY:
            ans=__bounded_print(STD_DISPLAY_MIN_ROW, STD_DISPLAY_MAX_ROW, &STD_DISPLAY_offset, buffer, count);
            //FIXME: use __set_cursor_position!
            __set_cursor_position(STD_DISPLAY_offset);
            return ans;
        case REG_DISPLAY:
            return __bounded_print(REG_DISPLAY_MIN_ROW, REG_DISPLAY_MAX_ROW, &REG_DISPLAY_offset, buffer, count);
        default:
            return INVALID_DISPLAY;
    }
}

size_t __bounded_print(int minRow, int maxRow, int * offset, const void* buffer, size_t count){
    char *video = (char*)VIDEO_ADDRESS;
    char c;
    int row, tab;
    size_t written;

    for(written=0; written < count; written++){
        c=((char *)buffer)[written];
        
        if (__getRowOf(*offset) > maxRow) {
            __shift_up(minRow,maxRow,offset,1);
        }

        switch(c){
            case '\n':
                __setOffset(offset,__getRowOf(*offset)+1,0);
                break;
            case '\b':
                if((__getRowOf(*offset)-1) > minRow){
                    video[(--(*offset))*2]='\0';
                }
                break;
            case '\t':
                for(tab=0; tab<TAB_LENGTH && (__getRowOf((*offset)+TAB_LENGTH)==__getRowOf(*offset)); tab++){
                    video[((*offset)++)*2]=' ';
                }
                break;
            default:
                if(__getColOf((*offset)+1) % MAX_COL == 0){
                    (*offset)=__getOffsetOfRow(__getRowOf((*offset)+1));
                }
                video[((*offset)++)*2]=c;
        }
    }
    return written;
}

int __paint_area(int disp, colour backgroundColour, colour textColour){
    switch(disp){
        case STD_DISPLAY:
            __bounded_paint_area(STD_DISPLAY_MIN_ROW, STD_DISPLAY_MAX_ROW, MIN_COL, MAX_COL, backgroundColour, textColour);
            break;
        case REG_DISPLAY:
            __bounded_paint_area(REG_DISPLAY_MIN_ROW, REG_DISPLAY_MAX_ROW, MIN_COL, MAX_COL, backgroundColour, textColour);
            break;
        default:
            return INVALID_DISPLAY;
    }
}

void __bounded_paint_area(int minRow, int maxRow, int minCol, int maxCol, colour backgroundColour, colour textColour){
    char *video = (char*)VIDEO_ADDRESS;
    int i,j;
    for(i=minRow;i<=maxRow;i++){
        for(j=minCol;j<=maxCol;j++){
            video[2*__getOffsetOf(i,j)+1]=AS_COLOUR_BYTE(backgroundColour,textColour);
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

    return 0;//TODO
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