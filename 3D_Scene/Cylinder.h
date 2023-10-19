#ifndef CYLINDER_H
#define CYLINDER_H

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

class Cylinder {
private:
    std::vector<float> cylinder_vertices;
    std::vector<int> cylinder_indices;
    GLuint VBO, VAO, EBO;
    float radius;
    float height;
    int sectorCount;
    int stackCount;
    GLuint texture;

public:
    Cylinder(float r, float h, int sectors, int stacks, const glm::vec4& color, GLuint *textureID);
    ~Cylinder();
    void Draw();

private:
    void GenerateCylinder();
};

#endif