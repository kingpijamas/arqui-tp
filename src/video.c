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
int vbindex=0;

int STD_DISPLAY_offset;
int REG_DISPLAY_offset;

int testflag=0;
int testflag1=0;

size_t __test_print(int minRow, int maxRow, int * offset, const void* buffer, size_t count){
    char c;
    int row;
    size_t written;
    
    //TODO not a circular buffer
    for(written=0; written < count; written++){
        c=((char *)buffer)[written];

        if (__getRowOf(*offset) > maxRow) {
            __shift_up(minRow,maxRow,offset,1);
        }

        __test_print_char(minRow,offset,c);

    }
    return written;
}

void __test_print_char(int minRow, int * offset, char c){
    char *video = (char*)VIDEO_ADDRESS;
    int tab;
    
    switch(c){
        case '\n':
            __setOffset(offset,__getRowOf(*offset)+1,0);
            return;
        case '\b':
            return;
        case '\t':
            for(tab=0; tab<TAB_LENGTH && (__getRowOf((*offset)+TAB_LENGTH)==__getRowOf(*offset)); tab++){
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

void __init_graphics(){
    STD_DISPLAY_offset=__getOffsetOfRow(STD_DISPLAY_MIN_ROW);
    vbindex=0;
    REG_DISPLAY_offset=__getOffsetOfRow(REG_DISPLAY_MIN_ROW);
    __paint_area(STD_DISPLAY,STD_DISPLAY_background_colour,STD_DISPLAY_text_colour);
    __paint_area(REG_DISPLAY,REG_DISPLAY_background_colour,REG_DISPLAY_text_colour);
    __set_cursor_position(0);

    // rprintf("vbuffer=");
    // testflag++;
    // rprintf("%d",VIDEO_BUFFER_SIZE);
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
            return /*__bounded*/__test_print(REG_DISPLAY_MIN_ROW, REG_DISPLAY_MAX_ROW, &REG_DISPLAY_offset, buffer, count);
        default:
            return INVALID_DISPLAY;
    }
}

size_t __bounded_print(int minRow, int maxRow, int * offset, const void* buffer, size_t count){
    char c;
    int row;
    size_t written;

    // if(testflag==0){
    //     testflag++;
    //     rprintf("vbindex=%d VIDEO_BUFFER_SIZE=%d",vbindex/*,written,*/,VIDEO_BUFFER_SIZE/*,(vbindex+written)%VIDEO_BUFFER_SIZE*/);
    // }
    
    //TODO not a circular buffer
    for(written=0; written < count; written++/*, vbindex=(vbindex+written)%VIDEO_BUFFER_SIZE*/){
        c=((char *)buffer)[written];

        // if(testflag==0){
            video_buffer[vbindex]=c;
        // }

        // if(testflag==1){
            //testflag++;
            // switch(video_buffer[vbindex]){
            //     case '\n':
            //         rprintf("\\n");
            //         break;
            //     case '\b':
            //         rprintf("\\b");
            //         break;
            //     case '\t':
            //         rprintf("\\t");
            //         //rprintf("\\t");
            //         break;
            //     default:
            //         rprintf("%c",video_buffer[vbindex]);
            // }
            // rprintf("(%d)\n",vbindex);
            //testflag--;
        // }

        if (__getRowOf(*offset) > maxRow) {
            __shift_up(minRow,maxRow,offset,1);
        }

        __bounded_print_char(minRow,offset,c);

        // if (testflag==0){
            vbindex++;
        // }
    }
    return written;
}

void __bounded_print_char(int minRow, int * offset, char c){
    char *video = (char*)VIDEO_ADDRESS;
    int tab;
    
    switch(c){
        case '\n':
            __setOffset(offset,__getRowOf(*offset)+1,0);
            return;
        case '\b':
            // testflag=1;
            // rprintf("__getRowOf((*offset)-1)=%d minRow=%d\n",__getRowOf((*offset)+1),minRow);
            // testflag=0;
            rprintf("Es un \\b. __getRowOf((*offset)-1)=%d minRow=%d\n",__getRowOf((*offset)-1),minRow);
            if(__getRowOf((*offset)-1) >= minRow){
                //rprintf("...%c|%c|%c... at <%s>\n",video_buffer[vbindex-3],video_buffer[vbindex-2],video_buffer[vbindex-1],video_buffer);
                switch(video_buffer[vbindex-1]){
                    case '\t':
                        //FIXME calcular bien el ancho del tab!!! No hace falta. Ver el primer caracter
                        //caso 1: me paso de linea
                        rprintf("borro un tab\n");
                        if ( __getRowOf((*offset)-TAB_LENGTH) < __getRowOf(*offset) ){
                            rprintf("caso tab de arriba\n");
                            for(tab=0; tab<TAB_LENGTH && video[((*offset)-1)*2]=='\0'; tab++){
                               video[(--(*offset))*2]='\0';
                            }
                        }else{//caso 2: me paso de linea
                            rprintf("caso tab de abajo\n");
                            for(tab=0; tab<TAB_LENGTH; tab++){
                               video[(--(*offset))*2]='\0';
                            }
                        }
                        break;
                    case '\n'://TODO
                        break;
                    default:
                        rprintf("no borro un tab\n");
                        video[(--(*offset))*2]='\0';
                }
                //\b suceeded, so two chars have to be removed from the video buffer (\b will be removed here, the previous one will be removed below)
                video_buffer[vbindex--]='\0';
            }
            //whether or not the \b succeeds, it has to be removed from the video buffer
            video_buffer[vbindex--]='\0';
            return;
        case '\t':
            for(tab=0; tab<TAB_LENGTH && (__getRowOf((*offset)+TAB_LENGTH)==__getRowOf(*offset)); tab++){
                //IMPORTANT: \ts have to be \0s, since spaces would make removing \ts with length<TAB_LENGTH impossible
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