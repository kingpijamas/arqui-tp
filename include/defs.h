/***************************************************
  Defs.h
	
****************************************************/

#ifndef _defs_
#define _defs_

#define byte unsigned char
#define word short int
#define dword int
#define NULL (void*)0

//Utility defines
typedef int bool;
#define true             1
#define false            0

typedef int size_t;
typedef short int ssize_t;

// Flags para derechos de acceso de los segmentos
#define ACS_PRESENT      0x80            // segmento presente en memoria
#define ACS_CSEG         0x18            // segmento de codigo
#define ACS_DSEG         0x10            // segmento de datos
#define ACS_READ         0x02            // segmento de lectura
#define ACS_WRITE        0x02            // segmento de escritura
#define ACS_IDT          ACS_DSEG
#define ACS_INT_386      0x0E		        // Interrupt GATE 32 bits
#define ACS_INT          ( ACS_PRESENT | ACS_INT_386 )

#define ACS_CODE         (ACS_PRESENT | ACS_CSEG | ACS_READ)
#define ACS_DATA         (ACS_PRESENT | ACS_DSEG | ACS_WRITE)
#define ACS_STACK        (ACS_PRESENT | ACS_DSEG | ACS_WRITE)

//File descriptors
  typedef enum {
    STD_IN=0,
    STD_OUT=1,
    STD_ERR=2,
    REG_OUT=3
  } FILE_DESC;

//System Calls
#define SYS_READ         0x03
#define SYS_WRITE        0x04

//C constants and defs
#define EOF             -1

#pragma pack (1) 		// Alinear las siguiente estructuras a 1 byte

// Descriptor de segmento
  typedef struct {
    word limit,
    base_l;
    byte base_m,
    access,
    attribs,
    base_h;
  } DESCR_SEG;


// Descriptor de interrupcion
  typedef struct {
    word      offset_l,
    selector;
    byte      cero,
    access;
    word	    offset_h;
  } DESCR_INT;

// IDTR
  typedef struct {
    word  limit;
    dword base;
  } IDTR;



#endif
