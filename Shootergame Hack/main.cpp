#include "globals.h"
#include "Offsets.h"
#include "Math.h"
#include "Aimbot.h"
#include "ESP.h"
#include "commands.h"
#include "Misc.h"
#include "Menu.h"


void Color(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}


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

			xyz.z += 20.00f;

			Commands::write<vector3D>(Globals->processHandle, (localGameInfo->rootComponent + Offsets::Translation), &xyz);
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



int		main()
{
	int			procId;

	DWORD64		moduleBase;


	Color(5);

	procId = Utils::FindProcessId(L"ShooterGame.exe");

	Globals->processID = procId;
	Globals->processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);

	moduleBase = Utils::GetModuleBaseAddress(procId, L"ShooterGame.exe");

	std::thread		overlayLoop(OverlayLoop);

	while(1)
	{
		/*	update all of the game info		*/
		updateInfo(moduleBase);

		handleMiscCommands(Globals->processHandle, localGameInfo->localPawn);
		Aimbot(localGameInfo->actorCount);
	}



	std::cin.get();

	return 0;
}