// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Injection.h"

#ifdef _WIN64
const char szDllFile[] = "C:\\Users\\konra\\Desktop\\Test Dll x64.dll";
const char szProc[] = "Test Console x64.exe";
#else
const char szDllFile[] = "C:\\Windows\\cheat.dll";
const char szProc[] = "csgo.exe";
#endif


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

bool IsCorrectTargetArchitecture(HANDLE hProc)
{
    BOOL bTarget = FALSE;
    if (!IsWow64Process(hProc, &bTarget))

        return false;
    

    BOOL bHost = FALSE;
    IsWow64Process(GetCurrentProcess(), &bHost);

    return (bTarget == bHost);
}

extern "C" __declspec(dllexport) void main_manualmap_fn(const TCHAR * pFile)
{
	std::string dllvar(pFile);
	PROCESSENTRY32 PE32{ 0 };
	PE32.dwSize = sizeof(PE32);
	const char* dll = dllvar.c_str();
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE)
	{
		DWORD Err = GetLastError();
		printf("CreateToolhelp32Snapshot failed: 0x%X\n", Err);
		system("PAUSE");
		
	}

	DWORD PID = 0;
	BOOL bRet = Process32First(hSnap, &PE32);
	while (bRet)
	{
		if (!strcmp(szProc, PE32.szExeFile))
		{
			PID = PE32.th32ProcessID;
			break;
		}
		bRet = Process32Next(hSnap, &PE32);
	}

	CloseHandle(hSnap);

	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	if (!hProc)
	{
		DWORD Err = GetLastError();
		printf("OpenProcess failed: 0x%X\n", Err);
		system("PAUSE");
	
	}

	if (!IsCorrectTargetArchitecture(hProc))
	{
		printf("Invalid target process.\n");
		CloseHandle(hProc);
		system("PAUSE");
	
	}

	if (!ManualMap(hProc, dll))
	{
		CloseHandle(hProc);
		printf("Something went wrong FeelsBadMan\n");
		system("PAUSE");
		
	}

	CloseHandle(hProc);

	
}