;====================================
; Parses parameters into buffer
;====================================
parse_param proc

	xor ch, ch
	mov cl, es:[80h]
	lea si, es:[81h]
	lea di, filename
	mov bx, 0
	cmp cl, 0
	je  no_param
	
param_loop:
	cmp bx, fbufsize
	je @@exit
	mov al, byte ptr [si + bx]
	cmp al, ' '
	je @@exit
	mov byte ptr [di + bx], al
	inc bx
	loop param_loop
	
	jmp @@exit
	
no_param:
	print no_parameters
	print help_msg
	exit 0
	
@@exit:
	mov byte ptr [di + bx], 0
	ret
parse_param endp