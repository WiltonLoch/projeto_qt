#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "Membro.h"

using namespace std;

/* O construtor recebe como parametro a posição do membro (vinda do corpo) e um ponteiro para uma seção do buffer */
/* contendo todos os vértices referentes à forma geométrica do membro. */
Membro::Membro(glm::vec3 posicao_vec3, GLfloat *vertices_individuais){
    this->vertices_individuais = vertices_individuais;
    this->posicao = glm::vec4(posicao_vec3.x, posicao_vec3.y, posicao_vec3.z, 1.0f);
}

Membro::~Membro(){}

glm::vec3 Membro::getPosicao(){
    return glm::vec3(posicao.x, posicao.y, posicao.z);
}

/* o ponteiro de vertices_individuais aponta para uma seção do buffer de vertices atribuido ao OpenGL, e armazenado no contexto da classe corpo */
/* A rotação é realizada a partir da base da seção do membro acessando diretamente o buffer de vértices no contexto do corpo */
/* o vertice_atual é um ponteiro que é deslocado para cada novo vértice em 6 posições (quantidade de floats do vertice, 3 pos, 3 cor) */
void Membro::rotacionar(GLfloat angulo, bool rotacao_paterna, glm::vec3 posicao_paterna){
    GLfloat *vertice_atual = vertices_individuais;
    for (int i = 0; i < 8; ++i) {
        rotacionarVertice(vertice_atual[0], vertice_atual[1], vertice_atual[2], angulo, rotacao_paterna, posicao_paterna);
        vertice_atual += 6;
    }
/* Se a rotação for um encadeamente de uma rotação do pai desse membro, a posição em si também deve ser rotacionada */
    if(rotacao_paterna) rotacionarVertice(posicao.x, posicao.y, posicao.z, angulo, rotacao_paterna, posicao_paterna);
}

/* Cada vértice da forma geométrica do membro é rotacionado através dessa função */
/* Ela recebe 3 GLfloats, referentes às coordenadas do vértice, porém como referências do cpp */
/* Caso outra alternativa fosse adotada, seria necessário copiar a memória do buffer para construir os vec* do glm e posteriormente copiar os resultados de volta */
/* Usando as referências em teoria as modificações são realizadas diretamente no buffer quando o valor de x, y ou z é modificado. */
/* é utilizado um bool pra identificar se essa rotação é um reflexo de uma rotação paterna, já que não há um valor padrão comparável no vec3 (já que qualquer valor pode ser uma coordenadada real) */
void Membro::rotacionarVertice(GLfloat &x, GLfloat &y, GLfloat &z, GLfloat angulo, bool rotacao_paterna, glm::vec3 posicao_paterna){
    glm::vec4 posicao_paterna_vec4;
    glm::vec4 vertice(x, y, z, 1.0f);       
    glm::mat4 rotacao = glm::mat4(1.0f);
    rotacao = glm::rotate(rotacao, glm::radians(angulo), glm::vec3(1.0f, 0.0f, 0.0f));

    if(rotacao_paterna)
        posicao_paterna_vec4 = glm::vec4(posicao_paterna.x, posicao_paterna.y, posicao_paterna.z, 1.0f);

    /* já que qualquer rotação de vetor acontece em relação à origem, se é rotação prória subtrai a própria posição, caso contrário subtrai a do pai */
    vertice -= rotacao_paterna ? posicao_paterna_vec4 : posicao;
    vertice = rotacao * vertice;
    vertice += rotacao_paterna ? posicao_paterna_vec4 : posicao;

    /* atualiza os vértices alterados no buffer através das referências */
    x = vertice.x;
    y = vertice.y;
    z = vertice.z;
}
