#include "Math.h"
#include <cmath>

extern "C" void asmMulBigBigInt(int length, INT64* a, INT64* b, INT64* wynik);
extern "C" void asmBigRor(INT64 iloscSlow, INT64 przesuniecie, INT64* src, INT64* target);
extern "C" void asmBigRol(INT64 iloscSlow, INT64 przesuniecie, INT64* src, INT64* target);

CString MBigDecimal::sAdd(CString& a, CString& b)
{
	CString sOut;
	int bLen = b.GetLength();
	int aLen = a.GetLength();

	int minLen;

	if (bLen < aLen)
	{
		CString sTmp = b;
		b = a;
		a = b;
		aLen = a.GetLength();
		bLen = b.GetLength();
	}

	int del = bLen - aLen;
	int carry = 0;
	for (int i = aLen - 1; i <= 0; i--)
	{
		int digitA = a.GetAt(i) - '0';
		int digitB = b.GetAt(i + del) - '0';
		int sum = digitB + digitA + carry;
		carry = sum / 10;
		int digitOut = sum - carry * 10;
		sOut.AppendFormat(_T("%d"), digitOut);
	}

	//rest of b
	for (int i = del - 1; i <= 0; i++)
	{
		int digitB = b.GetAt(i) - '0';

		int sum = digitB + carry;
		carry = sum / 10;
		int digitOut = sum - carry * 10;
		sOut.AppendFormat(_T("%d"), digitOut);
	}
	sOut = sOut.MakeReverse();
	return sOut;
}


MBigDecimal MBigDecimal::PackBitToWords(std::vector<bool> bits)
{
	//most significant bits first 
	int nOfBits = bits.size();
	byte nOfWords = nOfBits / 64;
	if (nOfWords * 64 < nOfBits)
		nOfWords++;
	MBigDecimal num(nOfWords);

	std::vector<bool>::iterator iBitIterator;
	for (iBitIterator = bits.begin(); iBitIterator != bits.end(); iBitIterator++)
	{
		num.exponent = 1;
		// format the number as having an exponent of 0 - increases the mantissa
		num.MoveToExp(0);
		if (*iBitIterator)
		{
			num.mantissa[0] = num.mantissa[0] | 1;
		}
	}
	return num;
}

CString MBigDecimal::SchoolMultiplying(const CString a, const CString b)
{
	// if one of the components is 0 returns 0		
	if (a.FindOneOf(_T("123456789")) == -1)
		return _T("0");
	if (b.FindOneOf(_T("123456789")) == -1)
		return _T("0");

	std::vector<CString> rows;
	std::vector<CString>::iterator rowsIterator;
	CString leadingZeros = _T("");

	for (int i = a.GetLength() - 1; i >= 0; i--)
	{
		int aDigit = a.GetAt(i) - '0';
		CString row;
		row += leadingZeros;
		int mem = 0; // decimal carry

		for (int j = b.GetLength() - 1; j >= 0; j--)
		{
			int bDigit = b.GetAt(j) - '0';
			int ab = aDigit * bDigit + mem;
			CString simpleResultAB;
			simpleResultAB.Format(_T("%d"), ab);
			int digitOut;
			mem = ab / 10;
			digitOut = ab - mem * 10;
			row.AppendFormat(_T("%d"), digitOut);
		}

		if (mem > 0)
		{
			row.AppendFormat(_T("%d"), mem);
		}
		rows.push_back(row);
		leadingZeros += "0";
	}

	rowsIterator = rows.end();
	rowsIterator--;
	int maxLenght = rowsIterator->GetLength();

	//addition of items
	int carry = 0;
	CString result;
	for (int col = 0; col < maxLenght; col++)
	{
		int sum = 0;
		for (rowsIterator = rows.begin(); rowsIterator != rows.end(); rowsIterator++)
		{
			if (rowsIterator->GetLength() > col)
				sum += rowsIterator->GetAt(col) - '0';
		}

		sum += carry;

		carry = sum / 10;
		int digitOut = sum - 10 * carry;

		result.AppendFormat(_T("%d"), digitOut);
	}

	if (carry > 0)
		result.AppendFormat(_T("%d"), carry);

	result = result.MakeReverse();
	return result;
}

