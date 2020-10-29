#include <QtWidgets>
#include "ComponenteOpenGL.h"


#define JANELA_LARGURA 1280
#define JANELA_ALTURA 720
#define BORDA 10

int main(int argc, char * argv[]){
    /* aplicacao do QT e janela */
    QApplication aplicacao(argc, argv);
    QWidget janela;
    janela.resize(JANELA_LARGURA, JANELA_ALTURA);
    janela.setWindowTitle("Projeto QT + OpenGL");

    janela.show();

    /* classe herdada do QOpenGLWidget */
    ComponenteOpenGL *componenteGL = new ComponenteOpenGL(&janela);
    componenteGL->move(BORDA, BORDA);
    int alturaGL = janela.height() - 2 * BORDA;
    componenteGL->resize(alturaGL * 800.0f / 600.0f, alturaGL);
    
    componenteGL->show();
    return aplicacao.exec();
}
