comFin equ 0
comRbyte equ 1
comRword equ 2
comRnull equ 3
comNotFin equ 4
comBalbo equ 5
comBclbo equ 6
comBdlbo equ 7
comBblbo equ 8
comBahbo equ 9
comBbhbo equ 10
comBchbo equ 11
comBdhbo equ 12
comWaxbo equ 13
comWcxbo equ 14
comWdxbo equ 15
comWbxbo equ 16
comWspbo equ 17
comWbpbo equ 18
comWsibo equ 19
comWdibo equ 20
comes equ 21
comcs equ 22
comss equ 23
comds equ 24
comax equ 25
comcx equ 26
comdx equ 27
combx equ 28
comsp equ 29
combp equ 30
comsi equ 31
comdi equ 32
comR2word equ 33

nmMov db 'mov $'
nmPush db 'push $'
nmPop db 'pop $'
nmAdd db 'add $'
nmInc db 'inc $'
nmSub db 'sub $'
nmDec db 'dec $'
nmCmp db 'cmp $'
nmMul db 'mul $'
nmDiv db 'div $'
nmCall db 'call $'
nmRet db 'ret $'
nmJmp db 'jmp $'
nmLoop db 'loop $'
nmInt db 'int $'
nmJo db 'jo $'
nmJno db 'jno $'
nmJnae db 'jnae $'
nmJae db 'jae $'
nmJe db 'je $'
nmJne db 'jne $'
nmJbe db 'jbe $'
nmJa db 'ja $'
nmJs db 'js $'
nmJns db 'jns $'
nmJp db 'jp $'
nmJnp db 'jnp $'
nmJl db 'jl $'
nmJge db 'jge $'
nmJle db 'nmJle'
nmJg db 'jg $'
nmJcxz db 'jcxz $'

nmAx db 'ax$'
nmBx db 'bx$'
nmCx db 'cx$'
nmDx db 'dx$'
nmSp db 'sp$'
nmBp db 'bp$'
nmSi db 'si$'
nmDi db 'di$'
nmAl db 'al$'
nmBl db 'bl$'
nmCl db 'cl$'
nmDl db 'dl$'
nmAh db 'ah$'
nmBh db 'bh$'
nmCh db 'ch$'
nmDh db 'dh$'

nmBxSi db 'bx + si$'
nmBxDi db 'bx + di$'
nmBpSi db 'bp + si$'
nmBpDi db 'bp + di$'
nmBxSiP db 'bx + si + $'
nmBxDiP db 'bx + di + $'
nmBpSiP db 'bp + si + $'
nmBpDiP db 'bp + di + $'
nmSiP db 'si + $'
nmDiP db 'di + $'
nmBpP db 'bp + $'
nmBxP db 'bx + $'

bit1_num dw 139 ; Number of bit 1 versions
;================================================
; 1st bit of every instruction
;================================================
bit1 db 088h, 089h, 08Ah, 08Bh, 08Eh, 08Ch, 0A0h
	 db 0A1h, 0A2h, 0A3h, 0B0h, 0B1h, 0B2h, 0B4h
	 db 0B3h, 0B5h, 0B6h, 0B7h, 0B8h, 0B9h, 0BAh
	 db 0BCh, 0BBh, 0BDh, 0BEh, 0BFh, 0C6h, 0C7h
	 db 006h, 00Eh, 016h, 01Eh, 050h, 051h, 052h
	 db 054h, 053h, 055h, 056h, 057h, 0FFh, 007h
	 db 00Fh, 017h, 01Fh, 058h, 059h, 05Ah, 05Ch
	 db 05Bh, 05Dh, 05Eh, 05Fh, 08Fh, 000h, 002h
	 db 003h, 001h, 004h, 005h, 080h, 081h, 082h
	 db 083h, 040h, 041h, 042h, 044h, 043h, 045h
	 db 046h, 046h, 0FEh, 0FFh, 028h, 02Ah, 029h
	 db 02Bh, 02Ch, 02Dh, 080h, 081h, 082h, 083h
	 db 0FEh, 0FFh, 048h, 049h, 04Ah, 04Ch, 04Bh
	 db 04Dh, 04Eh, 04Fh, 038h, 039h, 03Ah, 03Bh
	 db 03Ch, 03Dh, 080h, 081h, 082h, 083h, 0F6h
	 db 0F7h, 0F6h, 0F7h, 09Ah, 0E8h, 0FFh, 0FFh
	 db 0C2h, 0C3h, 0E9h, 0EAh, 0EBh, 0FFh, 0FFh
	 db 0E2h, 0CDh, 070h, 071h, 072h, 073h, 074h
	 db 075h, 076h, 077h, 078h, 079h, 07Ah, 07Bh
	 db 07Ch, 07Dh, 07Eh, 07Fh, 0E3h, 033h
