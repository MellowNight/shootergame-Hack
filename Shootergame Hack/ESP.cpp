#include "ESP.h"
#include "Render.h"
#include "Menu.h"
#include "Aimbot.h"



bool WorldToScreen(vector3D world, vector2D* screen, vector3D Rotation, vector3D cameraPosition, float FovAngle) {

	vector3D Screenlocation = vector3D(0, 0, 0);

	vMatrix tempMatrix = Matrix(Rotation, Screenlocation);

	vector3D vAxisX, vAxisY, vAxisZ;

	vAxisX = vector3D(tempMatrix[0][0], tempMatrix[0][1], tempMatrix[0][2]);
	vAxisY = vector3D(tempMatrix[1][0], tempMatrix[1][1], tempMatrix[1][2]);
	vAxisZ = vector3D(tempMatrix[2][0], tempMatrix[2][1], tempMatrix[2][2]);

	vector3D vDelta = world - cameraPosition;
	vector3D vTransformed = vector3D(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

	if (vTransformed.z < 1.f)
		vTransformed.z = 1.f;


	float ScreenCenterX = Globals->windowWidth / 2.0f;
	float ScreenCenterY = Globals->windowHeight / 2.0f;

	auto tmpFOV = tanf(FovAngle * (float)PI / 360.f);

	screen->x = ScreenCenterX + vTransformed.x * (ScreenCenterX / tmpFOV) / vTransformed.z;
	screen->y = ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(FovAngle * (float)PI / 360.f)) / vTransformed.z;


	return true;
}




DWORD64		currentActor;
DWORD64		rootComponent;
DWORD64		cameraManager;
vector3D	cameraLocation;
vector3D	cameraAngle;
float		cameraFOV;
vector3D	xyz;


int ESP(bool players, bool items, bool showActorID, bool showFOVcircle)
{

	for (int i = 0; i < localGameInfo->actorCount; ++i)
	{
		currentActor = Driver::read<DWORD64>(Globals->processHandle, (localGameInfo->actorList + 0x8 * i));

		int actorID = Driver::read<DWORD64>(Globals->processHandle, (currentActor + 0x18));

		rootComponent = Driver::read<DWORD64>(Globals->processHandle, (currentActor + Offsets::RootComponent));

		xyz = Driver::read<vector3D>(Globals->processHandle, (rootComponent + Offsets::Translation));

		cameraManager = Driver::read<DWORD64>(Globals->processHandle, (localGameInfo->playerController + Offsets::CameraManager));

		cameraLocation = Driver::read<vector3D>(Globals->processHandle, (cameraManager + Offsets::cameraCachePrivate + Offsets::POV +
			Offsets::cameraVector));

		cameraAngle = Driver::read<vector3D>(Globals->processHandle,
			(cameraManager + Offsets::cameraCachePrivate + Offsets::POV + Offsets::cameraRotation));

		cameraFOV = Driver::read<float>(Globals->processHandle, (cameraManager + Offsets::cameraCachePrivate + Offsets::POV + Offsets::cameraFOV));
		int health = Driver::read<float>(Globals->processHandle, (currentActor + Offsets::pawnHealth));



		vector2D	screenCoords(0, 0);
		bool hi = WorldToScreen(xyz, &screenCoords, cameraAngle, cameraLocation, cameraFOV);

		if (hi)
		{
			if (players)
			{
				/* 526275 for enemy, 325547 for local */
				if (actorID == 526317)
				{
					if (health > 0)
					{
						float distance = localGameInfo->position.distance(xyz);

						Render::DrawBox(ImColor(menuControl->ESPcolor[0], menuControl->ESPcolor[1], menuControl->ESPcolor[2], 1.0f),
							screenCoords.x - (35000/distance), screenCoords.y - (45000/distance), 45000 / distance, 110000 / distance);


						Render::Line(ImVec2(screenCoords.x - (35000 / distance), screenCoords.y - (45000 / distance) + (110000 / distance) - (2200 * health/distance)),
							ImVec2(screenCoords.x - (35000 / distance), screenCoords.y - (45000 / distance) + (110000 / distance)), ImColor(0, 255, 0), 4);
					}
				}
			}

			if (showActorID)
			{
				std::stringstream	ss;
				ss << std::dec << actorID;
				Render::EasyText(ImVec2(screenCoords.x, screenCoords.y), ImColor(menuControl->ESPcolor[0], menuControl->ESPcolor[1],
					menuControl->ESPcolor[2], 255.0f), ss.str().c_str(), 1100);
			}

			/*	899147 for rockets, 457714 for health	*/
			if (items)
			{
				switch (actorID)
				{
				case 457655:
					Render::EasyText(ImVec2(screenCoords.x, screenCoords.y), ImColor(menuControl->ESPcolor[0], menuControl->ESPcolor[1],
						menuControl->ESPcolor[2], 255.0f), "Rockets", 2000);
					break;
				case 457713:
					Render::EasyText(ImVec2(screenCoords.x, screenCoords.y), ImColor(menuControl->ESPcolor[0], menuControl->ESPcolor[1],
						menuControl->ESPcolor[2], 255.0f), "Health kit", 2000);
					break;
				case 457591:
					Render::EasyText(ImVec2(screenCoords.x, screenCoords.y), ImColor(menuControl->ESPcolor[0], menuControl->ESPcolor[1],
						menuControl->ESPcolor[2], 255.0f), "Bullets", 2000);
					break;
				default:
					break;
				}
			}

			if (showFOVcircle)
			{
				vector2D windowCenter;
				windowCenter.x = Globals->positionX + (Globals->windowWidth / 2);
				windowCenter.y = Globals->positionY + (Globals->windowHeight / 2);

				Render::Circle(ImVec2(windowCenter.x, windowCenter.y), ImColor(menuControl->ESPcolor[0], menuControl->ESPcolor[1], 
					menuControl->ESPcolor[2], 255.0f), menuControl->FOVcircleRadius, 50, 1);
			}
		}
	}

	return 0;
}

