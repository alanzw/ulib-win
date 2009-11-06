#include <windows.h>

#define HOOKAPI2_API __declspec( dllexport )

HHOOK g_hhook;
HINSTANCE g_hinstDll;
FARPROC pfMessageBoxA;

int WINAPI MyMessageBoxA(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType);
BYTE OldMessageBoxACode[5];
BYTE NewMessageBoxACode[5];

HMODULE hModule;
DWORD dwIdOld;
DWORD dwIdNew;
BOOL bHook = FALSE;

void HookOn();
void HookOff();
BOOL init();

LRESULT WINAPI MessageHook(int nCode, WPARAM wParam, LPARAM lParam);

extern "C"
{

HOOKAPI2_API BOOL InstallHook();

HOOKAPI2_API BOOL UninstallHook();

}

BOOL APIENTRY DllMain(HANDLE hModule,
    DWORD ul_reason_for_call,
    LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        if (!init())
        {
            MessageBoxA(NULL, "Init", "Error", MB_OK);
            return FALSE;
        }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        if (bHook) UninstallHook();
        break;
    }
    return TRUE;
}

LRESULT WINAPI MessageHook(int nCode, WPARAM wParam, LPARAM lParam)
{
    return CallNextHookEx(g_hhook, nCode, wParam, lParam);
}

HOOKAPI2_API BOOL InstallHook()
{
    g_hinstDll = ::LoadLibrary("apihook.dll");
    g_hhook = SetWindowsHookEx(WH_GETMESSAGE, (HOOKPROC)MessageHook, g_hinstDll, 0);

    if (!g_hhook)
    {
        MessageBoxA(NULL, "Set Error", "Error", MB_OK);
        return FALSE;
    }
    return TRUE;
}

HOOKAPI2_API BOOL UninstallHook()
{
    return UnhookWindowsHookEx(g_hhook);
}


BOOL init()
{
    hModule = LoadLibrary("user32.dll");
    pfMessageBoxA = GetProcAddress(hModule, "MessageBoxA");
    if (NULL == pfMessageBoxA)
    {
        return FALSE;
    }

#ifdef _MSC_VER
    _asm
    {
        lea edi, OldMessageBoxACode
        mov esi, pfMessageBoxA
        cld
        movsd
        movsb
    }
#else
    __asm__ __volatile__ ( "cld\n\t"
                           "movsd\n\t"
                           "movsb\n\t"
                          : /* outputs */
                          : /* inputs */ "D"(&OldMessageBoxACode[0]), "S"(pfMessageBoxA)
                          : /* clobbered */
                        );

#endif

    NewMessageBoxACode[0] = 0xe9; // jmp MyMessageBoxA

#ifdef _MSC_VER
    _asm
    {
        lea eax, MyMessageBoxA
        mov ebx, pfMessageBoxA
        sub eax, ebx
        sub eax, 5
        mov dword ptr [NewMessageBoxACode+1], eax
    }
#else
    DWORD *pdw = (DWORD *)&NewMessageBoxACode[1];
    __asm__ __volatile__ ( "sub %%ebx,  %%eax\n\t"
                           "sub $0x5, %%eax\n\t"
                           "mov %%eax, (%2)"
                          : /* outputs */
                          : /* inputs */ "a"(&MyMessageBoxA), "b"(pfMessageBoxA), "c"(pdw)
                          : "memory"
                        );

#endif

    dwIdNew = GetCurrentProcessId();
    dwIdOld = dwIdNew;

    HookOn();

    return TRUE;
}

int WINAPI MyMessageBoxA(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
{
    int nReturn = 0;
    HookOff();
    nReturn = MessageBoxA(hWnd, "Hook", lpCaption, uType);
    HookOn();
    return nReturn;
}

void HookOn()
{
    HANDLE hProc;
    dwIdOld = dwIdNew;
    hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, dwIdOld);
    VirtualProtectEx(hProc, (void *)pfMessageBoxA, 5, PAGE_READWRITE, &dwIdOld);
    WriteProcessMemory(hProc, (void *)pfMessageBoxA, NewMessageBoxACode, 5, 0);
    VirtualProtectEx(hProc, (void *)pfMessageBoxA, 5, dwIdOld, &dwIdOld);
    bHook = TRUE;
}

void HookOff()
{
    HANDLE hProc;
    dwIdOld = dwIdNew;
    hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, dwIdOld);
    VirtualProtectEx(hProc, (void *)pfMessageBoxA, 5, PAGE_READWRITE, &dwIdOld);
    WriteProcessMemory(hProc, (void *)pfMessageBoxA, OldMessageBoxACode, 5, 0);
    VirtualProtectEx(hProc, (void *)pfMessageBoxA, 5, dwIdOld, &dwIdOld);
    bHook = FALSE;
}
