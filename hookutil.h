#ifndef HOOKUTIL_H
#define HOOKUTIL_H


#include <Windows.h>
#include <DbgHelp.h>

int startKeyBoardHook();

bool stopKeyBoardHook();

int startMouseHook();

bool stopMouseHook();

#endif // HOOKUTIL_H
