#include "Offsets.h"


/*	general things	*/

DWORD64		Offsets::GWORLD = 0x83039B8;
DWORD64		Offsets::gameInstance = 0x188;
DWORD64		Offsets::persistentLevel = 0x30;
DWORD64		Offsets::localPlayers = 0x38;
DWORD64		Offsets::actorsArray = 0x98;
DWORD64		Offsets::actorCount = 0xA0;
DWORD64		Offsets::objectID = 0x18;


/*	Actor specific		*/

DWORD64		Offsets::playerController = 0x30;
DWORD64		Offsets::APawn = 0x250;
DWORD64		Offsets::RootComponent = 0x130;

/*	RootComponent specific	*/
DWORD64		Offsets::componentToWorld = 0x1C0;
DWORD64		Offsets::Translation = 0x1D0;


/*	game pawn specific	*/

DWORD64		Offsets::pawnHealth = 0x69C;
DWORD64		Offsets::CurrentWeapon = 0x4E8;
DWORD64		Offsets::RunningSpeedModifier = 0x624;
DWORD64		Offsets::IsTargeting = 0x620;    // this is an int, & 1 to get the status


/*	weapon specific		*/
DWORD64		Offsets::currentAmmo = 0x474;


/*	Camera	Offsets		*/
DWORD64		Offsets::CameraManager = 0x2B8;
DWORD64		Offsets::cameraCachePrivate = 0x1A60;
DWORD64		Offsets::POV = 0x10;
DWORD64		Offsets::cameraVector = 0x0;
DWORD64		Offsets::cameraRotation = 0xC;
DWORD64		Offsets::cameraFOV = 0x18;



