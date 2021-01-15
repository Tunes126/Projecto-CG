#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glext.h>
#include <math.h> 
#include <mmsystem.h>
#include <windows.h>
#pragma comment(lib, "glew32.lib")

using namespace std;
int J = 1;
GLuint solTex, mercurioTex, venusTex, terraTex, marteTex, jupiterTex, saturnoTex, uranoTex, neptunoTex, asteroidesTEX, fundoTex, logTexture, hatTexture, hat2Texture, hat3Texture;
float oldmouseX = 0;
float mouseY = 0;
float oldmouseY = 0;
float mouseX = 0;
int rotacao = 0;
int fAnimate = 0;
int orbPlaneta = 0;
int orbLuas = 0;
int fLua = 1;
int mudarvista = 0;
int frameCount = 0;
int fChapeu = 1;
int FName = 1;
int zoom = 100;
int ang1 = 0;
int ang2 = 0;
int r = 120;
float pi = 3.14159265359;
int transx = 0;
int transy = 0;
int transz = 0;
int pausa = 0;
int rx = 0;
int ry = 0;
int rz = 0;
float vAsteroides = 0;
float lightPos[] = { 0.0, 0.0, -65.0, 1.0 }; // Posição do Spotlight.
static float spotAngle = 360; // angulo de iluminação
float spotDirection[] = { 1.0, 0.0, 0.0 }; // Direção do Spotlight.
static float spotExponent = 1.0; // factor de atenuação do Spotlight.
class Planeta {
public:
	string nome;
	float raio, distancia, orbita, orbitaS;
	Planeta(float _raio, float _distancia, float _orbita, float _orbitaS) {
		raio = _raio;
		distancia = _distancia;
		orbita = _orbita;
		orbitaS = _orbitaS;
	}

	void criaPlaneta(GLUquadricObj* quadric, GLuint Texture) {
		glRotatef(orbita, 0.0, 1.0, 0.0);
		glTranslatef(distancia, 0.0, 0.0);
		glPushMatrix();
		if(distancia == 0)
		glRotatef(0, 0.0, 1.0, 0.0);
		else glRotatef(rotacao, 0.0, 1.0, 0.0);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		gluQuadricTexture(quadric, 1);
		gluSphere(quadric, raio, 20.0, 20.0);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
		if (raio == 3.0) {
			glPushMatrix();
			glColor3ub(158, 145, 137);
			glRotatef(-63.0, 1.0, 0.0, 0.0);
			glutSolidTorus(0.3, 5.0, 30.0, 30.0);
			glPopMatrix();
		}
	}

	void draworbLua(void) {
		glPushMatrix();
		glColor3ub(255, 255, 255);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		glutWireTorus(0.001, distancia, 100.0, 100.0);
		glPopMatrix();
	}

	void drawLua(void) {
		GLUquadricObj* quadric;
		quadric = gluNewQuadric();
		glPushMatrix();
		glColor3ub(255, 255, 255);
		glRotatef(orbita, 0.0, 1.0, 0.0);
		glTranslatef(distancia, 0.0, 0.0);
		gluSphere(quadric, raio, 20.0, 20.0);
		glPopMatrix();
	}

};


//Astros do Sistema
//Astros do Sistema
Planeta sun(15.0, 0, 0, 0);				//Sol
Planeta mer(1.0, 21, 0, 4.74);		//Mercurio
Planeta ven(1.5, 25, 0, 3.50);		//Venus
Planeta ear(2.0, 33, 0, 2.98);		//Terra
Planeta mar(1.7, 42, 0, 2);		//Marte
Planeta jup(3.5, 62, 0, 1.31);		//Jupiter
Planeta sat(3.0, 77, 0, 0.97);		//Saturno
Planeta ura(2.5, 86, 0, 0.68);	//Uranos
Planeta nep(2.3, 93.6, 0, 0.54);	//Neptuno
Planeta lun(.40, 3, 0, 5.40);			//Lua     (Terra)
Planeta pho(.20, 1.8, 0, 2.30);		//Phobos   (Marte)
Planeta dei(.24, 2.4, 0, 3.60);		//Deimos   (Marte)
Planeta eur(.24, 4, 0, 4.40);			//Europa   (Jupiter)
Planeta gan(.24, 4.7, 0, 5.00);		//Ganymede (Jupiter)
Planeta cal(.24, 5.3, 0, 2.30);		//Callisto (Jupiter)
Planeta nix(.10, 1.5, 0, 5.00);		//Nix	   (Plutão)
Planeta puc(.26, 2.9, 0, 7.00);		//Puck	   (Uranos)
Planeta tri(.36, 3.2, 0, 3.40);		//Triton   (Neptuno)



