;====================================
; File name buffer
;====================================
filename db 20 dup(0)
fbufsize dw 20

;====================================
; Error messages
;====================================
no_parameters db 'No parameters entered!', 10, 13, '$'
help_msg db 'Usage: Enter the COM filename as parameter.', 10, 13, 'Only COM files are supported', 10, 13
