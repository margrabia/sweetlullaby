# sweetlullaby
64-bit algorithm for factoring the product of two prime numbers

This program was created to test the algorithm for factoring the product of two prime numbers. 
It operates on floating-point numbers with an exponent in base two. 
It contains fast operations of 64-bit multiplication, addition, subtraction, significant bit conversion in assembly language, 
as well as fast division and square root operations in C++ using the Bakhshali method or finding its inverse using Newton's method.

Agoritm: program starts in mode1 and after reaching some value of indicator turn to mode2 - which stay faster then mode1 from specific point.
Mode 1:
Lets take number N which is the product of two prime number.
To find this prime factors lets assign b1 = SQRT(N) (without the fraction part) as a starting point.
So now we can create eqation:
N/b1 = c1 + d1/b1 - where c1,d1,b1 are natural numbers.
Assume that bn = b1+n.
increasing b1 by one we have b2 = b1+1 (and now assuming that d2 didn't overflow (is still less than b2 ) we find that c2 = c1-1;
So we can write:
N/b2 = c2 + d2/b2   
Now lets find how many increasing steps should be done to make dn overflow (it will be less than in (n-1)  step)
N = b1*c1 + d1. 
N = bn*cn + dn
where bn = b1+n and cn = c1-n.
So:
N = (b1+n)*(c1-n) + dn where dn = bn+1 (moment of overflow) = b1+n+1
comparing this equations for N we get:
(b1+n)*(c1-n)+b1+n+1 = b1*c1+d1
finaly we get:
n^2 + n*(b1-c1-1) + (d1-b1-1) = 0
So algoritm compute n as a root of a quadratic equation to find how long jump (n) should be done to find next bn.
After that we just need to ensure if bn devide N without the rest.
If there is any rest then we take this bn as new b1 and repeat seqence with this value b1

t was experimentally established that mode2 should be activated when n (jump) is less than 4.

Mode 2:

In experimental way If n (jump) is less than four 





