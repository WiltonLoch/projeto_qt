#include <iostream>
#include "Corpo.h"

using namespace std;

Corpo::Corpo(){
    initializeOpenGLFunctions();
    vertices.reserve(8 * 6 * 10);
    ordem.reserve(12 * 10);
    ordem_cubo_individual = {
            0, 2, 4,
            2, 4, 6,
            0, 1, 2,
            1, 2, 3,
            2, 3, 6,
            3, 6, 7,
            4, 6, 7,
            4, 5, 7,
            4, 5, 0,
            5, 0, 1,
            1, 3, 5, 
            3, 5, 7
    };
}

Corpo::~Corpo(){}

GLuint Corpo::getQtdDesenhoVertices(){
    return ordem.size();
}

void Corpo::adicionarMembro(glm::vec3 posicao, GLfloat escalaXY, GLfloat altura, const char* nome, const char *pai){

    unsigned int tamanho_anterior_ordem = ordem.size();
    ordem.insert(ordem.end(), ordem_cubo_individual.begin(), ordem_cubo_individual.end());

    if(tamanho_anterior_ordem != 0){
        unsigned int deslocamento_vertices_ordem = vertices.size()/6;
        for (int i = tamanho_anterior_ordem; i < tamanho_anterior_ordem + ordem_cubo_individual.size(); ++i)            
            ordem[i] += deslocamento_vertices_ordem;
    }

    unsigned int tamanho_anterior_vertices = vertices.size();
    std::vector<GLfloat> cubo = {
            -escalaXY/2 + posicao.x, -escalaXY/2 + posicao.y, 0.0f + posicao.z, 0.0f, 0.0f, 0.0f,
            -escalaXY/2 + posicao.x, -escalaXY/2 + posicao.y, altura + posicao.z, 0.0f, 0.0f, 1.0f,
            -escalaXY/2 + posicao.x,  escalaXY/2 + posicao.y, 0.0f + posicao.z, 0.0f, 1.0f, 0.0f,
            -escalaXY/2 + posicao.x,  escalaXY/2 + posicao.y, altura + posicao.z, 0.0f, 1.0f, 1.0f,
             escalaXY/2 + posicao.x, -escalaXY/2 + posicao.y, 0.0f + posicao.z, 1.0f, 0.0f, 0.0f,
             escalaXY/2 + posicao.x, -escalaXY/2 + posicao.y, altura + posicao.z, 1.0f, 0.0f, 1.0f,
             escalaXY/2 + posicao.x,  escalaXY/2 + posicao.y, 0.0f + posicao.z, 1.0f, 1.0f, 0.0f,
             escalaXY/2 + posicao.x,  escalaXY/2 + posicao.y, altura + posicao.z, 1.0f, 1.0f, 1.0f,
    };
    vertices.insert(vertices.end(), cubo.begin(), cubo.end());
    
    /* cout << ordem.size() << endl; */
    /* cout << endl << "NOVA ADIÇÃO" << endl; */

    /* for (int i = 0; i < vertices.size(); ++i) { */
    /*     if(i % 6 == 0) cout << endl; */
    /*        cout << vertices[i] << " "; */
    /* } */

    /* cout << endl; */

    /* for (int i = 0; i < ordem.size(); ++i) { */
    /*     if(i % 3 == 0) cout << endl; */
    /*        cout << ordem[i] << " "; */
    /* } */
    
    /* cout << tamanho_anterior_vertices << endl; */
    membros[nome] = unique_ptr<Membro>(new Membro(posicao, vertices.data() + tamanho_anterior_vertices));
    if(pai != nullptr)
        hierarquia[pai].push_back(nome);
    

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ordem.size() * sizeof(GLuint), ordem.data(), GL_DYNAMIC_DRAW);
}

void Corpo::rotacionarMembro(GLfloat angulo, const char* nome){
    membros[nome]->rotacionar(angulo);
    for (auto i : hierarquia[nome])
        membros[i]->rotacionar(angulo, true, membros[nome]->getPosicao());
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_DYNAMIC_DRAW);
     
}
