#pragma once
#include "../ew/mesh.h"

namespace myLib
{
	ew::MeshData createSphere(float radius, int numSegments);
	ew::MeshData createCylinder(float height, float radius, int numSegments);
	ew::MeshData createPlane(float size, int subdivisions);
	ew::MeshData createFire(float radius, int numSegments,float strength);
}