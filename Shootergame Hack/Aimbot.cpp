#include "Aimbot.h"
#include "Menu.h"
#include "ESP.h"
#include "globals.h"


void aimAt(float x, float y)
{
	float	ScreenCenterX = (Globals->windowWidth / 2);
	float	ScreenCenterY = (Globals->windowHeight / 2);
	int		AimSpeed = menuControl->aimSmooth;
	float	TargetX = 0;
	float	TargetY = 0;

	if (x != 0)
	{
		if (x > ScreenCenterX)
		{
			TargetX = -(ScreenCenterX - x);
			TargetX /= AimSpeed;
			if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
		}

		if (x < ScreenCenterX)
		{
			TargetX = x - ScreenCenterX;
			TargetX /= AimSpeed;
			if (TargetX + ScreenCenterX < 0) TargetX = 0;
		}
	}

	if (y != 0)
	{
		if (y > ScreenCenterY)
		{
			TargetY = -(ScreenCenterY - y);
			TargetY /= AimSpeed;
			if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
		}

		if (y < ScreenCenterY)
		{
			TargetY = y - ScreenCenterY;
			TargetY /= AimSpeed;
			if (TargetY + ScreenCenterY < 0) TargetY = 0;
		}
	}

	//WriteAngles(TargetX / 3.5f, TargetY / 3.5f);
	mouse_event(MOUSEEVENTF_MOVE, static_cast<DWORD>(TargetX), static_cast<DWORD>(TargetY), NULL, NULL);

	return;
}


bool isValidTarget = true;

int ix;
int iy;
void mouseThread()
{
	while (1)
	{

		if (menuControl->aimbot == true)
		{
			vector2D	windowCenter;
			windowCenter.x = Globals->positionX + (Globals->windowWidth / 2);
			windowCenter.y = Globals->positionY + (Globals->windowHeight / 2);

			float screenDistance;

			screenDistance = windowCenter.distance(vector2D(ix, iy));

			if (GetAsyncKeyState(VK_RBUTTON))
			{
				//std::this_thread::sleep_for(std::chrono::microseconds(1));

				if (ix != 0 && iy != 0 && screenDistance < (menuControl->FOVcircleRadius + 15))
				{
					isValidTarget = true;
					aimAt(ix, iy);
				}
				else
				{
					isValidTarget = false;
				}
			}
		}
	}
}


DWORD64		aCurrentActor;
DWORD64		aRootComponent;
vector3D	axyz; 
PVOID		aCameraManager;
vector3D	aCameraAngle;
vector3D	aCameraLocation;
float		aCameraFOV;

int mouseAimbot()
{
	std::thread thread(mouseThread);

	while (1)
	{
		if (menuControl->aimbot == true)
		{
			for (int i = 0; i < localGameInfo->actorCount; ++i)
			{
				aCurrentActor = Driver::read<DWORD64>(Globals->processHandle, (localGameInfo->actorList + 0x8 * i));

				int actorID = Driver::read<DWORD64>(Globals->processHandle, (aCurrentActor + 0x18));


				if (actorID == localGameInfo->localactorId || actorID == 526317 || actorID == 526275)
				{
					int health = Driver::read<float>(Globals->processHandle, (aCurrentActor + Offsets::pawnHealth));

					for (volatile int ii = 0; ii < 50000; ++ii)
					{
						if (health > 0)
						{
							health = Driver::read<float>(Globals->processHandle, (aCurrentActor + Offsets::pawnHealth));

							aRootComponent = Driver::read<DWORD64>(Globals->processHandle, (aCurrentActor + Offsets::RootComponent));

							axyz = Driver::read<vector3D>(Globals->processHandle, (aRootComponent + Offsets::Translation));

							aCameraManager = Driver::read<PVOID64>(Globals->processHandle, (localGameInfo->playerController + Offsets::CameraManager));

							aCameraAngle = Driver::read<vector3D>(Globals->processHandle,
								((DWORD64)aCameraManager + Offsets::cameraCachePrivate + Offsets::POV + Offsets::cameraRotation));

							aCameraLocation = Driver::read<vector3D>(Globals->processHandle,
								((DWORD64)aCameraManager + Offsets::cameraCachePrivate + Offsets::POV + Offsets::cameraVector));

							aCameraFOV = Driver::read<float>(Globals->processHandle,
								((DWORD64)aCameraManager + Offsets::cameraCachePrivate + Offsets::POV + Offsets::cameraFOV));


							axyz.z += 65;

							vector2D  screenLocation;
							WorldToScreen(axyz, &screenLocation, aCameraAngle, aCameraLocation, aCameraFOV);
							
							ix = screenLocation.x;
							iy = screenLocation.y;

							if (isValidTarget == false)
							{
								ii = 49999;
							}
						}
						else
						{
							ii = 49999;
						}
					}
				}
			}
		}
	}

	return 0;
}