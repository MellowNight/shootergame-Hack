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



int		main()
{
	int			procId = 0;

	Color(5);

	std::cout << "waiting for shooterGame " << std::endl;

	while (procId == 0)
	{
		procId = Utils::FindProcessId(L"ShooterGame-Win64-Shipping.exe");
	}

	std::cout << "shooterGame Found!" << std::endl;

	Globals->processID = procId;
	Globals->processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);

	Globals->moduleBase = Utils::GetModuleBaseAddress(procId, L"ShooterGame-Win64-Shipping.exe");
	updateInfo();

	std::thread		overlayLoop(OverlayLoop);
	std::thread		aimThread(mouseAimbot);

	while(1)
	{
		/*	update all of the game info		*/
		updateInfo();
		handleMiscCommands(Globals->processHandle, localGameInfo->localPawn);
	}

	std::cin.get();

	return 0;
}