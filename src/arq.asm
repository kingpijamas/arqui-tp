GLOBAL 	_closeCD
GLOBAL 	_ejectCD
GLOBAL 	_infoCD

SECTION .text
align 4


doNothing:
    mov ebx, 65000
loop3:
    dec ebx
    cmp ebx, 0
    jne loop3


isBSY:
    MOV DX, 1f7h
_LOOP1:
    IN AL, DX
    AND AL, 0x80
    JNE _LOOP1
    ret

isDRDY:
    ret

isDRQ:
    MOV DX, 1f7h
_LOOP2:
    IN AL, DX
    AND AL,0x08
    JE _LOOP2
    ret

isnotDRQ:
    MOV DX, 1f7h
_LOOP2222:
    IN AL, DX
    AND AL,0x08
    JNE _LOOP2222
    ret

_ejectCD:

    call isBSY

    mov dx, 0x1f6
    mov al, 10h
    out dx, al 

    mov dx, 0x1f1
    mov al, 0
    out dx, al 

	MOV DX, 3F6h ;Device Control register
	MOV AL, 00001010b ;nIEN is the second bit from the right here
	OUT DX, AL ;nIEN is now one!

    mov dx, 0x1f7
    mov al, 0xA0 ;ATAPI COMMAND
    out dx, al 

    call doNothing

    call isBSY
    call isDRQ

    mov dx, 0x1f0
    mov ax, 0x1E
    out dx, ax

    mov ax, 0
    out dx, ax

    mov ax, 0
    out dx, ax

    mov ax, 0
    out dx, ax

    mov ax, 0
    out dx, ax

    mov ax, 0
    out dx, ax

    call isBSY
    call isDRDY

    mov dx, 0x1f7
    mov al, 0xA0
    out dx, al

    call isBSY
    call isDRQ

    mov dx, 0x1f0
    mov ax, 1Bh
    out dx, ax

    mov ax, 0
    out dx, ax

    mov ax, 2
    out dx, ax

    mov ax, 0
    out dx, ax

    mov ax, 0
    out dx, ax

    mov ax, 0
    out dx, ax

    call isBSY
    ret

_closeCD:    
    call isBSY

    mov dx, 0x1f6
    mov al, 10h
    out dx, al 

    mov dx, 0x1f7
    mov al, 0xA0 ;ATAPI COMMAND
    out dx, al 

    call doNothing

    call isBSY
    call isDRQ

    mov dx, 0x1f0
    mov ax, 1Bh
    out dx, ax

    mov ax, 0
    out dx, ax

    mov ax, 3
    out dx, ax

    mov ax, 0
    out dx, ax

    mov ax, 0
    out dx, ax

    mov ax, 0
    out dx, ax

    call isBSY
    ret


_infoCD:

    call isBSY

    mov dx, 0x1f6
    mov al, 10h
    out dx, al 

    mov dx, 0x1F4
    mov al, 0x08
    out dx, al

    mov dx, 0x1F5
    mov al, 0x08
    out dx, al

    mov dx, 0x1f7
    mov al, 0xA0 ;ATAPI COMMAND
    out dx, al 

    call doNothing

    call isBSY

    call isDRQ

    mov dx, 0x1f0

    mov ax, 0x025
    out dx, ax

    mov ax, 0
    out dx, ax

    mov ax, 0
    out dx, ax

    mov ax, 0
    out dx, ax

    mov ax, 0
    out dx, ax

    mov ax, 0
    out dx, ax

    call isBSY

    call isnotDRQ

    call doNothing
    
    mov dx, 1f0h

    mov ebx, 0
    in ax, dx
    mov [buffer + ebx], ax
    
    add ebx, 2
    in ax, dx
    mov [buffer + ebx], ax

    add ebx, 2
    in ax, dx
    mov [buffer + ebx], ax

    add ebx, 2
    in ax, dx
    mov [buffer + ebx], ax

    mov eax, [buffer]
    mov ebx, [buffer + 4]
    push ebx
    push eax
    add esp,8

    ret

SECTION .bss

buffer resb 8

SECTION .data
	buff 		db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0	

	currentDrive	dw	0h
	extraPort	dw	3f6h
	basePort	dw	1f0h
	
	drive		dw	0h, 10h
	ports		dw	1f0h, 0170, 1e8h, 168h	
	extraPorts	dw	3f6h, 376h, 3e6h, 366h
	
	ejectCmd 	db 	1Bh, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0
