#include "Math.h"

extern "C" INT64 asmGetNoBits(INT64 bytes, INT64* result);
extern "C" INT64 asmGetNumberWordsInNumber(INT64 bytes, INT64* result);

//Multiplying two BigDecimal numbers
extern "C" void asmMulBigBigInt(int length, INT64* a, INT64* b, INT64* result);

//Adding two number. First = 64b*length1, second 64b*length2
//length >= length2
extern "C" void asmAddBigIntExp(int length, int length2, INT64* a, INT64* b, INT64* result);

//Substract two number first = 64b*length1, second 64b*length2
extern "C" void asmSubBigIntExp(int length1, int length2, INT64* a, INT64* b, INT64* result);

//Argument a should have one word reserved extra! 
//return number of incremented words with carry
extern "C" int asmIncBigInt(int length, INT64* a ); 

//quickly compares the number of bits a and b
//return:
//0 - equal
//2 - a has more bits then b
//1 - a has less bits then b
extern "C" int asmCompare(int length, INT64* a, INT64* b);


bool ComputationProcessorForInitialMode::computeOneStep(MBigDecimal& product, MBigDecimal& b, MBigDecimal& c, MBigDecimal& d, int& steps) 
{
	MBigDecimal number = product;
	bool isFirst = true;
	int count = 0;

	MBigDecimal three(1);
	three.mantissa[0] = 3;

	MBigDecimal two(1);
	two.mantissa[0] = 2;

	MBigDecimal one(1);
	MBigDecimal nothing(1);
	one.mantissa[0] = 1;
	int localCount = 0;
	b = Math::Sqrt(number, localCount, false, nothing, false);
	count += localCount;

	localCount = 0;
	c = Math::Divide(number, b, localCount, true);
	count += localCount;
	c.MoveToExp(0);

	MBigDecimal bcTemp = Math::Multiply(c, b);
	bcTemp.MoveToExp(0);

	if (Math::Compare(bcTemp, number) > 0)
	{
		--c;
	}

	MBigDecimal bc = Math::Multiply(b, c);
	bc.MoveToExp(0);
	d = Math::Sub(number, bc);

	steps += count;
	return true;
}



bool ComputationProcessorForPreprocessingMode::computeOneStep(MBigDecimal& product, MBigDecimal& b, MBigDecimal& c, MBigDecimal& d, int& steps) 
{
	if (d.isZero())
	{
		return true;
	}
	if (Math::Compare(d, c) > 0)
	{
		// count jump distance
		MBigDecimal cMul2 = c;
		cMul2.exponent = 1;
		cMul2.MoveToExp(0);

		MBigDecimal dMinus2c = Math::Sub(d, cMul2);
		//if (d - 2 * c > 0)
		if (dMinus2c.isBiggerThanZero())
		{
			int count = 0;
			MBigDecimal nJump = Math::Divide(d, c, count, true);
			steps += count;
			nJump.MoveToExp(0);

			// sprawdzamy czy nie jest czasem o jeden za du¿o z powodu zaokr¹glania - lepiej zrobic tak ni¿ bardzo dokladnie dzielic
			MBigDecimal nJumpMulc = Math::Multiply(nJump, c);

			if (Math::Compare(nJumpMulc, d) > 0)
			{
				--nJump;
			}
			else
			{
				// trzeba sprawdziæ czy czasem nie jest za ma³o - rzadko to sie zdarza, ale 
				//blisko szukanej liczby moze
				//MBigDecimal nPrzeskokMulCTmp = nPrzeskokMulc;
				//int incmullocal = 0;
				nJumpMulc = Math::Add(nJumpMulc, c);
				while (Math::Compare(nJumpMulc, d) < 0)
				{
					++nJump;
					nJumpMulc = Math::Add(nJumpMulc, c);
				}
			}
			b = Math::Add(b, nJump);
		}
		else
		{
			++b;
		}

		MBigDecimal bc = Math::Multiply(b, c);
		bc.MoveToExp(0);
		d = Math::Sub(product, bc);
	}
	if (d.isZero())
	{
		return true;
	}
	return false;
}

