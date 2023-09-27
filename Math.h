#pragma once
#include "MBigDecimal.h"


class IComputationProcessor
{
public:
	virtual bool computeOneStep(MBigDecimal& number, MBigDecimal& b, MBigDecimal& c, MBigDecimal& d, int& steps) = 0;
};

class ComputationProcessorForInitialMode : public IComputationProcessor
{
public:
	virtual bool computeOneStep(MBigDecimal& product, MBigDecimal& b, MBigDecimal& c, MBigDecimal& d, int& steps) override;	
};

class ComputationProcessorForPreprocessingMode : public IComputationProcessor
{
public:
	virtual bool computeOneStep(MBigDecimal& product, MBigDecimal& b, MBigDecimal& c, MBigDecimal& d, int& steps) override;
};

class ComputationProcessorForMode1 : public IComputationProcessor
{	
public:
	virtual bool computeOneStep(MBigDecimal& number, MBigDecimal& b, MBigDecimal& c, MBigDecimal& d, int& steps) override;
};

class ComputationProcessorForMode2 : public IComputationProcessor
{
public:
	virtual bool computeOneStep(MBigDecimal& number, MBigDecimal& b, MBigDecimal& c, MBigDecimal& d, int& steps) override;
};

static class Processorfactory
{	
public:
	enum MODE {
		initial = 0,
		preprocessing,
		first,
		second
	};

	static  std::shared_ptr<IComputationProcessor> getProcessor(MODE mode)
	{
		switch (mode)
		{
			case initial:
				return std::dynamic_pointer_cast<IComputationProcessor>(std::make_shared<ComputationProcessorForInitialMode>());
			case preprocessing:
				return std::dynamic_pointer_cast<IComputationProcessor>(std::make_shared<ComputationProcessorForPreprocessingMode>());
			case first:
				return std::dynamic_pointer_cast<IComputationProcessor>(std::make_shared<ComputationProcessorForMode1>());
			case second:
				return std::dynamic_pointer_cast<IComputationProcessor>(std::make_shared<ComputationProcessorForMode2>());
			default:
				return std::dynamic_pointer_cast<IComputationProcessor>(std::make_shared<ComputationProcessorForInitialMode>());
		}
	}

};


static class Math
{
private:
	static int InitialComputation(MBigDecimal& product, MBigDecimal& b, MBigDecimal& c, MBigDecimal& d);
public:	
	Math();		
	static MBigDecimal DiveByTwoPovX(MBigDecimal number, int x);
	static MBigDecimal MulByTwoPovX(MBigDecimal number, int x);
	static int getNoOfBit(MBigDecimal& number);
	static int getNumberWordsInNumber(MBigDecimal& number);
	static MBigDecimal Multiply(MBigDecimal& a, MBigDecimal& b);
	static MBigDecimal Divide(MBigDecimal& number, MBigDecimal& divider,int  &counter, bool isReal = false);
	static MBigDecimal Sqrt(MBigDecimal& number, int  &counter, bool isRoundUp, MBigDecimal& StartNumber, bool isFirstStep = false);
	static MBigDecimal Sqrt2(MBigDecimal& number, int  &count, bool isRoundUp, MBigDecimal& StartNumber, bool isFirstStep);
	static MBigDecimal Sub(MBigDecimal& a, MBigDecimal& b);
	static MBigDecimal Add(MBigDecimal& a, MBigDecimal& b);
	static int Compare(MBigDecimal& a, MBigDecimal& b);	
	static MBigDecimal FindMultiplicand(MBigDecimal& product, int& countGlobal, bool isSqrFast = false);	
};

