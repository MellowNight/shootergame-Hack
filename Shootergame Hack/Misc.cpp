#include "Misc.h"


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


	localGameInfo->localPlayer = localPlayer;
	localGameInfo->playerController = playerController;
	localGameInfo->localPawn = localPawn;
	localGameInfo->rootComponent = RootComponent;
	localGameInfo->persistentLevel = persistentLevel;
	localGameInfo->actorList = actorList;
	localGameInfo->actorCount = actorCount;
	localGameInfo->cameraCachePrivate = CameraManager + Offsets::cameraCachePrivate;
	localGameInfo->localPawn = localPawn;


	initWindow(Globals->processID);

}
