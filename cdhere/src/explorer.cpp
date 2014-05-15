#include "stdafx.h"
#include <shlobj.h>
#include <Shlwapi.h>
#include <exdisp.h>
#include "explorer.h"
#include <algorithm>
#include <comdef.h>
#include <iomanip>
#include <sstream>

#define CDHERE_MAX_PATH 520

Exception::Exception(std::wstring const& message)
    : _message(message)
{
}

void Exception::print()
{
    std::wcerr << _message;
}

void verify(HRESULT result, std::wstring const& call)
{
    if(!SUCCEEDED(result)) {
		_com_error error(result);
		std::wostringstream code;
		code << std::hex << result;

		throw Exception(L"Error calling '" + call +
						L"'.\n  Code: 0x" + code.str() + 
						L"'.\n  Message: " + error.ErrorMessage());
    }
}

std::vector<CComPtr<IDispatch> > getShellDispatches(IShellWindows* shellWindows)
{
    VARIANT v;
    V_VT(&v) = VT_I4;
    std::vector<CComPtr<IDispatch> > dispatches;

    IDispatch *pdisp;
    for (V_I4(&v) = 0; shellWindows->Item(v, &pdisp) == S_OK; V_I4(&v)++) {
        dispatches.push_back(pdisp);
    }

    return dispatches;
}

HWND getHwnd(IDispatch* dispatch)
{
    CComPtr<IWebBrowserApp> webBrowserApp;
    VERIFY(dispatch->QueryInterface(IID_IWebBrowserApp, (void**)&webBrowserApp));

    HWND hwnd;
	VERIFY(webBrowserApp->get_HWND((LONG_PTR*)&hwnd));
    return hwnd;
}

IDispatch* getFirstWindowInList(const std::vector<CComPtr<IDispatch> > dispatches, std::vector<HWND> windowList)
{
    std::vector<HWND> dispatchWindows;

    struct {
        IDispatch* dispatch;
        size_t index;
    } best = { NULL, MAXUINT };

    // go through all `dispatches`, and find the one whose hwnd is earliest in `windowList`
    for(auto dispatchIter = dispatches.begin(); dispatchIter != dispatches.end(); dispatchIter++)
    {
        HWND hwnd = getHwnd(*dispatchIter); 
        
        auto item = std::find(windowList.begin(), windowList.end(), hwnd);
        if(item != windowList.end())
        {
            size_t index = item - windowList.begin();
            if(index < best.index)
            {
                best.dispatch = *dispatchIter;
                best.index = item - windowList.begin();
            }
        }
    }

    return best.dispatch;
}

ExplorerInfo getExplorerPath(std::vector<HWND> windowsToSearch)
{
    
    ExplorerInfo info;
 
    CComPtr<IShellWindows> psw;
    VERIFY(CoCreateInstance(CLSID_ShellWindows, NULL, CLSCTX_ALL, IID_IShellWindows, (void**)&psw));

    auto dispatches = getShellDispatches(psw);
    
    IDispatch* dispatch = getFirstWindowInList(dispatches, windowsToSearch);
    if(dispatch == NULL)
    {
        throw Exception(L"No explorer window found");
    }

    CComPtr<IWebBrowserApp> webBrowserApp;
    VERIFY(dispatch->QueryInterface(IID_IWebBrowserApp, (void**)&webBrowserApp));

    CComPtr<IServiceProvider> serviceProvider;
    VERIFY(webBrowserApp->QueryInterface(IID_IServiceProvider, (void**)&serviceProvider));

    CComPtr<IShellBrowser> shellBrowser;
    VERIFY(serviceProvider->QueryService(SID_STopLevelBrowser, IID_IShellBrowser, (void**)&shellBrowser));

    CComPtr<IShellView> shellView;
    VERIFY(shellBrowser->QueryActiveShellView(&shellView));

    CComPtr<IFolderView> folderView;
    VERIFY(shellView->QueryInterface(IID_IFolderView, (void**)&folderView));

    CComPtr<IPersistFolder2> persistFolder2;
    VERIFY(folderView->GetFolder(IID_IPersistFolder2, (void**)&persistFolder2));
               
    CComHeapPtr<ITEMIDLIST> pidlFolder;
        
    VERIFY(persistFolder2->GetCurFolder(&pidlFolder));
    WCHAR path[CDHERE_MAX_PATH];
    if (!SHGetPathFromIDList(pidlFolder, path)) {
        throw Exception(L"Explorer window not pointed at a directory");
    }
    info.path = path;

    int iFocus;
    VERIFY(folderView->GetFocusedItem(&iFocus));
    
    if(iFocus == -1)
    {
        // no item selected. silently fail, since we currently do no use the item.
        return info;
    }

    CComHeapPtr<ITEMIDLIST> pidlItem;
    VERIFY(folderView->Item(iFocus, &pidlItem));

    CComPtr<IShellFolder> shellFolder;
    VERIFY(persistFolder2->QueryInterface(IID_IShellFolder, (void**)&shellFolder));

    STRRET str;
    VERIFY(shellFolder->GetDisplayNameOf(pidlItem, SHGDN_INFOLDER, &str));
    
    WCHAR item[CDHERE_MAX_PATH];
    StrRetToBuf(&str, pidlItem, item, CDHERE_MAX_PATH);
    info.item = item;

    return info;
}