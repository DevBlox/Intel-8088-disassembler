;====================================
; Prints out a line to console
;====================================
print macro string
	lea dx, string
	mov ah, 09h
	xor al, al
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
	push ax bx cx dx si di
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
	pop di si dx cx bx ax
endm
;====================================
; Creates a file
;====================================
fcreate macro filename, res_handle
	local @@error
	local @@continue
	lea dx, filename
	mov cx, 6
	mov ah, 3Ch
	int 21h
	jc @@error
	jmp @@continue
	
@@error:
	print create_fail
	exit 0
	
@@continue:
	mov res_handle, ax
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
;====================================
; Writes to file from buffer
;====================================
fwrite macro handle, buffer, sz
	push ax bx cx dx si di
	mov ah, 40h
	mov bx, handle
	mov cx, sz
	lea dx, buffer
	int 21h
	pop di si dx cx bx ax
endm
;====================================
; A utility for parameter parsing
;====================================
param_parse macro buffer
	lea di, buffer
	call parse_param
endm
;====================================
; Parses a byte of hexadecimal value
; Resulting characters in al, ah
;====================================
hxdb macro value
	push bx
	
	mov al, value
	xor ah, ah
	mov dh, 16
	div dh
	
	mov bl, al
	xor bh, bh
	mov al, hxd[bx]

	mov bl, ah
	xor bh, bh
	mov ah, hxd[bx]
	
	xchg ah, al ; Switch to retain order
	
	pop bx
endm
;====================================
; Copies a $ terminated string
; (emmiting $) to buffer
;====================================
swrite macro buffer, pos, string
	push di si ax bx cx dx
	lea si, string
	lea di, buffer
	mov bx, pos
	call __swrite
	mov pos, bx ; New last element position
	pop dx cx bx ax si di
endm
;====================================
; Puts a character on to buffer
;====================================
put macro buffer, pos, char
	push di si ax bx cx dx
	lea di, buffer
	mov bx, pos
	mov byte ptr [di + bx], char
	inc bx
	mov pos, bx
	pop dx cx bx ax si di
endm
;====================================
; Compares to value and calls a function if true
;====================================
cmpcall macro register, value, function
	local @@matches
	local @@exit
	push dx
	mov dl, register
	cmp dl, value
	je @@matches
	jmp @@exit
	
@@matches:
	call function
	
@@exit:
	pop dx
endm
;====================================
; Sets register and operand (byte)
;====================================
byteRegBo macro nm
	push bx
	lea bx, nm
	call __byteRegBo
	pop bx
endm
;====================================
; Sets register and operand (word)
;====================================
wordRegBo macro nm
	push bx
	lea bx, nm
	call __wordRegBo
	pop bx
endm
;====================================
; Checks x bit, zf = 1 if bit is 1
;====================================
chkbit macro value, bit
	local @@exit
	push ax dx cx
	mov cl, bit
	mov al, value
	mov dl, 1
	shl dl, cl
	and al, dl
@@exit:
	cmp dl, al
	pop cx dx ax	
endm
;====================================
; Compares first 4 bits
;====================================
cmp4bit macro x, y
	local @@equal
	local @@notequal
	local @@exit
	push ax bx cx
	mov al, x
	mov dl, y
	mov cl, 0	
	call load4bits	
	cmp cl, dl
	je @@equal
	jmp @@notequal
@@equal:
	mov temp, 1
	jmp @@exit
@@notequal:
	mov temp, 0
@@exit	:
	pop cx bx ax
endm