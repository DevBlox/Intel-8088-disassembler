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
	push ax bx cx dx di si

	lea si, bit1
	mov cx, bit1_num
	mov bx, 0
	mov curr_instruction, al
	
@@repeat_loop:
	cmp al, si[bx]
	je @@matches
	inc bx
	loop @@repeat_loop
	jmp @@no_match
	
@@matches:
	inc ip_count
	push bx
	mov bx, opbuffer_length
	hxdb al
	mov opbuffer[bx], ah
	inc bx
	mov opbuffer[bx], al
	inc bx
	mov opbuffer_length, bx
	pop bx
	mov ax, bx
	mov cx, 2
	mul cx
	xchg bx, ax
	lea di, opnm[bx]
	xchg bx, ax
	mov di, [di]
	swrite dopbuffer, dopbuffer_length, [di]
	call stage2
	jmp @@exit
	
@@no_match:
	inc ip_count
	push bx
	mov bx, opbuffer_length
	hxdb al
	mov opbuffer[bx], ah
	inc bx
	mov opbuffer[bx], al
	inc bx
	mov opbuffer_length, bx
	pop bx
	swrite dopbuffer, dopbuffer_length, undef_op
	
@@exit:
	push ax
	put opbuffer, opbuffer_length, '$'
	swrite rbuffer, rbuffer_length, opbuffer
	put rbuffer, rbuffer_length, ' '
	pop ax
	
	mov bx, dopbuffer_length
	mov dopbuffer[bx], '$'
	swrite rbuffer, rbuffer_length, dopbuffer
	swrite rbuffer, rbuffer_length, endl
	
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
	mov dopbuffer_length, 0
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
;====================================
; Implementation of the macro
;====================================
__byteRegBo proc
	swrite dopbuffer, dopbuffer_length, [bx]
	swrite dopbuffer, dopbuffer_length, separator
	
	push ax bx
	
	mov bx, input_buffer_curr
	inc bx
	mov al, input_buffer[bx]
	mov input_buffer_curr, bx
	
	push ax
	mov bx, opbuffer_length
	hxdb al
	mov opbuffer[bx], ah
	inc bx
	mov opbuffer[bx], al
	inc bx
	mov opbuffer_length, bx
	pop ax
	
	hxdb al
	put dopbuffer, dopbuffer_length, ah
	put dopbuffer, dopbuffer_length, al
	
	put dopbuffer, dopbuffer_length, 'h'
	
	pop bx ax
	inc ip_count
	ret
__byteRegBo endp
;====================================
; Implementation of the macro
;====================================
__wordRegBo proc
	swrite dopbuffer, dopbuffer_length, [bx]
	swrite dopbuffer, dopbuffer_length, separator

	push ax bx
	
	mov bx, input_buffer_curr
	inc bx
	mov ah, input_buffer[bx]
	inc bx
	mov al, input_buffer[bx]
	mov input_buffer_curr, bx
	
	mov bx, opbuffer_length
	
	push ax
	hxdb al
	mov opbuffer[bx], ah
	inc bx
	mov opbuffer[bx], al
	inc bx
	pop ax
	mov al, ah
	hxdb al
	mov opbuffer[bx], ah
	inc bx
	mov opbuffer[bx], al
	inc bx
	mov opbuffer_length, bx
	
	mov bx, ax
	hxdb bl
	put dopbuffer, dopbuffer_length, ah
	put dopbuffer, dopbuffer_length, al
	hxdb bh
	put dopbuffer, dopbuffer_length, ah
	put dopbuffer, dopbuffer_length, al
	
	put dopbuffer, dopbuffer_length, 'h'
	
	pop bx ax
	add ip_count, 2
	ret
