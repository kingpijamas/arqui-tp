#include "../include/video.h"

#include "../include/stdio.h"

colour STD_DISPLAY_background_colour    =    DEFAULT_STD_DISPLAY_BACKGROUND_COLOUR;
colour STD_DISPLAY_text_colour          =    DEFAULT_STD_DISPLAY_TEXT_COLOUR;
colour REG_DISPLAY_background_colour    =    DEFAULT_REG_DISPLAY_BACKGROUND_COLOUR;
colour REG_DISPLAY_text_colour          =    DEFAULT_REG_DISPLAY_TEXT_COLOUR;

int STD_DISPLAY_offset;
int REG_DISPLAY_offset;

int cursor_focus;//the disp the cursor is focusing
int cursor_row;
int cursor_col;

void __init_graphics(){
    STD_DISPLAY_offset=__getOffsetOf(STD_DISPLAY_MIN_ROW);
    REG_DISPLAY_offset=__getOffsetOf(REG_DISPLAY_MIN_ROW);
    __paint_area(STD_DISPLAY,STD_DISPLAY_background_colour,STD_DISPLAY_text_colour);
    __paint_area(REG_DISPLAY,REG_DISPLAY_background_colour,REG_DISPLAY_text_colour);
    __set_cursor_position_in(STD_DISPLAY,0,0);
}

size_t __print(int disp, const void * buffer, size_t count){
    int ans;
    switch(disp){
        case STD_DISPLAY:
            ans=__bounded_print(STD_DISPLAY_MIN_ROW, STD_DISPLAY_MAX_ROW, &STD_DISPLAY_offset, buffer, count);
            __bounded_set_cursor_position(STD_DISPLAY_MIN_ROW, STD_DISPLAY_MAX_ROW, 0, WIDTH, __getLineOf(STD_DISPLAY_offset), (STD_DISPLAY_offset)%WIDTH);
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
    int line, tab;
    size_t written;

    for(written=0; written < count; written++){
        c=((char *)buffer)[written];
        
        if (__getLineOf(*offset) > maxRow) {
            __bounded_shift_up(minRow,maxRow,offset,1);
        }

        switch(c){
            case '\n':
                (*offset)=__getOffsetOf(__getLineOf(*offset)+1);
                break;
            case '\b':
                if(((*offset)-1) > minRow){
                    video[((*offset)--)*2]='\0';
                }
                break;
            case '\t':
                for(tab=0; tab<TAB_LENGTH && (__getLineOf((*offset)+TAB_LENGTH)==__getLineOf(*offset)); tab++){
                    video[((*offset)++)*2]=' ';
                }
                break;
            default:
                if(((*offset)+1) % MAX_COL == 0){
                    (*offset)=__getOffsetOf(__getLineOf((*offset)+1));
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
    return 0;//TODO
}

void __bounded_paint_area(int minRow, int maxRow, int minCol, int maxCol, colour backgroundColour, colour textColour){
    char *video = (char*)VIDEO_ADDRESS;
    int i,j;
    for(i=minRow;i<=maxRow;i++){
        for(j=minCol;j<=maxCol;j++){
            video[2*(__getOffsetOf(i)+j)+1]=AS_COLOUR_BYTE(backgroundColour,textColour);
        }
    }
}

void __bounded_shift_up(int minRow, int maxRow, int *offset, int lines){
    char *video = (char*)VIDEO_ADDRESS;
    char c;
    int line, i;
    int killOffset,cloneOffset;

    for(;lines>0;lines--){
        for(line=minRow; line<maxRow; line++){
            killOffset=__getOffsetOf(line);
            cloneOffset=__getOffsetOf(line+1);
            for(i=0; i<2*WIDTH; i++){
                video[2*killOffset+i]=video[2*cloneOffset+i];
            }
        }
        killOffset=__getOffsetOf(maxRow);
        for(i=0; i<WIDTH; i++){
            video[2*(killOffset+i)]='\0';
        }
        *offset=killOffset;
        maxRow--;
    }
}

int __set_cursor_position_in(int disp, int relRow, int relCol){
    int ans;
    switch(disp){
        case STD_DISPLAY:
            ans=__bounded_set_cursor_position(STD_DISPLAY_MIN_ROW, STD_DISPLAY_MAX_ROW, MIN_COL, MAX_COL, relRow, relCol);
            break;
        case REG_DISPLAY:
            ans=__bounded_set_cursor_position(REG_DISPLAY_MIN_ROW, REG_DISPLAY_MAX_ROW, MIN_COL, MAX_COL, relRow, relCol);
            break;
        default:
            return INVALID_DISPLAY;
    }
    if(ans!=0){//TODO
        return ans;
    }
    cursor_focus=disp;
    cursor_row=relRow;
    cursor_col=relCol;
    return 0;//TODO
}

//Code taken from http://wiki.osdev.org/Text_Mode_Cursor
int __bounded_set_cursor_position(int minRow, int maxRow, int minCol, int maxCol, int row, int col){
    if(row<0 || row>maxRow || col<0 || col>maxCol){
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

int __getLineOf(int offset){
    return offset/WIDTH;
}

int __getOffsetOf(int line){
    return line*WIDTH;
}