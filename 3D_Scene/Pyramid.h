#ifndef PYRAMID_H
#define PYRAMID_H

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Pyramid {
public:
    Pyramid(GLuint* textureID);
    ~Pyramid();
    void Draw();

private:
    GLuint VAO, VBO;
    std::vector<float> pyramid_vertices;
    std::vector<unsigned int> pyramid_indices;
    GLuint texture;
};
#endif
