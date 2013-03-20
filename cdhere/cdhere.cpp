// cdhere.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "explorer.h"

#define CDHERE_MAX_PATH 620

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

        TCHAR path[CDHERE_MAX_PATH];
        TCHAR item[CDHERE_MAX_PATH];

        GetExplorerPath(path, CDHERE_MAX_PATH, item, CDHERE_MAX_PATH);

        printf("%s\n", path);
    }
    catch(Exception e)
    {
        e.print();
        exit(1);
    }
    
	return 0;
}

