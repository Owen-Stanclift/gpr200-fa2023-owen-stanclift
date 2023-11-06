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
				v.pos.x = radius * cos(theta) * sin(phi);
				v.pos.y = radius * cos(phi);
				v.pos.z = radius * sin(theta) * sin(phi);
				v.normal = (ew::Vec3(v.pos));
				v.uv = ew::Vec2({(float) col/numSegments, (float) row/numSegments});
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

				mesh.indices.push_back(start + 1);
				mesh.indices.push_back(start + columns + 1);
				mesh.indices.push_back(start + columns);
			}
		}
		poleStart = (numSegments * numSegments) + numSegments;
		sideStart = (numSegments * numSegments) - 1;
		for (i = 0; i < numSegments; i++)
		{
			mesh.indices.push_back(sideStart + i + 1);
			mesh.indices.push_back(poleStart + i);
			mesh.indices.push_back(sideStart + i);
		}

		return mesh;
	}
	ew::MeshData createCylinder(float height, float radius, int numSegments)
	{
		ew::MeshData mesh;
		ew::Vertex v;
		float thetaStep = (2 * ew::PI) / numSegments;

		int start = 1;
		float topY = height/2;

		mesh.vertices.push_back({ ew::Vec3(0,topY,0) });

		for (int i = 0; i <= numSegments; i++) //Top ring up
		{
			float theta = i * thetaStep;
			v.pos.x = cos(theta) * radius;
			v.pos.z = sin(theta) * radius;
			v.pos.y = topY;
			v.normal = (ew::Vec3(0, 1, 0));
			v.uv = ew::Vec2{ (float) (0.5*sin(theta)+0.5), (float)(0.5 * cos(theta) + 0.5) };
			mesh.vertices.push_back(v);
		}



		for (int i = 0; i <= numSegments; i++)
		{
			mesh.indices.push_back(start + i);
			mesh.indices.push_back(0);
			mesh.indices.push_back(start + i + 1);
		}
		for (int i = 0; i <= numSegments; i++)  //Top ring forward
		{
			float theta = i * thetaStep;
			v.pos.x = cos(theta) * radius;
			v.pos.z = sin(theta) * radius;
			v.pos.y = topY;
			v.normal = (ew::Vec3(cos(theta), 0, sin(theta)));
			v.uv = ew::Vec2{ (float)i / numSegments, 1.0f };
			mesh.vertices.push_back(v);
		}

		float bottomY = -topY;
		for (int i = 0; i <= numSegments; i++) //Bottom ring forward
		{
			float theta = i * thetaStep;

			v.pos.x = cos(theta) * radius;
			v.pos.z = sin(theta) * radius;
			v.pos.y = bottomY;
			v.normal = (ew::Vec3(cos(theta), 0, sin(theta)));
			v.uv = ew::Vec2{(float)i / numSegments, 0.0f };
			mesh.vertices.push_back(v);
		}
	
		for (int i = 0; i <= numSegments; i++) //Bottom ring down
		{
			float theta = i * thetaStep;

			v.pos.x = cos(theta) * radius;
			v.pos.z = sin(theta) * radius;
			v.pos.y = bottomY;
			v.normal = (ew::Vec3(0, -1, 0));
			v.uv = ew::Vec2{ (float)(0.5 * sin(theta) + 0.5), (float)(0.5 * cos(theta) + 0.5) };
			mesh.vertices.push_back(v);
		}
		mesh.vertices.push_back({ ew::Vec3(0,bottomY,0) });
		for (int i = 0; i <= numSegments; i++)
		{
			start = mesh.vertices.size()-2-numSegments;
			mesh.indices.push_back(start + i+1);
			mesh.indices.push_back(mesh.vertices.size() - 1);
			mesh.indices.push_back(start + i);
		}
		
		int sideStart = (numSegments)+1;
		int columns = numSegments+1;
		for (int i = 0; i < columns; i++)
		{
			int start = sideStart + i;
			mesh.indices.push_back(start);
			mesh.indices.push_back(start + 1);
			mesh.indices.push_back(start + columns);
			mesh.indices.push_back(start + 1);
			mesh.indices.push_back(start + columns + 1);
			mesh.indices.push_back(start + columns);
			mesh.vertices.push_back(v);

		}


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
				v.pos.x = size *  ((float)col / subdivisions);
				v.pos.z = -size *  ((float)row / subdivisions);
				v.normal = (ew::Vec3(0,1,0));
				v.uv = (ew::Vec2((float) col / subdivisions, (float) row / subdivisions));
				mesh.vertices.push_back(v);
			}

		}
		for (row = 0; row < subdivisions; row++)
		{
			for (col = 0; col < subdivisions; col++)
			{
				int start = row * columns + col;
				mesh.indices.push_back(start);
				mesh.indices.push_back(start + 1);
				mesh.indices.push_back(start + columns + 1);

				mesh.indices.push_back(start + columns + 1);
				mesh.indices.push_back(start + columns);
				mesh.indices.push_back(start);
			}

		}

		return mesh;
	}
}