.code
asmSubBigIntExp proc
    MOV R10,QWORD PTR [RSP+40] ; // ostatni parametr na stosie	
	PUSHF
	XOR R12,R12
	DEC RDX; // zmniejszenie o jeden RDX - czyli jak mamy 1 bajt to tu jest 0	
start:
; RCX - dlugosc dlugiego argumentu, RDX - dlugosc krotszego argumentu, R8 adres dlu¿szego, R9 - adres wdrugiego, stos - adres wyniku	    	
	MOV RAX, [R8+R12*8]
	XOR R11,R11
	CMP R12,RDX
	JG there ; skok jeœli wiêksze
	MOV R11, [R9+R12*8]		
there:
    POPF
	SBB RAX,R11; na wypadek po¿yczki
	MOV[R10+R12*8],RAX
	PUSHF
	INC R12
	DEC CL
	JNZ start
	POPF
	ret
asmSubBigIntExp endp
end