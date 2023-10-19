#ifndef CUBE_H
#define CUBE_H

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Cube {
private:
    std::vector<float> cube_vertices;
    std::vector<unsigned int> cube_indices;
    GLuint VBO, VAO;
    GLuint texture;

public:
    Cube(GLuint *texture);
    ~Cube();
    void Draw();
};

#endif