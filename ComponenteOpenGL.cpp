#include <cmath>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ComponenteOpenGL.h"

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
    initializeOpenGLFunctions();

    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,

        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,

         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,

        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,

        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f
};

    GLuint ordem[]{
        0, 1, 2,
        2, 3, 0
    };

    t_inicial = std::chrono::high_resolution_clock::now();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ordem), ordem, GL_DYNAMIC_DRAW);

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

    GLint indice_posicao = glGetAttribLocation(programa_shader, "posicao");
    glEnableVertexAttribArray(indice_posicao);
    glVertexAttribPointer(indice_posicao, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    
    GLint indice_cor = glGetAttribLocation(programa_shader, "cor_vertex");
    glEnableVertexAttribArray(indice_cor);
    glVertexAttribPointer(indice_cor, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));

    glm::mat4 visualizacao = glm::lookAt(
           glm::vec3(1.2f, 1.2f, 1.2f),
           glm::vec3(0.0f, 0.0f, 0.0f),
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
    /* glDrawArrays(GL_TRIANGLES, 0, 3); */
    float time = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - t_inicial).count();

    makeCurrent();

    glm::mat4 transformacao = glm::mat4(1.0f);
    transformacao = glm::rotate(transformacao, time/5 * glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    GLint transformacao_uniforme = glGetUniformLocation(programa_shader, "transformacao");
    glUniformMatrix4fv(transformacao_uniforme, 1, GL_FALSE, glm::value_ptr(transformacao));

    /* glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); */
    glDrawArrays(GL_TRIANGLES, 0, 36);

    update();

}

void ComponenteOpenGL::resizeGL(int w, int h)
{
    /* glViewport(0,0,w,h); */
    /* glMatrixMode(GL_PROJECTION); */
    /* /1* glLoadIdentity(); *1/ */
    /* /1* gluPerspective(45, (float)w/h, 0.01, 100.0); *1/ */
    /* glMatrixMode(GL_MODELVIEW); */
    /* glLoadIdentity(); */
    /* /1* gluLookAt(0,0,5,0,0,0,0,1,0); *1/ */
}
