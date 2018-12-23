#pragma once

#include "Constant.h"

extern HINSTANCE g_hInst;
extern HWND g_hWnd;

using namespace std;
using namespace Client;


#include <Vfw.h>
#pragma comment(lib,"vfw32.lib")

#include <process.h>

//#ifdef _DEBUG
//#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
//#endif