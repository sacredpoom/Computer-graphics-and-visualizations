#include <Cylinder.h>

Cylinder::Cylinder(float r, float h, int sectors, int stacks, const glm::vec4& color, GLuint *textureID) {
        radius = r;
        height = h;
        sectorCount = sectors;
        stackCount = stacks;
        texture = *textureID;

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

                // Add vertex position (x, y, z) to cylinder_vertices
                cylinder_vertices.push_back(x);
                cylinder_vertices.push_back(y);
                cylinder_vertices.push_back(z);

                // Add vertex color (for example, all red)
                cylinder_vertices.push_back(color.r); // Red
                cylinder_vertices.push_back(color.g); // Green
                cylinder_vertices.push_back(color.b); // Blue
                cylinder_vertices.push_back(color.a); // Alpha

                // Add vertex texture coordinates
                cylinder_vertices.push_back(sectorFraction);
                cylinder_vertices.push_back(stackFraction);
            }
        }
        // Generate cylinder indices
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

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, cylinder_vertices.size() * sizeof(float), cylinder_vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, cylinder_indices.size() * sizeof(unsigned int), cylinder_indices.data(), GL_STATIC_DRAW);

        // Define vertex attribute pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
}

Cylinder::~Cylinder() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

void Cylinder::Draw() {
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawElements(GL_TRIANGLES, cylinder_indices.size(), GL_UNSIGNED_INT, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
    }
