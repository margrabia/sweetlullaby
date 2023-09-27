.code
asmGetNumberWordsInNumber proc
start:		
	MOV RAX, [RDX+RCX*8-8]
	CMP RAX, 0	
	JNZ notzero;
	DEC CL
	JNZ start;
	XOR RAX,RAX; // wynik jest zero bitow - czyli nic. Uznaje, ¿e bity liczy od 1
	ret
notzero:    
	MOV RAX,RCX;
	ret;	
asmGetNumberWordsInNumber endp
end