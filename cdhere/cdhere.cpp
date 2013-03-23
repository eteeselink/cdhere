#include "stdafx.h"
#include "explorer.h"
#include "windowlist.h"
#include <vector>

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

std::vector<String> getArgs(int argc, _TCHAR* argv[])
{
    std::vector<String> args;
    for(int i = 0; i < argc; i++)
    {
        args.push_back(argv[i]);
    }
    return args;
}


int _tmain(int argc, _TCHAR* argv[])
{

    try
    {
        auto args = getArgs(argc, argv);
        
        if(args.size() == 2)
        {
            String& arg = args[1];
            if((arg == "-h") || (arg == "--help") || (arg == "/h"))
            {
                printf("Prints the location of the topmost Windows Explorer window to STDOUT");
                return 2;
            }
        }

        COMNess com;

        auto windows = getOrderedWindows();
        auto explorerInfo = getExplorerPath(windows);

        printf("%s\n", explorerInfo.path.c_str());
    }
    catch(Exception e)
    {
        e.print();
        return 1;
    }
    
	return 0;
}

