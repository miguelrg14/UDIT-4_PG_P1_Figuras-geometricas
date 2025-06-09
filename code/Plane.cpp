
// Este código es de dominio público
// miguelrodriguezgallego1@gmail.com

#include "Plane.hpp"
#include <glm.hpp>
#include <vector>
#include <iostream>

namespace udit
{

    const GLfloat Plane::coordinates[] =
    {
       -1,-1,+1,            // 0
       +1,-1,+1,            // 1
       +1,+1,+1,            // 2
       -1,+1,+1,            // 3
    };

    const GLfloat Plane::colors[] =
    {
        0, 0, 1,            // 0    Representa un plano RGB
        1, 0, 1,            // 1
        1, 1, 1,            // 2
        0, 1, 1,            // 3
    };

    const GLuint Plane::indices[] =
    {
        0, 1, 2,            // front
        0, 2, 3,
    };

    Plane::Plane()
    {
        // Se generan índices para los VBOs del plano:
        glGenBuffers(VBO_COUNT, vbo_ids);
        glGenVertexArrays(1, &vao_id);

        Subdivision(10);

        glBindVertexArray(0);
    }

    Plane::~Plane()
    {
        // Se liberan los VBOs y el VAO usados:
        glDeleteVertexArrays(1, &vao_id);
        glDeleteBuffers(VBO_COUNT, vbo_ids);
    }

    void Plane::Subdivision(int amount)
    {
        const int vertexCount = amount + 1;

        // Vector que almacena las posiciones (x, y, z) de cada vértice generado en la subdivisión.
        std::vector<glm::vec3> newCoords;

        // Vector que almacena los colores RGB de cada vértice del plano subdividido.
        std::vector<glm::vec3> newColors;

        /* Vector que contiene los índices de los vértices que forman los triángulos del plano subdividido.
            - Evita duplicar vértices cuando varios triángulos comparten puntos.
            - Permite usar un único buffer de vértices y construir triángulos reutilizando esos vértices.
        */
        std::vector<GLuint>    newIndices;

        // Generar vértices y colores
        for (int y = 0; y < vertexCount; ++y)
        {
            for (int x = 0; x < vertexCount; ++x)
            {
                float fx = -1.0f + 2.0f * x / amount;
                float fy =  1.0f - 2.0f * y / amount;
                newCoords.emplace_back(fx, fy, 1.0f); // z = 1.0 (plano XY)

                float r = static_cast<float>(x) / amount;
                float g = static_cast<float>(y) / amount;
                newColors.emplace_back(r, g, 1.0f); // degradado
            }
        }

        // Generar índices
        for (int y = 0; y < amount; ++y)
        {
            for (int x = 0; x < amount; ++x)
            {
                int topLeft     = y * vertexCount + x;
                int topRight    = topLeft + 1;
                int bottomLeft  = (y + 1) * vertexCount + x;
                int bottomRight = bottomLeft + 1;

                newIndices.push_back(topLeft    );
                newIndices.push_back(bottomLeft );
                newIndices.push_back(topRight   );

                newIndices.push_back(topRight   );
                newIndices.push_back(bottomLeft );
                newIndices.push_back(bottomRight);
            }
        }

        // Subir datos a los VBOs
        glBindVertexArray(vao_id);

        // Coordenadas
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
        glBufferData(GL_ARRAY_BUFFER, newCoords.size() * sizeof(glm::vec3), newCoords.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        // Colores
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COLORS_VBO]);
        glBufferData(GL_ARRAY_BUFFER, newColors.size() * sizeof(glm::vec3), newColors.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        // Índices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, newIndices.size() * sizeof(GLuint), newIndices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);

        this->indexCount = static_cast<GLsizei>(newIndices.size());
    }

    void Plane::render()
    {
        // Activar modo wireframe
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // Se selecciona el VAO que contiene los datos del objeto y se dibujan sus elementos:
        glBindVertexArray(vao_id);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0); 
        glBindVertexArray(0);
    }

}