void writeBitmapString(void* font, char* string)
{
	char* c;
	for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

#define EXIT {fclose(ficheiro);return -1;}
#define CTOI(C) (*(int*)&C)	

int LoadBMP(char* File)
{
	unsigned char* Data;
	FILE* ficheiro;
	unsigned char	Header[0x36];
	GLuint			DataPos, DataSize;
	GLint			Components;
	GLsizei			Width, Height;
	GLenum			Format, Type;
	GLuint			Name[1];

	FILE* fp;
	errno_t err;
	fopen_s(&ficheiro, File, "rb");
	if (!ficheiro) return -1;
	if (fread(Header, 1, 0x36, ficheiro) != 0x36) EXIT;
	if (Header[0] != 'B' || Header[1] != 'M')	EXIT;
	if (CTOI(Header[0x1E]) != 0)				EXIT;
	if (CTOI(Header[0x1C]) != 24)				EXIT;

	DataPos = CTOI(Header[0x0A]);
	DataSize = CTOI(Header[0x22]);
	Width = CTOI(Header[0x12]);
	Height = CTOI(Header[0x16]);
	Type = GL_UNSIGNED_BYTE;
	Format = GL_RGB;
	Components = 3;

	if (DataSize == 0) DataSize = Width * Height * Components;
	if (DataPos == 0)  DataPos = 0x36;

	fseek(ficheiro, DataPos, 0);
	Data = new unsigned char[DataSize];
	if (!Data) EXIT;

	if (fread(Data, 1, DataSize, ficheiro) != DataSize)
	{
		delete Data;
		fclose(ficheiro);
		return -1;
	}

	fclose(ficheiro);

	unsigned char t;
	for (int x = 0; x < Width * Height; x++)
	{
		t = Data[x * 3];
		Data[x * 3] = Data[x * 3 + 2];
		Data[x * 3 + 2] = t;
	}

	//Enviar a Textura ao OpenGL
	glGenTextures(1, Name);
	glBindTexture(GL_TEXTURE_2D, Name[0]);

	glTexImage2D
	(
		GL_TEXTURE_2D, 	//alvo
		0,				//nivel  mipmap 
		Components,		//nb cores
		Width,			//largura
		Height,			//altura
		0,			 	//largura
		GL_RGB,			//cores
		GL_UNSIGNED_BYTE,			//codificacao
		Data			//Imagem
	);

	return Name[0];
}

void setup(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);

	//Texturas
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	fundoTex = LoadBMP("space.bmp"); solTex = LoadBMP("sol.bmp"); mercurioTex = LoadBMP("mercurio.bmp");
	venusTex = LoadBMP("venus.bmp"); terraTex = LoadBMP("terra.bmp"); marteTex = LoadBMP("marte.bmp");
	jupiterTex = LoadBMP("jupiter.bmp"); saturnoTex = LoadBMP("saturno.bmp"); uranoTex = LoadBMP("uranos.bmp");
	hatTexture = LoadBMP("hat1.bmp"); hat2Texture = LoadBMP("hat2.bmp"); hat3Texture = LoadBMP("hat3.bmp");
	neptunoTex = LoadBMP("neptuno.bmp"); asteroidesTEX = LoadBMP("asteroides.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//Iluminação
	glEnable(GL_LIGHTING);
	float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
	float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float globAmb[] = { 0.5, 0.5, 0.5, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
	glEnable(GL_LIGHT0);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, spotAngle);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDirection);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, spotExponent);

}

