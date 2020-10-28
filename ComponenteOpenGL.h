#pragma once 

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_0>
#include <chrono>

class ComponenteOpenGL : public QOpenGLWidget, protected QOpenGLFunctions_3_0{

        Q_OBJECT

    public:
        explicit ComponenteOpenGL(QWidget *pai = nullptr);

        ~ComponenteOpenGL();
    protected:
        void initializeGL();        
        void paintGL();        
        void resizeGL(int w, int h);        
    private:
        std::chrono::high_resolution_clock::time_point t_inicial;
        GLuint vbo;
        GLuint vao;
        GLuint ebo;
        GLuint vertex_shader;
        GLuint fragment_shader;
        GLuint programa_shader;
};
