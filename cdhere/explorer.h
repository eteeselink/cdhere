#include "stdafx.h"

#define VERIFY(r) { HRESULT __result = (r); if(!SUCCEEDED(__result)) { throw Exception(#r); } }

void GetExplorerPath(TCHAR* path, int maxPathLength, TCHAR* item, int maxItemLength);

class Exception 
{
private:
    char* _message;

public:
    Exception(char* message);

    void print();
};