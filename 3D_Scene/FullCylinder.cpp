#include "FullCylinder.h"

FullCylinder::FullCylinder(float r, float h, int sectors, int stacks, GLuint* textureID) {
    radius = r;
    height = h;
    sectorCount = sectors;
    stackCount = stacks;
    texture = *textureID;
    GenerateCylinder();
}

void FullCylinder::GenerateCylinder() {
    // Generate cylinder vertices and indices
    for (int i = 0; i <= stackCount; ++i) {
        float stackFraction = static_cast<float>(i) / stackCount;
        float stackHeight = stackFraction * height - (height / 2.0f);

        for (int j = 0; j <= sectorCount; ++j) {
            float sectorFraction = static_cast<float>(j) / sectorCount;
            float angle = sectorFraction * glm::two_pi<float>();

            float x = radius * cos(angle);
            float y = radius * sin(angle);
            float z = stackHeight;

            //calculate vertex normal (unnormalized)
            glm::vec3 vertexNormal(x, y, 0.0f); //normal in the xy-plane for the sides

            // Add vertex position (x, y, z) to cylinder_vertices
            cylinder_vertices.push_back(x);
            cylinder_vertices.push_back(y);
            cylinder_vertices.push_back(z);

            // add vertex normals
            cylinder_vertices.push_back(vertexNormal.x);
            cylinder_vertices.push_back(vertexNormal.y);
            cylinder_vertices.push_back(vertexNormal.z);

            // Add vertex texture coordinates
            cylinder_vertices.push_back(sectorFraction);
            cylinder_vertices.push_back(stackFraction);
        }
    }

    // Generate cylinder indices for the sides
    for (int i = 0; i < stackCount; ++i) {
        for (int j = 0; j < sectorCount; ++j) {
            int k1 = i * (sectorCount + 1) + j;
            int k2 = k1 + 1;
            int k3 = (i + 1) * (sectorCount + 1) + j;
            int k4 = k3 + 1;

            // Add indices for two triangles
            cylinder_indices.push_back(k1);
            cylinder_indices.push_back(k2);
            cylinder_indices.push_back(k3);
            cylinder_indices.push_back(k2);
            cylinder_indices.push_back(k4);
            cylinder_indices.push_back(k3);
        }
    }

    // Generate cylinder vertices and indices for the top and bottom circles
    int baseVertexIndex = static_cast<int>(cylinder_vertices.size()) / 9;

    for (int j = 0; j <= sectorCount; ++j) {
        float sectorFraction = static_cast<float>(j) / sectorCount;
        float angle = sectorFraction * glm::two_pi<float>();

        float x = radius * cos(angle);
        float y = radius * sin(angle);
        float zTop = height / 2.0f;
        float zBottom = -height / 2.0f;

        // Top circle vertex
        cylinder_vertices.push_back(x);
        cylinder_vertices.push_back(y);
        cylinder_vertices.push_back(zTop);

        cylinder_vertices.push_back(sectorFraction);
        cylinder_vertices.push_back(0.0f);

        cylinder_vertices.push_back(0.0f);
        cylinder_vertices.push_back(0.0f);
        cylinder_vertices.push_back(1.0f); // normal for positive z direction

        // Bottom circle vertex
        cylinder_vertices.push_back(x);
        cylinder_vertices.push_back(y);
        cylinder_vertices.push_back(zBottom);

        cylinder_vertices.push_back(sectorFraction);
        cylinder_vertices.push_back(1.0f);

        cylinder_vertices.push_back(0.0f);
        cylinder_vertices.push_back(0.0f);
        cylinder_vertices.push_back(-1.0f); // normal for negative z direction

        // Indices for top and bottom circles
        if (j < sectorCount) {
            int k1 = baseVertexIndex + j * 2;
            int k2 = k1 + 2;
            int k3 = baseVertexIndex + 2 * sectorCount + 1;

            // Top circle triangle
            cylinder_indices.push_back(baseVertexIndex);
            cylinder_indices.push_back(k1);
            cylinder_indices.push_back(k2);

            // Bottom circle triangle
            cylinder_indices.push_back(baseVertexIndex + 1);
            cylinder_indices.push_back(k2 + 1);
            cylinder_indices.push_back(k1 + 1);
        }
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, cylinder_vertices.size() * sizeof(float), cylinder_vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cylinder_indices.size() * sizeof(unsigned int), cylinder_indices.data(), GL_STATIC_DRAW);

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

FullCylinder::~FullCylinder() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void FullCylinder::Draw() {
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawElements(GL_TRIANGLES, cylinder_indices.size(), GL_UNSIGNED_INT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

