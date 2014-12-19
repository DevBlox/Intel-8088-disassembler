.model small

buferioDydis	EQU	121

;.stack 100h

;*******************Perkelta � kodo segmento pabaig�************
;.data
;	bufDydis DB  buferioDydis
;	nuskaite DB  ?
;	buferis	 DB  buferioDydis dup ('$')
;	ivesk	 DB  'Iveskite eilute:', 13, 10, '$'
;	rezult	 DB  'Radau tiek didziuju raidziu: '
;	rezult2	 DB  3 dup (' ')
;	enteris	 DB  13, 10, '$'
;***************************************************************
			
;*************************Pakeista******************************
;.code
BSeg SEGMENT
;***************************************************************

;*******************Prid�ta*************************************
	ORG	100h
	ASSUME ds:BSeg, cs:BSeg, ss:BSeg
;***************************************************************

Pradzia:
;	MOV	ax, @data
;	MOV	ds, ax

	db 11000011b ; ret
	db 11000010b, 32h, 21h ;ret 2132h
	db 11001011b ; ret
	db 11001010b, 98h, 76h ; ret 7698h

;****nuskaito eilute****
	MOV	ah, 9
	MOV	dx, offset ivesk
	INT	21h

	MOV	ah, 0Ah
	MOV	dx, offset bufDydis
	INT	21h

	MOV	ah, 9
	MOV	dx, offset enteris
	INT	21h

;****algoritmas****
	XOR	ch, ch
	SUB	ax, ax
	MOV	cl, nuskaite
	MOV	bx, offset buferis
	MOV	dl, 'A'
	MOV	dh, 'Z'

ciklas1:
	CMP	dl, [ds:bx]
	JG	nelygu
	CMP	dh, [ds:bx]
	JL	nelygu
	INC	ax

nelygu:
	INC	bx
	DEC	cx
	CMP	cx, 0
	JG	ciklas1

;****Spausdinimas****
	MOV	dl, 10
	DIV	dl
	MOV	[rezult2 + 2], ah
	ADD	[rezult2 + 2], 030h
	XOR	ah, ah
	DIV	dl
	MOV	[rezult2 + 1], ah
	ADD	[rezult2 + 1], 030h
	MOV	[rezult2], al
	ADD	[rezult2], 030h

	MOV	ah, 9
	MOV	dx, offset rezult
	INT	21h

	MOV	ah, 4Ch
	MOV	al, 0
	INT	21h

;*******************Atkelta i� duomen� segmento*****************
	bufDydis DB  buferioDydis
	nuskaite DB  ?
	buferis	 DB  buferioDydis dup ('$')
	ivesk	 DB  'Iveskite eilute:', 13, 10, '$'
	rezult	 DB  'Radau tiek didziuju raidziu: '
	rezult2	 DB  3 dup (' ')
	enteris	 DB  13, 10, '$'
;***************************************************************

;*******************Prid�ta*************************************
BSeg ENDS
;***************************************************************
END	Pradzia		