bool ComputationProcessorForMode1::computeOneStep(MBigDecimal& number, MBigDecimal& b, MBigDecimal& c, MBigDecimal& d, int& steps) 
{
	int count = 0;
	int countLocal = 0;
	MBigDecimal sqrDelta(1);
	MBigDecimal three(1);
	three.mantissa[0] = 3;
	MBigDecimal nothing(1);
	MBigDecimal four(1);
	four.mantissa[0] = 4;

	MBigDecimal a1 = Math::Sub(b, c);
	++a1;

	MBigDecimal a1Min4 = Math::Sub(four, a1);

	//long a1 = b - c + 1;
	//long a1Min4 = (4 - a1);
	MBigDecimal deltaFirstPart = Math::Sub(d, b);
	deltaFirstPart = Math::Sub(deltaFirstPart, a1);
	++deltaFirstPart;
	++deltaFirstPart;

	MBigDecimal a1Min4Pov2 = Math::Multiply(a1Min4, a1Min4);
	a1Min4Pov2.MoveToExp(0);

	MBigDecimal deltaFirstPartMul4 = deltaFirstPart;
	deltaFirstPartMul4.exponent = 2;
	deltaFirstPartMul4.MoveToExp(0);

	MBigDecimal delta = Math::Sub(a1Min4Pov2, deltaFirstPartMul4);

	//long delta = a1Min4 * a1Min4 - 4 * deltaFirstPart;
	if (steps == 0)
	{
		sqrDelta = Math::Sqrt(delta, countLocal, false, nothing, false);
	}
	else
		sqrDelta = Math::Sqrt(delta, countLocal, false, a1, true);

	steps += countLocal;
	MBigDecimal  testkw = Math::Multiply(sqrDelta, sqrDelta);


	MBigDecimal nJump = Math::Add(a1Min4, sqrDelta);
	
	nJump.exponent = -1;
	nJump.MoveToExp(0);

	b = Math::Add(b, nJump);
	--b;

	countLocal = 0;
	//tylko w modzie zero musi na nowo policzyæ.
	c = Math::Divide(number, b, countLocal, true);
	steps += countLocal;
	c.MoveToExp(0);

	// sprawdzamy czy nie jest czasem o jeden za du¿o z powodu zaokr¹glania - lepiej zrobic tak ni¿ bardzo dokladnie dzielic
	MBigDecimal cb = Math::Multiply(c, b);

	if (Math::Compare(cb, number) > 0)
	{
		--c;
	}
	else
	{
		// trzeba sprawdziæ czy czasem nie jest za ma³o - rzadko to sie zdarza, ale 
		//blisko szukanej liczby moze
		MBigDecimal cTmp = Math::Add(cb, b);
		while (Math::Compare(cTmp, number) < 0)
		{
			++c;
			cTmp = Math::Add(cb, b);
		}
	}

	MBigDecimal bc = Math::Multiply(c, b);
	d = Math::Sub(number, bc);

	//c = (long)number / b;
	//d = number - c * b;			

	if (Math::Compare(nJump, four) < 0)
	{
		return true;
	}
	return false;
}


bool ComputationProcessorForMode2::computeOneStep(MBigDecimal& number, MBigDecimal& b, MBigDecimal& c, MBigDecimal& d, int& steps) 
{
	MBigDecimal one(1);
	one.mantissa[0] = 1;

	MBigDecimal a1 = Math::Sub(b, c);
	++a1;
	//long a1 = b - c + 1;

	MBigDecimal del(1);
	MBigDecimal dtmp;

	do
	{
		d = Math::Add(d, a1);
		d = Math::Add(d, del);

		++del;
		++del;

		++b;
		//b++;
		c = Math::Sub(c, one);
		//c--;

		if (Math::Compare(d, b) == 0)
		{
			return true;
		}
		dtmp = Math::Add(d, a1);
		dtmp = Math::Add(dtmp, del);
	} while (Math::Compare(dtmp, b) < 0);
	return false;
}





Math::Math()
{
}


int Math::Compare(MBigDecimal& a, MBigDecimal& b)
{
	if (a.exponent < b.exponent)
	{
		b.MoveToExp(a.exponent);
	}
	else
	{
		a.MoveToExp(b.exponent);
	}

	a.refreshSize();
	b.refreshSize();

	if (a.nOfWords < b.nOfWords)
	{
		return -1;
	}
	else
	{
		if (a.nOfWords == b.nOfWords)
		{
			// fast compares number of bits 			
			int cmp = asmCompare(a.nOfWords, a.mantissa, b.mantissa);
			if (cmp == 2)
			{
				return 1;
			}
			else
			{
				if (cmp == 1)
				{
					return -1;
				}
				else
				{
					return 0;
				}
			}
		}
		else
		{
			return 1;
		}
	}


}

