#include "Math.h"



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

vector3D	calcAngle(vector3D	pos1, vector3D	pos2)
{
	vector3D	delta = pos1 - pos2;

	vector3D	result;
	result.x = atanf(delta.x/delta.y);

	float XYdist = sqrt(pow(delta.x, 2) + pow(delta.y, 2));

	result.y = atanf(delta.z / XYdist);

	return result;
}