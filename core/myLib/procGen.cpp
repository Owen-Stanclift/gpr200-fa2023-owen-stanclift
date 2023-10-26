#include "procGen.h"


ew::MeshData createShere(float radius, int numSegments)
{
	ew::MeshData mesh;

	return mesh;
}
ew::MeshData createCylinder(float height, float radius, int numSegments)
{
	ew::MeshData mesh;
	ew::Vertex v;
	float topY = height / 2;
	float bottomY = -topY;

	mesh.vertices.push_back({ 0,topY,0 });
	mesh.vertices.push_back({ topY});
	mesh.vertices.push_back({bottomY});
	mesh.vertices.push_back({ 0,bottomY,0 });

	float thetaStep = (2 * ew::PI)/numSegments;

	for (int i = 0; i <= numSegments; i++)
	{
		float theta = i * thetaStep;

		v.pos.x = cos(theta) * radius;
		v.pos.z = sin(theta) * radius;
		v.pos.y = topY;
		mesh.vertices.push_back(v);
	}

	return mesh;
}
ew::MeshData createPlane(float size, float subdivisions)
{
	int row, col;
	ew::MeshData mesh;
	ew::Vertex v;

	float columns = subdivisions + 1;
	for (row = 0; row <= subdivisions; row++)
	{
		for (col = 0; col <= subdivisions; col++)
		{
			v.pos.x = size * (col / subdivisions);
			v.pos.z = -size * (row / subdivisions);
			mesh.vertices.push_back(v);

			int start = row * columns + col;
			mesh.indices.push_back(start);
			mesh.indices.push_back(start+1);
			mesh.indices.push_back(start+columns+1);
		}
	}
	return mesh;
}