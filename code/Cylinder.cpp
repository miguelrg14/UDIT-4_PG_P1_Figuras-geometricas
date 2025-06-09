
#include "Cylinder.hpp"
#include <vector>
#include <cmath>

namespace udit
{
    //Valores fijos, los mantenemos para init() clásico
    // Coordenadas: 6 puntos base, 6 puntos arriba, 2 centros
    const GLfloat Cylinder::coordinates[] =
    {
        // Base inferior (y = 0)
         1.0f, 0.0f,  0.0f,
         0.5f, 0.0f,  0.866f,
        -0.5f, 0.0f,  0.866f,
        -1.0f, 0.0f,  0.0f,
        -0.5f, 0.0f, -0.866f,
         0.5f, 0.0f, -0.866f,

         // Base superior (y = 1)
          1.0f, 1.0f,  0.0f,
          0.5f, 1.0f,  0.866f,
         -0.5f, 1.0f,  0.866f,
         -1.0f, 1.0f,  0.0f,
         -0.5f, 1.0f, -0.866f,
          0.5f, 1.0f, -0.866f,

          // Centro base inferior
           0.0f, 0.0f,  0.0f,

           // Centro base superior
            0.0f, 1.0f,  0.0f
    };

    // Colores simples
    const GLfloat Cylinder::colors[] =
    {
        // Base inferior
        0.7f, 0.2f, 0.2f,
        0.7f, 0.2f, 0.2f,
        0.7f, 0.2f, 0.2f,
        0.7f, 0.2f, 0.2f,
        0.7f, 0.2f, 0.2f,
        0.7f, 0.2f, 0.2f,

        // Base superior
        0.2f, 0.7f, 0.2f,
        0.2f, 0.7f, 0.2f,
        0.2f, 0.7f, 0.2f,
        0.2f, 0.7f, 0.2f,
        0.2f, 0.7f, 0.2f,
        0.2f, 0.7f, 0.2f,

        // Centro base inferior
        1.0f, 1.0f, 0.0f,

        // Centro base superior
        1.0f, 1.0f, 0.0f
    };

    const GLuint Cylinder::indices[] =
    {
        // LATERALES (triángulos en sentido antihorario)
        6, 1, 0,
        7, 1, 6,

        7, 2, 1,
        8, 2, 7,

        8, 3, 2,
        9, 3, 8,

        9, 4, 3,
        10, 4, 9,

        10, 5, 4,
        11, 5, 10,

        11, 0, 5,
        6, 0, 11,

        // BASE INFERIOR (sentido antihorario desde abajo)
        12, 0, 1,
        12, 1, 2,
        12, 2, 3,
        12, 3, 4,
        12, 4, 5,
        12, 5, 0,

        // BASE SUPERIOR (sentido antihorario desde arriba)
        13, 7, 6,
        13, 8, 7,
        13, 9, 8,
        13,10, 9,
        13,11,10,
        13, 6,11
    };

    Cylinder::Cylinder() : indexCount(0), vao_id(0)
    {
        glGenVertexArrays(1, &vao_id);
        glGenBuffers(VBO_COUNT, vbo_ids);
        init(); // Carga con datos fijos al inicio

        Subdivision(6);

        glBindVertexArray(0);
    }

    Cylinder::~Cylinder()
    {
        glDeleteBuffers(VBO_COUNT, vbo_ids);
        glDeleteVertexArrays(1, &vao_id);
    }

    void Cylinder::init()
    {
        indexCount = sizeof(indices) / sizeof(GLuint);

        glBindVertexArray(vao_id);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(coordinates), coordinates, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COLORS_VBO]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    void Cylinder::Subdivision(int slices)
    {
        if (slices < 3) slices = 3; // mínimo 3 vértices para base

        std::vector<GLfloat> coords;
        std::vector<GLfloat> cols;
        std::vector<GLuint> inds;

        const float radius = 1.0f;
        const float height = 1.0f;
        const float PI2 = 6.2831853f;

        // Índices para los centros
        const GLuint baseCenterIndex = 2 * slices;
        const GLuint topCenterIndex = 2 * slices + 1;

        // Generar vértices de la base inferior
        for (int i = 0; i < slices; ++i)
        {
            float angle = PI2 * i / slices;
            float x = radius * cos(angle);
            float z = radius * sin(angle);

            coords.push_back(x);
            coords.push_back(0.0f);
            coords.push_back(z);

            // Color base inferior (rojo)
            cols.push_back(0.7f);
            cols.push_back(0.2f);
            cols.push_back(0.2f);
        }

        // Generar vértices de la base superior
        for (int i = 0; i < slices; ++i)
        {
            float angle = PI2 * i / slices;
            float x = radius * cos(angle);
            float z = radius * sin(angle);

            coords.push_back(x);
            coords.push_back(height);
            coords.push_back(z);

            // Color base superior (verde)
            cols.push_back(0.2f);
            cols.push_back(0.7f);
            cols.push_back(0.2f);
        }

        // Centro base inferior
        coords.push_back(0.0f); coords.push_back(0.0f); coords.push_back(0.0f);
        cols.push_back(1.0f); cols.push_back(1.0f); cols.push_back(0.0f); // amarillo

        // Centro base superior
        coords.push_back(0.0f); coords.push_back(height); coords.push_back(0.0f);
        cols.push_back(1.0f); cols.push_back(1.0f); cols.push_back(0.0f); // amarillo

        // Crear índices

        // Lados (caras laterales)
        for (int i = 0; i < slices; ++i)
        {
            int next = (i + 1) % slices;

            GLuint bottom_current = i;
            GLuint bottom_next = next;
            GLuint top_current = i + slices;
            GLuint top_next = next + slices;

            // Dos triángulos por cara lateral
            inds.push_back(top_current);
            inds.push_back(bottom_next);
            inds.push_back(bottom_current);

            inds.push_back(top_next);
            inds.push_back(bottom_next);
            inds.push_back(top_current);
        }

        // Base inferior (triángulos fan desde el centro)
        for (int i = 0; i < slices; ++i)
        {
            int next = (i + 1) % slices;
            inds.push_back(baseCenterIndex);
            inds.push_back(i);
            inds.push_back(next);
        }

        // Base superior (triángulos fan desde el centro)
        for (int i = 0; i < slices; ++i)
        {
            int next = (i + 1) % slices;
            inds.push_back(topCenterIndex);
            inds.push_back(next + slices);
            inds.push_back(i + slices);
        }

        indexCount = inds.size();

        // Subir datos a OpenGL
        glBindVertexArray(vao_id);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
        glBufferData(GL_ARRAY_BUFFER, coords.size() * sizeof(GLfloat), coords.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COLORS_VBO]);
        glBufferData(GL_ARRAY_BUFFER, cols.size() * sizeof(GLfloat), cols.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, inds.size() * sizeof(GLuint), inds.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    void Cylinder::render()
    {
        glBindVertexArray(vao_id);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }
}
