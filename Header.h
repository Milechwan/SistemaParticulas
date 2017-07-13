#include "GL/glut.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>

#define MODIFIED -1
#define IDLE -2
#define FPS 30
struct Ponto {
	GLfloat x;
	GLfloat y;
};

struct Particula {
	//float massa;
	Ponto posicao;
	//Vector3 posicao; //ler como usar vector3
	//Ponto posicao_anterior;
	//Vector3 velocidade;
	//Vector3 aceleracao;
	Ponto velocidade;
	float tamanho;
	//bool chegou; //para saber se chegou no ponto
	//float mudaTamanho; //size = size_antigo+taxa_de_varia�ao*random
};

struct SistemaParticulas {
	Particula p[1000]; //por enquanto, definido estaticamente para fins de teste
					   //int n; //numero de particulas
	float t; //timestep
			 //int maxP; //numero m�ximo de part�culas
	float *forcas; //ver se o valor de acumulador de for�as � float
	Ponto origem;
};
