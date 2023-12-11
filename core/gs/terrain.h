#pragma once

#include <vector>

class Terrain {
public:
    Terrain(int gridSize, float stepSize);
    ~Terrain();

    void generateTerrain();
    float generateTerrainHeight(float x, float y);

    std::vector<float> getVertices() const;
    std::vector<unsigned int> getIndices() const;

private:
    int gridSize;
    float stepSize;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};