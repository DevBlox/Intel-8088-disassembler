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
	call add_spaces
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
; Adds a specific amount of spaces
; to align text
;====================================
add_spaces proc
@@loop:
	put opbuffer, opbuffer_length, ' '
	cmp opbuffer_length, 15
	jl @@loop
	ret
add_spaces endp
;====================================
; Resets counter variables to null
;====================================
nullify proc
	mov opbuffer_length, 0
	mov rbuffer_length, 0
	mov dopbuffer_length, 0
	mov modvalue, 0
	mov dirvalue, 0
	mov sizvalue, 0
	mov regvalue, 0
	mov rmvalue, 0
	mov segvalue, 0
	mov widvalue ,0 
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
	
	mov temp, ax
	hxdb ah
	mov opbuffer[bx], ah
	inc bx
	mov opbuffer[bx], al
	inc bx
	mov ax, temp
	mov dl, al
	hxdb al
	mov opbuffer[bx], ah
	inc bx
	mov opbuffer[bx], al
	inc bx
	mov opbuffer_length, bx
	
	mov ax, temp
	hxdb al
	put dopbuffer, dopbuffer_length, ah
	put dopbuffer, dopbuffer_length, al
	mov ax, temp
	hxdb ah
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
	cmpcall al, comNotFin, stage3
	
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
	mov dl, al
	hxdb al
	mov dopbuffer[bx], ah
	inc bx
	mov dopbuffer[bx], al
	inc bx
	mov dopbuffer_length, bx
	put dopbuffer, dopbuffer_length, 'h'
	
	hxdb dl
	put opbuffer, opbuffer_length, ah
	put opbuffer, opbuffer_length, al
	
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
	add ip_count, 2
	
	push ax
	hxdb al
	put dopbuffer, dopbuffer_length, ah
	put dopbuffer, dopbuffer_length, al
	pop ax
	push ax
	hxdb ah
	put dopbuffer, dopbuffer_length, ah
	put dopbuffer, dopbuffer_length, al
	put opbuffer, opbuffer_length, ah
	put opbuffer, opbuffer_length, al
	pop ax
	hxdb al
	put opbuffer, opbuffer_length, ah
	put opbuffer, opbuffer_length, al
	put dopbuffer, dopbuffer_length, 'h'
	pop bx ax
	ret
__comRword endp
;-------------------------------------
__comRnull proc
	ret
__comRnull endp
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
	swrite dopbuffer, dopbuffer_length, nmEs
	ret
__comes endp
;-------------------------------------
__comcs proc
	swrite dopbuffer, dopbuffer_length, nmCs
	ret
__comcs endp
;-------------------------------------
__comss proc
	swrite dopbuffer, dopbuffer_length, nmSs
	ret
__comss endp
;-------------------------------------
__comds proc
	swrite dopbuffer, dopbuffer_length, nmDs
	ret
__comds endp
;-------------------------------------
__comax proc
	swrite dopbuffer, dopbuffer_length, nmAx
	ret
__comax endp
;-------------------------------------
__comcx proc
	swrite dopbuffer, dopbuffer_length, nmCx
	ret
__comcx endp
;-------------------------------------
__comdx proc
	swrite dopbuffer, dopbuffer_length, nmDx
	ret
__comdx endp
;-------------------------------------
__combx proc
	swrite dopbuffer, dopbuffer_length, nmBx
	ret
__combx endp
;-------------------------------------
__comsp proc
	swrite dopbuffer, dopbuffer_length, nmSp
	ret
__comsp endp
;-------------------------------------
__combp proc
	swrite dopbuffer, dopbuffer_length, nmBp
	ret
__combp endp
;-------------------------------------
__comsi proc
	swrite dopbuffer, dopbuffer_length, nmSi
	ret
__comsi endp
;-------------------------------------
__comdi proc
	swrite dopbuffer, dopbuffer_length, nmDi
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
	hxdb ah
	mov opbuffer[bx], ah
	inc bx
	mov opbuffer[bx], al
	inc bx
	pop ax
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
	hxdb ah
	mov opbuffer[bx], ah
	inc bx
	mov opbuffer[bx], al
	inc bx
	pop ax
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
;=====================================
; Stage 3, long instructions
;=====================================
stage3 proc	

	mov bx, input_buffer_curr
	mov al, input_buffer[bx]
	mov input_buffer_curr, bx
	
	mov cx, stg3_sz
	mov bx, 0
	
