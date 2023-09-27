.code
asmMulBy3BigInt proc
	MOV R9,RCX
start:
	JNC bezprzepelnienia
	SAL QWORD PTR[RDX + RCX * 8], 1
	INC QWORD PTR[RDX + RCX * 8]
	DEC CL
	JNZ start
	jmp startdodawanie
bezprzepelnienia:
	SAL QWORD PTR[RDX + RCX * 8], 1
	DEC CL
	JNZ start
startdodawanie:
	MOV RCX,R9
startdodawanie2:	
	MOV R10, [R8 + RCX*8]	
	ADC QWORD PTR[RDX + RCX*8],R10
	DEC CL
	JNZ startdodawanie2	
	ret
asmMulBy3BigInt endp
end