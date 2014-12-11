;====================================
; Parses parameters into buffer
; di must have effective address ready
;====================================
parse_param proc
	xor ch, ch
	mov cl, es:[80h]
	mov bx, 81h
	cmp cl, 0
	je  no_param
	inc bx
	dec cl
	mov si, parameter_offset
	sub cx, si ; Get actual counter
	
param_loop:
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
	inc si
	mov parameter_offset, si
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
;====================================
; Parses commands, the main procedure
;====================================
parser proc
	push si
	push bx

	lea si, bit1
	mov cx, bit1_num
	mov bx, 0
	
@@repeat_loop:
	cmp al, si[bx]
	je @@matches
	inc bx
	loop @@repeat_loop
	jmp @@no_match
	
@@matches:
	mov ax, bx
	mov cx, 2
	mul cx
	mov bx, ax
	lea di, opnm[bx]
	mov di, [di]
	print [di]
	print endl
	jmp @@exit
	
@@no_match:
	print undef_op
	print endl
	
@@exit:
	pop bx
	pop si
	ret
parser endp
;====================================
; Puts the ip counter into the result buffer
;====================================
put_ip_counter proc
	mov dx, ip_count
	
	hxdb dh
	mov byte ptr rbuffer_length[di], ah
	inc rbuffer_length
	mov byte ptr rbuffer_length[di], al
	inc rbuffer_length
	
	hxdb dl
	mov byte ptr rbuffer_length[di], ah
	inc rbuffer_length
	mov byte ptr rbuffer_length[di], al
	inc rbuffer_length
	ret
put_ip_counter endp
;====================================
; Resets important variables to null
;====================================
nullify proc
	mov opbuffer_length, 0
	mov rbuffer_length, 0
	ret
nullify endp