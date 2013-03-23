#include "stdafx.h"

class Exception 
{
private:
    char* _message;

public:
    Exception(char* message);

    void print();
};

struct ExplorerInfo
{
    String path;
    String item;
};

#define VERIFY(r) { HRESULT __result = (r); if(!SUCCEEDED(__result)) { throw Exception(#r); } }

ExplorerInfo getExplorerPath();

