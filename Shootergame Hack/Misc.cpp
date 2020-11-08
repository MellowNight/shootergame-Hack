#include "Misc.h"
#include "globals.h"

/*	get GWorld	0F 2E c1 74 ? 48 8B 1D ? ? ? ? 48 85 DB 74	*/
void	updateInfo()
{
	localGameInfo->updateLocalPlayer();
	localGameInfo->updateplayerController();
	localGameInfo->updatelocalPawn();
	localGameInfo->updateRootComponent();
	localGameInfo->updatepersistentLevel();
	localGameInfo->updateactorList();


	localGameInfo->updateCameraCache();
	localGameInfo->updateactorCount();
	localGameInfo->updatePosition();

	localGameInfo->localactorId = Driver::read<DWORD64>(Globals->processHandle, (localGameInfo->localPlayer + 0x18));

	setWindowPosition(Globals->processID);

	Globals->screenWidth = GetScreenInfo().realWidth;
	Globals->screenHeight = GetScreenInfo().realHeight;
}

int lastTick = 0;

void	handleMiscCommands(HANDLE	processHandle, DWORD64	localPawn)
{
	float		health = 100.f;
	int			Ammo = 50;
	vector3D	xyz;
	char		scopeLock = 0xFF;

	localGameInfo->currentWeapon = Driver::read<DWORD64>(Globals->processHandle, localPawn + Offsets::CurrentWeapon);


	if (menuControl->godMode == true)
	{
		Driver::write<float>(Globals->processHandle, (localPawn + Offsets::pawnHealth), &health);
	}


	if (menuControl->infinitAmmo == true)
	{
		Driver::write(Globals->processHandle, (localGameInfo->currentWeapon + Offsets::currentAmmo), &Ammo);
	}



	if (menuControl->highJump == true)
	{

		if (GetAsyncKeyState(VK_SPACE))
		{
				xyz = Driver::read<vector3D>(Globals->processHandle, (localGameInfo->rootComponent + Offsets::Translation));

				xyz.z += 0.5f;

				Driver::write<vector3D>(Globals->processHandle, (localGameInfo->rootComponent + Offsets::Translation), &xyz);
				lastTick = GetTickCount();
		}
	}

	if (menuControl->highSpeed)
	{
		Driver::write<float>(Globals->processHandle, (localGameInfo->localPawn + Offsets::RunningSpeedModifier), &menuControl->runningSpeed);
	}


	if (menuControl->scopeIn == true)
	{
		scopeLock = 0xFF;
		Driver::write<char>(Globals->processHandle, (localGameInfo->localPawn + Offsets::IsTargeting), &scopeLock);
	}
}

