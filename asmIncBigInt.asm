.code
asmIncBigInt proc	
	XOR RAX,RAX;
start:  			
	INC QWORD PTR[RDX + RAX * 8]
	JNZ nocarry;
	INC RAX;	
	DEC CL
	JNZ start
	;bylo przeniesienie - powinien dodac jeszcze jedno slowo na koniec
	MOV R9,1;
	MOV [RDX + RAX * 8],R9;	
nocarry:
	INC RAX; w RAX zwracamy ilosc slow dodanych
	ret
asmIncBigInt endp
end