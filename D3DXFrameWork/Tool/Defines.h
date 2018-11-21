#pragma once
#include "Engine_Defines.h"

namespace Tool
{
	const unsigned int	g_iBackCX = 800;
	const unsigned int	g_iBackCY = 600;

	const unsigned int g_iFormCX = 300;
	const unsigned int g_iFormCY = 600;
}

enum MOUSE_STATE { STATE_DRAWUP, STATE_DRAWDOWN, STATE_TILE1, STATE_TILE2, STATE_TILE3, STATE_TILE4, STATE_ADD_OBJECT, STATE_ADD_NAV, STATE_ADD_CAM, STATE_END };


#include <vector>

using namespace Tool;
using namespace std;