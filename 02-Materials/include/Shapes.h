#ifndef CLASS_SHAPES_H
#define CLASS_SHAPES_H

#include <vector>

// A simple 2D rhombus, each set of 3 numbers is the x, y, z, and r, g, b of a single vertice
extern float Rhombus[24];

// A simple cube, each set of 3 numbers is the x, y, z, and r, g, b of a single vertice
extern float Cube[48];

// A simple square pyramid, each set of 3 numbers is the x, y, z, and r, g, b of a single vertice
extern float SquarePyramid[30];

// This is the order of the vertices will be drawn to allow us to make a rhombus out of two triangles
extern unsigned int Rhombus_i[6];

// This is the order of the vertices will be drawn to make a cube out of two triangles
extern unsigned int Cube_i[36];

// This is the order of the vertices will be drawn to make a square pyramid
extern unsigned int SquarePyramid_i[24];

class Sphere
{
protected:
    std::vector<unsigned int> positions;
    std::vector<float> normals;
    std::vector<float> uv;
  public:
    std::vector<unsigned int> indices;
    std::vector<float> vertices;
    int triangles = 0;
    unsigned int *GetIndices();
    float *GetVertices();
    // Generates a sphere vertices containing position and normal
    void GenerateLatLongSphere_PN(int resolution, float radius);
    // Generates a sphere vertices containing position, normal, and UV coords
    void GenerateLatLongSphere_PNU(int resolution, float radius);
};

#endif
