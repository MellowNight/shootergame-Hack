#pragma once
#include <iostream>
#include <thread>
#include <Windows.h>
#include <TlHelp32.h>
#include <cmath>
#include <vector>
#include <thread>

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <d3dx9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>


#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dwmapi.lib")


class MenuControl
{
public:
	bool	laserPoint;
	bool	godMode;
	bool	infinitAmmo;
	bool	highJump;
	float	runningSpeed;
	bool	scopeIn;
	bool	highSpeed;
	bool	ESP;

	MenuControl()
	{
		laserPoint = false;
		godMode = false;
		infinitAmmo = false;
		highJump = false;
		runningSpeed = 2.2;
		scopeIn = false;
		highSpeed = false;
		ESP = false;
	}

};

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
};

class GLOBALS
{
public:
	int			processID;
	HANDLE		processHandle;
	LONG		windowWidth;
	LONG		windowHeight;
	LONG		RealWindowWidth;
	LONG		RealWindowHeight;
	LONG		positionX;
	LONG		positionY;
};

extern GLOBALS*			Globals;
extern MenuControl*		menuControl;
extern GAME_INFO*		localGameInfo;