@@cmploop:
	cmp4bit al, stg3[bx]
	cmp temp, 1
	je @@found
	inc bx
	loop @@cmploop
	jmp @@exit
	
@@found:
	mov ax, bx
	mov dx, 2
	mul dx
	mov bx, ax
	mov ax, fnc3[bx]
	call ax
@@exit:
	ret
stage3 endp
;=====================================
; Loads 4 bits of al to cl
;=====================================
load4bits proc
	chkbit al, 7
	je @@bit1is1
@@1:
	chkbit al, 6
	je @@bit2is1
@@2:
	chkbit al, 5
	je @@bit3is1
@@3:
	chkbit al, 4
	je @@bit4is1
	jmp @@exit
@@bit1is1:
	add cl, 128
	jmp @@1
@@bit2is1:
	add cl, 64
	jmp @@2
@@bit3is1:
	add cl, 32
	jmp @@3
@@bit4is1:
	add cl, 16
@@exit:
	ret
load4bits endp
;=====================================
; Sets the mod value
;=====================================
getmod proc
	; Finding mod
	mov dl, 128
	xor dl, al
	cmp al, dl
	je @@t0
	jmp @@t1
	
@@t1:
	mov dl, 64
	xor dl, al
	cmp al, dl
	je @@modis11
	jmp @@modis10
	
@@t0:
	mov dl, 64
	xor dl, al
	cmp al, dl
	je @@modis00
	jmp @@modis01
	
@@modis10:
	mov	modvalue, mod10
	jmp @@exit
	
@@modis01:
	mov modvalue, mod01
	jmp @@exit
	
@@modis00:
	mov modvalue, mod00
	jmp @@exit
	
@@modis11:
	mov modvalue, mod11
	jmp @@exit
	
@@exit:
	
getmod endp
;=====================================
; Stage 3 logic functions
;=====================================
fn80h proc
	mov bx, input_buffer_curr
	mov al, input_buffer[bx]
	call fn20h
	ret
fn80h endp
;-------------------------------------
fnA0h proc
	mov bx, input_buffer_curr
	mov al, input_buffer[bx]
	chkbit al, 1
	je @@toakum
	jmp @@tomem
@@toakum:
	swrite dopbuffer, dopbuffer_length, nmAx
	swrite dopbuffer, dopbuffer_length, separator
	mov bx, input_buffer_curr
	inc bx
	mov ah, input_buffer[bx]
	inc bx
	mov al, input_buffer[bx]
	mov input_buffer_curr, bx
	add ip_count, 2
	push ax
	put dopbuffer, dopbuffer_length, '['
	hxdb ah
	put dopbuffer, dopbuffer_length, ah
	put dopbuffer, dopbuffer_length, al
	pop ax
	push ax
	hxdb al
	put dopbuffer, dopbuffer_length, ah
	put dopbuffer, dopbuffer_length, al
	put opbuffer, opbuffer_length, ah
	put opbuffer, opbuffer_length, al
	pop ax
	hxdb ah
	put opbuffer, opbuffer_length, ah
	put opbuffer, opbuffer_length, al
	put dopbuffer, dopbuffer_length, ']'
	jmp @@exit

@@tomem:
	mov bx, input_buffer_curr
	inc bx
	mov ah, input_buffer[bx]
	inc bx
	mov al, input_buffer[bx]
	mov input_buffer_curr, bx
	add ip_count, 2
	push ax
	put dopbuffer, dopbuffer_length, '['
	hxdb ah
	put dopbuffer, dopbuffer_length, ah
	put dopbuffer, dopbuffer_length, al
	pop ax
	push ax
	hxdb al
	put dopbuffer, dopbuffer_length, ah
	put dopbuffer, dopbuffer_length, al
	put opbuffer, opbuffer_length, ah
	put opbuffer, opbuffer_length, al
	pop ax
	hxdb ah
	put opbuffer, opbuffer_length, ah
	put opbuffer, opbuffer_length, al
	swrite dopbuffer, dopbuffer_length, separator
	swrite dopbuffer, dopbuffer_length, nmAx
	put dopbuffer, dopbuffer_length, ']'
@@exit:
	ret
fnA0h endp
;-------------------------------------
fnC0h proc
	mov bx, input_buffer_curr
	mov al, input_buffer[bx]
	chkbit al, 0
	je @@isword
	mov widvalue, 0
	jmp @@findmod
