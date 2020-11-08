#include "globals.h"
#include "utils.h"
#include "Offsets.h"
#include "commands.h"

GLOBALS*		Globals = new GLOBALS;
GAME_INFO*		localGameInfo = new GAME_INFO;


void	GAME_INFO::updateGameInstance()
{
	DWORD64		gWorld = Driver::read<DWORD64>(Globals->processHandle, (Globals->moduleBase + Offsets::GWORLD));
	this->gameInstance = Driver::read<DWORD64>(Globals->processHandle, (gWorld + Offsets::gameInstance));
}

void	GAME_INFO::updateLocalPlayer()
{
	DWORD64	localPlayers = Driver::read<DWORD64>(Globals->processHandle, (this->gameInstance + Offsets::localPlayers));
	this->localPlayer = Driver::read<DWORD64>(Globals->processHandle, (localPlayers));
}

void	GAME_INFO::updateplayerController()
{
	this->playerController = Driver::read<DWORD64>(Globals->processHandle, (localPlayer + Offsets::playerController));
}

void	GAME_INFO::updatelocalPawn()
{
	this->localPawn = Driver::read<DWORD64>(Globals->processHandle, (playerController + Offsets::APawn));
}

void	GAME_INFO::updateRootComponent()
{
	this->rootComponent = Driver::read<DWORD64>(Globals->processHandle, (localPawn + Offsets::RootComponent));
}

void	GAME_INFO::updatepersistentLevel()
{
	DWORD64		gWorld = Driver::read<DWORD64>(Globals->processHandle, (Globals->moduleBase + Offsets::GWORLD));
	this->persistentLevel = Driver::read<DWORD64>(Globals->processHandle, (gWorld + Offsets::persistentLevel));
}

void	GAME_INFO::updateactorList()
{
	this->actorList = Driver::read<DWORD64>(Globals->processHandle, (persistentLevel + Offsets::actorsArray));
}

void	GAME_INFO::updateCameraCache()
{
	DWORD64		CameraManager = Driver::read<DWORD64>(Globals->processHandle, (this->localPawn + Offsets::CameraManager));
	this->cameraCachePrivate = CameraManager + Offsets::cameraCachePrivate;
}
void	GAME_INFO::updatePosition()
{
	this->position = Driver::read<vector3D>(Globals->processHandle, (localGameInfo->rootComponent + Offsets::Translation));
}
void	GAME_INFO::updateactorCount()
{
	this->actorCount = Driver::read<DWORD64>(Globals->processHandle, (localGameInfo->persistentLevel + Offsets::actorCount));
}