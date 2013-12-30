#include "stdafx.h"
#include "explorer.h"
#include "windowlist.h"
#include <vector>
#include <iostream>
#include "browsefolder.h"

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

int findCurrentExplorerLocation()
{
    COMNess com;

    auto windows = getOrderedWindows();
    auto explorerInfo = getExplorerPath(windows);

    std::wcout << explorerInfo.path;

    return 0;
}

int usage()
{
    std::wcout << L"Prints the location of the topmost Windows Explorer window to STDOUT\n";
    std::wcout << L"Add a /d switch to manually browse for a folder and print its name.";
    std::wcout << L"Add /d [initial_path] to select an initial path in the folder browse dialog.";
    return 2;
}

int _tmain(int argc, WCHAR* argv[])
{

    try
    {
        auto args = getArgs(argc, argv);
        
        if(args.size() > 1)
        {
            auto arg = args[1];
            if((arg == L"-h") || (arg == L"--help") || (arg == L"/h"))
            {
                return usage();
            }

            if((arg == L"-d") || (arg == L"/d"))
            {
                std::wstring initialDir = args.size() > 2 
                                        ? args[2] 
                                        : std::wstring();

                return browseForFolder(initialDir);
            }
            
            throw Exception(L"Unknown switch.");
        }
        
        return findCurrentExplorerLocation();       
    }
    catch(Exception e)
    {
        e.print();
        return 1;
    }
    
    return 0;
}

