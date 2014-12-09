;====================================
; Parses parameters into buffer
;====================================
parse_param proc
	xor ch, ch
	mov cl, es:[80h]
	mov bx, 81h
	cmp cl, 0
	je  no_param
	inc bx
	dec cl
	lea di, filename
	mov si, 0
	
param_loop:
	cmp bx, fbufsize
	je @@exit
	mov al, byte ptr es:[si + bx]
	cmp al, ' '
	je @@exit
	mov byte ptr [di], al
	inc si
	inc di
	loop param_loop
	
	jmp @@exit
	
no_param:
	print no_parameters
	print help_msg
	exit 0
	
@@exit:
	mov byte ptr [di], 0
	ret
parse_param endp
;====================================
; Checks if the parameter parsed is '/?'
;====================================
check_help proc
	mov al, filename[0]
	cmp al, '/'
	je @@stage2
	jmp @@not_match
	
@@stage2:
	mov al, filename[1]
	cmp al, '?'
	je @@matches
	jmp @@not_match
	
@@matches:
	print description_msg
	print help_msg
	exit 0
	
@@not_match:
	ret
check_help endp