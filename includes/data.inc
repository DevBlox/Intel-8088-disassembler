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
; Error messages
;====================================
no_parameters db 'No parameters entered!', 10, 13, '$'
help_msg db 'Usage: Enter the COM filename as parameter.', 10, 13, 'Note: Only COM files are supported', 10, 13
		 db 'If you want to save the dump to file you should use the arrow directive of the command line (eg. DASM COM.COM > OUTPUT.TXT)',10, 13, '$'
open_fail db 'Failed to open file.', 10, 13, '$'
read_fail db 'Failure to read file contents', 10, 13, '$'
create_fail db 'Failed to create output file', 10, 13, '$'

;====================================
; Read buffer
;====================================
input_buffer db 100h dup(0)
input_buffer_size dw 100h
characters_read dw 0

;====================================
; Description
;====================================
description_msg db 'COM File disassembler', 10, 13, 'Author: Lukas Praninskas', 10, 13
				db 'Course: Programu sistemos', 10, 13
				db 'Group: 2', 10, 13
				db 'Description: This program disassembles a COM file, supports all instructions specified by the lecturer', 10, 13
				db '$'