
#include "utils.h"
#include "EngineClasses.h"

TNameEntryArray* GNames { };
FUObjectArray*   GObjects { };


/*	FName::ToString	*/
FString	getNameByFName(FName  name)
{

}


void dumpObjects()
{
	std::ofstream file {"GObjectsDump.txt"};

	if (file.is_open())
	{
		for (auto i {0}; i < GObjects->ObjObjects->NumElements; i++)
		{
			if (!GObjects->ObjObjects->Objects[i]->Object)
				continue;

			TArray<TCHAR> charsArray = (getNameByFName(GObjects->ObjObjects->Objects[i]->Object->Name).DataType.Data);

			*(TCHAR*)((DWORD64)charsArray.Data + charsArray.Num) = '\0';

			file << "[" << std::right << std::dec << std::setw(6) << std::setfill('0')
				<< i << std::setfill(' ') << "] " << std::left << std::setw(130)
				<< Utils::ws2s(charsArray.Data) << std::dec << " Object ID: " << *(DWORD*)(GObjects->ObjObjects->Objects[i]->Object + 0x18)
				<< std::endl;


			FProperty* propertyEntry;

			FProperty* firstEntry = (FProperty*)GObjects->ObjObjects->Objects[i]->Object->ClassPrivate->PropertyLink;


			for (propertyEntry = (FProperty*)firstEntry->Next;
				!IsBadReadPtr((void*)propertyEntry, sizeof(FProperty));
				propertyEntry = (FProperty*)propertyEntry->Next)
			{
				if (!GObjects->ObjObjects->Objects[i]->Object)
					continue;

				DWORD	offset_internal = propertyEntry->Offset_Internal;
				FName	fName = propertyEntry->NamePrivate;

		
				TArray<TCHAR> charArray = (getNameByFName(fName).DataType);
				
				*(TCHAR*)((DWORD64)charArray.Data + charArray.Num) = '\0';

				file << "Object Property " << Utils::ws2s(charArray.Data)  << " Property Offset: " <<
					std::hex <<	offset_internal << std::endl;


				if (firstEntry == propertyEntry)
				{
					break;
				}
			}
		}

		file.close();
	}
}



HRESULT onAttach(HMODULE hModule)
{
	GNames = *reinterpret_cast<TNameEntryArray**>(reinterpret_cast<uintptr_t>(
			Utils::getModuleInfo(L"UE4-Core-Win64-Shipping.dll").lpBaseOfDll) + 0x41EC58
	);
	GObjects = reinterpret_cast<FUObjectArray*>(reinterpret_cast<uintptr_t>(
		Utils::getModuleInfo(L"UE4-Core-Win64-Shipping.dll").lpBaseOfDll) + 0x2FD2F8
	);

	dumpObjects();

	std::ostringstream strMsgBox { };
	strMsgBox << "Finished dumping\n\nObjects Count: " << GObjects->ObjObjects->NumElements << "\nNames Count: " << GNames->Num();
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
