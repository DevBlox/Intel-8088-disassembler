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
	push ax
	hxdb al
	put opbuffer, opbuffer_length, ah
	put opbuffer, opbuffer_length, al
	put opbuffer, opbuffer_length, '$'
	swrite rbuffer, rbuffer_length, opbuffer
	put rbuffer, rbuffer_length, ' '
	pop ax
	
	push ax bx cx dx di si

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
	swrite rbuffer, rbuffer_length, [di]
	swrite rbuffer, rbuffer_length, endl
	jmp @@exit
	
@@no_match:
	swrite rbuffer, rbuffer_length, undef_op
	swrite rbuffer, rbuffer_length, endl
	
@@exit:
	pop si di dx cx bx ax
	ret
parser endp
;====================================
; Puts the ip counter into the result buffer
;====================================
put_ip_counter proc
	push ax bx cx dx

	mov bx, rbuffer_length
	mov dx, ip_count
	
	hxdb dh
	mov byte ptr di[bx], ah
	inc bx
	mov byte ptr di[bx], al
	inc bx
	
	hxdb dl
	mov byte ptr di[bx], ah
	inc bx
	mov byte ptr di[bx], al
	inc bx
	
	mov rbuffer_length, bx
	
	put rbuffer, rbuffer_length, ':'
	put rbuffer, rbuffer_length, ' '
	pop dx cx bx ax
	ret
put_ip_counter endp
;====================================
; Resets counter variables to null
;====================================
nullify proc
	mov opbuffer_length, 0
	mov rbuffer_length, 0
	ret
nullify endp
;====================================
; Implementation of the swrite
;====================================
__swrite proc
@@loop:
	mov al, [si]
	cmp al, '$'
	je @@exit
	mov [di + bx], al
	inc bx
	inc si
	jmp @@loop

@@exit:
	ret
__swrite endp