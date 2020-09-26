#include "ESP.h"
#include "Render.h"


vMatrix matrix;
vMatrix Matrix(vector3D rot, vector3D origin)
{
	origin = vector3D(0, 0, 0);
	float radPitch = (rot.x * float(PI) / 180.f);
	float radYaw = (rot.y * float(PI) / 180.f);
	float radRoll = (rot.z * float(PI) / 180.f);

	float SP = sinf(radPitch);
	float CP = cosf(radPitch);
	float SY = sinf(radYaw);
	float CY = cosf(radYaw);
	float SR = sinf(radRoll);
	float CR = cosf(radRoll);


	matrix[0][0] = CP * CY;
	matrix[0][1] = CP * SY;
	matrix[0][2] = SP;
	matrix[0][3] = 0.f;

	matrix[1][0] = SR * SP * CY - CR * SY;
	matrix[1][1] = SR * SP * SY + CR * CY;
	matrix[1][2] = -SR * CP;
	matrix[1][3] = 0.f;

	matrix[2][0] = -(CR * SP * CY + SR * SY);
	matrix[2][1] = CY * SR - CR * SP * SY;
	matrix[2][2] = CR * CP;
	matrix[2][3] = 0.f;

	matrix[3][0] = origin.x;
	matrix[3][1] = origin.y;
	matrix[3][2] = origin.z;
	matrix[3][3] = 1.f;

	return matrix;
}



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


int ESPLogic()
{


	for (int i = 0; i < localGameInfo->actorCount; ++i)
	{

		currentActor = Commands::read<DWORD64>(Globals->processHandle, (localGameInfo->actorList + 0x8 * i));


		rootComponent = Commands::read<DWORD64>(Globals->processHandle, (currentActor + Offsets::RootComponent));

		xyz = Commands::read<vector3D>(Globals->processHandle, (rootComponent + Offsets::Translation));

		cameraManager = Commands::read<DWORD64>(Globals->processHandle, (localGameInfo->playerController + Offsets::CameraManager));

		cameraLocation = Commands::read<vector3D>(Globals->processHandle, (cameraManager + Offsets::cameraCachePrivate  + Offsets::POV + 
			Offsets::cameraVector));


		cameraAngle = Commands::read<vector3D>(Globals->processHandle, 
			(cameraManager + Offsets::cameraCachePrivate + Offsets::POV + Offsets::cameraRotation));

		cameraFOV = Commands::read<float>(Globals->processHandle, (cameraManager + Offsets::cameraCachePrivate + Offsets::POV + Offsets::cameraFOV));



		vector2D	screenCoords(0, 0);

		bool hi = WorldToScreen(xyz, &screenCoords, cameraAngle, cameraLocation, cameraFOV);

		Render::DrawBox(ImColor(0.25, 0.15, 0.60, 1.0f), screenCoords.x, screenCoords.y, 30, 30);
	}

	return 0;
}

int ESP()
{

	ESPLogic();

	return 0;
}

