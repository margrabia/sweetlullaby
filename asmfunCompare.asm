.code
asmCompare proc	
; RCX - ilosc slow
; RDX - adres pierwszej liczby
; R8 - adres drugiej liczby
; zaczyna od najstarszej
start:
MOV RAX,[RDX + RCX * 8-8]
CMP RAX,[R8 + RCX * 8-8]
JC mniejsze
JNZ wieksze
DEC CL;
JNZ start;
; rowne
XOR RAX,RAX;
ret;
wieksze:
MOV RAX,2;
ret;
mniejsze: 
; wynik 1 
MOV RAX,1
ret;
asmCompare endp
end