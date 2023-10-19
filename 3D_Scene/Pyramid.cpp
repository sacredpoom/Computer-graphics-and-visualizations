#include "Pyramid.h"

Pyramid::Pyramid(GLuint* textureID) {
    texture = *textureID;
    // Define pyramid vertices (positions and colors)
    pyramid_vertices = {
        // coordinate         // normal                 // text pos
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f, 0.0f,    1.0f, 1.0f, //base
        -1.0f, -1.0f,  1.0f,  0.0f, -1.0f, 0.0f,    1.0f, 0.0f, //base
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f, 0.0f,    0.0f, 0.0f, //base
         1.0f, -1.0f, -1.0f,  0.0f, -1.0f, 0.0f,    0.0f, 1.0f, //base
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f, 0.0f,    1.0f, 1.0f, //base
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f, 0.0f,    0.0f, 0.0f, //base

         0.0f,  1.0f,  0.0f,  -1.0f, -0.5f, 0.0f,    0.5f, 1.0f, //side 1
        -1.0f, -1.0f,  1.0f,  -1.0f, -0.5f, 0.0f,    0.0f, 0.0f, //side 1
        -1.0f, -1.0f, -1.0f,  -1.0f, -0.5f, 0.0f,    1.0f, 0.0f, //side 1

         0.0f,  1.0f,  0.0f,  1.0f, -0.5f, 0.0f,    0.5f, 1.0f, //side 2
         1.0f, -1.0f,  1.0f,  1.0f, -0.5f, 0.0f,    0.0f, 0.0f, //side 2
         1.0f, -1.0f, -1.0f,  1.0f, -0.5f, 0.0f,    1.0f, 0.0f, //side 2

         0.0f,  1.0f,  0.0f,  0.0f, -0.5f, 1.0f,    0.5f, 1.0f, //side 3
         1.0f, -1.0f,  1.0f,  0.0f, -0.5f, 1.0f,    0.0f, 0.0f, //side 3
        -1.0f, -1.0f,  1.0f,  0.0f, -0.5f, 1.0f,    1.0f, 0.0f, //side 3

         0.0f,  1.0f,  0.0f,  0.0f, -0.5f, -1.0f,    0.5f, 1.0f, //side 4
        -1.0f, -1.0f, -1.0f,  0.0f, -0.5f, -1.0f,    0.0f, 0.0f, //side 4
         1.0f, -1.0f, -1.0f,  0.0f, -0.5f, -1.0f,    1.0f, 0.0f  //side 4
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, pyramid_vertices.size() * sizeof(float), pyramid_vertices.data(), GL_STATIC_DRAW);

    // Define vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Pyramid::~Pyramid() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Pyramid::Draw() {
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLES, 0, 18);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}