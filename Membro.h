#pragma once
#include <GL/gl.h>
#include <cstdlib>
#include <vector>
#include <GL/freeglut.h>

#define X_MEMBRO posicao[0]
#define Y_MEMBRO posicao[1]
#define Z_MEMBRO posicao[2]

class Membro{
    public:
        Membro(GLfloat raio, GLfloat altura, GLfloat x, GLfloat y, GLfloat z, GLfloat ang_rot, GLfloat vet_rot_x, GLfloat vet_rot_y, GLfloat vet_rot_z);
        Membro(GLfloat raio, GLfloat altura);
        ~Membro();

        void rotacionar(GLfloat angulo);
        void mover(GLfloat x, GLfloat y, GLfloat z);
        void adicionarMembroFilho(Membro *membro_filho);
        void display();   

        void setVetorRotacaoPaterno(int index, GLfloat ang_rot, GLfloat vet_rot_x, GLfloat vet_rot_y, GLfloat vet_rot_z); 
        void setNivelFiliacao(int fil){nivelFiliacao = fil;};
        int getNivelFiliacao(){return nivelFiliacao;};

        GLfloat getRaio(){return raio;};
        GLfloat getAltura(){return altura;};
        GLfloat getPosX(){return X_MEMBRO;};
        GLfloat getPosY(){return Y_MEMBRO;};
        GLfloat getPosZ(){return Z_MEMBRO;};
        GLfloat getAngulo(){return vet_rotacao[0];};

    private:  
        int nivelFiliacao = 0;
        GLfloat raio = 0;
        GLfloat altura = 0;
        GLfloat cor[3] = {0.0, 0.45, 1.0};
        GLfloat posicao[3] = {0.0, 0.0, 0.0};
        GLfloat vet_rotacao[4] = {0.0, 0.0, 0.0, 0.0};
        GLfloat vetores_rotacao_paternos[10][4] = {{0, 0, 0, 0}};
        GLfloat vetores_posicao_paternos[10][3] = {{0, 0, 0}};
        std::vector<Membro*> membros_dependentes;
};
