;====================================
; File name buffer
;====================================
filename db 20 dup(0)
result_file db 20 dup(0)
fbufsize dw 20
handle dw 0
result_handle dw 0
parameter_offset dw 0 ; Sets the offset for the parameters, to continue from where left off

;====================================
; Description
;====================================
description_msg db 'COM File disassembler', 10, 13, 'Author: Lukas Praninskas', 10, 13
				db 'Course: Programu sistemos', 10, 13
				db 'Group: 2', 10, 13
				db 'Description: This program disassembles a COM file, supports all instructions specified by the lecturer', 10, 13, '$'

;====================================
; Error messages
;====================================
no_parameters db 'No parameters entered!', 10, 13, '$'
help_msg db 'Usage: Enter the COM file name and output file name as parameters.', 10, 13
		 db 'Note: Only COM files are supported', 10, 13, '$'
open_fail db 'Failed to open file.', 10, 13, '$'
read_fail db 'Failure to read file contents', 10, 13, '$'
create_fail db 'Failed to create output file', 10, 13, '$'

;====================================
; Utility strings
;====================================
endl db 10, 13, '$'
undef_op db 'Undefined $'
def_op db 'Defined $'
hxd db '0123456789ABCDEF'
separator db ', $'

;====================================
; Read buffer
;====================================
input_buffer db 100h dup(0)
input_buffer_size dw 100h
input_buffer_curr dw 0
characters_read dw 0

;====================================
; Main working variables
;====================================
; Resulting disassembled instructions buffer
dopbuffer db 50 dup (0)
dopbuffer_size dw 50
dopbuffer_length dw 0

; Operation buffer
opbuffer db 20 dup(0)
opbuffer_size dw 20
opbuffer_length dw 0

; Result buffer
rbuffer db 50 dup(0)
rbuffer_size dw 50
rbuffer_length dw 0

; Raw instructions
rawbuffer db 10 dup(0)
rawbuffer_size dw 10
rawbuffer_length dw 0

ip_count dw 0
curr_instruction db 0
temp dw 0

modvalue dw 0
dirvalue dw 0
sizvalue dw 0
regvalue dw 0
rmvalue dw 0
segvalue dw 0