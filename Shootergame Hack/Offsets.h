#pragma once
#include "globals.h"

namespace Offsets
{
	enum Offset : DWORD64
	{
		/*	general things	*/

		GWORLD = 0x83039B8,
		gameInstance = 0x188,
		persistentLevel = 0x30,
		localPlayers = 0x38,
		actorsArray = 0x98,
		actorCount = 0xA0,
		objectID = 0x18,

		/*	Actor specific		*/

		playerController = 0x30,
		APawn = 0x250,
		RootComponent = 0x130,


		/*	RootComponent specific	*/

		componentToWorld = 0x1C0,
		Translation = 0x1D0,	/*	(x, y, z)	*/


		/*	game pawn specific	*/

		pawnHealth = 0x69C,
		CurrentWeapon = 0x4E8,
		RunningSpeedModifier = 0x624,
		IsTargeting = 0x620,    // this is an int, & 1 to get the status


		/*	weapon specific		*/
		currentAmmo = 0x474,


		/*	Camera	Offsets		*/
		CameraManager = 0x2B8,
		cameraCachePrivate = 0x1A60,
		POV = 0x10,
		cameraVector = 0x0,
		cameraRotation = 0xC,
		cameraFOV = 0x18
	};
}