MBigDecimal Math::Sub(MBigDecimal& a, MBigDecimal& b)
{
	//equates the exponents of numbers		
	if (a.exponent < b.exponent)
	{
		b.MoveToExp(a.exponent);
	}
	else
	{
		a.MoveToExp(b.exponent);
	}

	a.refreshSize();
	b.refreshSize();

	bool changeSign = false;
		
	if (a.sign == b.sign)
	{	
		if (a.nOfWords < b.nOfWords)
		{
			changeSign = true;
		}
		else
		{
			if (a.nOfWords == b.nOfWords)
			{
				// check number of significant bits
				if (asmCompare(a.nOfWords, a.mantissa, b.mantissa)==1)
				{
					changeSign = true;
				}
			}
		}		

		if (changeSign)
		{
			MBigDecimal result(b.nOfWords);
			asmSubBigIntExp(b.nOfWords, a.nOfWords, b.mantissa, a.mantissa, result.mantissa);
			result.sign = !a.sign;
			result.refreshSize();

			if (result.nOfWords == 1 && result.mantissa[0] == 0)
			{
				// zero
				result.sign = true;
				return result;
			}

			result.exponent = a.exponent;
			return result;
		}
		else
		{
			MBigDecimal result(a.nOfWords);
			asmSubBigIntExp(a.nOfWords, b.nOfWords, a.mantissa, b.mantissa, result.mantissa);
			result.sign = a.sign;
			result.refreshSize();

			if (result.nOfWords == 1 && result.mantissa[0] == 0)
			{
				// zero
				result.sign = true;
				return result;
			}
			result.exponent = a.exponent;
			return result;
		}
	}
	else
	{		
		if (a.nOfWords < b.nOfWords)
		{
			MBigDecimal result(b.nOfWords+1);

			asmAddBigIntExp(b.nOfWords, a.nOfWords, b.mantissa, a.mantissa, result.mantissa);
			result.refreshSize();
			result.sign = a.sign;

			if (result.nOfWords == 1 && result.mantissa[0] == 0)
			{				
				result.sign = true;
				return result;
			}			
			result.exponent = a.exponent;
			return result;
		}
		else
		{
			MBigDecimal result(a.nOfWords+1);
			
			asmAddBigIntExp(a.nOfWords, b.nOfWords, a.mantissa, b.mantissa, result.mantissa);
			result.refreshSize();
			result.sign = a.sign;
			
			if (result.nOfWords == 1 && result.mantissa[0] == 0)
			{
				// zero
				result.sign = true;
				return result;
			}

			result.exponent = a.exponent;
			return result;
		}
	
	}
}


MBigDecimal Math::Add(MBigDecimal& a, MBigDecimal& b)
{
	if (a.exponent < b.exponent)
	{
		b.MoveToExp(a.exponent);
	}
	else
	{
		a.MoveToExp(b.exponent);
	}

	a.refreshSize();
	b.refreshSize();

	// Adding if the same signs 
	// if not substracting
	if (a.sign == b.sign)
	{
		if (a.nOfWords < b.nOfWords)
		{
			MBigDecimal result(b.nOfWords);
			asmAddBigIntExp(b.nOfWords, a.nOfWords, b.mantissa, a.mantissa, result.mantissa);
			result.refreshSize();
			result.sign = a.sign;

			if (result.nOfWords == 1 && result.mantissa[0] == 0)
			{
				// zero
				result.sign = true;
				return result;
			}
			result.exponent = a.exponent;
			return result;
		}
		else
		{
			MBigDecimal result(a.nOfWords);
			asmAddBigIntExp(a.nOfWords, b.nOfWords, a.mantissa, b.mantissa, result.mantissa);
			result.refreshSize();
			result.sign = a.sign;
			
			if (result.nOfWords == 1 && result.mantissa[0] == 0)
			{
				// zero
				result.sign = true;
				return result;
			}
			result.exponent = a.exponent;
			return result;
		}
	}
	else
	{
		bool changeSign = false;
		
		if (a.nOfWords < b.nOfWords)
		{
			changeSign = true;
		}
		else
		{
			if (a.nOfWords == b.nOfWords)
			{			
				if (asmCompare(a.nOfWords, a.mantissa, b.mantissa)==1)
				{
					changeSign = true;
				}
			}
		}

		if (changeSign)
		{
			MBigDecimal result(b.nOfWords);
			asmSubBigIntExp(b.nOfWords, a.nOfWords, b.mantissa, a.mantissa, result.mantissa);
			result.refreshSize();
			result.sign = b.sign;

			if (result.nOfWords == 1 && result.mantissa[0] == 0)
			{
				// zero
				result.sign = true;
				return result;
			}
			result.exponent = a.exponent;
			return result;
		}
		else
		{
			MBigDecimal result(a.nOfWords);
			asmSubBigIntExp(a.nOfWords, b.nOfWords, a.mantissa, b.mantissa, result.mantissa);
			result.refreshSize();
			result.sign = a.sign;
			if (result.nOfWords == 1 && result.mantissa[0] == 0)
			{
				// zero
				result.sign = true;
				return result;
			}
			result.exponent = a.exponent;
			return result;
		}
	}	
}



