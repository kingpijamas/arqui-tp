GLOBAL _ejectCD


SECTION .text

doNothing: 
	mov ebx, 65000

loop3: 
	dec ebx
	cmp ebx, 0 
	jne loop3 

isBSY: 
	MOV DX, 177h
LOOP1: 
	mov ax,0
	IN AL, DX 
	AND AL, 0x80
	JNE LOOP1 
	ret 

isDRDY: 
	ret

isDRQ: 
	MOV DX, 177h
LOOP2: 
	mov ax,0
	IN AL, DX 
	AND AL,0x08 
	JE LOOP2 
	ret

 _ejectCD: 
 	mov ebp, esp
 	push ebp

 	mov cx,[ebp+8] ;bus
 	mov bx,[ebp+12] ;drive

	call isBSY 
	mov dx,0
	add cx,6 ; 0x176
	mov dx, cx
	sub cs,6


	and bx,0x10 
	mov al,bx 
	out dx, al 

	add cd, 1 ; 0x171
	mov dx, cd
	sub cd,1 
	mov al, 0
	out dx, al
	
	add cd,7 ;0x177
	mov dx, cd
	sub cd,7 :cd=0x170
	mov al, 0xA0 
	out dx, al
	call doNothing
	call isBSY
	call isDRQ

	mov dx, cd
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
	
	add cx,7
	mov dx, cx
	sub cx,7
	mov al, 0xA0
	out dx, al
	call isBSY
	call isDRQ

	mov dx, cx
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