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

int _tmain(int argc, _TCHAR* argv[])
{

    try
    {
        COMNess com;

        auto windows = getOrderedWindows();
        auto explorerInfo = getExplorerPath(windows);

        printf("%s\n", explorerInfo.path.c_str());
    }
    catch(Exception e)
    {
        e.print();
        exit(1);
    }
    
	return 0;
}

