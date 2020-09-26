#pragma once
#include "globals.h"

namespace Offsets
{
	/*	general things	*/

	extern	DWORD64		GWORLD;
	extern  DWORD64		gameInstance;
	extern  DWORD64		persistentLevel;
	extern  DWORD64		localPlayers;
	extern  DWORD64		actorsArray;
	extern  DWORD64		actorCount;


	/*	Actor specific		*/

	extern  DWORD64		playerController;
	extern  DWORD64		APawn;
	extern  DWORD64		RootComponent;


	/*	RootComponent specific	*/

	extern	DWORD64		componentToWorld;
	extern	DWORD64		Translation;	/*	(x, y, z)	*/


	/*	game pawn specific	*/
	
	extern  DWORD64		pawnHealth;
	extern  DWORD64		CurrentWeapon;
	extern  DWORD64		RunningSpeedModifier;
	extern  DWORD64		IsTargeting;    // this is an int, & 1 to get the status
	

	/*	weapon specific		*/
	extern  DWORD64		currentAmmo;
	extern	DWORD64		LastFireTime;

	/*	camera Offsets	*/
	extern	DWORD64		CameraManager;
	extern	DWORD64		cameraCachePrivate;
	extern	DWORD64		POV;
	extern	DWORD64		cameraVector;
	extern	DWORD64		cameraRotation;
	extern	DWORD64		cameraFOV;
}
