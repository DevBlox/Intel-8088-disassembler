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
;====================================
; Opens a file, prints message on fail and exits
;====================================
fopen macro filename, handle
	local @@error
	local @@contunue
	lea dx, filename
	mov ah, 3Dh
	mov al, 00h
	int 21h
	jc @@error
	jmp @@contunue
	
@@error:
	print open_fail
	exit 0
	
@@contunue:
	mov handle, ax
endm
;====================================
; Reads file contents to buffer
; Print an error and exits on fail
;====================================
fread macro handle, buffer, buffersize, count
	local @@error
	local @@continue
	mov ah, 3Fh
	mov bx, handle
	mov cx, buffersize
	lea dx, buffer
	int 21h
	jc @@error
	jmp @@continue
	
@@error:
	print read_fail
	exit 0
	
@@continue:
	mov count, ax
endm
;====================================
; Prints buffer contents
;====================================
printbuf macro buffer, count
	mov ah, 40h
	mov bx, 1
	mov cx, count
	lea dx, buffer
	int 21h
endm