#pragma once 

#include <QWidget>
#include <QOpenGLWidget>

class ComponenteOpenGL : public QOpenGLWidget {

        Q_OBJECT

    public:
        explicit ComponenteOpenGL(QWidget *pai = nullptr);

        ~ComponenteOpenGL();
    protected:
        void initializeGL();        
        void paintGL();        
        void resizeGL(int w, int h);        
};