__wordRegBo endp
;====================================
; Stage 2, determining the completion
;====================================
stage2 proc
	push ax bx cx dx si di
	
	mov al, ope1[bx]
	
	cmpcall al, comFin, __comFin
	cmpcall al, comRbyte, __comRbyte
	cmpcall al, comRword, __comRword
	cmpcall al, comRnull, __comRnull
	cmpcall al, comNotFin, __comNotFin
	cmpcall al, comBalbo, __comBalbo
	cmpcall al, comBclbo, __comBclbo
	cmpcall al, comBdlbo, __comBdlbo
	cmpcall al, comBblbo, __comBblbo
	cmpcall al, comBahbo, __comBahbo
	cmpcall al, comBbhbo, __comBbhbo
	cmpcall al, comBchbo, __comBchbo
	cmpcall al, comBdhbo, __comBdhbo
	cmpcall al, comWaxbo, __comWaxbo
	cmpcall al, comWcxbo, __comWcxbo
	cmpcall al, comWdxbo, __comWdxbo
	cmpcall al, comWbxbo, __comWbxbo
	cmpcall al, comWspbo, __comWspbo
	cmpcall al, comWbpbo, __comWbpbo
	cmpcall al, comWsibo, __comWsibo
	cmpcall al, comWdibo, __comWdibo
	cmpcall al, comes, __comes
	cmpcall al, comcs, __comcs
	cmpcall al, comss, __comss
	cmpcall al, comds, __comds
	cmpcall al, comax, __comax
	cmpcall al, comcx, __comcx
	cmpcall al, comdx, __comdx
	cmpcall al, combx, __combx
	cmpcall al, comsp, __comsp
	cmpcall al, combp, __combp
	cmpcall al, comsi, __comsi
	cmpcall al, comdi, __comdi
	cmpcall al, comR2word, __comR2word
	
	pop di si dx cx bx ax
	ret
stage2 endp
;#####################################
; The stage 2 logic
;#####################################
__comFin proc
	ret
__comFin endp
;-------------------------------------
__comRbyte proc
	push ax bx
	
	mov bx, input_buffer_curr
	inc bx
	mov al, input_buffer[bx]
	mov input_buffer_curr, bx
	
	mov bx, dopbuffer_length
	hxdb al
	mov dopbuffer[bx], ah
	inc bx
	mov dopbuffer[bx], al
	inc bx
	mov dopbuffer_length, bx
	put dopbuffer, dopbuffer_length, 'h'
	
	hxdb al
	put rbuffer, rbuffer_length, ah
	put rbuffer, rbuffer_length, al
	
	pop bx ax
	inc ip_count
	ret
__comRbyte endp
;-------------------------------------
__comRword proc
	push ax bx
	
	mov bx, input_buffer_curr
	inc bx
	mov ah, input_buffer[bx]
	inc bx
	mov al, input_buffer[bx]
	mov input_buffer_curr, bx
	
	mov bx, dopbuffer_length
	push ax
	hxdb al
	mov dopbuffer[bx], ah
	inc bx
	mov dopbuffer[bx], al
	inc bx
	pop ax
	mov al, ah
	hxdb al
	mov dopbuffer[bx], ah
	inc bx
	mov dopbuffer[bx], al
	inc bx
	mov dopbuffer_length, bx
	put dopbuffer, dopbuffer_length, 'h'
	
	mov bx, ax
	hxdb bl
	put dopbuffer, dopbuffer_length, ah
	put dopbuffer, dopbuffer_length, al
	hxdb bh
	put dopbuffer, dopbuffer_length, ah
	put dopbuffer, dopbuffer_length, al
	
	pop bx ax
	add ip_count, 2
	ret
__comRword endp
;-------------------------------------
__comRnull proc
	ret
__comRnull endp
;-------------------------------------
__comNotFin proc
	swrite dopbuffer, dopbuffer_length, undef_op
	ret
__comNotFin endp
;-------------------------------------
__comBalbo proc
	byteRegBo nmAl
	ret
__comBalbo endp
;-------------------------------------
__comBclbo proc
	byteRegBo nmCl
	ret
__comBclbo endp
;-------------------------------------
__comBdlbo proc
	byteRegBo nmDl
	ret
__comBdlbo endp
;-------------------------------------
__comBblbo proc
	byteRegBo nmBl
	ret
