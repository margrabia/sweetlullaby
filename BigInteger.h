#pragma once
#include "stdafx.h"

//extern "C" INT64 asmAdd(INT64 a, INT64 b);

class BigInteger
{
	
public:
	INT64* part;
	int worldLength;

	BigInteger( int a_sizeInBytes);
	~BigInteger();
	//INT64 asmAdd(INT32 a, INT32 b)
	//{
	//	__asm {
	//		/*mov rax, a;
	//		mov rdx, b;
	//		add rcx, rdx;
	//		ret;*/
	//		/*add rcx, rdx;*/
	//		mov eax, a;
	//		mov ecx, b;
	//		adc ecx;
	//		ret;
	//	}
	//}

	void Add( BigInteger argNumber )
	{
		// sumowanie inline
		
	}


};



