GLOBAL  _read_msw,_lidt
GLOBAL  _int_08_hand,_int_09_hand,_int_80_hand
GLOBAL  _mascaraPIC1,_mascaraPIC2,_Cli,_Sti
GLOBAL  _debug
GLOBAL  _Sys_Call

EXTERN  int_08
EXTERN  int_80
EXTERN  dummy_handler
EXTERN  int_09

SECTION .text


_Cli:
	cli			; limpia flag de interrupciones
	ret

_Sti:

	sti			; habilita interrupciones por flag
	ret

_mascaraPIC1:			; Escribe mascara del PIC 1
	push    ebp
        mov     ebp, esp
        mov     ax, [ss:ebp+8]  ; ax = mascara de 16 bits
        out	21h,al
        pop     ebp
        retn

_mascaraPIC2:			; Escribe mascara del PIC 2
	push    ebp
        mov     ebp, esp
        mov     ax, [ss:ebp+8]  ; ax = mascara de 16 bits
        out	0A1h,al
        pop     ebp
        retn

_read_msw:
        smsw    ax		; Obtiene la Machine Status Word
        retn


_lidt:				; Carga el IDTR
        push    ebp
        mov     ebp, esp
        push    ebx
        mov     ebx, [ss: ebp + 6] ; ds:bx = puntero a IDTR 
	rol	ebx,16		    	
	lidt    [ds: ebx]          ; carga IDTR
        pop     ebx
        pop     ebp
        retn


_int_08_hand:				; Handler de INT 8 ( Timer tick)
        push    ds
        push    es                      ; Se salvan los registros
        pusha                           ; Carga de DS y ES con el valor del selector
        mov     ax, 10h			; a utilizar.
        mov     ds, ax
        mov     es, ax                  
        call    int_08                 
        mov	al,20h			; Envio de EOI generico al PIC
	out	20h,al
	popa                            
        pop     es
        pop     ds
        iret

_int_09_hand:
        push ebp
        mov ebp, esp

        push ds
        push es
        pusha

        and ax,0
        in al, 60h

        push ax
        call int_09
        pop ax

        mov al,20h ; EOI command code
        out 20h,al ; IO base address for master pic
        
        popa
        pop es
        pop ds
        
        mov esp,ebp
        pop ebp

        iret

_int_80_hand:
        ; Build stack frame:
        push ebp
        mov ebp, esp

        ; Push parameters according to C convention
        push edx
        push ecx
        push ebx
        push esp        ; void ** args
        push eax        ; sysCallNo
    
        ; Call handler function
        call int_80

        ; send dummy EOI to the PIC
        mov al,20h
        out 20h,al
        
        ; Pop registers from stack
        pop eax
        pop esp

        pop ebx
        pop ecx
        pop edx

        ; Destroy stack frame:
        mov esp,ebp
        pop ebp

        iret

_Sys_Call:
        ; Build the stack frame:
        push ebp
        mov ebp, esp
        pusha

        ; Build the call params
        mov eax, [ebp+8]        ; sysCallNo
        mov ebx, [ebp+12]       ; fd
        mov ecx, [ebp+16]       ; buf
        mov edx, [ebp+20]       ; count

        ; Make the call
        int 80h
        pop eax

        ; Destroy the stack frame
        pop di
        pop si
        pop bx
        pop dx
        pop cx
        mov esp, ebp
        pop ebp
        ret

; Debug para el BOCHS, detiene la ejecució; Para continuar colocar en el BOCHSDBG: set $eax=0
;

_debug:
        push    bp
        mov     bp, sp
        push	ax
vuelve:	mov     ax, 1
        cmp	ax, 0
	jne	vuelve
	pop	ax
	pop     bp
        retn
