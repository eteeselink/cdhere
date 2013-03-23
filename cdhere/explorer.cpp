#include "stdafx.h"
#include <shlobj.h>
#include <Shlwapi.h>
#include <exdisp.h>
#include "explorer.h"
#include <vector>

#define CDHERE_MAX_PATH 520

Exception::Exception(char* message)
{
    _message = message;
}

void Exception::print()
{
    fprintf(stderr, _message);
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

ExplorerInfo getExplorerPath()
{
    HWND hwndFind = GetForegroundWindow();
    ExplorerInfo info;
 
    CComPtr<IShellWindows> psw;
    VERIFY(CoCreateInstance(CLSID_ShellWindows, NULL, CLSCTX_ALL, IID_IShellWindows, (void**)&psw));

    auto dispatches = getShellDispatches(psw);

    if(dispatches.empty())
    {
        throw Exception("No Explorer window found");
    }
    IDispatch* dispatch = dispatches[0];

    CComPtr<IWebBrowserApp> webBrowserApp;
    VERIFY(dispatch->QueryInterface(IID_IWebBrowserApp, (void**)&webBrowserApp));

    HWND hwndWBA;
    VERIFY(webBrowserApp->get_HWND((LONG_PTR*)&hwndWBA));

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
    char path[CDHERE_MAX_PATH];
    if (!SHGetPathFromIDList(pidlFolder, path)) {
        throw Exception("Explorer window not pointed at a directory");
        //lstrcpyn(path, TEXT("<not a directory>"), MAX_PATH);
    }
    info.path = path;

    int iFocus;
    VERIFY(folderView->GetFocusedItem(&iFocus));

    CComHeapPtr<ITEMIDLIST> pidlItem;
    VERIFY(folderView->Item(iFocus, &pidlItem));

    CComPtr<IShellFolder> shellFolder;
    VERIFY(persistFolder2->QueryInterface(IID_IShellFolder, (void**)&shellFolder));

    STRRET str;
    VERIFY(shellFolder->GetDisplayNameOf(pidlItem, SHGDN_INFOLDER, &str));
    
    char item[CDHERE_MAX_PATH];
    StrRetToBuf(&str, pidlItem, item, CDHERE_MAX_PATH);
    info.item = item;

    return info;
}