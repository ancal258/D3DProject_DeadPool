#pragma once

#pragma warning (disable : 4251)
#pragma warning (disable : 4005)



//#define _CRTDBG_MAP_ALLOC 
//#include <stdlib.h> 
//#include <crtdbg.h>



#include <vector>
#include <list>
#include <map>
#include <algorithm>




#include <d3d9.h>
#include <d3dx9.h> 


/*
#ifdef _DEBUG 
#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 
#endif 
#endif  // _DEBUG 
*/
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "Engine_Macro.h"
#include "Engine_Typedef.h"
#include "Engine_Functor.h"
#include "Engine_Funtion.h"
#include "Engine_Struct.h"


using namespace std;
using namespace Engine;

