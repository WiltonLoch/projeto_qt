#pragma once
#include <unordered_map>
#include <QOpenGLFunctions_3_0>
#include <memory>
#include <glm/glm.hpp>
#include "Membro.h"

class Corpo : public QOpenGLFunctions_3_0{
    public:
        Corpo();
        ~Corpo();
        GLuint getQtdDesenhoVertices();

        void adicionarMembro(glm::vec3 posicao, GLfloat escalaXY, GLfloat altura, const char* nome, const char *pai = nullptr);
        void rotacionarMembro(GLfloat angulo, const char* nome);
    private:
        std::vector<GLfloat> vertices;
        std::vector<GLuint> ordem;
        std::vector<GLuint> ordem_cubo_individual;
        std::unordered_map<std::string, std::unique_ptr<Membro>> membros;
        std::unordered_map<std::string, std::vector<std::string>> hierarquia;
};
