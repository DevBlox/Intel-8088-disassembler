;====================================
; Disassembler of .exe files
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
	
start:
	mov ax, seg data_seg
	mov ds, ax

	call parse_param
	fopen filename, handle
	fread handle, input_buffer, input_buffer_size, characters_read
	printbuf input_buffer, characters_read
	exit 0
	
code_seg ends
end start