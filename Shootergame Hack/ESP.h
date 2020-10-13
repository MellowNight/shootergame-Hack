#pragma once
#include "Render.h"
#include "Offsets.h"
#include "commands.h"
#include "Math.h"


int		ESP(bool players, bool items, bool ActorID, bool showFOVcircle);
bool	WorldToScreen(vector3D world, vector2D* screen, vector3D Rotation, vector3D cameraPosition, float FovAngle);