#include "stdafx.h"
#include "BigInteger.h"


BigInteger::BigInteger(int a_sizeInBytes)
{
	worldLength = a_sizeInBytes;

	part = new INT64[a_sizeInBytes];
	// od razu ustawia zero
	for (int i = 0; i < a_sizeInBytes; i++)
	{
		part[i] = 0;
	}
}

BigInteger::~BigInteger()
{
	delete[] part;
}