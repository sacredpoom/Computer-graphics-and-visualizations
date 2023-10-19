#include <Cube.h>

Cube::Cube(GLuint* textureID) {
    texture = *textureID;
    // Define cube vertices (positions and colors)
    cube_vertices = {
        // Position (3)        // Normal (3)    texture (2)
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 4.0f, 1.0f, // Front bottom-left 0
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 4.0f, 0.0f, // Front bottom-right 1
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Front top-right 2
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Front top-right 2
        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, // Front top-left 3
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 4.0f, 1.0f, // Front bottom-left 0

        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 4.0f, 1.0f, // Back bottom-left 4
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 4.0f, 0.0f, // Back bottom-right 5
         0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Back top-right 6
         0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Back top-right 6
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // Back top-left 7
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 4.0f, 1.0f, // Back bottom-left 4

        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 0 bottom
        -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 2.0f, 0.0f, // 4 
        -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 2.0f, 2.0f, // 7
        -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 2.0f, 2.0f, // 7
        -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 2.0f, // 3
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 0

        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 1 top
        0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 2.0f, 0.0f, // 5
        0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 2.0f, 2.0f, // 6
        0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 2.0f, 2.0f, // 6
        0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 2.0f, // 2
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 1

       -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 4.0f, 1.0f, // 0 back
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 4.0f, 0.0f, // 1
        0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // 5
        0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // 5
       -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // 4
       -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 4.0f, 1.0f, // 0

        0.5f,  0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 4.0f, 1.0f, // 2
       -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 4.0f, 0.0f, // 3
       -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, // 7
       -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, // 7
        0.5f,  0.5f,  0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, // 6
        0.5f,  0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 4.0f, 1.0f, // 2
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, cube_vertices.size() * sizeof(float), cube_vertices.data(), GL_STATIC_DRAW);

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

Cube::~Cube() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Cube::Draw() {
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}