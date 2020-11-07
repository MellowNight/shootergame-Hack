#pragma once
#include "utils.h"
#include "EngineClasses.h"




void	formattedPrint(std::ofstream& file, FName fname, TArray<TCHAR>	charsArray, int i)
{
	file << "[" << std::right << std::dec << std::setw(6) << std::setfill('0')
		<< i << std::setfill(' ') << "] " << std::left << std::setw(70) << Utils::ws2s(charsArray.Data)
		<< std::dec << " Object ID: " << *(DWORD64*)(&fname)
		<< std::endl;

}


DWORD64 findOffset(const wchar_t* className, const wchar_t*	fieldName)
{

	FUObjectItem* objectArray = (*GObjects->ObjObjects.Objects);


	for (int i = 0; i < GObjects->ObjObjects.NumElements; i++)
	{
		if (!objectArray[i].Object)
		{
			continue;
		}

		TArray<TCHAR> charArray1 = objectArray[i].Object->Name.ToString();


		if (!wcscmp(className, charArray1.Data))
		{
			FField* fieldEntry;

			FField* firstEntry = (FField*)(((UStruct*)objectArray[i].Object)->childProperties);


			for (fieldEntry = firstEntry->Next;
				!IsBadReadPtr((void*)fieldEntry, 8);
				fieldEntry = fieldEntry->Next)
			{

				DWORD		offset_internal = ((FProperty*)fieldEntry)->Offset_Internal;
				FName		fName = fieldEntry->NamePrivate;


				FString   propertyName;
				TArray<TCHAR> charArray = objectArray[i].Object->Name.ToString();

				if (!wcscmp(charArray.Data, fieldName))
				{
					return offset_internal;
				}

				if (firstEntry == fieldEntry)
				{
					break;
				}
			}

		}
	}

	return 0x9999999999;	/*	custom error code	*/
}




DWORD64 dump_class(std::ofstream&	file, const wchar_t* className)
{

	FUObjectItem* objectArray = (*GObjects->ObjObjects.Objects);


	for (int i = 0; i < GObjects->ObjObjects.NumElements; i++)
	{
		if (!objectArray[i].Object)
			continue;


		FString	objName;
		TArray<TCHAR> charArray1 = objectArray[i].Object->Name.ToString();

		if (!wcscmp(className, charArray1.Data))
		{

			file << "\n Class " << Utils::ws2s(charArray1.Data) << ": \n";

			FField* fieldEntry;

			FField* firstEntry = (FField*)(((UStruct*)objectArray[i].Object)->childProperties);

			if (!IsBadReadPtr((void*)firstEntry, 8))
			{
				for (fieldEntry = firstEntry->Next;
					!IsBadReadPtr((void*)fieldEntry, 8);
					fieldEntry = fieldEntry->Next)
				{

					DWORD	offset_internal = ((FProperty*)fieldEntry)->Offset_Internal;
					FName	fName = fieldEntry->NamePrivate;

					TArray<TCHAR> charArray = fName.ToString();


					file << "\t [+]  " << Utils::ws2s(charArray1.Data) << "->" << std::left << std::setw(60) << Utils::ws2s(charArray.Data)
						<< "Offset:  0x" << std::hex << offset_internal << std::endl;

					file.flush();

					if (firstEntry == fieldEntry)
					{
						break;
					}
				}
			}
		}
	}

	return 0x9999999999;	/*	custom error code	*/
}



void	dumpOffsets()
{
	std::ofstream file{ "OffsetDump.txt" };


	dump_class(file, L"World");
	dump_class(file, L"Default__GameInstance");
	dump_class(file, L"Pickup_Health_C");

	file.close();
}