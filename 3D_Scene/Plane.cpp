#include <Plane.h>

Plane::Plane(GLuint* textureID) : texture(*textureID) {
    // Define plane vertices (positions and colors)
    plane_vertices = {
        // Position            //normal            //texture      
         1.0f,  0.0f,  1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, // Top Right Vertex 0
         1.0f,  0.0f, -1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f, // Bottom Right Vertex 1
        -1.0f,  0.0f, -1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // Bottom Left Vertex 2
        -1.0f,  0.0f,  1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f // Top Left Vertex 3
    };

    // Define plane indices (triangles)
    plane_indices = {
        0, 1, 3, // Triangle 1
        1, 2, 3, // Triangle 2
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, plane_vertices.size() * sizeof(float), plane_vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, plane_indices.size() * sizeof(unsigned int), plane_indices.data(), GL_STATIC_DRAW);

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

Plane::~Plane() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Plane::Draw() {
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawElements(GL_TRIANGLES, plane_indices.size(), GL_UNSIGNED_INT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}