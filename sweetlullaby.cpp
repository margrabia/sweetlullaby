// sweetlullaby.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "framework.h"
#include "sweetlullaby.h"
#include "Math.h"
#include <conio.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);
	CString a;
    if (hModule != nullptr)
    {
        // initialize MFC and print and error on failure
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: code your application's behavior here.
            wprintf(L"Fatal Error: MFC initialization failed\n");
            nRetCode = 1;
        }
        else
        {
			MBigDecimal product;
			//102233 x 3002981 = 20700240073
			//20003297 x 30003509 = 600169101569173
			//30003509 x 50003659 = 1500285232839431
			//30003509 x 5000004067 =    150017667024271103
			//50000005127 x 5000004067 =    250000228985020851509
			
			product.ParsFromString("250000228985020851509");
			int countGlobal = 0;
			DWORD dw1 = GetTickCount();
			MBigDecimal multiplier = Math::FindMultiplicand( product, countGlobal );
			DWORD dw2 = GetTickCount();
			CString sMultiplayer = multiplier.toStringDecimal();
			cout << "Multiplayer is: "<< sMultiplayer<<endl;
			cout << "time operation (ms) is: " << (dw2-dw1);
			_getch();
            // TODO: code your application's behavior here.
        }
    }
    else
    {
        // TODO: change error code to suit your needs
        wprintf(L"Fatal Error: GetModuleHandle failed\n");
        nRetCode = 1;
    }

    return nRetCode;
}
