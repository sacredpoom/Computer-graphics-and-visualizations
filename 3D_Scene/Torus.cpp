#include <Torus.h>

Torus::Torus(float majorR, float minorR, int majorSegments, int minorSegments)
    : majorRadius(majorR), minorRadius(minorR), numMajorSegments(majorSegments), numMinorSegments(minorSegments) {
    GenerateTorus();
}

Torus::~Torus() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Torus::Draw() {
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawElements(GL_TRIANGLES, torus_indices.size(), GL_UNSIGNED_INT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

void Torus::setTexture(GLuint *textureID) {
    texture = *textureID;
}

void Torus::GenerateTorus() {
    // Generate torus vertices and indices here
    for (int i = 0; i <= numMajorSegments; ++i) {
        float majorAngle = 2 * glm::pi<float>() * static_cast<float>(i) / numMajorSegments;

        for (int j = 0; j <= numMinorSegments; ++j) {
            float minorAngle = 2 * glm::pi<float>() * static_cast<float>(j) / numMinorSegments;

            float x = (majorRadius + minorRadius * cos(minorAngle)) * cos(majorAngle);
            float y = (majorRadius + minorRadius * cos(minorAngle)) * sin(majorAngle);
            float z = minorRadius * sin(minorAngle);

            Vertex vertex;
            vertex.position = glm::vec3(x, y, z);
            vertex.normal = glm::normalize(glm::vec3(x, y, z));

            // Calculate and assign texture coordinates (UV)
            vertex.texCoord.x = static_cast<float>(i) / numMajorSegments;
            vertex.texCoord.y = static_cast<float>(j) / numMinorSegments;

            torus_vertices.push_back(vertex);
        }
    }

    // Generate torus indices
    for (int i = 0; i < numMajorSegments; ++i) {
        for (int j = 0; j < numMinorSegments; ++j) {
            int k1 = i * (numMinorSegments + 1) + j;
            int k2 = k1 + 1;
            int k3 = (i + 1) * (numMinorSegments + 1) + j;
            int k4 = k3 + 1;

            // Add indices for two triangles
            torus_indices.push_back(k1);
            torus_indices.push_back(k2);
            torus_indices.push_back(k3);
            torus_indices.push_back(k2);
            torus_indices.push_back(k4);
            torus_indices.push_back(k3);
        }
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, torus_vertices.size() * sizeof(Vertex), torus_vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, torus_indices.size() * sizeof(unsigned int), torus_indices.data(), GL_STATIC_DRAW);

    // Define vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

