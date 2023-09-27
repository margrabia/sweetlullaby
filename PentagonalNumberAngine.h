#pragma once
class PentagonalNumberAngine
{
	// klasa generujaca kolejne liczby pentagonalne typu 1
	// maksymalna liczba pentagonalna to 2^512
	//daje to 8 - 64 bitowe liczby
	// na razeie w celach testowych ogranicze sie do jednaj 64 bitowej
	unsigned __int64 number;
	unsigned __int64 secondFactor;
	unsigned __int16 modulo;

public:
	PentagonalNumberAngine()
	{
		secondFactor = 1;
		number = 1;
	}

	void computeNext()
	{
		secondFactor = secondFactor + 3;
		number = number + secondFactor;
	}

	unsigned __int64& getNumber()
	{
		return number;
	}

	// zwraca pentagonal number modulo 512
	// jest to po prostu ostatnie 9 bitów
	__int16 getModulo512()
	{
		return (((__int16)number) & 0x01ff);
	}

};


class ModuloCompute
{
	// tworzy tabele szybkich przejsc z dwoma parametrami - jeden liczba wejsciowa modulo: 0-511, druga wielokrotnosc kata - te¿ 0-511
	__int16 lookUp[512][512];

public:
	ModuloCompute()
	{
		int x, y;
		for (x = 0; x< 512; x++)
		{
			for (y = 0; y< 512; y++)
			{
				lookUp[x][y] = ((__int16)x*y) & 0x01ff;
			}
		}
	}

	__int16& getModuloFroMultiples(__int16 base, __int16 multipleFactor)
	{
		return lookUp[base][multipleFactor];
	}
};


class PentagonalNumberAngine2
{
	// klasa generujaca kolejne liczby pentagonalne typu 2
	// maksymalna liczba pentagonalna to 2^512
	//daje to 8 - 64 bitowe liczby
	// na razeie w celach testowych ogranicze sie do jednaj 64 bitowej
	unsigned __int64 number;
	unsigned __int64 secondFactor;
	unsigned __int16 modulo;

public:
	PentagonalNumberAngine2()
	{
		secondFactor = 2;
		number = 2;
	}

	void computeNext()
	{
		secondFactor = secondFactor + 3;
		number = number + secondFactor;
	}

	unsigned __int64& getNumber()
	{
		return number;
	}

	// zwraca pentagonal number modulo 512
	// jest to po prostu ostatnie 9 bitów
	__int16 getModulo512()
	{
		return (((__int16)number) & 0x01ff);
	}

};

//zastosowanie:

//for (int i = 0; i<400; i++)
//{
//	pentNumber = pentManager.getNumber();
//	pentModulo = pentManager.getModulo512();
//	pentManager.computeNext();
//
//	// dla danego pentagonal number biorê modulo dla wszystkich wielokrotnosci czestotliwosci
//	for (unsigned __int16 m = 0; m< 512; m++)
//	{
//		moduloForMultiple = modComp.getModuloFroMultiples(pentModulo, m);
//	}
//
//	pentNumber = pentManager2.getNumber();
//	pentModulo = pentManager2.getModulo512();
//	pentManager2.computeNext();
//
//	// dla danego pentagonal number biorê modulo dla wszystkich wielokrotnosci czestotliwosci
//	for (unsigned __int16 m = 0; m< 512; m++)
//	{
//		moduloForMultiple = modComp.getModuloFroMultiples(pentModulo, m);
//	}
//
//
//	int s = 0;
//	s++;
//}


// klasa liczaca 512 cosinusow i sinusów od 0 - 2*pi

class TriFunManager
{
	// oprócz wartosci 512 cosinusow i sinusów

	//mamy tablele
};