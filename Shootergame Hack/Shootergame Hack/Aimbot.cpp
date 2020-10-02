#include "Aimbot.h"
#include "Menu.h"

struct AimInfo
{
	vector3D	position;
	float		distance;

	AimInfo(vector3D v)
	{
		position = v;
		distance = v.distance(localGameInfo->position);
	}
};





bool sortPolicy(AimInfo a, AimInfo b) { return (a.distance < b.distance); }

DWORD64		aCurrentActor;
DWORD64		aRootComponent;
DWORD64		aCameraManager;
vector3D	aCameraLocation;
vector3D	aCameraAngle;
float		aCameraFOV;
vector3D	axyz;

void aimAt(AimInfo	aimInfo)
{
	vector3D angle = calcAngle(aimInfo.position, localGameInfo->position);

	angle.x = 300.f;
	angle.y = 240.f;

	Commands::write<vector3D>(Globals->processHandle,
		(aCameraManager + Offsets::cameraCachePrivate + Offsets::POV + Offsets::cameraRotation), &angle);
}

int Aimbot()
{
	while (1)
	{
		std::vector<AimInfo>	aimList;

		for (int i = 0; i < localGameInfo->actorCount; ++i)
		{
			aCurrentActor = Commands::read<DWORD64>(Globals->processHandle, (localGameInfo->actorList + 0x8 * i));

			int actorID = Commands::read<DWORD64>(Globals->processHandle, (aCurrentActor + 0x18));

			aCameraManager = Commands::read<DWORD64>(Globals->processHandle, (localGameInfo->playerController + Offsets::CameraManager));

			aCameraLocation = Commands::read<vector3D>(Globals->processHandle, (aCameraManager + Offsets::cameraCachePrivate + Offsets::POV +
				Offsets::cameraVector));


			if (actorID == localGameInfo->localactorId || actorID == localGameInfo->localactorId + 200728 || actorID == 526275)
			{
				aRootComponent = Commands::read<DWORD64>(Globals->processHandle, (aCurrentActor + Offsets::RootComponent));

				axyz = Commands::read<vector3D>(Globals->processHandle, (aRootComponent + Offsets::Translation));


				aCameraAngle = Commands::read<vector3D>(Globals->processHandle,
					(aCameraManager + Offsets::cameraCachePrivate + Offsets::POV + Offsets::cameraRotation));

				aCameraFOV = Commands::read<float>(Globals->processHandle, (aCameraManager + Offsets::cameraCachePrivate + Offsets::POV + Offsets::cameraFOV));
				int health = Commands::read<float>(Globals->processHandle, (aCurrentActor + Offsets::pawnHealth));


				if (health > 0)
				{
					if (aRootComponent != localGameInfo->rootComponent)
					{
						aimList.push_back(AimInfo(axyz));
					}
				}
			}
		}


		std::sort(aimList.begin(), aimList.end(), sortPolicy);

		if (menuControl->aimbot == true)
		{
			if (GetAsyncKeyState(VK_RBUTTON))
			{
				aimAt(aimList[0]);
			}
		}
	}
	return 0;
}