__comBblbo endp
;-------------------------------------
__comBahbo proc
	byteRegBo nmAh
	ret
__comBahbo endp
;-------------------------------------
__comBbhbo proc
	byteRegBo nmBh
	ret
__comBbhbo endp
;-------------------------------------
__comBchbo proc
	byteRegBo nmCh
	ret
__comBchbo endp
;-------------------------------------
__comBdhbo proc
	byteRegBo nmDh
	ret
__comBdhbo endp
;-------------------------------------
__comWaxbo proc
	wordRegBo nmAx
	ret
__comWaxbo endp
;-------------------------------------
__comWcxbo proc
	wordRegBo nmCx
	ret
__comWcxbo endp
;-------------------------------------
__comWdxbo proc
	wordRegBo nmDx
	ret
__comWdxbo endp
;-------------------------------------
__comWbxbo proc
	wordRegBo nmBx
	ret
__comWbxbo endp
;-------------------------------------
__comWspbo proc
	wordRegBo nmSp
	ret
__comWspbo endp
;-------------------------------------
__comWbpbo proc
	wordRegBo nmBp
	ret
__comWbpbo endp
;-------------------------------------
__comWsibo proc
	wordRegBo nmSi
	ret
__comWsibo endp
;-------------------------------------
__comWdibo proc
	wordRegBo nmDi
	ret
__comWdibo endp
;-------------------------------------
__comes proc
	ret
__comes endp
;-------------------------------------
__comcs proc
	ret
__comcs endp
;-------------------------------------
__comss proc
	ret
__comss endp
;-------------------------------------
__comds proc
	ret
__comds endp
;-------------------------------------
__comax proc
	ret
__comax endp
;-------------------------------------
__comcx proc
	ret
__comcx endp
;-------------------------------------
__comdx proc
	ret
__comdx endp
;-------------------------------------
__combx proc
	ret
__combx endp
;-------------------------------------
__comsp proc
	ret
__comsp endp
;-------------------------------------
__combp proc
	ret
__combp endp
;-------------------------------------
__comsi proc
	ret
__comsi endp
;-------------------------------------
__comdi proc
	ret
__comdi endp
;-------------------------------------
__comR2word proc
	push ax bx
	
	mov bx, input_buffer_curr
	inc bx
	mov ah, input_buffer[bx]
	inc bx
	mov al, input_buffer[bx]
	mov input_buffer_curr, bx
	
	mov bx, opbuffer_length
	push ax
	hxdb al
	mov opbuffer[bx], ah
	inc bx
	mov opbuffer[bx], al
	inc bx
	pop ax
	mov al, ah
	hxdb al
	mov opbuffer[bx], ah
	inc bx
	mov opbuffer[bx], al
	inc bx
	mov opbuffer_length, bx
	
	mov bx, ax
	hxdb bl
	put dopbuffer, dopbuffer_length, ah
	put dopbuffer, dopbuffer_length, al
	hxdb bh
	put dopbuffer, dopbuffer_length, ah
	put dopbuffer, dopbuffer_length, al
	
	swrite dopbuffer, dopbuffer_length, separator
	
	mov bx, input_buffer_curr
	inc bx
	mov ah, input_buffer[bx]
	inc bx
	mov al, input_buffer[bx]
	mov input_buffer_curr, bx
	
	mov bx, opbuffer_length
	push ax
	hxdb al
	mov opbuffer[bx], ah
	inc bx
	mov opbuffer[bx], al
	inc bx
	pop ax
	mov al, ah
	hxdb al
	mov opbuffer[bx], ah
	inc bx
	mov opbuffer[bx], al
	inc bx
	mov opbuffer_length, bx
	
	mov bx, ax
	hxdb bl
	put dopbuffer, dopbuffer_length, ah
	put dopbuffer, dopbuffer_length, al
	hxdb bh
	put dopbuffer, dopbuffer_length, ah
	put dopbuffer, dopbuffer_length, al
	
	pop bx ax
	add ip_count, 4
	ret
__comR2word endp
;-------------------------------------