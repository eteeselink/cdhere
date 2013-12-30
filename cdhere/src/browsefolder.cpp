#include "stdafx.h"
#include <vector>
#include <iostream>

static int CALLBACK BrowseFolderCallback(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
    if (uMsg == BFFM_INITIALIZED) {
        LPCTSTR path = reinterpret_cast<LPCTSTR>(lpData);
        ::SendMessage(hwnd, BFFM_SETSELECTION, true, (LPARAM) path);
    }
    return 0;
}


int browseForFolder(std::wstring const& initial_dir_arg)
{
    TCHAR selected_dir[MAX_PATH];
    TCHAR initial_dir[MAX_PATH];
    if(!initial_dir_arg.empty())
    {
        // SHBrowseForFolder doesn't like relative initial paths.
        _tfullpath(initial_dir, initial_dir_arg.c_str(), MAX_PATH);
    }
    else
    {
        GetCurrentDirectory(MAX_PATH, initial_dir);
    }

    BROWSEINFO br_info = { 0 };
    br_info.lParam = reinterpret_cast<LPARAM>(initial_dir); 
    br_info.lpfn = BrowseFolderCallback;
    br_info.lpszTitle = L"Please select a folder";
    br_info.ulFlags = BIF_SHAREABLE | BIF_USENEWUI;
    LPITEMIDLIST lpItem = SHBrowseForFolder(&br_info);

    if(lpItem != NULL && SHGetPathFromIDList(lpItem, selected_dir))
    {
        std::wcout << selected_dir << "\n";
        return 0;
    }
    else
    {
        return 1;
    }
}