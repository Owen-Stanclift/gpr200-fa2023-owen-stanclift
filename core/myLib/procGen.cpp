#include "procGen.h"

namespace myLib
{
	ew::MeshData createSphere(float radius, int numSegments)
	{
		ew::MeshData mesh;
		ew::Vertex v;
		int i, row, col, start;
		float thetaStep = (2 * ew::PI) / numSegments;
		float phiStep = (ew::PI) / numSegments;

		int poleStart = 1;
		int sideStart = numSegments + 1;
		int columns = numSegments + 1;


	for (row = 0; row <= numSegments; row++)
	{
		float phi = row * phiStep;
		for (col = 0; col <= numSegments; col++)
		{
			float theta = col * thetaStep;
			v.pos.x = radius * sin(phi) * sin(theta);
			v.pos.y = radius * cos(phi);
			v.pos.z = radius * sin(phi) * cos(theta);
			mesh.vertices.push_back(v);
		}
	}

		for (i = 0; i < numSegments; i++)
		{
			mesh.indices.push_back(sideStart + i);
			mesh.indices.push_back(poleStart + i);
			mesh.indices.push_back(sideStart + i + 1);
		}

		for (row = 1; row < numSegments - 1; row++)
		{
			for (col = 0; col < numSegments; col++)
			{
				start = row * columns + col;

			mesh.indices.push_back(start);
			mesh.indices.push_back(start + 1);
			mesh.indices.push_back(start + columns);
		}
	}

	return mesh;
}
ew::MeshData createCylinder(float height, float radius, int numSegments)
{
	ew::MeshData mesh;
	ew::Vertex v;
	float thetaStep = (2 * ew::PI) / numSegments;
	
	
	float topY = height / 2;
	mesh.vertices.push_back({ 0,topY,0 });
	for (int i = 0; i <= numSegments; i++)
	{
		int start = 1;
		float theta = i * thetaStep;
		v.pos.x = cos(theta) * radius;
		v.pos.z = sin(theta) * radius;
		v.pos.y = topY;
		mesh.vertices.push_back(v);
		mesh.indices.push_back(start + i);
		mesh.indices.push_back(topY);
		mesh.indices.push_back(start + i + 1);
	}
	float bottomY = -topY;
	for (int i = 0; i <= numSegments; i++)
	{
		int start = numSegments;
		float theta = i * thetaStep;

		v.pos.x = cos(theta) * radius;
		v.pos.z = sin(theta) * radius;
		v.pos.y = bottomY;
		mesh.vertices.push_back(v);
		mesh.indices.push_back(start + i);
		mesh.indices.push_back(topY);
		mesh.indices.push_back(start + i + 1);
	}

	int sideStart = 1;
	int columns = numSegments + 1;

	for (int i = 0; i < columns; i++)
	{
		int start = sideStart + i;

		mesh.indices.push_back(start);
		mesh.indices.push_back(start + 1);
		mesh.indices.push_back(start+columns);
	}
	mesh.vertices.push_back({ 0,bottomY,0 });

		return mesh;
	}
	ew::MeshData createPlane(float size, int subdivisions)
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