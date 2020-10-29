#include <cmath>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ComponenteOpenGL.h"

#define MAX_CUBOS 10

using namespace std;

ComponenteOpenGL::ComponenteOpenGL(QWidget *pai) : QOpenGLWidget(pai){
}

ComponenteOpenGL::~ComponenteOpenGL(){
    glDetachShader(programa_shader, vertex_shader);
    glDetachShader(programa_shader, fragment_shader);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void ComponenteOpenGL::initializeGL()
{
/* DISCLAMER: o editor não está permitindo a entrada de circunflexos no a, por isso todas essas palavras não tem acento hehe */
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    t_inicial = t_anterior = std::chrono::high_resolution_clock::now();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    /* pede ao opengl para gerar o buffer que armazena os vértices */
    /* os dados desse vetor são administrados e ligados ao GL_ARRAY_BUFFER através de uma instancia do corpo */
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    /* pede ao opengl para gerar o vetor de elementos */
    /* esse vetor servirá para dizer os índices dos vértices que devem ser usados para desenhar cada triangulo */
    /* este vetor também é administrado e ligado ao GL_ELEMENT_ARRAY_BUFFER através do corpo */
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    corpo = new Corpo();
    corpo->adicionarMembro(glm::vec3(0.0f, 0.0f, 0.0f), 0.2f, 0.5f, "braco");
    corpo->adicionarMembro(glm::vec3(0.0f, 0.0f, 0.5f), 0.2f, 0.5f, "antebraco", "braco");

    /* Descomente essas duas linhas para rotação estática (necessário descomentar a rotação "dinamica com o seno Linhas 141 e 142 ") */
    /* corpo->rotacionarMembro(45.0f, "braco"); */
    /* corpo->rotacionarMembro(45.0f, "antebraco"); */

    /* vertex e fragment shader */
    const char *codigo_vertex_shader = R"glsl(
        #version 130
        in vec3 posicao;
        in vec3 cor_vertex;
        uniform mat4 transformacao; 
        uniform mat4 visualizacao; 
        uniform mat4 projecao; 

        out vec3 cor_fragment;

        void main(){
            cor_fragment = cor_vertex; 
            gl_Position = projecao * visualizacao * transformacao * vec4(posicao, 1.0f);
        }
    )glsl";

    const char *codigo_fragment_shader = R"glsl(
        #version 130
        in vec3 cor_fragment;        

        out vec4 cor;
        void main(){
            cor = vec4(cor_fragment, 1.0f);
        }
    )glsl";

    /* as proximas linhas criam e compilam os shaders, bem como os linkam para a criação do programa de shader utilizado pelo opengl */
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &codigo_vertex_shader, NULL);
    glCompileShader(vertex_shader);

    GLint retorno;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &retorno);
    if(retorno != GL_TRUE){
        char log[1024];
        glGetShaderInfoLog(vertex_shader, 1024, NULL, log);
        cout << "O vertex shader não pode ser compilado!" << endl << log << endl;
    }

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &codigo_fragment_shader, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &retorno);
    if(retorno != GL_TRUE){
        char log[1024];
        glGetShaderInfoLog(vertex_shader, 1024, NULL, log);
        cout << "O fragment shader não pode ser compilado!" << endl << log << endl;
    }

    programa_shader = glCreateProgram();
    glAttachShader(programa_shader, vertex_shader);
    glAttachShader(programa_shader, fragment_shader);
    glLinkProgram(programa_shader);
    glUseProgram(programa_shader);

    /* diz ao opengl como as coordenadas e as cores devem ser lidas do buffer de vértices e passadas às variáveis de entrada do shader */
    GLint indice_posicao = glGetAttribLocation(programa_shader, "posicao");
    glEnableVertexAttribArray(indice_posicao);
    glVertexAttribPointer(indice_posicao, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    
    GLint indice_cor = glGetAttribLocation(programa_shader, "cor_vertex");
    glEnableVertexAttribArray(indice_cor);
    glVertexAttribPointer(indice_cor, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));

    /* matrizes de visualização e projeção, ambas também associadas a entradas do vertex shader */
    glm::mat4 visualizacao = glm::lookAt(
           glm::vec3(1.2f, 1.0f, 1.3f),
           glm::vec3(0.0f, 0.0f, 0.5f),
           glm::vec3(0.0f, 0.0f, 1.0f)
    );
    GLint indice_visualizacao = glGetUniformLocation(programa_shader, "visualizacao");
    glUniformMatrix4fv(indice_visualizacao, 1, GL_FALSE, glm::value_ptr(visualizacao));
    
    glm::mat4 projecao = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 10.0f);
    GLint indice_projecao = glGetUniformLocation(programa_shader, "projecao");
    glUniformMatrix4fv(indice_projecao, 1, GL_FALSE, glm::value_ptr(projecao));
}


void ComponenteOpenGL::paintGL()
{
    auto t_atual = std::chrono::high_resolution_clock::now();
    float tempo_decorrido = std::chrono::duration_cast<std::chrono::duration<float>>(t_atual - t_inicial).count();
    float derivada_tempo = std::chrono::duration_cast<std::chrono::duration<float>>(t_atual - t_anterior).count();
    t_anterior = t_atual;

    makeCurrent();

    /* matriz de transformação para rotacionar os objetos, também ligada à uma variável do vertex shader */
    glm::mat4 transformacao = glm::mat4(1.0f);
    transformacao = glm::rotate(transformacao, tempo_decorrido/5 * glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    GLint transformacao_uniforme = glGetUniformLocation(programa_shader, "transformacao");
    glUniformMatrix4fv(transformacao_uniforme, 1, GL_FALSE, glm::value_ptr(transformacao));

    /* Como não houve tempo para implementar rotação por um delta e há apenas rotação absoluta, utiliza-se o seno e a derivada(mais ou menos) de tempo */ 
    /* cada segundo representa 90 graus nesse caso, para aumentar/diminuir o espaço de movimentação dos membros basta substituir as ocorrências do 90 */
    corpo->rotacionarMembro(sin(glm::radians(tempo_decorrido * 90)) * 90 * derivada_tempo, "braco");
    corpo->rotacionarMembro(sin(glm::radians(tempo_decorrido * 90)) * 90 * derivada_tempo, "antebraco");

    glDrawElements(GL_TRIANGLES, corpo->getQtdDesenhoVertices(), GL_UNSIGNED_INT, 0);

    update();

}

void ComponenteOpenGL::resizeGL(int w, int h)
{
}
