#include "stdafx.h"
#include "windowlist.h"

std::vector<HWND> getOrderedWindows()
{
    std::vector<HWND> windows;
    HWND win = GetTopWindow(NULL);

    while(win != NULL)
    {
        windows.push_back(win);
        win = GetNextWindow(win, GW_HWNDNEXT);
    }

    return windows;
}