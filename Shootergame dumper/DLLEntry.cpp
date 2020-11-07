#include "Offsets.h"
#include "utils.h"
#include "EngineClasses.h"
#include "dump_offsets.h"

void dumpObjects()
{
	std::ofstream file {"GObjectsDump.txt"};

	FUObjectItem*	objectArray = (*GObjects->ObjObjects.Objects);

	if (file.is_open())
	{
		for (int i = 0; i < GObjects->ObjObjects.NumElements; i++)
		{
			if (!objectArray[i].Object)
			{
				continue;
			}

			TArray<TCHAR> charsArray = objectArray[i].Object->Name.ToString();

			formattedPrint(file, objectArray[i].Object->Name, charsArray, i);
		}

		file.close();
	}
}


HRESULT onAttach(HMODULE hModule)
{
	moduleBase = (DWORD64)Utils::getModuleInfo(L"ShooterGame-Win64-Shipping.exe").lpBaseOfDll;

	std::ostringstream MsgBox{ };
	MsgBox << "Module base " << moduleBase << std::endl;
	MessageBoxA(nullptr, MsgBox.str().c_str(), "UE4 Instance Logger", MB_OK);

	GObjects = (FUObjectArray*)(moduleBase + Offsets::GObjects);
	FnameToString = (pFNameToString)(moduleBase + Offsets::fnameToString);


	dumpObjects();
	dumpOffsets();


	std::ostringstream strMsgBox { };
	strMsgBox << "Finished dumping\n\nObjects Count: " << GObjects->ObjObjects.NumElements << "\n";
	MessageBoxA(nullptr, strMsgBox.str().c_str(), "UE4 Instance Logger", MB_OK);

	FreeLibraryAndExitThread(hModule, 0);
}



BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		CloseHandle(
			CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(onAttach), hModule, 0, nullptr)
		);
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
