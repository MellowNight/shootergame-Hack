#include "Misc.h"
#include "globals.h"

/*	get GWorld	0F 2E c1 74 ? 48 8B 1D ? ? ? ? 48 85 DB 74	*/
DWORD64		GWorldAddr;
DWORD64		gameInstance;
DWORD64		localPlayers;
DWORD64		localPlayer;
DWORD64		playerController;
DWORD64		localPawn;
DWORD64		RootComponent;
DWORD64		persistentLevel;
DWORD64		CameraManager;

DWORD64	actorList;
int		actorCount;

void	updateInfo(DWORD64	moduleBase)
{
	/*	get GWorld	0F 2E c1 74 ? 48 8B 1D ? ? ? ? 48 85 DB 74	*/
	GWorldAddr = Commands::read<DWORD64>(Globals->processHandle, (moduleBase + Offsets::GWORLD));

	gameInstance = Commands::read<DWORD64>(Globals->processHandle, (GWorldAddr + Offsets::gameInstance));

	localPlayers = Commands::read<DWORD64>(Globals->processHandle, (gameInstance + Offsets::localPlayers));

	localPlayer = Commands::read<DWORD64>(Globals->processHandle, (localPlayers));

	playerController = Commands::read<DWORD64>(Globals->processHandle, (localPlayer + Offsets::playerController));

	localPawn = Commands::read<DWORD64>(Globals->processHandle, (playerController + Offsets::APawn));

	RootComponent = Commands::read<DWORD64>(Globals->processHandle, (localPawn + Offsets::RootComponent));

	persistentLevel = Commands::read<DWORD64>(Globals->processHandle, (GWorldAddr + Offsets::persistentLevel));

	actorList = Commands::read<DWORD64>(Globals->processHandle, (persistentLevel + Offsets::actorsArray));

	CameraManager = Commands::read<DWORD64>(Globals->processHandle, (playerController + Offsets::CameraManager));

	localPawn = Commands::read<DWORD64>(Globals->processHandle, (playerController + Offsets::APawn));

	actorCount = Commands::read<DWORD64>(Globals->processHandle, (localGameInfo->persistentLevel + Offsets::actorCount));

	localGameInfo->position = Commands::read<vector3D>(Globals->processHandle, (localGameInfo->rootComponent + Offsets::Translation));

	int localActorId = Commands::read<DWORD64>(Globals->processHandle, (localPlayer + 0x18));
	
	localGameInfo->localPlayer = localPlayer;
	localGameInfo->playerController = playerController;
	localGameInfo->localPawn = localPawn;
	localGameInfo->rootComponent = RootComponent;
	localGameInfo->persistentLevel = persistentLevel;
	localGameInfo->actorList = actorList;
	localGameInfo->actorCount = actorCount;
	localGameInfo->cameraCachePrivate = CameraManager + Offsets::cameraCachePrivate;
	localGameInfo->localPawn = localPawn;
	localGameInfo->localactorId = localActorId;



	initWindow(Globals->processID);

}

int lastTick = 0;

void	handleMiscCommands(HANDLE	processHandle, DWORD64	localPawn)
{
	float		health = 100.f;
	int			Ammo = 50;
	vector3D	xyz;
	char		scopeLock = 0xFF;

	localGameInfo->currentWeapon = Commands::read<DWORD64>(Globals->processHandle, localPawn + Offsets::CurrentWeapon);


	if (menuControl->godMode == true)
	{
		Commands::write<float>(Globals->processHandle, (localPawn + Offsets::pawnHealth), &health);
	}


	if (menuControl->infinitAmmo == true)
	{
		Commands::write(Globals->processHandle, (localGameInfo->currentWeapon + Offsets::currentAmmo), &Ammo);
	}



	if (menuControl->highJump == true)
	{

		if (GetAsyncKeyState(VK_SPACE))
		{
				xyz = Commands::read<vector3D>(Globals->processHandle, (localGameInfo->rootComponent + Offsets::Translation));

				xyz.z += 0.5f;

				Commands::write<vector3D>(Globals->processHandle, (localGameInfo->rootComponent + Offsets::Translation), &xyz);
				lastTick = GetTickCount();
		}
	}

	if (menuControl->highSpeed)
	{
		Commands::write<float>(Globals->processHandle, (localGameInfo->localPawn + Offsets::RunningSpeedModifier), &menuControl->runningSpeed);
	}


	if (menuControl->scopeIn == true)
	{
		scopeLock = 0xFF;
		Commands::write<char>(Globals->processHandle, (localGameInfo->localPawn + Offsets::IsTargeting), &scopeLock);
	}
}

