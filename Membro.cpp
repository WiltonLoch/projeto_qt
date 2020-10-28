#include "Membro.h"

using namespace std;

Membro::Membro(GLfloat raio, GLfloat altura, GLfloat x, GLfloat y, GLfloat z, GLfloat ang_rot, GLfloat vet_rot_x, GLfloat vet_rot_y, GLfloat vet_rot_z){
  this->raio = raio;
  this->altura = altura;
  posicao[0] = x;
  posicao[1] = y;
  posicao[2] = z;
  vet_rotacao[0] = ang_rot;
  vet_rotacao[1] = vet_rot_x;
  vet_rotacao[2] = vet_rot_y;
  vet_rotacao[3] = vet_rot_z;
}

Membro::Membro(GLfloat raio, GLfloat altura){
  this->raio = raio;
  this->altura = altura;
  posicao[0] = 0;
  posicao[1] = 0;
  posicao[2] = 0;
  vet_rotacao[0] = 0;
  vet_rotacao[1] = 0;
  vet_rotacao[2] = 0;
  vet_rotacao[3] = 0;
}

void Membro::rotacionar(GLfloat angulo){
  vet_rotacao[0] = angulo;
  for(size_t i = 0; i < membros_dependentes.size(); ++i)
  {
    membros_dependentes.at(i)->setVetorRotacaoPaterno(nivelFiliacao, vet_rotacao[0], vet_rotacao[1], vet_rotacao[2], vet_rotacao[3]);
  }
}

void Membro::adicionarMembroFilho(Membro *membro_filho){
    membro_filho->setNivelFiliacao(nivelFiliacao + 1);
    for (int i = 0; i < nivelFiliacao; ++i)
    {
      membro_filho->vetores_rotacao_paternos[i][0] = vetores_rotacao_paternos[i][0];
      membro_filho->vetores_rotacao_paternos[i][1] = vetores_rotacao_paternos[i][1];
      membro_filho->vetores_rotacao_paternos[i][2] = vetores_rotacao_paternos[i][2];
      membro_filho->vetores_rotacao_paternos[i][3] = vetores_rotacao_paternos[i][3];
    }
    membro_filho->vetores_rotacao_paternos[nivelFiliacao][0] = vet_rotacao[0];
    membro_filho->vetores_rotacao_paternos[nivelFiliacao][1] = vet_rotacao[1];
    membro_filho->vetores_rotacao_paternos[nivelFiliacao][2] = vet_rotacao[2];
    membro_filho->vetores_rotacao_paternos[nivelFiliacao][3] = vet_rotacao[3];
    for (int i = 0; i < nivelFiliacao; ++i)
    {
      membro_filho->vetores_posicao_paternos[i][0] = vetores_posicao_paternos[i][0];
      membro_filho->vetores_posicao_paternos[i][1] = vetores_posicao_paternos[i][1];
      membro_filho->vetores_posicao_paternos[i][2] = vetores_posicao_paternos[i][2];
    }
    membro_filho->vetores_posicao_paternos[nivelFiliacao][0] = posicao[0];
    membro_filho->vetores_posicao_paternos[nivelFiliacao][1] = posicao[1];
    membro_filho->vetores_posicao_paternos[nivelFiliacao][2] = posicao[2];
    membros_dependentes.push_back(membro_filho);

}

void Membro::display(){
    glPushMatrix();
     glColor3f(0.0, 0.5, 1.0);        
     for (int i = 0; i < nivelFiliacao; ++i)
     {
       glTranslatef(vetores_posicao_paternos[i][0], vetores_posicao_paternos[i][1], vetores_posicao_paternos[i][2]); 
       glRotatef(vetores_rotacao_paternos[i][0], vetores_rotacao_paternos[i][1], vetores_rotacao_paternos[i][2], vetores_rotacao_paternos[i][3]);           
     }     
     glTranslatef(X_MEMBRO, Y_MEMBRO, Z_MEMBRO);  
     glRotatef(vet_rotacao[0], vet_rotacao[1], vet_rotacao[2], vet_rotacao[3]);       
     glutSolidCylinder(raio, altura, 20, 20);
   glPopMatrix();
}

void Membro::setVetorRotacaoPaterno(int index, GLfloat ang_rot, GLfloat vet_rot_x, GLfloat vet_rot_y, GLfloat vet_rot_z){
  vetores_rotacao_paternos[index][0] = ang_rot;
  vetores_rotacao_paternos[index][1] = vet_rot_x;
  vetores_rotacao_paternos[index][2] = vet_rot_y;
  vetores_rotacao_paternos[index][3] = vet_rot_z;
  for(size_t i = 0; i < membros_dependentes.size(); ++i)
  {
    membros_dependentes.at(i)->setVetorRotacaoPaterno(index, vetores_rotacao_paternos[index][0], vetores_rotacao_paternos[index][1], vetores_rotacao_paternos[index][2], vetores_rotacao_paternos[index][3]);
  }
}