@@isword:
	mov widvalue, 1
@@findmod:
	inc bx
	mov al, input_buffer[bx]
	mov input_buffer_curr, bx
	inc ip_count
	push ax
	hxdb al
	put opbuffer, opbuffer_length, ah
	put opbuffer, opbuffer_length, al
	pop ax
	chkbit al, 7
	je @@mod1is1
	jmp @@mod2
@@mod1is1:
	add modvalue, 1
@@mod2:
	chkbit al, 6
	je @@mod2is1
	jmp @@findrm
@@mod2is1:
	add modvalue, 2
@@findrm:
	chkbit al, 2
	je @@rm1is1
	jmp @@rm2
@@rm1is1:
	add rmvalue, 4
@@rm2:
	chkbit al, 1
	je @@rm2is1
	jmp @@rm3
@@rm2is1:
	add rmvalue, 2
@@rm3:
	chkbit al, 0
	je @@rm3is1
@@rm3is1:
	add rmvalue, 1
@@parse:
	call parse_bo_mov
	ret
fnC0h endp
;-------------------------------------
fn00h proc
	call fn20h
	ret
fn00h endp
;-------------------------------------
fn20h proc
	mov bx, input_buffer_curr
	mov al, input_buffer[bx]
	chkbit al, 1
	je @@diris1
	mov dirvalue, dirrm
@@1:
	chkbit al, 0
	je @@widthis1
	mov widvalue, 0
	jmp @@findmod
@@diris1:
	mov dirvalue, dirreg
	jmp @@1
@@widthis1:
	mov widvalue, 1
@@findmod:
	mov bx, input_buffer_curr
	inc bx
	mov al, input_buffer[bx]
	mov input_buffer_curr, bx
	inc ip_count
	push ax
	hxdb al
	put opbuffer, opbuffer_length, ah
	put opbuffer, opbuffer_length, al
	pop ax
	chkbit al, 7
	je @@mod1
@@2:
	chkbit al, 6
	je @@mod2
	jmp @@findreg
@@mod1:
	add modvalue, 1
	jmp @@2
@@mod2:
	add modvalue, 2
@@findreg:
	chkbit al, 5
	je @@reg1
@@r1:
	chkbit al, 4
	je @@reg2
@@r2:
	chkbit al, 3
	je @@reg3
	jmp @@findrm
@@reg1:
	add regvalue, 4
	jmp @@r1
@@reg2:
	add regvalue, 2
	jmp @@r2
@@reg3:
	add regvalue, 1
@@findrm:
	chkbit al, 2
	je @@rm1
@@m1:
	chkbit al, 1
	je @@rm2
@@m2:
	chkbit al, 0
	je @@rm3
	jmp @@allfound
@@rm1:
	add rmvalue, 4
	jmp @@m1
@@rm2:
	add rmvalue, 2
	jmp @@m2
@@rm3:
	add rmvalue, 1
@@allfound:
	call parse_compex
	ret
fn20h endp
;-------------------------------------
fn30h proc
	call fn20h
	ret
fn30h endp
;-------------------------------------
fnF0h proc
	mov bx, input_buffer_curr
	mov al, input_buffer[bx]
	mov curr_instruction, al
	inc bx
	mov al, input_buffer[bx]
	mov input_buffer_curr, bx
	inc ip_count
	push ax
	hxdb al
	put opbuffer, opbuffer_length, ah
	put opbuffer, opbuffer_length, al
	pop ax
	chkbit al, 5
	je @@ins1
	jmp @@02
@@ins1:
	add regvalue, 4
@@02:
	chkbit al, 4
	je @@ins2
	jmp @@03
@@ins2:
	add regvalue, 2
@@03:
	chkbit al, 3
	je @@ins3
	jmp @@endins
@@ins3:
	add regvalue, 1
@@endins:
	mov ax, regvalue
	mov bx, 2
	mul bx
	mov bx, ax
	lea si, F0names[bx]
	mov si, [si]
	mov dopbuffer_length, 0
	swrite dopbuffer, dopbuffer_length, [si]
	call jmpcalllogic
	ret
fnF0h endp
;#####################################
parse_compex proc
	cmp dirvalue, dirreg
	je @@toreg
	jmp @@tomemreg
@@toreg:
	cmp widvalue, 1
	je @@wide1
	jmp @@wide2
