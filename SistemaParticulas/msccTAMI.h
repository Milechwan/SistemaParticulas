#include <GL\glew.h>﻿
#include <GL\freeglut.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <vector>

#define FPS 50
struct Ponto {
	GLfloat x;
	GLfloat y;
	Ponto(GLfloat x = 0.0, GLfloat y = 0.0) :x(x), y(y) {}
};

struct Particula {
	//float massa;
	Ponto posicao;
	//Vector3 posicao; //ler como usar vector3
	//Ponto posicao_anterior;
	//Vector3 velocidade;
	//Vector3 aceleracao;
	Ponto velocidade;
	//float tamanho;
	//bool chegou; //para saber se chegou no ponto
	//float mudaTamanho; //size = size_antigo+taxa_de_varia�ao*random
	Particula(Ponto posicao = { 0,0 }, Ponto velocidade = { 1.5,1.5 }) :posicao(posicao),
		velocidade(velocidade) {}
};