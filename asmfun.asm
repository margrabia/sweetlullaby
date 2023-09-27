.code
asmAdd proc
	adc rcx,rdx
	mov rax,rcx
	jc przeniesienie
	mov rcx, 0
	mov[r8],rcx
	ret
przeniesienie:	
	mov rcx, 1
	mov[r8],rcx
	ret
asmAdd endp
end