// to do - create more efficient method for parsing string to 64b based MBigDecimal
void MBigDecimal::ParsFromString(const CString sValue)
{
	// estimate number of bits		
	// wsp - proportion of size between binary and decimal number representation
	double wsp = 3.321928094872935397460708447895298357516432603791965364674;
	int bits = (int)((double)wsp * sValue.GetLength());
	bits++;
	int nWords = bits / 64;
	if (nWords * 64 < bits)
		nWords++;

	MBigDecimal suma(nWords);
	std::vector<bool> bity;
	// all bits to false
	for (int i = 0; i < bits; i++)
	{
		bity.push_back(false);
	}

	MBigDecimal testNumber;

	std::vector<bool>::iterator iBits;

	//set/reset bits and test if ASCII representation is bigger or less than sValue
	//most significiant bits first.

	for (iBits = bity.begin(); iBits != bity.end(); iBits++)
	{
		*iBits = true;
		testNumber = PackBitToWords(bity);
		CString sTestNumner = testNumber.toStringDecimal();
		int cmp = sTestNumner.Compare(sValue);
		if (cmp == 0)
		{
			*this = testNumber;
			return;
		}
		if (cmp > 0)
			*iBits = false;
	}

	*this = testNumber;
	return;
}

void MBigDecimal::ParsFromByteString(const CString byteString)
{
	int pos = 0;
	std::vector<CString> byteToken;
	std::vector<CString>::iterator iterator;

	while (true)
	{
		CString sToken = byteString.Tokenize(_T("|"), pos);
		if (!sToken.IsEmpty())
		{
			byteToken.push_back(sToken);
		}
		else
		{
			break;
		}
	}
	int numberOfBytes = byteToken.size();
	int numberOf64Words = numberOfBytes / 8;
	if (numberOf64Words * 8 < numberOfBytes)
	{
		numberOf64Words++;
	}

	nOfWords = numberOf64Words;
	unsigned long long word = 0;
	unsigned long long multiplication = 256;
	unsigned long long multiplicationAll = 1;

	unsigned int b;
	int bNo = 0;
	int wNo = 0;

	for (iterator = byteToken.begin(); iterator != byteToken.end(); iterator++)
	{
		if (iterator->IsEmpty())
			break;

		CStringA sTest = CStringA(*iterator);
		sscanf_s((const char*)sTest, "%d", &b);
		word += b * multiplicationAll;
		bNo++;
		if (bNo == 8)
		{
			multiplicationAll = 1;
			this->mantissa[wNo] = word;
			bNo = 0;
			wNo++;
			word = 0;
		}
		else
		{
			multiplicationAll = multiplicationAll * multiplication;
		}

	}
	if (word > 0)
	{
		this->mantissa[wNo] = word;
	}
	this->refreshSize();
}

bool MBigDecimal::Compare(const MBigDecimal& anotherNumber)
{
	if (nOfWords < anotherNumber.nOfWords)
	{
		for (int i = 0; i < nOfWords; i++)
		{
			if (mantissa[i] != anotherNumber.mantissa[i])
			{
				return false;
			}
		}
	}
	else
	{
		for (int i = 0; i < anotherNumber.nOfWords; i++)
		{
			if (mantissa[i] != anotherNumber.mantissa[i])
			{
				return false;
			}
		}
	}
	return true;
}

