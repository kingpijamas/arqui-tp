#include "../include/video.h"

colour std_out_background_colour=DEFAULT_STD_OUT_BACKGROUND_COLOUR;
colour std_out_text_colour=DEFAULT_STD_OUT_TEXT_COLOUR;
colour reg_out_background_colour=DEFAULT_REG_OUT_BACKGROUND_COLOUR;
colour reg_out_text_colour=DEFAULT_REG_OUT_TEXT_COLOUR;

int std_out_offset;
int reg_out_offset;

int cursor_focus;//the fd the cursor is focusing
int cursor_row;
int cursor_col;

//Doesn't move the cursor. That is the shell's responsibility. (CHECK THIS)
void __init_graphics(){
    std_out_offset=__getOffsetOf(STD_OUT_MIN_ROW);
    reg_out_offset=__getOffsetOf(REG_OUT_MIN_ROW);
    __paint_area(STD_OUT,std_out_background_colour,std_out_text_colour);
    __paint_area(REG_OUT,reg_out_background_colour,reg_out_text_colour);
}

size_t __print(int fd, const void * buffer, size_t count){
    switch(fd){
        case STD_ERR:
        case STD_OUT:
            return __bounded_print(STD_OUT_MIN_ROW, STD_OUT_MAX_ROW, &std_out_offset, buffer, count);
        case REG_OUT:
            return __bounded_print(REG_OUT_MIN_ROW, REG_OUT_MAX_ROW, &reg_out_offset, buffer, count);
        default:
            return INVALID_DISPLAY;
    }
}

size_t __bounded_print(int minRow, int maxRow, int * offset, const void* buffer, size_t count){
    char *video = (char*)VIDEO_ADDRESS;
    char c;
    int line, aux;
    size_t written;

    for(written=0; written < count && __getLineOf(*offset) <= maxRow; written++){
        c=((char *)buffer)[written];
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
                if(((*offset)+TAB_LENGTH) % MAX_COL == 0){
                    (*offset)=__getOffsetOf(__getLineOf((*offset)+TAB_LENGTH));
                }
                for(aux=0;aux<TAB_LENGTH;aux++){
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

int __paint_area(int fd, colour backgroundColour, colour textColour){
    switch(fd){
        case STD_ERR:
        case STD_OUT:
            __bounded_paint_area(STD_OUT_MIN_ROW, STD_OUT_MAX_ROW, MIN_COL, MAX_COL, backgroundColour, textColour);
            break;
        case REG_OUT:
            __bounded_paint_area(REG_OUT_MIN_ROW, REG_OUT_MAX_ROW, MIN_COL, MAX_COL, backgroundColour, textColour);
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

int __shift_up(int fd, int lines){
    switch(fd){
        case STD_ERR:
        case STD_OUT:
            __bounded_shift_up(STD_OUT_MIN_ROW, STD_OUT_MAX_ROW, lines);
            break;
        case REG_OUT:
            __bounded_shift_up(REG_OUT_MIN_ROW, REG_OUT_MAX_ROW, lines);
            break;
        default:
            return INVALID_DISPLAY;
    }
    return 0;//TODO
}

void __bounded_shift_up(int minRow, int maxRow, int lines){
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
        maxRow--;
    }
}

int __set_cursor_position_in(int fd, int relRow, int relCol){
    int ans;
    switch(fd){
        case STD_ERR:
        case STD_OUT:
            ans=__bounded_set_cursor_position(STD_OUT_MIN_ROW, STD_OUT_MAX_ROW, MIN_COL, MAX_COL, relRow, relCol);
            break;
        case REG_OUT:
            ans=__bounded_set_cursor_position(REG_OUT_MIN_ROW, REG_OUT_MAX_ROW, MIN_COL, MAX_COL, relRow, relCol);
            break;
        default:
            return INVALID_DISPLAY;
    }
    if(ans!=0){//TODO
        return ans;
    }
    cursor_focus=fd;
    cursor_row=relRow;
    cursor_col=relCol;
    return 0;//TODO
}

//Code taken from http://wiki.osdev.org/Text_Mode_Cursor
int __bounded_set_cursor_position(int minRow, int maxRow, int minCol, int maxCol, int relRow, int relCol){
    int row=minRow+relRow;
    int col=minCol+relCol;

    if(relRow<0 || row>maxRow || relCol<0 || col>maxCol){
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