// Este código es de dominio público
// miguelrodriguezgallego1@gmail.com

#ifndef CYLINDER_HEADER
#define CYLINDER_HEADER

#include <glad/glad.h>

namespace udit
{

    class Cylinder
    {
    private:

        enum
        {
            COORDINATES_VBO,
            COLORS_VBO,
            INDICES_EBO,
            VBO_COUNT
        };

        // Estos arrays se mantienen para init() clásico si se quiere
        static const GLfloat coordinates[];
        static const GLfloat colors[];
        static const GLuint  indices[];

        int indexCount = 0;

        GLuint vbo_ids[VBO_COUNT];
        GLuint vao_id;

    public:

        Cylinder();
        ~Cylinder();

        void init();                // Carga buffers con datos fijos
        void Subdivision(int slices); // Carga buffers con cilindro subdividido
        void render();              // Dibuja el cilindro

    };

}

#endif