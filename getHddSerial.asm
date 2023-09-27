.code
asmGetHddSerial proc	
	MOV AX, 6900h	
	MOV RDX,RCX
	MOV BL, 0
	INT 21h
	ret
asmGetHddSerial endp
end