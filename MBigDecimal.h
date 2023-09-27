#pragma once
#include "framework.h"
#include <vector>
#define _CRT_SECURE_NO_WARNINGS 1 
extern "C" int asmIncBigInt(int length, INT64* a);
extern "C" int asmDecBigInt(int length, INT64* a);
#define maxWords 200


// exponent is based on 2.
// so the whole number equals: mantissa * 2^exponent
class MBigDecimal
{
private:
	CString sAdd(CString& a, CString& b);
	MBigDecimal PackBitToWords(std::vector<bool> bits);		
	//school multiplication method - for multiplying representations of sequences
	CString SchoolMultiplying(CString a, CString b);

public:
	INT64 mantissa[maxWords];	
	byte nOfWords;
	unsigned int accuracy;
	CString toStringDecimal();
	CString toStringHex();

	int exponent;
	bool sign; // true = > 0 false < 0


	void MoveRight(int noBits);
	void MoveLeft(int noBits);

	bool isBiggerThanZero()
	{
		if ( isZero()) return false;
		return sign;
	}

	bool isZero()
	{
		this->MoveToExp(0);
		this->refreshSize();

		if (this->nOfWords == 1 && mantissa[0] == 0)
			return true;
		return false;
	}

	bool isRawZero()
	{
		if (this->nOfWords == 1 && mantissa[0] == 0)
			return true;
		return false;
	}
	
	void ParsFromString(CString sValue);
	void ParsFromByteString(CString byteString);
	void refreshSize();
	void setAccuracy(unsigned int a_accuracy)
	{
		accuracy = a_accuracy;
	}

	void AllignDataToAccuracy(int accuracy);
	void AllignDataToAccuracy();	
	bool Compare(MBigDecimal& anotherNumber);
	void MoveToExp(int expValu);	
	MBigDecimal();
	MBigDecimal(byte a_nOfBytes);
	MBigDecimal(const MBigDecimal& copy)
	{
		nOfWords = copy.nOfWords;			
		sign = copy.sign;
		accuracy = copy.accuracy;
		exponent = copy.exponent;

		for (int i = 0; i < copy.nOfWords; i++)
		{
			mantissa[i] = copy.mantissa[i];
		}
	}
	void Inc();
	void Dec();
};

