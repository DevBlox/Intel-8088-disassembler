;====================================
; File name buffer
;====================================
filename db 20 dup(0)
fbufsize dw 20
handle dw 0

;====================================
; Error messages
;====================================
no_parameters db 'No parameters entered!', 10, 13, '$'
help_msg db 'Usage: Enter the COM filename as parameter.', 10, 13, 'Note: Only COM files are supported', 10, 13, '$'
open_fail db 'Failed to open file.', 10, 13, '$'
read_fail db 'Failure to read file contents', 10, 13, '$'

;====================================
; Read buffer
;====================================
input_buffer db 100h dup(0)
input_buffer_size dw 100h
characters_read dw 0