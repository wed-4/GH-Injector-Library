## GH Injector Library

x86、WOW64、x64でのDLLインジェクションに対応した、機能豊富なDLLインジェクションライブラリです。5つのインジェクションメソッド、6つのシェルコード実行メソッド、および様々な追加オプションを備えています。すべてのメソッドでセッション分離を回避することができます。

このライブラリをGUIで使用したい場合は、[GH Injector GUI](https://github.com/Broihon/GH-Injector-GUI)をチェックしてください。

----

### Injection methods

- LoadLibraryExW
- LdrLoadDll
- LdrpLoadDll
- LdrpLoadDllInternal
- ManualMapping

### Shellcode execution methods

- NtCreateThreadEx
- Thread hijacking
- SetWindowsHookEx
- QueueUserAPC
- KernelCallback
- FakeVEH

### Manual mapping features:

- Section mapping
- Base relocation
- Imports
- Delayed imports
- SEH support
- TLS initialization
- Security cookie initalization
- Loader Lock
- Shift image
- Clean datadirectories

### Additional features:

- Various cloaking options
	- PEB unlinking
	- PE header cloaking
	- Thread cloaking
- Handle hijacking
- Hook scanning/restoring

----

### Getting started

You can easily use mapper by including the compiled binaries in your project. Check the provided Injection.h header for more information.
Make sure you have the compiled binaries in the working directory of your program.
On first run the injection module has to download PDB files for the native (and when run on x64 the wow64) version of the ntdll.dll to resolve symbol addresses. Use the exported StartDownload function to begin the download.
The injector can only function if the downloads are finished. The injection module exports GetSymbolState and GetImportState which will return INJ_ERROR_SUCCESS (0) if the PDB download and resolving of all required addresses is completed.
Additionally GetDownloadProgress can be used to determine the progress of the download as percentage. If the injection module is to be unloaded during the download process call InterruptDownload or there's a chance that the dll will deadlock your process.

```cpp

#include "Injection.h"

HINSTANCE hInjectionMod = LoadLibrary(GH_INJ_MOD_NAME);
	
auto InjectA = (f_InjectA)GetProcAddress(hInjectionMod, "InjectA");
auto GetSymbolState = (f_GetSymbolState)GetProcAddress(hInjectionMod, "GetSymbolState");
auto GetImportState = (f_GetSymbolState)GetProcAddress(hInjectionMod, "GetImportState");
auto StartDownload = (f_StartDownload)GetProcAddress(hInjectionMod, "StartDownload");

StartDownload();

while (GetSymbolState() != 0)
{
	Sleep(10);
}

while (GetImportState() != 0)
{
	Sleep(10);
}

DWORD TargetProcessId;

INJECTIONDATAA data =
{
	"",
	TargetProcessId,
	INJECTION_MODE::IM_LoadLibraryExW,
	LAUNCH_METHOD::LM_NtCreateThreadEx,
	NULL,
	0,
	NULL,
	true
};

strcpy(data.szDllPath, DllPathToInject);

InjectA(&data);

```

---

### Credits

First of all I want to credit Joachim Bauch whose Memory Module Library was a great source to learn from:  
https://github.com/fancycode/MemoryModule

He also made a great write-up explaining the basics of mapping a module:  
https://www.joachim-bauch.de/tutorials/loading-a-dll-from-memory/

I also want to thank Akaion/Dewera for helping me with SEH support and their C# mapping library which was another great resource to learn from:  
https://github.com/Dewera/Lunar

Big thanks to mambda who made this PDB parser which I could steal code from to verify GUIDs:  
https://bitbucket.org/mambda/pdb-parser/src/master/
