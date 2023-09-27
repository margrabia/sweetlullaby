.code
asmMulBy2BigInt proc
start:
	JNC bezprzepelnienia
	SAL QWORD PTR[RDX+RCX*8],1
	INC QWORD PTR[RDX+RCX*8]
	DEC CL
	JNZ start	
	ret
bezprzepelnienia:
	SAL QWORD PTR[RDX+RCX*8],1
	DEC CL
	JNZ start	
	ret
asmMulBy2BigInt endp
end