// dividing by 2^x
MBigDecimal Math::DiveByTwoPovX(MBigDecimal number, int x)
{
	number.exponent -= x;
	return number;
}
// multiplying by 2^x
MBigDecimal Math::MulByTwoPovX(MBigDecimal number, int x)
{
	number.exponent += x;
	return number;
}

//returns number of bits including exponent
// 2^(result) > number
int Math::getNoOfBit(MBigDecimal& number)
{
	// zwraca ilosc bitów ale uwzglêdniaj¹c eksponent;
	// czyli tak¹ liczbê bit, ¿e 2^(bit) > number
	return asmGetNoBits(number.nOfWords, number.mantissa);
}

int Math::getNumberWordsInNumber(MBigDecimal& number)
{	
	return asmGetNumberWordsInNumber(number.nOfWords, number.mantissa);
}

//Multyplying two MBigDecimal
MBigDecimal Math::Multiply(MBigDecimal& a, MBigDecimal& b)
{	
	if (a.nOfWords == 1 && a.mantissa[0] == 0)
		return MBigDecimal(1);

	int aNumberOfBits = Math::getNoOfBit(a);
	int bNumberOfBits = Math::getNoOfBit(b);
	
	int multiplyNumberOfBits = aNumberOfBits + bNumberOfBits;	
	INT64 numberOfWords = multiplyNumberOfBits / 64;
	
	if (numberOfWords * 64 != multiplyNumberOfBits)
		numberOfWords++;

	MBigDecimal amantissa(numberOfWords);
	MBigDecimal bmantissa(numberOfWords);

	int i;
	for (i = 0; i < a.nOfWords; i++)
	{
		amantissa.mantissa[i] = a.mantissa[i];
	}

	for (i = 0; i < b.nOfWords; i++)
	{
		bmantissa.mantissa[i] = b.mantissa[i];
	}		

	MBigDecimal result(numberOfWords);

	asmMulBigBigInt(numberOfWords, amantissa.mantissa, bmantissa.mantissa, result.mantissa);


	result.refreshSize();
	result.exponent = a.exponent + b.exponent;

	if (a.sign == b.sign)
		result.sign = true;
	else
		result.sign = false;

	return result;
}

MBigDecimal Math::Divide(MBigDecimal& number, MBigDecimal& divider, int &count, bool isReal )
{
	bool sign = true;
	if (number.sign != divider.sign)
		sign = false;

	number.sign = true;
	divider.sign = true;

	//D - Divider	
	number.refreshSize();
	divider.refreshSize();	
	int nOfDividerBits = Math::getNoOfBit(divider)+divider.exponent;
	divider.refreshSize();

	//double nOfDividerBitsPlus = 10 * (nOfDividerBits / 10);	
	double nOfDividerBitsPlus = 1.26 * nOfDividerBits;
	int remasteredAcucuracy = 10 + (int) nOfDividerBitsPlus;

	//int remasteredAcucuracy = nOfDividerBits + nOfDividerBitsPlus;
	// x(i+1) = x(i) *( 2 - D*x(i))
	// finding reciprocal of D

	MBigDecimal mem;
	MBigDecimal x;		

	x.exponent = -nOfDividerBits;
	x.mantissa[0] = 1;  // 1* 2^(-nOfDividerBits)
	x.nOfWords = 1;

	count = 0;
	while (true)
	{		
		MBigDecimal product1 = Math::Multiply(x, divider);
		
		product1.setAccuracy( remasteredAcucuracy );
		product1.AllignDataToAccuracy();
	
		MBigDecimal Two;
		Two.mantissa[0] = 2;	
		Two.nOfWords = 1;
		Two.MoveToExp(product1.exponent);

	
		MBigDecimal difference = Math::Sub(Two, product1);
		
		x = Math::Multiply(x, difference);

		x.setAccuracy(remasteredAcucuracy);
		x.AllignDataToAccuracy();

		count++;
		if (x.Compare(mem))
		{
			//Normalisation due to 0 exponent			
			if (isReal)
			{
				MBigDecimal result = Math::Multiply(x, number);	
				result.sign = sign;
				return result;
			}
			else
			{
				MBigDecimal result = Math::Multiply(x, number);
				result.sign = sign;
				if (result.exponent >= 0)
				{
					result.MoveToExp(-1);
					//test last bit
					int firstbit = result.mantissa[0] & 1;
					
					result.MoveToExp(0);
					if (firstbit)
						result.Inc();
				}				
				return result;
			}
		}		
		mem = x;
	}

}

