.code
asmGetNoBits proc
start:		
	MOV RAX, [RDX+RCX*8-8]
	CMP RAX, 0	
	JNZ notzero;
	DEC CL
	JNZ start;
	XOR RAX,RAX; // wynik jest zero bitow - czyli nic. Uznaje, ¿e bity liczy od 1
	ret
notzero:
    ; mnozy to co zostalo z Cl razy 64 i dodatje BSR RAX,RCX
	DEC RCX;
	BSR RAX,[RDX+RCX*8] ; testuje pierwszy znaczacy bit liczac
	INC RAX	; dodaje jeden zeby w mysl konwencji, ze zerowy bit jest 1
	SHL RCX,6;
	ADD RAX,RCX;
	ret;	
asmGetNoBits endp
end
