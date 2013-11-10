#ifndef VIDEO_H
#define VIDEO_H

#include "../include/defs.h"

#define VIDEO_ADDRESS			0xb8000
#define BASE_PORT				0x3D4

#define VIDEO_BUFFER_SIZE		MAX(VIDEO_AREA(WIDTH,REG_DISPLAY_HEIGHT),VIDEO_AREA(WIDTH,STD_DISPLAY_HEIGHT))

//FIXME all these should be some sort of enum
#define INVALID_DISPLAY 		-1
#define INVALID_CURSOR			-2

#define STD_DISPLAY				0
#define REG_DISPLAY 			1

#define WIDTH					80
#define HEIGHT  				25

#define MIN_COL					0
#define MAX_COL					WIDTH-1

#define MIN_ROW 				0
#define MAX_ROW 				HEIGHT-1

#define REG_DISPLAY_HEIGHT		10
#define REG_DISPLAY_MIN_ROW 	0
#define REG_DISPLAY_MAX_ROW 	REG_DISPLAY_MIN_ROW+REG_DISPLAY_HEIGHT-1

#define STD_DISPLAY_HEIGHT		15
#define STD_DISPLAY_MIN_ROW 	REG_DISPLAY_MAX_ROW+1
#define STD_DISPLAY_MAX_ROW 	STD_DISPLAY_MIN_ROW+STD_DISPLAY_HEIGHT-1

#define TAB_LENGTH				4

//Text mode colours
#define BLACK					0x0
#define BLUE					0x1
#define GREEN					0x2
#define CYAN					0x3
#define RED						0x4
#define MAGENTA					0x5
#define BROWN					0x6
#define LIGHT_GREY				0x7
#define DARK_GRAY				0x8
#define LIGHT_BLUE				0x9
#define LIGHT_GREEN				0xA
#define LIGHT_CYAN				0xB
#define LIGHT_RED				0xC
#define LIGHT_MAGENTA			0xD
#define YELLOW					0xE
#define WHITE					0xF

#define AS_COLOUR_BYTE(backColour,textColour)	((backColour<<4) | textColour & 0x0F)

#define DEFAULT_STD_DISPLAY_BACKGROUND_COLOUR	BLACK
#define DEFAULT_STD_DISPLAY_TEXT_COLOUR			GREEN
#define DEFAULT_REG_DISPLAY_BACKGROUND_COLOUR	MAGENTA
#define DEFAULT_REG_DISPLAY_TEXT_COLOUR			WHITE

#define MAX(x,y)								(x>y)? x:y
#define VIDEO_AREA(width,height)				(width*height)

typedef char colour;

void __init_graphics();
size_t __print(int disp, const void * buffer, size_t count);
size_t __bounded_print(int minRow, int maxRow, int * offset, const void* buffer, size_t count);
int __paint_area(int disp, colour backgroundColour, colour textColour);
void __bounded_paint_area(int minRow, int maxRow, int minCol, int maxCol, colour backgroundColour, colour textColour);
void __shift_up(int minRow, int maxRow, int *offset, int rows);
int __set_cursor_position(int offset);
int __getRowOf(int offset);
int __getColOf(int offset);
int __getOffsetOf(int row,int col);
void __setOffset(int * offset, int row, int col);
#define __getOffsetOfRow(row)					__getOffsetOf(row,0)

#endif