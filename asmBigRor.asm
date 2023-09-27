.code
asmBigRor proc
; w RCX ilosc wszystkich slow
; w RDX - mamy ilosc bitow do przesuniecia w prawo
; R8 - Ÿród³o
; r9 - target
MOV R10, RCX; zabezpieczamy zawartosc RCX
MOV CL,6;
;dzielimy ja na 64 by wiedziec ile slow po prostu przepisac
;w bitach 0-5 mamy o ile calych swów przesun¹æ;
;w bitach 6-63 mamy o ile bitów przesun¹c poŸniej wszystko
ROR RDX,CL; wynik w RDX
MOV RAX,RDX;
MOV R11, 288230376151711743;
AND RAX, R11;
; wyciagamy z RDX czesc pozostala  18446744073709551552
MOV R11, 18158513697557839872
AND RDX, R11;
; rorujemy RDX w druga stronê 6 razy
MOV CL,6;
ROL RDX,CL;
; w RDX mamy konieczne przesunieci bitowe
;odtwarzamy poczatkowa wartosc RCX

SUB R10,RAX; odejmujemy od RCX przesuniecie w slowach
; przesuwamy cale slowa pamieci 
; zrodlo wskazywane jest przez R8. - dodajemy do niego przesuniecie
; zmniejszami licznik o przesuniecie z RAX
MOV CL,3; // razy 8 - bo tyle bajtow ma slowo 64 bitowe
; obliczamy adres zrodla
SHL RAX,CL;
ADD R8,RAX;
MOV RCX,R10;
loop1:
MOV RAX, [R8+RCX*8-8]
MOV [R9+RCX*8-8], RAX
DEC CL;
JNZ loop1;
; mamy zrobione. teraz pozostalo zrobic przesuniecie bitowe
; tworzymy maske wycinaj¹c¹ to co powinno wejœæ do m³odszego s³owa ze s³owa starszego
; na podstawie tego ile bitów jest do przesuniêcia co jest teraz w RDX
MOV CL,DL
MOV R11, 18446744073709551615
SHR R11,CL;
MOV R12,R11;
NOT R12;
; teraz w R11 mamy maske z zerami na najmniej znaczacych pozycjach
; a w R12 idwrotnosc tej maski
; teraz jadac od najstarszego s³owa w wyniku do obecnej dlugosci wyniku robimy ROR
XOR R14,R14; na razie zeruje R14
MOV RDX,R10;
loop2:
MOV RAX,[R9+RDX*8-8];
ROR RAX, cl;
MOV R13,RAX;
; wyciagamy czesc do przeniesienia do mlodszego slowa
AND RAX,R12; // RAX mamy teraz reszte z przesuniecia
AND R13,R11; w R13 mamy teraz faktycznie przesuniete slowo bez syfu
; dodaje do R13 wartosc z poprzedniej R14;
ADD R13,R14;
MOV [R9+RDX*8-8],R13;
MOV R14,RAX; nowa wartosc w R14
DEC RDX
JNZ loop2;
RET;
asmBigRor endp
end