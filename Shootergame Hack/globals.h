#pragma once
#include <iostream>
#include <thread>
#include <Windows.h>
#include <TlHelp32.h>
#include <cmath>
#include <vector>
#include <thread>
#include <dinput.h>
#include <tchar.h>
#include <sstream>
#include "Math.h"
#include <algorithm>

#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dwmapi.lib")


class GAME_INFO
{
public:
	DWORD64		rootComponent;
	DWORD64		localPawn;
	DWORD64		actorList;
	DWORD64		currentWeapon;
	DWORD64		cameraCachePrivate;
	DWORD64		localPlayer;
	DWORD64		playerController;
	DWORD64		persistentLevel;
	int			actorCount;
	int			localactorId;
	vector3D	position;
};

class GLOBALS
{
public:
	int			processID;
	HANDLE		processHandle;
	LONG		windowWidth;
	LONG		windowHeight;
	LONG		screenWidth;
	LONG		screenHeight;
	LONG		positionX;
	LONG		positionY;
};

extern GLOBALS*			Globals;
extern GAME_INFO*		localGameInfo;

enum ACTOR_ID : int32_t
{

};