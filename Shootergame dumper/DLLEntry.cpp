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
				continue;

			FString	objName;

			fnameToString(&(objectArray[i].Object->Name), &objName);

			TArray<TCHAR> charsArray = objName.DataType;
			*(TCHAR*)(charsArray.Data + charsArray.Num-1) = '\0';


			file << "[" << std::right << std::dec << std::setw(6) << std::setfill('0')
				<< i << std::setfill(' ') << "] " << std::left << std::setw(70) << Utils::ws2s(charsArray.Data)
				<< std::dec << " Object ID: " << *(DWORD64*)(&objectArray[i].Object->Name)
				<< std::endl;

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
	fnameToString = (pFNameToString)(moduleBase + Offsets::fnameToString);


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
