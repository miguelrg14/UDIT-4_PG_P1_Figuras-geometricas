// Este código es de dominio público
// miguelrodriguezgallego1@gmail.com

#ifndef CONE_HEADER
#define CONE_HEADER

#include <glad/glad.h>

namespace udit
{

    class Cone
    {
    private:

        // Índices para acceder a los buffers en el array vbo_ids:
        enum
        {
            COORDINATES_VBO,
            COLORS_VBO,
            INDICES_EBO,
            VBO_COUNT
        };

        int indexCount = 0;

        GLuint vbo_ids[VBO_COUNT];   // Ids de los VBOs (coordenadas, colores, índices)
        GLuint vao_id;               // Id del VAO del cono

    public:

        Cone();
        ~Cone();

        void Subdivision(int subdivisions);

        void render();

    };

}

#endif