.code
asmDecBigInt proc	
	XOR RAX,RAX;
	MOV R8,1;
start:  			
	SBB [RDX + RAX * 8],R8;
	JNC noborrow;
	;DEC QWORD PTR[RDX + RAX * 8]
	;JZ noborrow;
	INC RAX;	
	;XOR R8,R8;
	DEC CL
	JNZ start		
noborrow:
	INC RAX; w RAX zwracamy ilosc slow dodanych
	ret
asmDecBigInt endp
end