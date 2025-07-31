#include "WindowsPaths.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shlobj.h>
#include <string>
#include "Utilities/Logger/Logger.h"

std::string GetAppDataPath()
{
    char appdata[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, appdata))) 
    {
        return std::string(appdata);
    }
    mycerr << "Can't find path";
    return "C:/Users/Default/AppData/Roaming";
}
