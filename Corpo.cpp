#include <iostream>
#include "Corpo.h"

using namespace std;

/* Essa classe administra tanto o vetor de vértices quanto o vetor de elementos(mais informações nos commentários do ComponenteOpenGL) */
/* Ela utiliza fundamentalmente dois vectors para guardar os vértices e os índices dos vértices utilizados para construir cada triangulo do cubo */
/* Inicialmente ambos estão vazios e quando um vértice é criado os vectors são povoados com base nos deslocamentos previamente definidos dos vértices */
Corpo::Corpo(){
    initializeOpenGLFunctions();
    /* Prepara uma região de memória pros vetores que comporta até 10 cubos */
    /* Esta é uma operação sensível já que se eventualmente a capacidade estourar, o vector pode procurar outra região de memória e ponteiros para*/ 
    /* o que anteriormente era a primeira posição ficam com referências soltas */
    /* Como não tive tempo suficiente não tratei essa condição de excessão do tamanho, mas poderia ser resolvida através de uma nova atualização nos ponteiros existentes dos membros */
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

/* Adiciona um novo membro, dimensões em x e y são iguais, altura independente */
/* todos os membros são guardados em um hashmap (unordered_map com acesso geralmente em O(1)) indexado por uma string e conteúdo em um unique_ptr */
/* Para armazenar a hierarquia existe outro hashmap (provavelmente não a melhor opção) que guarda para o nome de um membro, um vetor com o nome dos filhos */
void Corpo::adicionarMembro(glm::vec3 posicao, GLfloat escalaXY, GLfloat altura, const char* nome, const char *pai){

    unsigned int tamanho_anterior_ordem = ordem.size();
    /* copia o "template" básico para a construção dos triangulos do cubo para o vetor de elementos */
    ordem.insert(ordem.end(), ordem_cubo_individual.begin(), ordem_cubo_individual.end());

    /* se não for o primeiro membro adiciona o deslocamento referente ao índice do membro gerado (cada cubo possui 8 vértices, portanto para o segundo membro o primeiro índice do vértice deve ser 8) */
    if(tamanho_anterior_ordem != 0){
        unsigned int deslocamento_vertices_ordem = vertices.size()/6;
        for (size_t i = tamanho_anterior_ordem; i < tamanho_anterior_ordem + ordem_cubo_individual.size(); ++i)            
            ordem[i] += deslocamento_vertices_ordem;
    }

    unsigned int tamanho_anterior_vertices = vertices.size();
    /* constroi um cubo com base nos parametros, devido ao limite de tempo um anchor point fixo em (50%, 50%, 0%) do cubo é utilizado */
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
    /* copia os vértices novos para o vetor geral de vértices */
    vertices.insert(vertices.end(), cubo.begin(), cubo.end());
    
    /* cria um novo membro e o armazena no hashmap, passa a posição do membro e um ponteiro para o buffer, deslocado para a seção específica do membro */
    membros[nome] = unique_ptr<Membro>(new Membro(posicao, vertices.data() + tamanho_anterior_vertices));
    /* adiciona o membro ao pai, se este foi informado */
    if(pai != nullptr)
        hierarquia[pai].push_back(nome);
    

    /* atualiza o buffer de vértices e da ordem de renderização administrados pelo opengl */
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ordem.size() * sizeof(GLuint), ordem.data(), GL_DYNAMIC_DRAW);
}

/* rotaciona um membro e caso existam filhos, aplica a rotação hierárquica sobre eles */ 
void Corpo::rotacionarMembro(GLfloat angulo, const char* nome){
    membros[nome]->rotacionar(angulo);
    for (auto i : hierarquia[nome])
        membros[i]->rotacionar(angulo, true, membros[nome]->getPosicao());
    /* atualiza novamente buffer após atualização dos dados */ 
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_DYNAMIC_DRAW);
     
}
