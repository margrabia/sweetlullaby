.code
asmSubBigInt proc
start:
	MOV RAX, [RDX+RCX*8-8]
	MOV R10, [R8+RCX*8-8]
	SBB RAX,R10
	MOV[R9+RCX*8-8],RAX
	DEC CL
	JNZ start
	ret
asmSubBigInt endp
end