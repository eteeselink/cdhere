#include "stdafx.h"
#include <shlobj.h>
#include <Shlwapi.h>
#include <exdisp.h>
#include "explorer.h"
#include <vector>

Exception::Exception(char* message)
{
    _message = message;
}

void Exception::print()
{
    printf(_message);
}

template <typename T> class Wrapper 
{
protected:
    T obj;
public:
    T& get()
    {
        return obj;
    }

    virtual ~Wrapper() {}
};

template <typename T> class COMObj : public Wrapper<T*> {

public:
    virtual ~COMObj() 
    {
        obj->Release();
    }
};

template <typename T> class COMMemObj : public Wrapper<T> {

public:
    virtual ~COMMemObj() 
    {
        CoTaskMemFree(obj);
    }
};




class ShellWindows : public COMObj<IShellWindows>
{
private:
    std::vector<IDispatch*> _dispatches;

    void getDispatches()
    {
        VARIANT v;
        V_VT(&v) = VT_I4;

        IDispatch *pdisp;
        for (V_I4(&v) = 0; obj->Item(v, &pdisp) == S_OK; V_I4(&v)++) {
            _dispatches.push_back(pdisp);
        }
    }
public:
    ShellWindows()
    {
        VERIFY(CoCreateInstance(CLSID_ShellWindows, NULL, CLSCTX_ALL, IID_IShellWindows, (void**)&obj));

        getDispatches();
    }

    ~ShellWindows()
    {
        for(auto i = _dispatches.begin(); i != _dispatches.end(); i++)
        {
            (*i)->Release();
        }
    }

    std::vector<IDispatch*> dispatches()
    {
        return _dispatches;
    }
};

class WebBrowserApp : public COMObj<IWebBrowserApp>
{
public:
    WebBrowserApp(IDispatch  *pdisp)
    {
        VERIFY(pdisp->QueryInterface(IID_IWebBrowserApp, (void**)&obj));
    }
};

class ServiceProvider : public COMObj<IServiceProvider>
{
public:
    ServiceProvider(WebBrowserApp& webBrowserApp)
    {
        VERIFY(webBrowserApp.get()->QueryInterface(IID_IServiceProvider, (void**)&obj));
    }
};

class ShellBrowser : public COMObj<IShellBrowser> 
{
public:
    ShellBrowser(ServiceProvider& serviceProvider)
    {
        VERIFY(serviceProvider.get()->QueryService(SID_STopLevelBrowser, IID_IShellBrowser, (void**)&obj));
    }
};

class ShellView : public COMObj<IShellView>
{
public:
    ShellView(ShellBrowser& shellBrowser)
    {
        VERIFY(shellBrowser.get()->QueryActiveShellView(&obj));
    }
};

class FolderView : public COMObj<IFolderView>
{
public:
    FolderView(ShellView& shellView) 
    {
        VERIFY(shellView.get()->QueryInterface(IID_IFolderView, (void**)&obj));
    }
};

class PersistFolder2 : public COMObj<IPersistFolder2>
{
public:
    PersistFolder2(FolderView& folderView)
    {
        VERIFY(folderView.get()->GetFolder(IID_IPersistFolder2, (void**)&obj));
    }
};

class ItemIdList : public COMMemObj<LPITEMIDLIST>
{
public:
    ItemIdList(PersistFolder2 persistFolder2)
    {
        persistFolder2.get()->GetCurFolder(&obj);
    }
};

class TaskMemory
{
private:
    void* obj;
public:
    TaskMemory(void* obj)
        : obj(obj)
    {}

    ~TaskMemory()
    {
        CoTaskMemFree(obj);
    }
}


void GetExplorerPath(TCHAR* path, int maxPathLength, TCHAR* item, int maxItemLength)
{
    HWND hwndFind = GetForegroundWindow();
    path[0] = TEXT('\0');
    item[0] = TEXT('\0');
 
    ShellWindows psw;

    std::vector<IDispatch*> dispatches = psw.dispatches();

    IDispatch* dispatch = dispatches[0];

    WebBrowserApp webBrowserApp(dispatch);
     
    HWND webBrowserWnd;
    VERIFY(webBrowserApp.get()->get_HWND((LONG_PTR*)&webBrowserWnd));

    ServiceProvider serviceProvider(webBrowserApp);
    ShellBrowser shellBrowser(serviceProvider);
    ShellView shellView(shellBrowser);
    FolderView folderView(shellView);
    PersistFolder2 persistFolder2(folderView);

    ItemIdList folder(persistFolder2);

    if (!SHGetPathFromIDList(folder.get(), path)) {
        lstrcpyn(path, TEXT("<not a directory>"), maxPathLength);
    }
    int iFocus;
    if (SUCCEEDED(folderView.get()->GetFocusedItem(&iFocus))) {
        LPITEMIDLIST pidlItem;
        if (SUCCEEDED(folderView.get()->Item(iFocus, &pidlItem))) {
        IShellFolder *psf;
        if (SUCCEEDED(persistFolder2.get()->QueryInterface(IID_IShellFolder, (void**)&psf))) {
            STRRET str;
            if (SUCCEEDED(psf->GetDisplayNameOf(pidlItem, SHGDN_INFOLDER, &str))) {
                StrRetToBuf(&str, pidlItem, item, maxItemLength);
            }
            psf->Release();
        }
        CoTaskMemFree(pidlItem);
        }
    }

}