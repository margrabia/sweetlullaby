;   multiply two long number
;	rdi -- address of #1 (long number)
;	rsi -- address of #2 (long number)
;	rcx -- length of long numbers in qwords
;	mod equals 2 ^ (rcx * 64)
; result:
;	mul = (#1 * #2) % mod is written to rdi

.code
asmMulBigBigInt proc
mul_long_long:
mov			R12,RCX
mov			R13,R9

mov         rdi, rdx
mov         rsi, r8
sub     	rsp, 128 * 8  ; get place for result
mov     	rbx, rdx      ; store rdi
mov     	rdi, rsp
call    	set_zero      ; result = 0
mov     	rdi, rbx      ; get rdi
mov     	rbx, rsp

main_loop:
mov		r9, rcx
mov		r10, rsi
mov		r11, rbx

xor     	rdx, rdx
xor     	r8, r8

looop:
add[rbx], r8; add carry
xor     	r8, r8
adc     	r8, 0

add[rbx], rdx			; high part from last mul
adc     	r8, 0

mov     	rax, [rsi]
mov     	rdx, [rdi]
mul     	rdx

add[rbx], rax			; add low part from current mul
adc     	r8, 0

lea     	rbx, [rbx + 8]	; shift addresses
lea     	rsi, [rsi + 8]
dec     	rcx
jnz         looop
lea     	rdi, [rdi + 8]
mov		rcx, r9
mov		rsi, r10
mov		rbx, r11
lea     	rbx, [rbx + 8]
dec     	rcx
jnz         main_loop

mov         rdx, rsp

sub     	rbx, 128 * 8
mov     	rcx, 128
add     	rsp, 128 * 8

mov			R9,R13
mov			RCX,R12


; przepisujemy wynik na wyjœcie
; wynik jest w rejestrach RSP
sub			rdx,8;
;sub			r9,8;
my_copy_loop:

MOV RAX,[RDX+RCX*8]
MOV[R9+RCX*8-8],RAX
DEC CL
JNZ my_copy_loop

ret

set_zero:
push            rax
push            rdi
push            rcx
xor             rax, rax
rep stosq
pop             rcx
pop             rdi
pop             rax
ret
asmMulBigBigInt endp
end