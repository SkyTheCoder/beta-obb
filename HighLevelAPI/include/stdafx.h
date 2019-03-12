//------------------------------------------------------------------------------
//
// File Name:	stdafx.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		Beta Engine
// Course:		WANIC VGP2
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once

// Disable warnings for externals
#pragma warning(push, 0)

// Windows
#include <windows.h>
#include <gdiplus.h>

#define _USE_MATH_DEFINES
#include <cmath> // M_PI
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <algorithm>

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

// Used to track memory leaks to specific files and line numbers
#ifdef _DEBUG
	#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif

// Used to clean /W4 unused parameters for functions that must match a function-pointer type 
// NOTE: copied from winnt.h, but we don't want to include that here, otherwise 
#define UNREFERENCED_PARAMETER(P) (P)

// Re-enable warnings
#pragma warning(pop)
