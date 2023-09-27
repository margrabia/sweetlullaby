.code
asmAddBigIntExp proc
    MOV R10,QWORD PTR [RSP+40] ; // ostatni parametr na stosie		
	XOR R13,R13;
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
    ADD RAX,R13; // przeniesienie
	XOR R13,R13;// zeruje
	ADC RAX,R11; na wypadek przeniesienia
	JNC nieprzepelnienie
	; jesli przepelnienie to musi dodaæ jeden do nastêpnego bajtu
	INC R13;
	; w R13 jest jeden
nieprzepelnienie: 
	; w R13 jest zero
	MOV[R10+R12*8],RAX			
	INC R12
	DEC CL
	JNZ start
	; jesli jest przepelnienie to dochodzi dodatkowy bajt do wyniku rowny 1
	MOV[R10+R12*8],R13;
	ret
asmAddBigIntExp endp
end