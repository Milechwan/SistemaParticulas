#include "msccTAMI.h"
using namespace std;
int countPart = 0; //contador de particulas
Particula colisao[5]; //tratar colisao entre as 5 partículas mais próximas
int indiceColisao;
GLfloat window_width = 500;
GLfloat window_height = 500;
int estado;
class SistemaParticulas {
public:
	Particula p[1000]; //por enquanto, definido estaticamente para fins de teste
	float t; //timestep
			 //float *forcas; //ver se o valor de acumulador de for�as � float
	Ponto origem;

	Ponto getOrigem() {
		return origem;
	}

	Particula getParticula(int index) {
		return p[index];
	}
};
SistemaParticulas sp;
/*class SistemaParticulas {
public:
SistemaParticulas(int maxParts, Vector3 origem);
virtual void Atualiza(float tempoCorrido) = 0; //come�a em 0
virtual void render() = 0; //talvez usando coisa de opengGL :D
virtual int emitir(int num); //nascer quantidade num de particulas
virtual void inicializa();
virtual void termina();//matar as part�culas
protected:
virtual void inicializaP(int indice) = 0;
Particula *p;//lista de particulas
int maxParts;
int numParts;
Vector3 origem;
float tempoAcc;
Vector3 forca; //forca atuante no sistema; ver o que modela fogo!

}*/
bool mataParticula(Particula p) { //verifica se a partícula já passou da tela
	bool retorno = false;
	if (p.posicao.x > 1080 || p.posicao.x <0 || p.posicao.y>720 || p.posicao.y<0) {
		retorno = true;
	}
	return retorno;
}

/*void myreshape(GLsizei w, GLsizei h)
{
glViewport(0, 0, w, h);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
window_width = (GLfloat)w;
window_height = (GLfloat)h;
glOrtho(0, window_width, 0, window_height, -1.0, -1.0);
}*/

void geraParticula() {
	std::cout << "geraParticula" << std::endl;
	std::cout << countPart << std::endl;
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	if (countPart < 10) {
			GLfloat aux_x = (rand() % 50);
			GLfloat aux_y = sp.getOrigem().y;
			glColor3f(0.5, 0.5, 0.5);
			glPointSize(6.0f);
			glBegin(GL_POINTS);
			glVertex2f(aux_x,aux_y); //gera partícula com ponto 2d
			glEnd();
			sp.p[countPart].posicao.x = aux_x;
			std::cout << sp.getParticula(countPart).posicao.x << std::endl;
			countPart++; //atualiza contador de partículas
	}

	//glLoadIdentity();
	//glutSwapBuffers();
	glFlush();
}

void trataColisao(Particula part, int indice) {
	if (indice < countPart - 20) {
		for (int i = indice; indice < countPart && indiceColisao<5; i++) {
			GLfloat distancia_x = part.posicao.x - sp.p[i].posicao.x;
			GLfloat distancia_y = part.posicao.y - sp.p[i].posicao.y;
			GLfloat distancia = sqrt(pow(distancia_x, 2) - pow(distancia_y, 2));
			if (distancia <= 1) {//testar esse valor!
				colisao[indiceColisao] = sp.p[i];
			}
		}
	}

}

void atualizaParticula() {
	for (int i = 0; i < countPart; i++) {
		if (!mataParticula(sp.p[i])) {//primeiro verifica se a partícula vai ser eliminada
			sp.p[i].posicao.x = sp.p[i].posicao.x + sp.p[i].velocidade.x;
		}
		else {
			for (int j = i; j < countPart - 1; j++) { //todas as partículas de posições posteriores são migradas
				sp.p[j] = sp.p[j + 1];
			}
			countPart--;
		}
	}

	geraParticula(); //caso tenha matado particula(s), gerar outra(s)

}
void loop(int id)
{
	if (estado == MODIFIED) {
		geraParticula();
		estado = IDLE;
	}
	else if (estado != IDLE) {
		geraParticula();
	}
	glutTimerFunc(1000 / FPS, loop, id);
}

void myinit() {
	std::cout << "cheguei no myinit" << std::endl;
	glClearColor(0, 0, 0, 0); // moved this line to be in the init function
	glDisable(GL_DEPTH_TEST);

	// next four lines are new
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, glutGet(GLUT_WINDOW_WIDTH) - 1, glutGet(GLUT_WINDOW_HEIGHT) - 1, 0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	for (int i = 0; i < 1000; i++) {
		Particula temp_p = {};//supondo que dessa forma inicializa os pontos de posicao em 0.0 p/ x e y e velocidade=1.5
		sp.p[i] = temp_p;
	}
	std::cout << "acabou o for" << std::endl;
	for (int j = 0; j < 5; j++) {
		Particula temp_p = {};
		colisao[j] = temp_p;
	}
	indiceColisao = 0;
	estado = MODIFIED;
	//geraParticula();
}

int main(int argc, char **argv)
{
	sp.origem.x = 0;
	sp.origem.y = 120;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	//glViewport(0, 0, glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
	//glLoadIdentity();
	glutInitWindowSize(1080, 720);
	glutCreateWindow("teste");
	myinit();
	glutDisplayFunc(geraParticula);
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cout << "Glew error" << std::endl;
		return 1;
	}
//	loop(0);
	glutMainLoop();
	return 0;
}
