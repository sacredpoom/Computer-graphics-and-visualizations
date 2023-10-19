#ifndef PLANE_H
#define PLANE_H

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Plane {
public:
    Plane(GLuint* textureID);
    ~Plane();
    void Draw();

private:
    GLuint VAO, VBO, EBO;
    std::vector<float> plane_vertices;
    std::vector<unsigned int> plane_indices;
    glm::vec3 normals;
    GLuint texture;
};

#endif // PLANE_H