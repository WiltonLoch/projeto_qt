#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "Membro.h"

using namespace std;

Membro::Membro(glm::vec3 posicao_vec3, GLfloat *vertices_individuais){
    this->vertices_individuais = vertices_individuais;
    this->posicao = glm::vec4(posicao_vec3.x, posicao_vec3.y, posicao_vec3.z, 1.0f);
}

Membro::~Membro(){}

glm::vec3 Membro::getPosicao(){
    return glm::vec3(posicao.x, posicao.y, posicao.z);
}

void Membro::rotacionar(GLfloat angulo, bool rotacao_paterna, glm::vec3 posicao_paterna){
    GLfloat *vertice_atual = vertices_individuais;
    for (int i = 0; i < 8; ++i) {
        rotacionarVertice(vertice_atual[0], vertice_atual[1], vertice_atual[2], angulo, rotacao_paterna, posicao_paterna);
        vertice_atual += 6;
    }
    if(rotacao_paterna) rotacionarVertice(posicao.x, posicao.y, posicao.z, angulo, rotacao_paterna, posicao_paterna);

}

void Membro::rotacionarVertice(GLfloat &x, GLfloat &y, GLfloat &z, GLfloat angulo, bool rotacao_paterna, glm::vec3 posicao_paterna){
    glm::vec4 posicao_paterna_vec4;
    glm::vec4 vertice(x, y, z, 1.0f);       
    glm::mat4 rotacao = glm::mat4(1.0f);
    rotacao = glm::rotate(rotacao, glm::radians(angulo), glm::vec3(1.0f, 0.0f, 0.0f));

    if(rotacao_paterna)
        posicao_paterna_vec4 = glm::vec4(posicao_paterna.x, posicao_paterna.y, posicao_paterna.z, 1.0f);

    vertice -= rotacao_paterna ? posicao_paterna_vec4 : posicao;
    vertice = rotacao * vertice;
    vertice += rotacao_paterna ? posicao_paterna_vec4 : posicao;

    x = vertice.x;
    y = vertice.y;
    z = vertice.z;
}