MBigDecimal Math::Sqrt2(MBigDecimal& number, int  &count, bool isRoundUp, MBigDecimal& StartNumber, bool isFirstStep )
{
	number.refreshSize();
	MBigDecimal one(1);
	one.mantissa[0] = 1;

	MBigDecimal three(1);
	three.mantissa[0] = 3;

	int numberMantisaNOfBits = Math::getNoOfBit(number);
	double remasteredAcucuracyD = numberMantisaNOfBits * 1.45;
	int remasteredAcucuracy = remasteredAcucuracyD;

	int nOfBits = numberMantisaNOfBits + number.exponent;
	int nOfWord = number.nOfWords;
	int nOfBitsSqr = nOfBits / 2;

	int localCount=0;
	MBigDecimal invertNumber = Math::Divide(one, number, localCount);

	count += localCount;
	invertNumber.setAccuracy(remasteredAcucuracy);
	invertNumber.AllignDataToAccuracy();

	if (nOfBitsSqr * 2 != nOfBits)
	{
		nOfBitsSqr++;
	}

	MBigDecimal x(2 + nOfWord / 2);// with some margin

	if (!isFirstStep)
	{
		x.mantissa[0] = 1;// 1*2^nOfBitsSqr;
		x.MoveLeft(nOfBitsSqr);
	}
	else
	{
		x = StartNumber;
	}


	//calculate the initial inverse of the approximate root, i.e. y

	MBigDecimal y = Math::Divide(one, x, localCount);
	count += localCount;	
	y.setAccuracy(remasteredAcucuracy);
	y.AllignDataToAccuracy();

	MBigDecimal numberMinusOne = number;
	numberMinusOne.Dec();

	MBigDecimal numberMulNumberMinusOne = Math::Multiply(number, numberMinusOne);

	MBigDecimal comparedAccuracy = Math::Divide(one, numberMulNumberMinusOne, localCount);
	comparedAccuracy.setAccuracy(remasteredAcucuracy);
	comparedAccuracy.AllignDataToAccuracy();

	count += localCount;

	// meat
	while (true)
	{
		MBigDecimal yPow2 = Math::Multiply(y, y);
		yPow2.setAccuracy(remasteredAcucuracy);
		yPow2.AllignDataToAccuracy();

		MBigDecimal xMulyPow2 = Math::Multiply(x, yPow2);
		
		xMulyPow2.setAccuracy(remasteredAcucuracy);
		xMulyPow2.AllignDataToAccuracy();

		MBigDecimal threeWithAccuracy = three;

		threeWithAccuracy.setAccuracy(remasteredAcucuracy);
		threeWithAccuracy.AllignDataToAccuracy();

		MBigDecimal threeMinusxMulyPow2 = Math::Sub(threeWithAccuracy, xMulyPow2);
		MBigDecimal denominator = Math::Multiply(y, threeMinusxMulyPow2);

		// dividing denominator per 2
		denominator.exponent--;
		y = denominator;

		y.setAccuracy(remasteredAcucuracy);
		y.AllignDataToAccuracy();

		// Check if ok
		MBigDecimal subYFromBase = Math::Sub(yPow2, invertNumber);
		if (Math::Compare(subYFromBase, comparedAccuracy) < 0)
		{						
			MBigDecimal result = Math::Divide(one, y, count);

			count += localCount;
			result.MoveToExp(0);
			// testing result by fast multiplying
			MBigDecimal xPov2 = Math::Multiply(result, result);

			// if not enought inc result;
			if (isRoundUp && !xPov2.Compare(number))
			{
				result.Inc();
			}
			return result;
		}
	}
}



