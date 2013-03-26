#include "stdafx.h"
#include "explorer.h"
#include "windowlist.h"
#include <vector>
#include <iostream>

class COMNess
{
public:
    COMNess()
    {
        VERIFY(CoInitialize(NULL));
    }

    ~COMNess()
    {
        CoUninitialize();
    }
};

std::vector<std::wstring> getArgs(int argc, WCHAR* argv[])
{
    std::vector<std::wstring> args;
    for(int i = 0; i < argc; i++)
    {
        args.push_back(argv[i]);
    }
    return args;
}


int _tmain(int argc, WCHAR* argv[])
{

    try
    {
        auto args = getArgs(argc, argv);
        
        if(args.size() == 2)
        {
            auto arg = args[1];
            if((arg == L"-h") || (arg == L"--help") || (arg == L"/h"))
            {
                std::wcout << L"Prints the location of the topmost Windows Explorer window to STDOUT";
                return 2;
            }
        }

        COMNess com;

        auto windows = getOrderedWindows();
        auto explorerInfo = getExplorerPath(windows);

        std::wcout << explorerInfo.path;
    }
    catch(Exception e)
    {
        e.print();
        return 1;
    }
    
	return 0;
}

