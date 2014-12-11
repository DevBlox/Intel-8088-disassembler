;====================================
; Disassembler of COM files
;
; Supports instructions specified
; by the lecturer
;
; Intel 8088 architecture
; Turbo Assembler
;
; Made as an assignment for
; Vilnius University
; Faculty Of Mathematics and Informatics
; Computer architecture
;
; Author: Lukas Praninskas
; Email:  l.praninskas@gmail.com

; Included files:
; code.inc -> The main code of the program
; util.inc -> Useful macros
; data.inc -> Variables, messages, names
; omap.inc -> A map of instructions
;====================================
.model small
assume ss:stack_seg, ds:data_seg, cs:code_seg
;====================================
; Stack segment
;====================================
stack_seg segment para stack 'stack'
	db 100h dup(0)
stack_seg ends
;====================================
; Data segment
;====================================
data_seg segment para public 'data'
	include includes/data.inc
	include includes/omap.inc
data_seg ends 
;====================================
; Code segment
;====================================
code_seg segment para public 'code'
	locals @@
	include includes/util.inc
	include includes/code.inc
;====================================
; Start of execution here
;====================================	
start:
	mov ax, seg data_seg
	mov ds, ax

	param_parse filename
	call check_help
	param_parse result_file
	fopen filename, handle
	fcreate result_file, result_handle
	
	fread handle, input_buffer, input_buffer_size, characters_read
	lea si, input_buffer
	lea di, rbuffer
	mov bx, 0
	mov ip_count, 100h
	
@@repeat:

	call nullify
	push ax bx cx dx
	call put_ip_counter
	mov bx, rbuffer_length
	mov byte ptr di[bx], '$'
	fwrite result_handle, rbuffer, rbuffer_length
	pop dx cx bx ax
	
	mov al, si[bx]
	call parser
	inc bx
	cmp bx, characters_read
	jnge @@repeat
	
	exit 0
	
code_seg ends
end start