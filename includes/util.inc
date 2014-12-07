;====================================
; Prints out a line to console
;====================================
print macro string
	lea dx, string
	mov ah, 09h
	int 21h
endm
;====================================
; Exits program with code
;====================================
exit macro code
	mov ah, 4Ch
	mov al, code
	int 21h
endm