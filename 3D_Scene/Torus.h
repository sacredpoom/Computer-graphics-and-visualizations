#ifndef TORUS_H
#define TORUS_H

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <cmath>
#include <glm/gtc/constants.hpp>

class Torus {
private:
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoord;
    };

    std::vector<Vertex> torus_vertices;
    std::vector<unsigned int> torus_indices;
    GLuint VBO, VAO, EBO;
    float majorRadius;
    float minorRadius;
    int numMajorSegments;
    int numMinorSegments;
    GLuint texture;

public:
    Torus(float majorR, float minorR, int majorSegments, int minorSegments);
    ~Torus();
    void setTexture(GLuint *textureID);
    void Draw();
    void SetColor(const glm::vec4& color);

private:
    void GenerateTorus();
};

#endif