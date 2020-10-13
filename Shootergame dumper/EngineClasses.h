#pragma once

template <class T>
struct TArray
{
	T*      Data;
	int32_t Num;
	int32_t Max;
};

struct FName
{
	int32_t Index;
	int32_t Number;
};

struct UClass;

struct UObject
{
	void*    VTableObject;
	int32_t  ObjectFlags;
	int32_t  InternalIndex;
	UClass*  ClassPrivate;
	FName    Name;
	UObject* Outer;
};

struct FField
{
	PVOID		vTable;
	PVOID		Owner;
	FField*		Next;
	FName		NamePrivate;
	int32_t		FlagsPrivate;
};


struct FProperty : public FField
{
	int32_t		ArrayDim;
	int32_t		ElementSize;
	DWORD64		PropertyFlags;
	uint16_t	RepIndex;
	uint8_t		TEnumAsByte;
	int32_t		Offset_Internal;

};


struct UField : public UObject
{
	UField* Next;
};

struct UStruct : public UField
{
	UStruct*		SuperStruct;
	UField*			Children;
	PVOID			childProperties;
	int32_t			PropertySize;
	int32_t			MinAlignment;
	TArray<char>	Script;
	FProperty*		PropertyLink;
	FProperty*		RefLink;
	FProperty*		DestructorLink;
	FProperty*		PostConstructLink;
	TArray<UObject*> ScriptAndPropertyObjectReferences;
};

struct UClass : public UStruct
{
	UCHAR unknowndata_00[0x456];
};

struct FUObjectItem
{
	UObject* Object;
	int32_t  Flags;
	int32_t  ClusterIndex;
	int32_t  SerialNumber;
	UCHAR    unknowndata_00[0x4];
};

struct FChunkedFixedUObjectArray
{
	FUObjectItem** Objects;
	DWORD64		   PreAllocatedObjects;
	int32_t        MaxElements;
	int32_t        NumElements;
};

struct FUObjectArray
{
	int32_t        ObjFirstGCIndex;
	int32_t        ObjLastNonGCIndex;
	int32_t        MaxObjectsNotConsideredByGC;
	int32_t        OpenForDisregardForGC;
	FChunkedFixedUObjectArray* ObjObjects;
};

struct FNameEntry
{
	int32_t     Index;
	UCHAR       unknowndata_00[0x4];
	FNameEntry* HashNext;

	union
	{
		UCHAR AnsiName[1024];
		WCHAR WideName[1024];
	};
};

template <typename ElementType, int32_t MaxTotalElements, int32_t ElementsPerChunk>
class TStaticIndirectArrayThreadSafeRead
{
public:
	int32_t Num() const
	{
		return numElements;
	}

	bool IsValidIndex(const int32_t index) const
	{
		return index >= 0 && index < Num() && GetById(index) != nullptr;
	}

	ElementType const* const& GetById(int32_t index) const
	{
		return *GetItemPtr(index);
	}

private:
	ElementType const* const* GetItemPtr(const int32_t Index) const
	{
		const auto ChunkIndex       = Index / ElementsPerChunk;
		const auto WithinChunkIndex = Index % ElementsPerChunk;
		const auto Chunk            = chunks[ChunkIndex];

		return Chunk + WithinChunkIndex;
	}

	enum
	{
		ChunkTableSize = (MaxTotalElements + ElementsPerChunk - 1) / ElementsPerChunk
	};

	ElementType** chunks[ChunkTableSize];
	int32_t       numElements;
	int32_t       numChunks;
};

using TNameEntryArray = TStaticIndirectArrayThreadSafeRead<FNameEntry, 2 * 1024 * 1024, 16384>;


struct FString
{
	TArray<TCHAR> DataType;
};