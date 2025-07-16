#include "Shapes.h"
#include <GLM/trigonometric.hpp>
#include <iostream>

const float Pi = 3.14159;

float Rhombus[24] = {
    -0.75f, -0.4f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom Left
    0.25f,  -0.4f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom Right
    -0.25f, 0.4f,  0.0f, 0.0f, 0.0f, 1.0f, // Top Middle
    0.7f,   0.4f,  0.0f, 0.0f, 0.0f, 0.0f, // Top right
};

unsigned int Rhombus_i[6] = {0, 1, 2, 1, 3, 2};

float Cube[48] = {
    -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, // Bottom Back Left
    1.0f,  -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // Bottom Back Right
    -1.0f, 1.0f,  -1.0f, 0.0f, 0.0f, 1.0f, // Top Back Left
    1.0f,  1.0f,  -1.0f, 0.5f, 0.5f, 0.5f, // Top Back Right
    -1.0f, -1.0f, 1.0f,  1.0f, 0.0f, 0.0f, // Bottom Front Left
    1.0f,  -1.0f, 1.0f,  0.0f, 1.0f, 0.0f, // Bottom Front Right
    -1.0f, 1.0f,  1.0f,  0.0f, 0.0f, 1.0f, // Top Front Left
    1.0f,  1.0f,  1.0f,  0.5f, 0.5f, 0.5f, // Top Front Right
};

unsigned int Cube_i[36] = {0, 1, 2, 3, 1, 2, 2, 0, 6, 4, 0, 6, 5, 4, 7, 6, 4, 7,
                           1, 5, 3, 7, 5, 3, 1, 0, 5, 4, 0, 5, 3, 2, 7, 6, 2, 7};

float SquarePyramid[30] = {
    -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, //  Back Left
    1.0f,  -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, //  Back Right
    -1.0f, -1.0f, 1.0f,  0.0f, 0.0f, 1.0f, //  Front Left
    1.0f,  -1.0f, 1.0f,  0.5f, 0.5f, 0.5f, //	Front Right
    0.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // Top
};

unsigned int SquarePyramid_i[24] = {1, 0, 3, 2, 0, 3, 0, 1, 4, 0, 2, 4, 2, 3, 4, 1, 3, 4};

float *Sphere::GetVertices()
{
    return vertices.data();
}

unsigned int *Sphere::GetIndices()
{
    return indices.data();
}

void Sphere::GenerateLatLongSphere_PN(int resolution, float radius)
{
    // This UV code was based on the code implemented in https://songho.ca/opengl/gl_sphere.html with a lot of changes

    const float AngleChange = Pi / resolution;
    float LatAngle, LongAngle;
    int index = 0;
    float x, y, z, xz;
    triangles = 0;

    for (int i = 0; i <= resolution; i++)
    {

        LatAngle = Pi / 2 - i * AngleChange;
        xz = cosf(LatAngle);
        y = sinf(LatAngle);

        for (int j = 0; j <= resolution * 2; j++)
        {
            LongAngle = j * AngleChange;

            x = xz * sin(LongAngle);
            z = xz * cos(LongAngle);

            vertices.push_back(x * radius);
            vertices.push_back(y * radius);
            vertices.push_back(z * radius);
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            if (i != 0 && i < resolution - 1)
            {
                if (j < resolution * 2)
                {
                    indices.push_back(index);
                    indices.push_back(index + resolution * 2 + 1);
                    indices.push_back(index + 1);
                    indices.push_back(index + 1);
                    indices.push_back(index + resolution * 2 + 1);
                    indices.push_back(index + resolution * 2 + 2);
                    triangles += 2;
                }
            }
            else
            {
                if (i == 0)
                {
                    for (int k = 1; k < resolution * 2 + 1; k++)
                    {
                        indices.push_back(0);
                        indices.push_back(k);
                        indices.push_back(k + 1);
                        triangles++;
                    }
                    index++;
                    break;
                }

                if (i == resolution - 1 && j < resolution * 2)
                {
                    indices.push_back(index);
                    indices.push_back(resolution + 2 * resolution * (resolution - 1));
                    indices.push_back(index + 1);
                    triangles++;
                }
                if (i == resolution)
                {
                    break;
                }
            }

            index++;
        }
    }
}

void Sphere::GenerateLatLongSphere_PNU(int resolution, float radius)
{
    // This UV code was based on the code implemented in https://songho.ca/opengl/gl_sphere.html with a lot of changes

    const float AngleChange = Pi / resolution;
    float LatAngle, LongAngle;
    int index = 0;
    float x, y, z, xz;
    triangles = 0;

    for (int i = 0; i <= resolution; ++i)
    {

        LatAngle = Pi / 2 - i * AngleChange;
        xz = cosf(LatAngle);
        y = sinf(LatAngle);

        for (int j = 0; j <= resolution * 2; ++j)
        {
            //std::cout << index << '\n';

            LongAngle = (j < resolution * 2) ? j * AngleChange : 0;

            x = xz * sin(LongAngle);
            z = xz * cos(LongAngle);

            /*if (j == resolution * 2) { // Show seam
                x *= 1.1f;
                y *= 1.1f;
                z *= 1.1f;
            }*/

            vertices.push_back(x * radius);
            vertices.push_back(y * radius);
            vertices.push_back(z * radius);

            positions.push_back(x * radius);
            positions.push_back(y * radius);
            positions.push_back(z * radius);

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            normals.push_back(x);
            normals.push_back(y);
            normals.push_back(z);

            //std::cout << x << " " << y << " " << z << '\n';
            
            float u = (float)j / (resolution * 2);
            float v = (float)i / (resolution);

            //std::cout << u << " " << v << '\n';

            uv.push_back(u);
            uv.push_back(v);

            vertices.push_back(u);
            vertices.push_back(v);

            int notLast = j < resolution * 2;
            if (i > 0 && i < resolution - 1)
            {
                indices.push_back(j + i * (resolution * 2 + 1));
                indices.push_back(j + (i + 1) * (resolution * 2 + 1));
                indices.push_back(j * notLast + i * (resolution * 2 + 1) + notLast);

                indices.push_back(j + (i + 1) * (resolution * 2 + 1));
                indices.push_back(j * notLast + i * (resolution * 2 + 1) + notLast);
                indices.push_back(j + (i + notLast) * (resolution * 2 + 1) + 1);
                triangles += 2;
            } 
            else
            {
                int South = i == resolution - 1;
                indices.push_back(j + i * (resolution * 2 + 1) * South);
                indices.push_back(j + (i * South + 1) * (resolution * 2 + 1));
                indices.push_back((South) ? j * notLast + i * (resolution * 2 + 1) + notLast : j + notLast * (resolution * 2 + 1) + 1);
                ++triangles;
            }

            ++index;
        }
    }
}