void MBigDecimal::Inc()
{
	if (isRawZero())
	{
		sign = true;
	}

	//assumes that mantisa has enough size reserved		

	if (sign)
	{
		int wordsWithCarry = asmIncBigInt(nOfWords, mantissa);
		if (wordsWithCarry > nOfWords)
			nOfWords = wordsWithCarry;
	}
	else
	{
		int wordsWithCarry = asmDecBigInt(nOfWords, mantissa);
		refreshSize();
		if (isRawZero())
		{
			sign = true;
		}
	}
}

void MBigDecimal::Dec()
{
	if (isRawZero())
	{
		int wordsWithCarry = asmIncBigInt(nOfWords, mantissa);
		sign = false;
	}

	if (sign)
	{
		int wordsWithCarry = asmDecBigInt(nOfWords, mantissa);
		if (wordsWithCarry > nOfWords)
			nOfWords = wordsWithCarry;
	}
	else
	{
		int wordsWithCarry = asmIncBigInt(nOfWords, mantissa);
		if (wordsWithCarry > nOfWords)
			nOfWords = wordsWithCarry;
	}

}

//convert number to it's string representation
CString MBigDecimal::toStringDecimal()
{		
	CString TwoTo32 = _T("4294967296");
	CString TwoTo32Pov = _T("1");
	INT64 twoPow32 = 4294967296;
	std::vector<CString> rows;

	for (int wordNo = 0; wordNo < nOfWords; wordNo++)
	{
		CString word32S;
		CString word32MultiplyTwoTo32Pov;

		unsigned long long wordTested = mantissa[wordNo];
		// split 64b to 32bit parts
		unsigned long lWord;
		unsigned long HWord;

		HWord = wordTested / twoPow32;
		lWord = wordTested - twoPow32 * HWord;

		word32S.Format(_T("%u"), lWord);

		word32MultiplyTwoTo32Pov = SchoolMultiplying(word32S, TwoTo32Pov);
		word32MultiplyTwoTo32Pov = word32MultiplyTwoTo32Pov.MakeReverse();
		rows.push_back(word32MultiplyTwoTo32Pov);

		TwoTo32Pov = SchoolMultiplying(TwoTo32Pov, TwoTo32);
		
		word32S.Format(_T("%u"), HWord);

		word32MultiplyTwoTo32Pov = SchoolMultiplying(word32S, TwoTo32Pov);
		word32MultiplyTwoTo32Pov = word32MultiplyTwoTo32Pov.MakeReverse();
		rows.push_back(word32MultiplyTwoTo32Pov);
		TwoTo32Pov = SchoolMultiplying(TwoTo32Pov, TwoTo32);
	}

	std::vector<CString>::iterator rowsIterator;

	int maxLenght = 0;
	for (rowsIterator = rows.begin(); rowsIterator != rows.end(); rowsIterator++)
	{
		if (rowsIterator->GetLength() > maxLenght)
			maxLenght = rowsIterator->GetLength();
	}
	
	//Adding
	int carry = 0;
	CString wynik;
	for (int col = 0; col < maxLenght; col++)
	{
		int sum = 0;
		for (rowsIterator = rows.begin(); rowsIterator != rows.end(); rowsIterator++)
		{
			if (rowsIterator->GetLength() > col)
				sum += rowsIterator->GetAt(col) - '0';
		}

		sum += carry;		
		carry = sum / 10;
		int cyfra = sum - 10 * carry;

		wynik.AppendFormat(_T("%d"), cyfra);
	}

	if (carry > 0)
		wynik.AppendFormat(_T("%d"), carry);
	
	wynik = wynik.MakeReverse();
	return wynik;
}

//To do.
CString toStringHex()
{
	return CString();
}

void MBigDecimal::AllignDataToAccuracy(int a_accuracy)
{
	accuracy = a_accuracy;
	AllignDataToAccuracy();
}

