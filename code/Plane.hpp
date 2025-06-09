
// Este código es de dominio público
// miguelrodriguezgallego1@gmail.com

#ifndef PLANE_HEADER
#define PLANE_HEADER

    #include <glad/glad.h>

    namespace udit
    {

        class Plane
        {
        private:

            // Índices para indexar el array vbo_ids:
            enum
            {
                COORDINATES_VBO,
                COLORS_VBO,
                INDICES_EBO,
                VBO_COUNT
            };

            // Arrays de datos del plano base:
            static const GLfloat    coordinates [];
            static const GLfloat    colors      [];
            static const GLuint     indices     [];

            int indexCount = 0;

        private:

            GLuint vbo_ids[VBO_COUNT];      // Ids de los VBOs que se usan
            GLuint vao_id;                  // Id del VAO del plano

        public:

            Plane();
            ~Plane();

            void Subdivision(int amount);

            void render();

        };

    }

#endif