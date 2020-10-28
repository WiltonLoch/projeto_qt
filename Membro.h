#pragma once
#include <QOpenGLFunctions_3_0>
#include <glm/glm.hpp>

class Membro : public QOpenGLFunctions_3_0{
    public:
        Membro(glm::vec3 posicao_vec3, GLfloat *vertices_individuais);
        ~Membro();
        glm::vec3 getPosicao();

        void rotacionar(GLfloat angulo, bool rotacao_paterna = false, glm::vec3 posicao_paterna = glm::vec3());
        void rotacionarVertice(GLfloat &x, GLfloat &y, GLfloat &z, GLfloat angulo, bool rotacao_paterna = false, glm::vec3 posicao_paterna = glm::vec3());
    private:
        GLfloat *vertices_individuais = nullptr;
        glm::vec4 posicao;

};

