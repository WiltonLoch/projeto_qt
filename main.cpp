#include <QtWidgets>
#include "ComponenteOpenGL.h"


#define JANELA_LARGURA 1280
#define JANELA_ALTURA 720
#define BORDA 10

int main(int argc, char * argv[]){
    QApplication aplicacao(argc, argv);
    QWidget janela;
    janela.resize(JANELA_LARGURA, JANELA_ALTURA);
    janela.setWindowTitle("Projeto QT + OpenGL");

    janela.show();

    ComponenteOpenGL *componenteGL = new ComponenteOpenGL(&janela);
    componenteGL->move(BORDA, BORDA);
    componenteGL->resize(janela.width() * 3/4, janela.height() - 2 * BORDA);
    
    componenteGL->show();
    return aplicacao.exec();
}
