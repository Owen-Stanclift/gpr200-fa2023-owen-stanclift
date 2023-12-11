////Glenn Sakowicz
#include "terrain.h"
#include "Perlin.h" 

Terrain::Terrain(int gridSize, float stepSize) : gridSize(gridSize), stepSize(stepSize) 
{
    generateTerrain();
}

Terrain::~Terrain() {}

void Terrain::generateTerrain() 
{
    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            float x = i * stepSize;
            float y = j * stepSize;
            float z = generateTerrainHeight(x, y);

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }

    for (int i = 0; i < gridSize - 1; ++i) 
        for (int j = 0; j < gridSize - 1; ++j) 
        {
            int topLeft = i * gridSize + j;
            int topRight = topLeft + 1;
            int bottomLeft = (i + 1) * gridSize + j;
            int bottomRight = bottomLeft + 1;

            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    
}

float Terrain::generateTerrainHeight(float x, float y) 
{
    float scale = 0.1f;
    PerlinNoise perlin;
    return perlin.noise(x * scale, y * scale) * 5.0f;
}

std::vector<float> Terrain::getVertices() const 
{
    return vertices;
}

std::vector<unsigned int> Terrain::getIndices() const 
{
    return indices;
}