void orbitalTrails(void) {
	glPushMatrix();
	glColor3ub(255, 255, 255);
	glTranslatef(0.0, 0.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glutWireTorus(0.001, mer.distancia, 100.0, 100.0);
	glutWireTorus(0.001, ven.distancia, 100.0, 100.0);
	glutWireTorus(0.001, ear.distancia, 100.0, 100.0);
	glutWireTorus(0.001, mar.distancia, 100.0, 100.0);
	glutWireTorus(0.001, jup.distancia, 100.0, 100.0);
	glutWireTorus(0.001, sat.distancia, 100.0, 100.0);
	glutWireTorus(0.001, ura.distancia, 100.0, 100.0);
	glutWireTorus(0.001, nep.distancia, 100.0, 100.0);
	glPopMatrix();
}

void drawScene(void) {
	frameCount++;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//Rotação
	glRotatef(rx, 1, 0, 0);
    gluLookAt(transx + r * cos(ang2 * pi / 180) * sin(ang1 * pi / 180), zoom, transz + r * cos(ang1 * pi / 180), transx, transy, transz, 0, 1, 0);
	if (orbPlaneta == 1) orbitalTrails();

	glRotatef(rx, 1, 0, 0);
	glRotatef(ry, 0, 1, 0);
	glRotatef(rz, 0, 0, 1);
	GLUquadric* quadric;
	quadric = gluNewQuadric();

	//Sol
	glPushMatrix();
	sun.criaPlaneta(quadric, solTex);
	if (fChapeu == 0) {
		//codigo chapeu
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, hatTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		gluQuadricTexture(quadric, 1);
		glBindTexture(GL_TEXTURE_2D, hatTexture);
		glTranslatef(0, 15, 0);
		glRotatef(-90, 1, 0, 0);
		gluCylinder(quadric, 4, 0, 7, 20, 20);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
		//fim codigo chapeu
	}
	if (FName == 0) {
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Sol");
		glPopMatrix();
	}

	//Mercurio
	glPushMatrix();
	mer.criaPlaneta(quadric, mercurioTex);
	if (fChapeu == 0) {

		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, hatTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		gluQuadricTexture(quadric, 1);
		glBindTexture(GL_TEXTURE_2D, hatTexture);
		glTranslatef(0, 1, 0);
		glRotatef(-90, 1, 0, 0);
		gluCylinder(quadric, 0.5, 0, 1.5, 20, 20);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
	if (FName == 0) {
		glRasterPos3f(0.0, 3, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Mercurio");
	}
	glPopMatrix();
	//cometa
	int i = 1;

	glPushMatrix();
	glRotatef(0, 0, 1.0, 0);
	glTranslatef(J * 1.5 - 200, 25 - J * 0.35, -J);
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, asteroidesTEX);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, 2.5, 100.0, 2.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

	//Venus
	glPushMatrix();
	ven.criaPlaneta(quadric, venusTex);
	if (fChapeu == 0) {
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, hat2Texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  //criar chapeu com textura
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		gluQuadricTexture(quadric, 1);

		glBindTexture(GL_TEXTURE_2D, hat2Texture);
		glTranslatef(0, 1.5, 0);
		glRotatef(-90, 1, 0, 0);
		gluCylinder(quadric, 0.5, 0, 1.25, 20, 20);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
	if (FName == 0) {
		glRasterPos3f(0.0, 3, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Venus");
	}
	glPopMatrix();



	//cintura de asteroides 1
	i = 1;

	for (; i < 1400; i++) {

		float r3 = 20 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (26 - 20)));
		glPushMatrix();
		glRotatef((vAsteroides + (i * 0.5274)), 0.0, 1.0, 0.0);
		glTranslatef(49 + (2.5 * cos(i)), 0.0, 0.0);
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, asteroidesTEX);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		gluQuadricTexture(quadric, 1);
		gluSphere(quadric, .30, 100.0, 2.0);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
		glPopMatrix();
	}

	//cintura de asteroides 2
	i = 0;
	for (; i < 1400; i++) {

		float r3 = 20 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (26 - 20)));
		glPushMatrix();
		glRotatef((vAsteroides + (i * 0.5274)), 0.0, 1.0, 0.0);
		glTranslatef(110 + (2.5 * cos(i)), 0.0, 0.0);
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, asteroidesTEX);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		gluQuadricTexture(quadric, 1);
		gluSphere(quadric, .30, 100.0, 2.0);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
		glPopMatrix();
	}


	//Terra, Orbita, Lua
	glPushMatrix();
	ear.criaPlaneta(quadric, terraTex);
	if (fChapeu == 0) {
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, hat3Texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  //criar chapeu com textura
		gluQuadricTexture(quadric, 1);
		glBindTexture(GL_TEXTURE_2D, hat3Texture);
		glTranslatef(0, 2, 0);
		glRotatef(-90, 1, 0, 0);
		gluCylinder(quadric, 0.75, 0, 1.50, 20, 20);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
	if (FName == 0) {
		glRasterPos3f(0.0, 3, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Terra");
	}

	if (fLua == 1) {
		if (orbLuas == 1) {
			lun.draworbLua();
		}
		lun.drawLua();
	}
	glPopMatrix();

	//Marte (luas,orbitas)
	glPushMatrix();
	mar.criaPlaneta(quadric, marteTex);
	if (fChapeu == 0) {


		glPushMatrix();

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, hat2Texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		gluQuadricTexture(quadric, 1);

		glBindTexture(GL_TEXTURE_2D, hat2Texture);
		glTranslatef(0, 1.7, 0);
		glRotatef(-90, 1, 0, 0);
		gluCylinder(quadric, 0.50, 0, 1.525, 20, 20);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
	if (FName == 0) {
		glRasterPos3f(0.0, 3, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Marte");
	}
	if (fLua == 1) {
		if (orbLuas == 1) {
			pho.draworbLua();
			dei.draworbLua();
		}
		pho.drawLua();
		dei.drawLua();
	}
	glPopMatrix();

	//Jupiter
	glPushMatrix();
	jup.criaPlaneta(quadric, jupiterTex);
	if (fChapeu == 0) {
		glPushMatrix();

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, hatTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		gluQuadricTexture(quadric, 1);
		glBindTexture(GL_TEXTURE_2D, hatTexture);
		glTranslatef(0, 3.5, 0);
		glRotatef(-90, 1, 0, 0);
		gluCylinder(quadric, 1, 0, 2.50, 20, 20);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
	if (FName == 0) {
		glRasterPos3f(0.0, 4.4, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Jupiter");
	}
	if (fLua == 1) {
		if (orbLuas == 1) {
			eur.draworbLua();
			gan.draworbLua();
			cal.draworbLua();
		}
		eur.drawLua();
		gan.drawLua();
		cal.drawLua();
	}
	glPopMatrix();

	//Saturo
	glPushMatrix();
	sat.criaPlaneta(quadric, saturnoTex);
	if (fChapeu == 0) {
		glPushMatrix();

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, hat3Texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		gluQuadricTexture(quadric, 1);
		glBindTexture(GL_TEXTURE_2D, hat3Texture);
		glTranslatef(0, 3, 0);
		glRotatef(-90, 1, 0, 0);
		gluCylinder(quadric, 0.75, 0, 2, 20, 20);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();

	}
	if (FName == 0) {
		glRasterPos3f(0.0, 4.4, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Saturno");
	}

	glPopMatrix();

	glColor3ub(255, 255, 255);

	//Uranos
	glPushMatrix();
	ura.criaPlaneta(quadric, uranoTex);
	if (fChapeu == 0) {
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, hat2Texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		gluQuadricTexture(quadric, 1);
		glBindTexture(GL_TEXTURE_2D, hat2Texture);
		glTranslatef(0, 2.5, 0);
		glRotatef(-90, 1, 0, 0);
		gluCylinder(quadric, 0.75, 0, 1.75, 20, 20);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
	if (FName == 0) {
		glRasterPos3f(0.0, 4.4, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Urano");
	}
	if (fLua == 1) {
		if (orbLuas == 1) {
			puc.draworbLua();
		}
		puc.drawLua();
	}
	glPopMatrix();

	//Neptuno
	glPushMatrix();
	nep.criaPlaneta(quadric, neptunoTex);
	if (fChapeu == 0) {
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, hatTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		gluQuadricTexture(quadric, 1);
		glBindTexture(GL_TEXTURE_2D, hatTexture);
		glTranslatef(0, 2.3, 0);
		glRotatef(-90, 1, 0, 0);
		gluCylinder(quadric, 0.75, 0, 1.75, 20, 20);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
	if (FName == 0) {
		glRasterPos3f(0.0, 4.4, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Neptuno");
	}
	if (fLua == 1) {
		if (orbLuas == 1) {
			tri.draworbLua();
		}
		tri.drawLua();
	}
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, fundoTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBegin(GL_POLYGON);
	glTexCoord2f(-1.0, 0.0); glVertex3f(-300, -300, -150);
	glTexCoord2f(2.0, 0.0); glVertex3f(300, -300, -150);
	glTexCoord2f(2.0, 2.0); glVertex3f(300, 300, -150);
	glTexCoord2f(-1.0, 2.0); glVertex3f(-300, 300, -150);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, fundoTex);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-300, -166, 300);
	glTexCoord2f(8.0, 0.0); glVertex3f(300, -166, 300);
	glTexCoord2f(8.0, 8.0); glVertex3f(300, -166, -300);
	glTexCoord2f(0.0, 8.0); glVertex3f(-300, -166, -300);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glutSwapBuffers();
}


void resize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 400.0);
	glMatrixMode(GL_MODELVIEW);
}

void animate(int n) {
	if (fAnimate) {
		mer.orbita += mer.orbitaS;
		ven.orbita += ven.orbitaS;
		ear.orbita += ear.orbitaS;
		mar.orbita += mar.orbitaS;
		jup.orbita += jup.orbitaS;
		sat.orbita += sat.orbitaS;
		ura.orbita += ura.orbitaS;
		nep.orbita += nep.orbitaS;
		lun.orbita += lun.orbitaS;
		pho.orbita += pho.orbitaS;
		dei.orbita += dei.orbitaS;
		eur.orbita += eur.orbitaS;
		gan.orbita += gan.orbitaS;
		cal.orbita += cal.orbitaS;
		puc.orbita += puc.orbitaS;
		tri.orbita += tri.orbitaS;
		vAsteroides += 1.5;
		if (mer, ven, ear, mar, jup, sat, ura, nep, lun, pho, dei, eur, gan, cal, puc, tri.orbita > 360.0) {
			mer, ven, ear, mar, jup, sat, ura, nep, lun, pho, dei, eur, gan, cal, puc, tri.orbita -= 360.0;
		}
		rotacao += 10.0;
		if (rotacao > 360.0) {
			rotacao -= 360.0;
		}
		J += 1.5;
		glutPostRedisplay();
		glutTimerFunc(30, animate, 1);
	}
}



void mouseWheel(int wheel, int direction, int x, int y)
{
	if (direction > 0 && zoom < 100) zoom++;
	if (direction < 0 && zoom > -75) zoom--;
	glutPostRedisplay();
}

void menu_act(unsigned char key, int x, int y) {
	switch (key) {
	case 27: exit(0); break;
	case ' ': if (fAnimate) fAnimate = 0; else { fAnimate = 1; animate(1); } break;
	case 'o': if (orbPlaneta) orbPlaneta = 0; else orbPlaneta = 1; glutPostRedisplay(); break;
	case 'm': if (fLua) fLua = 0; else fLua = 1; glutPostRedisplay(); break;
	case 'n': if (FName) FName = 0; else FName = 1; glutPostRedisplay(); break;
	case 'l': if (fChapeu) fChapeu = 0; else fChapeu = 1; glutPostRedisplay(); break;
	case 'P': PlaySound(TEXT("stars.wav"), NULL, SND_FILENAME | SND_ASYNC);; glutPostRedisplay(); break;//Start musica
	case 'p': PlaySound(NULL, NULL, NULL);; glutPostRedisplay(); break;//STOP musica
	case 'z': // Rotação DE Z
		rz = rz + 10;
		glutPostRedisplay();
		break;
	case 'x': // Rotação DE Z2
		rz = rz - 10;
		glutPostRedisplay();
		break;
	case 'w':
	case 'W':
		ang2 = ang2 - 10;
		transz = transz - 10;
		glutPostRedisplay();
		break;
	case 's':
	case 'S':
		ang2 = ang2 + 10;
		transz = transz + 10;
		glutPostRedisplay();
		break;
	case 'd':
	case 'D':
		//Rotação inversa DE Y2
		ry = ry + 2;
		glutPostRedisplay();
		break;
	case 'a':
	case 'A'://Rotação inversa DE Y2
		ry = ry - 2;
		glutPostRedisplay();
		break;

	case '0': 
		       ang1 = 0; ang2 = 0; transx = 0; transy = 0; transz = 0; rx = 0; ry = 0; rz = 0; 
			   glutPostRedisplay(); 
			   break;
	}
}

void Menu(void) {
	cout << "##################################################################" << endl;
	cout << "##                            MENU                              ##" << endl;
	cout << "##################################################################" << endl;
	cout << "-->  ESPACO para pausar/iniciar a simulacao" << endl;
	cout << "-->  ESC para sair da simulacao" << endl;
	cout << "-->  o para mostrar/esconder as Orbitas dos Planetas." << endl;
	cout << "-->  l para mostrar chapeus" << endl;
	cout << "-->  Scroll para aproximar/afastar a janela" << endl;
	cout << "-->  P/p para iniciar/parar musica" << endl;
	cout << "-->  w a s d para mover a camara" << endl;
	cout << "-->  z x rodar camara" << endl;
	cout << "-->  1 reset vista" << endl;
	cout << "#################################################################" << endl;

}


int main(int argc, char** argv) {

	Menu();
	glutInit(&argc, argv);
	glutInitContextVersion(4, 4);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(500, 0);
	glutCreateWindow("SISTEMA SOLAR");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(menu_act);
	glutMouseWheelFunc(mouseWheel);
	glewExperimental = GL_TRUE;
	glewInit();

	setup();
	glutMainLoop();
}