;================================================
; Instruction completion tables
;================================================
ope1 db comNotFin, comNotFin, comNotFin, comNotFin
	 db comNotFin, comNotFin, comRbyte, comRword
	 db comRbyte, comRword, comBalbo, comBclbo
	 db comBdlbo, comBahbo, comBblbo, comBclbo
	 db comBdhbo, comBblbo, comWaxbo, comWcxbo
	 db comWdxbo, comWspbo, comWbxbo, comWbpbo
	 db comWsibo, comWdibo, comNotFin, comNotFin
	 db comes, comcs, comss, comds
	 db comax, comcx, comdx, comsp
	 db combx, combp, comsi, comdi
	 db comNotFin, comes, comcs, comss
	 db comds, comax, comcx, comdx
	 db comsp, combx, comdi, comsi
	 db comdi, comNotFin, comNotFin, comNotFin
	 db comNotFin, comNotFin, comBalbo, comWaxbo
	 db comNotFin, comNotFin, comNotFin, comNotFin
	 db comax, comcx, comdx, comsp
	 db combx, combp, comsi, comdi
	 db comNotFin, comNotFin, comNotFin, comNotFin
	 db comNotFin, comNotFin, comBalbo, comWaxbo
	 db comNotFin, comNotFin, comNotFin, comNotFin
	 db comNotFin, comNotFin, comax, comcx
	 db comdx, comsp, combx, combp
	 db comsi, comdi, comNotFin, comNotFin
	 db comNotFin, comNotFin, comBalbo, comWaxbo
	 db comNotFin, comNotFin, comNotFin, comNotFin
	 db comNotFin, comNotFin, comNotFin, comNotFin
	 db comR2word, comRword, comNotFin, comNotFin
	 db comRword, comFin, comRword, comR2word
	 db comRbyte, comNotFin, comNotFin, comRbyte
	 db comRbyte, comRbyte, comRbyte, comRbyte
	 db comRbyte, comRbyte, comRbyte, comRbyte
	 db comRbyte, comRbyte, comRbyte, comRbyte
	 db comRbyte, comRbyte, comRbyte, comRbyte
	 db comRbyte, comRbyte, comRbyte
;================================================
; Instruction name table
;================================================	 
opnm dw offset nmMov, offset nmMov, offset nmMov, offset nmMov
	 dw offset nmMov, offset nmMov, offset nmMov, offset nmMov
	 dw offset nmMov, offset nmMov, offset nmMov, offset nmMov
	 dw offset nmMov, offset nmMov, offset nmMov, offset nmMov
	 dw offset nmMov, offset nmMov, offset nmMov, offset nmMov
	 dw offset nmMov, offset nmMov, offset nmMov, offset nmMov
	 dw offset nmMov, offset nmMov, offset nmMov, offset nmMov
	 dw offset nmPush, offset nmPush, offset nmPush, offset nmPush
	 dw offset nmPush, offset nmPush, offset nmPush, offset nmPush
	 dw offset nmPush, offset nmPush, offset nmPush, offset nmPush
	 dw offset nmPush, offset nmPop, offset nmPop, offset nmPop
	 dw offset nmPop, offset nmPop, offset nmPop, offset nmPop
	 dw offset nmPop, offset nmPop, offset nmPop, offset nmPop
	 dw offset nmPop, offset nmPop, offset nmAdd, offset nmAdd
	 dw offset nmAdd, offset nmAdd, offset nmAdd, offset nmAdd
	 dw offset nmAdd, offset nmAdd, offset nmAdd, offset nmAdd
	 dw offset nmInc, offset nmInc, offset nmInc, offset nmInc
	 dw offset nmInc, offset nmInc, offset nmInc, offset nmInc
	 dw offset nmInc, offset nmInc, offset nmSub, offset nmSub
	 dw offset nmSub, offset nmSub, offset nmSub, offset nmSub
	 dw offset nmSub, offset nmSub, offset nmSub, offset nmSub
	 dw offset nmDec, offset nmDec, offset nmDec, offset nmDec
	 dw offset nmDec, offset nmDec, offset nmDec, offset nmDec
	 dw offset nmDec, offset nmDec, offset nmCmp, offset nmCmp
	 dw offset nmCmp, offset nmCmp, offset nmCmp, offset nmCmp
	 dw offset nmCmp, offset nmCmp, offset nmCmp, offset nmCmp
	 dw offset nmMul, offset nmMul, offset nmDiv, offset nmDiv
	 dw offset nmCall, offset nmCall, offset nmCall, offset nmCall
	 dw offset nmRet, offset nmRet, offset nmJmp, offset nmJmp
	 dw offset nmJmp, offset nmJmp, offset nmJmp, offset nmLoop
	 dw offset nmInt, offset nmJo, offset nmJno, offset nmJnae
	 dw offset nmJae, offset nmJe, offset nmJne, offset nmJbe
	 dw offset nmJa, offset nmJs, offset nmJns, offset nmJp
	 dw offset nmJnp, offset nmJl, offset nmJge, offset nmJle
	 dw offset nmJg, offset nmJcxz, 0 ; Missing table entry?
	 
	 