@@wide1:
	call print_reg_word
	swrite dopbuffer, dopbuffer_length, separator
	jmp @@continuetoreg
@@wide2:
	call print_reg_byte
	swrite dopbuffer, dopbuffer_length, separator
@@continuetoreg:
	call by_mod
	jmp @@exit
@@tomemreg:
	call by_mod
	swrite dopbuffer, dopbuffer_length, separator
	cmp widvalue, 1
	je @@wide3
	jmp @@wide4
@@wide3:
	call print_reg_word
	jmp @@exit
@@wide4:
	call print_reg_byte
@@exit:
	ret
parse_compex endp
;======================================
print_reg_byte proc
	push ax bx
	mov ax, regvalue
	mov bx, 2
	mul bx
	mov bx, ax
	mov bx, rmtable_mod11_byte[bx]
	swrite dopbuffer, dopbuffer_length, [bx]
	pop bx ax
	ret
print_reg_byte endp
;===========================================
print_reg_word proc
	push ax bx
	mov ax, regvalue
	mov bx, 2
	mul bx
	mov bx, ax
	mov bx, rmtable_mod11_word[bx]
	swrite dopbuffer, dopbuffer_length, [bx]
	pop bx ax
	ret
print_reg_word endp
;=========================================
by_mod proc
	cmp modvalue, mod00
	je @@modnooff_temp
	cmp modvalue, mod11
	je @@modtoreg_temp
	jmp @@continueafter
@@modnooff_temp:
	jmp @@modnooff
@@modtoreg_temp:
	jmp @@modtoreg
@@continueafter:
	mov ax, rmvalue
	mov bx, 2
	mul bx
	mov bx, ax
	put dopbuffer, dopbuffer_length, '['
	mov bx, rmtable_mod10_01[bx]
	swrite dopbuffer, dopbuffer_length, [bx]
	mov bx, input_buffer_curr
	inc bx
	mov al, input_buffer[bx]
	inc bx
	mov ah, input_buffer[bx]
	mov input_buffer_curr, bx
	add ip_count, 2
	push ax
	hxdb ah
	put dopbuffer, dopbuffer_length, ah
	put dopbuffer, dopbuffer_length, al
	pop ax
	push ax
	hxdb al
	put dopbuffer, dopbuffer_length, ah
	put dopbuffer, dopbuffer_length, al
	put opbuffer, opbuffer_length, ah
	put opbuffer, opbuffer_length, ah
	pop ax
	hxdb ah
	put opbuffer, opbuffer_length, ah
	put opbuffer, opbuffer_length, al
	put dopbuffer, dopbuffer_length, ']'
	jmp @@exit
@@modnooff:
	mov ax, rmvalue
	mov bx, 2
	mul bx
	mov bx, ax
	put dopbuffer, dopbuffer_length, '['
	mov bx, rmtable_mod00[bx]
	cmp bx, offset nmNone
	je @@2byteaddrr
	swrite dopbuffer, dopbuffer_length, [bx]
	jmp @@noaddrr
@@2byteaddrr:
	call addaddr2bytes
@@noaddrr:
	put dopbuffer, dopbuffer_length, ']'
	jmp @@exit
@@modtoreg:
	mov ax, rmvalue
	mov bx, 2
	mul bx
	mov bx, ax
	cmp widvalue, 1
	je @@wordreg
	mov bx, rmtable_mod11_byte[bx]
	swrite dopbuffer, dopbuffer_length, rmtable_mod11_byte[bx]
	jmp @@exit
@@wordreg:
	mov ax, bx
	mov bx, 2
	mul bx
	mov bx, ax
	mov bx, rmtable_mod11_word[bx]
	swrite dopbuffer, dopbuffer_length, [bx]
@@exit:
	ret
by_mod endp
;======================================
addaddr2bytes proc
	mov bx, input_buffer_curr
	inc bx
	mov ah, input_buffer[bx]
	inc bx
	mov al, input_buffer[bx]
	mov input_buffer_curr, bx
	add ip_count, 2
	push ax
	hxdb al
	put dopbuffer, dopbuffer_length, ah
	put dopbuffer, dopbuffer_length, al
	pop ax
	push ax
	hxdb ah
	put dopbuffer, dopbuffer_length, ah
	put dopbuffer, dopbuffer_length, al
	put opbuffer, opbuffer_length, ah
	put opbuffer, opbuffer_length, al
	pop ax
	hxdb al
	put opbuffer, opbuffer_length, ah
	put opbuffer, opbuffer_length, al
	put dopbuffer, dopbuffer_length, 'h'
	ret
