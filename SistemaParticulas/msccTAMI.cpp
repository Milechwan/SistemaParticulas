#include "msccTAMI.h"
using namespace std;
int countPart = 0; //contador de particulas
Particula colisao[5]; //tratar colisao entre as 5 partículas mais próximas
int indiceColisao;
int antigoCount;
GLfloat window_width = 500;
GLfloat window_height = 500;
int estado;
bool atualizarParticula;
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
	if (p.posicao.x > 500 || p.posicao.x <0 || p.posicao.y>500 || p.posicao.y<0) {
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

void trataColisao(Particula part, int indice) {
	for (int i = indice; i < countPart && indiceColisao<5; i++) {
		GLfloat distancia_x = part.posicao.x - sp.p[i].posicao.x;
		GLfloat distancia_y = part.posicao.y - sp.p[i].posicao.y;
		GLfloat distancia = sqrt(pow(distancia_x, 2) - pow(distancia_y, 2));
		if (distancia <= 2) {//testar esse valor!
			colisao[indiceColisao] = sp.p[i];
			sp.p[i].posicao.x = sp.p[i].posicao.x + (rand()%10);
			indiceColisao++;
		}
	}

}
void atualizaParticula() {
	for (int i = 0; i < countPart; i++) {
		if (!mataParticula(sp.p[i])) {//primeiro verifica se a partícula vai ser eliminada
			sp.p[i].posicao.x = sp.p[i].posicao.x + sp.p[i].velocidade.x;
			sp.p[i].posicao.y = sp.p[i].posicao.y + (rand() % 16) + sp.p[i].velocidade.y;
			trataColisao(sp.p[i],i+1);
		}
		else {
			for (int j = i; j < countPart - 1; j++) { //todas as partículas de posições posteriores são migradas
				sp.p[j] = sp.p[j + 1];
			}
			countPart--;
			atualizarParticula = false;
		}
	}
}

void geraParticula() {
	std::cout << "geraParticula" << std::endl;
	std::cout << countPart << std::endl;
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	//glLoadIdentity();
	if (countPart < 19 && !atualizarParticula) {
		 antigoCount = countPart;
		for (int i = countPart; i < (antigoCount + 9) && (antigoCount+9)<1000; i++) {
			GLfloat aux_x = sp.getOrigem().x;
			GLfloat aux_y = (rand() % 400)+1;
			glColor3f(0.5, 0.5, 0.5);
			glPointSize(3.0f);
			glBegin(GL_POINTS);
			glVertex2f(aux_x, aux_y); //gera partícula com ponto 2d
			glEnd();
			sp.p[i].posicao.y = aux_y;
			sp.p[i].posicao.x = aux_x;
			countPart++;//atualiza contador de partículas
		}
		for (int j = 0; j < antigoCount; j++) {//for para não 'apagar' partículas já geradas
			GLfloat aux_x = sp.p[j].posicao.x;
			GLfloat aux_y = sp.p[j].posicao.y;
			glColor3f(0.5, 0.5, 0.5);
			glPointSize(3.0f);
			glBegin(GL_POINTS);
			glVertex2f(aux_x, aux_y); 
			glEnd();
		}
		std::cout << "gerei" << std::endl;
		atualizarParticula = true;
	}
	else if (atualizarParticula) {
		atualizaParticula();
		for (int i = 0; i < countPart; i++) {
			GLfloat ax = sp.p[i].posicao.x;
			GLfloat ay = sp.p[i].posicao.y;
				//std::cout << "Posicao y particula" << " " << i << " "<< ay << std::endl;
				//std::cout << "Posicao x particula" << " " << i << " " << ax << std::endl;
			glColor3f(0.5, 0.5, 0.5);
			glPointSize(3.0f);
			glBegin(GL_POINTS);
			glVertex2f(ax, ay); 
			glEnd();
		}
		std::cout << "Como proceder?" << std::endl;
	}

	//glLoadIdentity();
	//glutSwapBuffers();
	glFlush();
}

void handleSpecialKeyboard(int key, int x, int y) {
	if (key==GLUT_KEY_RIGHT && countPart>0) {
		glutPostRedisplay();
	}
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
	glClearColor(0, 0, 0, 0); 
	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, glutGet(GLUT_WINDOW_WIDTH) - 1, glutGet(GLUT_WINDOW_HEIGHT) - 1, 0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	/*for (int i = 0; i < 1000; i++) {
		Particula temp_p = {};//supondo que dessa forma inicializa os pontos de posicao em 0.0 p/ x e y e velocidade=1.5
		sp.p[i] = temp_p;
	}*/
	for (int j = 0; j < 5; j++) {
		Particula temp_p = {};
		colisao[j] = temp_p;
	}
	indiceColisao = 0;
	estado = MODIFIED;
	atualizarParticula = false;
	antigoCount = 0;
	//geraParticula();
}

int main(int argc, char **argv)
{
	sp.origem.x = 10;
	sp.origem.y = 0;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	//glViewport(0, 0, glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
	//glLoadIdentity();
	glutInitWindowSize(500, 500);
	glutCreateWindow("teste");
	
	glutDisplayFunc(geraParticula);
	myinit();
	glutSpecialUpFunc(handleSpecialKeyboard);
	//glutIdleFunc(idle);
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cout << "Glew error" << std::endl;
		return 1;
	}
//	loop(0);
	glutMainLoop();
	return 0;
}