//assume that the square root result is an integer
//square roots using the Bakhshali method
MBigDecimal Math::Sqrt(MBigDecimal& number, int  &count, bool isRoundUp, MBigDecimal& StartNumber, bool isFirstStep )
{	
	number.refreshSize();
	int numberMantisaNOfBits = Math::getNoOfBit(number);
	double remasteredAcucuracyD = numberMantisaNOfBits * 1.45;
	int remasteredAcucuracy = remasteredAcucuracyD;


	int nOfBits = numberMantisaNOfBits + number.exponent;
	int nOfWord = number.nOfWords;

	int nOfBitsSqr = nOfBits / 2;
	
	if (nOfBitsSqr * 2 != nOfBits)
	{
		nOfBitsSqr++;
	}


	MBigDecimal x(2 + nOfWord/2);// with some margin
	if (!isFirstStep)
	{
		x.mantissa[0] = 1;// 1*2^nOfBitsSqr;
		x.MoveLeft(nOfBitsSqr); // initial x
	}
	else
	{
		x = StartNumber;		
	}
	//S > x^2
	count = 0;
	int localcount = 0;


	while (true)
	{
		if (count > 1000)
			return MBigDecimal();

		MBigDecimal xTo2 = Math::Multiply(x, x);				
		MBigDecimal xTo2Test = xTo2;
		xTo2Test.MoveToExp(-1);
		//testing last bit 
		int firstbit = xTo2Test.mantissa[0] & 1;		
		xTo2Test.MoveToExp(0);	
		if (firstbit)
			xTo2Test.Inc();

		xTo2Test.refreshSize();
		if (xTo2Test.nOfWords == number.nOfWords)
		{
			if (xTo2Test.Compare(number))
			{
				// found
				x.MoveToExp(0);
				// testing
				MBigDecimal xPov2 = Math::Multiply(x, x);// jedno mno¿enie wiêcejxPov2

				//increasing x if necessary
				if (isRoundUp && !xPov2.Compare(number))
				{
					x.Inc();
				}
				return x; 				
			}
		}

		MBigDecimal xMul2 = x;
		xMul2.MoveLeft(1); 
	
		MBigDecimal numberTmp = number;
				
		MBigDecimal minus_a_denominator = Math::Sub(numberTmp, xTo2);

		minus_a_denominator.AllignDataToAccuracy(remasteredAcucuracy);
		xMul2.AllignDataToAccuracy(remasteredAcucuracy);

		MBigDecimal a = Math::Divide(minus_a_denominator, xMul2, localcount, true);
		count += localcount;
			
		MBigDecimal aPow2 = Math::Multiply(a, a); //(-a)*(-a) = a^2								
		MBigDecimal  b = Math::Add(x, a);				
		MBigDecimal bMul2 = b;
		bMul2.MoveLeft(1); // b*2

		MBigDecimal aPow2Div2b = Math::Divide(aPow2, bMul2, localcount, true); //Maximum accuracy
		count += localcount;		
		x = Math::Sub(b, aPow2Div2b);		
		x.AllignDataToAccuracy(remasteredAcucuracy); // wanted accuracy
	}

	return x;
}

MBigDecimal Math::FindMultiplicand(MBigDecimal& product, int& countGlobal, bool isSqrFast)
{
	Processorfactory::MODE mode = Processorfactory::MODE::initial;
	countGlobal = 0;
	MBigDecimal b = 0;
	MBigDecimal c = 0;
	MBigDecimal d = 0;
	int preprocesingCount = 0;
	Processorfactory::getProcessor(mode)->computeOneStep(product, b, c, d, preprocesingCount);
	int maxAddToMul = 0;
	MBigDecimal sqrDelta(1);
	MBigDecimal four(1);
	four.mantissa[0] = 4;
	mode = Processorfactory::MODE::first;

	do
	{

		if (Processorfactory::getProcessor(Processorfactory::MODE::preprocessing)->computeOneStep(product, b, c, d, preprocesingCount))
		{
			countGlobal += preprocesingCount;
			return b;
		}

		if (Processorfactory::getProcessor(mode)->computeOneStep(product, b, c, d, countGlobal))
		{
			if (mode == Processorfactory::MODE::first)
				mode = Processorfactory::MODE::second;
			else
			{
				countGlobal += preprocesingCount;
				return b;
			}
		}		
	} while (!d.isZero() );	

	countGlobal += preprocesingCount;
	return b;
}