addaddr2bytes endp
;==================================
parse_bo_mov proc
	cmp widvalue, 1
	je @@word
	jmp @@byte
@@word:
	swrite dopbuffer, dopbuffer_length, nmWordPtr
	jmp @@parsebymod
@@byte:
	swrite dopbuffer, dopbuffer_length, nmBytePtr
@@parsebymod:
	call by_mod
	swrite dopbuffer, dopbuffer_length, separator
	cmp widvalue, 1
	je @@bobyte
	jmp @@boword
@@bobyte:
	mov bx, input_buffer_curr
	inc bx
	mov al, input_buffer[bx]
	mov input_buffer_curr, bx
	inc ip_count
	hxdb al
	put dopbuffer, dopbuffer_length, ah
	put dopbuffer, dopbuffer_length, al
	put opbuffer, opbuffer_length, ah
	put opbuffer, opbuffer_length, al
	jmp @@exit
@@boword:
	mov bx, input_buffer_curr
	inc bx
	mov ah, input_buffer[bx]
	inc bx
	mov al, input_buffer[bx]
	mov input_buffer_curr, bx
	add ip_count, 2
	push ax
	hxdb al
	put dopbuffer, dopbuffer_length, ah
	put dopbuffer, dopbuffer_length, al
	pop ax
	push ax
	hxdb ah
	put dopbuffer, dopbuffer_length, ah
	put dopbuffer, dopbuffer_length, al
	put opbuffer, opbuffer_length, ah
	put opbuffer, opbuffer_length, al
	pop ax
	hxdb al
	put opbuffer, opbuffer_length, ah
	put opbuffer, opbuffer_length, al
@@exit:
	ret
parse_bo_mov endp
;=================================
jmpcalllogic proc
	mov bx, input_buffer_curr
	mov al, input_buffer[bx]
	chkbit al, 7
	je @@mod1
	jmp @@02
@@mod1:
	add modvalue, 1
@@02:
	chkbit al, 6
	je @@mod2
	jmp @@findrm
@@mod2:
	add modvalue, 2
@@findrm:
	chkbit al, 2
	je @@rm1
	jmp @@r02
@@rm1:
	add rmvalue, 4
@@r02:
	chkbit al, 1
	je @@rm2
	jmp @@r03
@@rm2:
	add rmvalue, 2
@@r03:
	chkbit al, 0
	je @@rm3
	jmp @@logic
@@rm3:
	add rmvalue, 1
@@logic:
	mov ax, rmvalue
	mov bx, 2
	mul bx
	mov bx, ax
	cmp modvalue, mod00
	je @@nooff
	jmp @@001
@@nooff:
	lea si, rmtable_mod00[bx]
	mov si, [si]
	swrite dopbuffer, dopbuffer_length, [si]
	jmp @@endmod
@@001:
	cmp modvalue, mod11
	je @@onreg
	jmp @@002
@@onreg:
	cmp widvalue, 1
	je @@widereg
	jmp @@bytereg
@@widereg:
	lea si, rmtable_mod11_word[bx]
	jmp @@prreg
@@bytereg:
	lea si, rmtable_mod11_byte[bx]
@@prreg:
	mov si, [si]
	swrite dopbuffer, dopbuffer_length, [si]
	jmp @@exit
@@002:
	lea si, rmtable_mod10_01[bx]
	mov si, [si]
	swrite dopbuffer, dopbuffer_length, si[bx]
@@endmod:
	mov bx, input_buffer_curr
	inc bx
	mov ah, input_buffer[bx]
	inc bx
	mov al, input_buffer[bx]
	mov input_buffer_curr, bx
	push ax
	hxdb al
	put dopbuffer, dopbuffer_length, ah
	put dopbuffer, dopbuffer_length, al
	pop ax
	push ax
	hxdb ah
	put dopbuffer, dopbuffer_length, ah
	put dopbuffer, dopbuffer_length, al
	put opbuffer, opbuffer_length, ah
	put opbuffer, opbuffer_length, al
	pop ax
	hxdb al
	put opbuffer, opbuffer_length, ah
	put opbuffer, opbuffer_length, al
	add ip_count, 2
@@exit:
	ret
jmpcalllogic endp
;=====================================