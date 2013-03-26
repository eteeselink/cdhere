#include "stdafx.h"
#include <vector>

class Exception 
{
private:
    std::wstring _message;

public:
    Exception(std::wstring const& message);

    void print();
};

struct ExplorerInfo
{
    std::wstring path;
    std::wstring item;
};

#define VERIFY(r) { \
    HRESULT __result = (r); if(!SUCCEEDED(__result)) { \
        throw Exception(std::wstring(L"Error while executing `" L#r L"`. Code: ") + std::to_wstring((long long)__result)); \
    } \
}

ExplorerInfo getExplorerPath(std::vector<HWND> windowsToSearch);

