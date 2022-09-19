#include "hookutil.h"
#include <keyboardhistory.h>
#include <mousehistory.h>
#include <QDebug>


LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam);

HMODULE WINAPI ModuleFromAddress(PVOID pv);
static HHOOK hKeyBoardHook = NULL;
static HHOOK hMouseHook = NULL;

LRESULT KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    KBDLLHOOKSTRUCT *Key_Info = (KBDLLHOOKSTRUCT *)lParam;
    if (HC_ACTION == nCode)
    {
        if (WM_KEYDOWN == wParam || WM_SYSKEYDOWN == wParam)
        {
//            if (Key_Info->vkCode <= 107 && Key_Info->vkCode >= 65)
            {
                if (KeyboardHistory::instance())
                {
                    KeyboardHistory::instance()->setkeyValue(Key_Info->vkCode);
                }
            }
        }
    }
    return CallNextHookEx(hKeyBoardHook, nCode, wParam, lParam);
}

HMODULE ModuleFromAddress(PVOID pv)
{
    MEMORY_BASIC_INFORMATION mbi;
    if (VirtualQuery(pv, &mbi, sizeof(mbi)) != 0)
    {
        return (HMODULE)mbi.AllocationBase;
    }
    else
    {
        return NULL;
    }
}

int startKeyBoardHook()
{
    hKeyBoardHook = SetWindowsHookExW(WH_KEYBOARD_LL, KeyboardHookProc, ModuleFromAddress((PVOID)KeyboardHookProc), 0);
    int error = GetLastError();
    return error;
}

bool stopKeyBoardHook()
{
    return UnhookWindowsHookEx(hKeyBoardHook);
}

bool stopMouseHook()
{
    if (hMouseHook == NULL)
    {
        return FALSE;
    }
    UnhookWindowsHookEx(hMouseHook);
    hMouseHook = NULL;
    return TRUE;
}
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if(HC_ACTION == nCode){
        if (WM_LBUTTONDOWN == wParam || WM_RBUTTONDOWN == wParam
                || WM_MBUTTONDOWN == wParam)
        {
            if (MouseHistory::instance())
            {
                MouseHistory::instance()->setkeyValue(1);
            }
        }
    }
    return CallNextHookEx(hMouseHook, nCode, wParam, lParam);
}

BOOL startMouseHook()
{
    if (hMouseHook != NULL)
    {
        return FALSE;
    }
    hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, ModuleFromAddress((PVOID)KeyboardHookProc), NULL);
    if (NULL == hMouseHook)
    {
        qDebug() << "regiter hook for mouse failed";
        return FALSE;
    }
    return TRUE;
}