void MBigDecimal::AllignDataToAccuracy()
{	
	int numberOfBits  = Math::getNoOfBit(*this);
	if (numberOfBits > accuracy)
	{
		INT64 delToMove = numberOfBits - accuracy;		
		INT64 numberOfWords = Math::getNumberWordsInNumber(*this);				
		MBigDecimal result(numberOfWords);
		
		asmBigRor(numberOfWords, delToMove, mantissa, result.mantissa);
		
		for (int i = 0; i < numberOfWords; i++)
		{
			mantissa[i] = result.mantissa[i];
		}
		
		this->nOfWords = Math::getNumberWordsInNumber(*this);
		exponent += delToMove;
	}
}

// change exponent
void MBigDecimal::MoveToExp(int expValu)
{	
	// number of bits to shift value left
	INT64 delToMove = expValu - exponent;
	if (delToMove == 0)
		return;

	if (delToMove < 0)
	{		
		INT64 totalBitsAfterOperation = Math::getNoOfBit(*this) - delToMove;
		INT64 totalWordsAfterOperation =  totalBitsAfterOperation / 64;
		if (totalWordsAfterOperation * 64 != totalBitsAfterOperation)
			totalWordsAfterOperation++;	
		MBigDecimal result(totalWordsAfterOperation);		
		INT64 srcNumberOfWords = this->nOfWords;
		asmBigRol(srcNumberOfWords, -delToMove, mantissa, result.mantissa);		
		for (int i = 0; i < totalWordsAfterOperation; i++)
		{
			mantissa[i] = result.mantissa[i] ;
		}
		
		this->nOfWords = totalWordsAfterOperation;
		exponent += delToMove;		
	}
	else
	{
		// shifting bits to the right
		INT64 totalBitsAfterOperation = Math::getNoOfBit(*this) - delToMove;
		INT64 totalWordsAfterOperation = totalBitsAfterOperation / 64;
		if (totalWordsAfterOperation * 64 != totalBitsAfterOperation)
			totalWordsAfterOperation++;
		
		MBigDecimal oUt(totalWordsAfterOperation);		
		INT64 srcNumberOfWords = this->nOfWords;

		asmBigRor(srcNumberOfWords, delToMove, mantissa, oUt.mantissa);
		
		for (int i = 0; i < totalWordsAfterOperation; i++)
		{
			mantissa[i] = oUt.mantissa[i];
		}
		this->nOfWords = totalWordsAfterOperation;
		exponent += delToMove;
	}
}


void MBigDecimal::refreshSize()
{
	byte nOfWordsWithZero = Math::getNumberWordsInNumber(*this);
	if (nOfWordsWithZero > 0)
		nOfWords = nOfWordsWithZero;
	else
		nOfWords = 1;// minimum
}

MBigDecimal::MBigDecimal()
{
	nOfWords = maxWords;

	exponent = 0;
	sign = true;
	for (int i = 0; i < nOfWords; i++)
	{
		mantissa[i] = 0;
	}
}

MBigDecimal::MBigDecimal(byte a_nOfWords)
{
	nOfWords = a_nOfWords;			
	exponent = 0;
	sign = true;
	for (int i = 0; i < nOfWords; i++)
	{
		mantissa[i] = 0;
	}
}

void MBigDecimal::MoveRight(int noBits)
{
	refreshSize();
	MBigDecimal wynik(this->nOfWords);
	asmBigRor(nOfWords, noBits, mantissa, wynik.mantissa);
	wynik.refreshSize();
	for (int i = 0; i < wynik.nOfWords; i++)
	{
		mantissa[i] = wynik.mantissa[i];
	}
	nOfWords = wynik.nOfWords;
}


void MBigDecimal::MoveLeft(int noBits)
{
	refreshSize();
	int adWords = noBits / 64;

	MBigDecimal wynik(this->nOfWords + 1 + adWords);
	asmBigRol(nOfWords, noBits, mantissa, wynik.mantissa);

	wynik.refreshSize();
	nOfWords = wynik.nOfWords;
	for (int i = 0; i < wynik.nOfWords; i++)
	{
		mantissa[i] = wynik.mantissa[i];
	}
}
