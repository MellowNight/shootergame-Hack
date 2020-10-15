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



/*	needs fixing	*/
struct FField
{
	PVOID		vTable;
	PVOID		classPrivate;
	char		Owner[0x10];
	FField*		Next;
	FName		NamePrivate;
	int32_t		FlagsPrivate;
};



/*	needs fixing	*/
struct FProperty : public FField
{
	int32_t		ArrayDim;
	int32_t		ElementSize;
	DWORD64		PropertyFlags;
	uint16_t	RepIndex;
	uint8_t		TEnumAsByte;
	int32_t		Offset_Internal;

};

struct UStruct;

/*	needs fixing	*/
struct UObject
{
	void*	VTableObject;
	int32_t  ObjectFlags;
	int32_t  InternalIndex;
	UStruct* ClassPrivate;
	FName    Name;
	UObject* Outer;
};



struct UField : public UObject
{
	UField* Next;
};


/*	needs fixing	*/
struct UStruct : public UField
{
	char			inheritedPad[0x10];
	UStruct*		SuperStruct;
	UField*			Children;
	FField*			childProperties;
	int32_t			PropertySize;
	int32_t			MinAlignment;
	TArray<char>	Script;
	FProperty*		PropertyLink;
	FProperty*		RefLink;
	FProperty*		DestructorLink;
	FProperty*		PostConstructLink;
	TArray<UObject*> ScriptAndPropertyObjectReferences;
};


/*	needs fixing	*/
struct FUObjectItem
{
	UObject* Object;
	int32_t  Flags;
	int32_t  ClusterIndex;
	int32_t  SerialNumber;
	UCHAR    unknowndata_00[0x4];
};



/*	needs fixing	*/
struct FChunkedFixedUObjectArray
{
	FUObjectItem** Objects;
	DWORD64		   PreAllocatedObjects;
	int32_t        MaxElements;
	int32_t        NumElements;
};



/*	needs fixing	*/
struct FUObjectArray
{
	int32_t        ObjFirstGCIndex;
	int32_t        ObjLastNonGCIndex;
	int32_t        MaxObjectsNotConsideredByGC;
	int32_t        OpenForDisregardForGC;
	FChunkedFixedUObjectArray ObjObjects;
};



struct FString
{
	TArray<TCHAR> DataType;
};



FUObjectArray* GObjects{ };

DWORD64		moduleBase;

typedef		FString* (__fastcall* pFNameToString)(FName* This, FString* retstr);
pFNameToString	fnameToString;
