#ifndef FULLCYLINDER_H
#define FULLCYLINDER_H

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

class FullCylinder {
private:
    std::vector<float> cylinder_vertices;
    std::vector<int> cylinder_indices;
    std::vector<float> cylinder_normals;
    GLuint VBO, VAO, EBO;
    float radius;
    float height;
    int sectorCount;
    int stackCount;
    GLuint texture;

public:
    FullCylinder(float r, float h, int sectors, int stacks, GLuint* textureID);
    ~FullCylinder();
    void GenerateCylinder();
    void Draw();
};

#endif