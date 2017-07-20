#include "msccTAMI.h"
using namespace std;
int countPart = 0; //contador de particulas
Ponto auxColisao[1000];
int antigoCount;
Ponto obstaculos[9];
GLfloat window_width = 500;
GLfloat window_height = 500;
int countMortas;
bool atualizarParticula;
class SistemaParticulas {
public:
	Particula p[1000];
	Ponto origem;

	Ponto getOrigem() {
		return origem;
	}

	Particula getParticula(int index) {
		return p[index];
	}
};
SistemaParticulas sp;

bool mataParticula(Particula p) { //verifica se a partícula já passou da tela
	bool retorno = false;
	if (p.posicao.x > glutGet(GLUT_WINDOW_WIDTH) || p.posicao.x <0 ||
		p.posicao.y>glutGet(GLUT_WINDOW_HEIGHT) || p.posicao.y<0) {
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
	if (indice <= floor(countPart / 2)) {
		for (int i = 0; i < countPart; i++) {//procura entre todas as partículas
			Ponto k = auxColisao[i];
			double distancia_y = abs(part.posicao.y - k.y);
			double distancia_x = abs(part.posicao.x - k.x);
			if (indice - 1 != i && k.y <= part.posicao.y+4 &&
				k.y>=part.posicao.y-4) {
				sp.p[i].posicao.y = sp.p[i].posicao.y + distancia_y;//usa o valor da distância no eixo entre os dois pontos para atualizar a posição da partícula
			}
			else if (indice - 1 != i && k.x <= part.posicao.x + 3 &&
				k.x >= part.posicao.x - 3) {
				sp.p[i].posicao.x = sp.p[i].posicao.x + distancia_x;
			}
			//colisão nos obstáculos
			for (int a = 0; a < 15; a++) {
				Ponto aux = obstaculos[a];
				double distancia_y = abs(part.posicao.y - aux.y);
				double distancia_x = abs(part.posicao.x - aux.x);
				if (distancia_y <= 2 && distancia_x > 2) {
					sp.p[i].posicao.x = sp.p[i].posicao.x + 2;
				}
				else if (distancia_y > 2 && distancia_x <= 2) {
					sp.p[i].posicao.y = sp.p[i].posicao.y + 2;
				}
			}
		}
	}
	else {
		for (int j = countPart - 1; j > -1; j--) {//procura de trás pra frente
			Ponto k = auxColisao[j];
			double distancia_y = abs(part.posicao.y - k.y);
			double distancia_x = abs(part.posicao.x - k.x);
			if (indice - 1 != j && k.y <= part.posicao.y + 3 &&
				k.y >= part.posicao.y - 3) {//indice-1 é o índice da partícula part
				sp.p[j].posicao.y = sp.p[j].posicao.y + distancia_y;
			}
			else if (indice - 1 != j && k.x <= part.posicao.x + 3 &&
				k.x >= part.posicao.x - 3) {
				sp.p[j].posicao.x = sp.p[j].posicao.x + distancia_x;
			}
			//colisão nos obstáculos
			for (int a = 0; a < 9; a++) {
				Ponto aux = obstaculos[a];
				double distancia_y = abs(part.posicao.y - aux.y);
				double distancia_x = abs(part.posicao.x - aux.x);
				if (distancia_y <= 4 && distancia_x>4) {
					sp.p[j].posicao.x = sp.p[j].posicao.x + 1;
				}
				else if (distancia_y > 4 && distancia_x <= 4) {
					sp.p[j].posicao.y = sp.p[j].posicao.y + 1;
				}
			}
		}
	}
}
void atualizaParticula() {
	for (int i = 0; i < countPart; i++) {
		if (!mataParticula(sp.p[i])) {//primeiro verifica se a partícula vai ser eliminada
			sp.p[i].posicao.x = sp.p[i].posicao.x + sp.p[i].velocidade.x;
			sp.p[i].posicao.y = sp.p[i].posicao.y + (rand() % 3)+sp.p[i].velocidade.y;
			//atualiza posições levando em conta somente velocidade
			Ponto aux = {};
			aux.x = sp.p[i].posicao.x;
			aux.y = sp.p[i].posicao.y;
			auxColisao[i] = aux;//põe o ponto no array auxiliar para tratar colisão
			trataColisao(sp.p[i], i + 1);
		}
		else {
			for (int j = i; j < countPart - 1; j++) { //todas as partículas de posições posteriores são migradas
				sp.p[j] = sp.p[j + 1];
			}
			countPart--;
			countMortas++;
			if (countMortas == 3) {//deixar que morram 3 partículas para gerar as outras 9
				atualizarParticula = false;
				countMortas = 0;
			}
		}
	}
}

void geraParticula(void) {
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	std::cout << "Particulas: "<<countPart << std::endl;
	if (countPart <1000 && !atualizarParticula) {
		 antigoCount = countPart;
		for (int i = countPart; i < (antigoCount + 9) && (antigoCount+9)<1000; i++) {
			GLfloat aux_x = (rand() % 400) + 1;
			GLfloat aux_y = sp.getOrigem().y;
			glColor3f(0.5, 0.5, 0.5);
			glPointSize(4.0f);
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
			glPointSize(4.0f);
			glBegin(GL_POINTS);
			glVertex2f(aux_x, aux_y); 
			glEnd();
		}
		atualizarParticula = true;
	}
	else if (atualizarParticula) {
		atualizaParticula();
		for (int i = 0; i < countPart; i++) {
			GLfloat ax = sp.p[i].posicao.x;
			GLfloat ay = sp.p[i].posicao.y;
			glColor3f(0.5, 0.5, 0.5);
			glPointSize(4.0f);
			glBegin(GL_POINTS);
			glVertex2f(ax, ay); 
			glEnd();
		}
	}
	for (int a = 0; a < 9; a++) { //desenhando os obstáculos
		glColor3f(0.0, 1.0, 0.0);
		glPointSize(3.0f);
		glBegin(GL_POINTS);
		glVertex2f(obstaculos[a].x, obstaculos[a].y);
		glEnd();
	}
	glFlush();
}

void handleSpecialKeyboard(int key, int x, int y) {
	if (key==GLUT_KEY_RIGHT && countPart>0) {
		glutPostRedisplay(); //para passar as 'cenas'
	}
}

void loop(int id)
{
	glutPostRedisplay();
	glutTimerFunc(FPS*10, loop, id);
}

void myinit() {
	glClearColor(0, 0, 0, 0); 
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, glutGet(GLUT_WINDOW_WIDTH) - 1, glutGet(GLUT_WINDOW_HEIGHT) - 1, 0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	countMortas = 0;
	atualizarParticula = false;
	antigoCount = 0;
	obstaculos[0].x = 200;
	obstaculos[0].y = 200;
	for (int j = 1; j < 5; j++) { //obstáculo 1
		obstaculos[j].x = obstaculos[j-1].x + 2;
		obstaculos[j].y = 200;
	}
	obstaculos[5].x = 100;
	obstaculos[5].y = 100;
	for (int k = 6; k < 9; k++) {//obstáculo 2
		obstaculos[k].x = 100;
		obstaculos[k].y = obstaculos[k - 1].y + 2;
	}
}

int main(int argc, char **argv)
{
	sp.origem.x = 0;
	sp.origem.y = 2;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(700, 700);
	glutCreateWindow("teste");
	glutDisplayFunc(geraParticula);
	myinit();
	//glutIdleFunc(geraParticula);
	glutSpecialUpFunc(handleSpecialKeyboard);
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cout << "Glew error" << std::endl;
		return 1;
	}
	loop(1);
	glutMainLoop();
	return 0;
}
