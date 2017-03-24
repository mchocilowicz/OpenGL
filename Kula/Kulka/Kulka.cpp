#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h> 
#define _USE_MATH_DEFINES
#include <math.h> 
#include "freeglut.h"

float kam_alfa = 0;
float kam_beta = 0;

int szer = 800;
int wys = 600;
int fps = 30;
int mx, my;

float e = 0;
float de = 90;

float t = 0.0;
float dt = 1.0 / float(fps);
int dti = 1000 / fps;
float skala_myszy = 0.5;
bool obracaj = false;

float amp = 3.0;
float freq = 0.5;
float sprezyna_dlugosc = 10.0;
float sprezyna_grubosc = 0.2;
float sprezyna_srednica = 0.5;
int sprezyna_zwoje = 6;

float promien_kuli = 2.0;
int fragmenty = 20;

static GLUquadricObj * quadric;

enum TEKSTURY {
	tekstura_drewno,
	tekstura_metal,
	tekstura_ilosc
};

GLuint tekstury[tekstura_ilosc];

void uklad()
{
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);	glVertex3f(-5, 0, 0);	glVertex3f(5, 0, 0);	glVertex3f(5, 0, 0);	glVertex3f(4, 0.5, 0);	glVertex3f(5, 0, 0);	glVertex3f(4, -0.5, 0);	glVertex3f(5, 0, 0);	glVertex3f(4, 0, 0.5);	glVertex3f(5, 0, 0);	glVertex3f(4, 0, -0.5);
	glColor3f(0, 1, 0);	glVertex3f(0, -5, 0);	glVertex3f(0, 5, 0);	glVertex3f(0, 5, 0);	glVertex3f(-0.5, 4, 0);	glVertex3f(0, 5, 0);	glVertex3f(0.5, 4, 0);	glVertex3f(0, 5, 0);	glVertex3f(0, 4, -0.5);	glVertex3f(0, 5, 0);	glVertex3f(0, 4, 0.5);
	glColor3f(0, 0, 1);	glVertex3f(0, 0, -5);	glVertex3f(0, 0, 5);	glVertex3f(0, 0, 5);	glVertex3f(-0.5, 0, 4);	glVertex3f(0, 0, 5);	glVertex3f(0.5, 0, 4);	glVertex3f(0, 0, 5);	glVertex3f(0, -0.5, 4);	glVertex3f(0, 0, 5);	glVertex3f(0, 0.5, 4);
	glEnd();
	glEnable(GL_LIGHTING);
}

void szescian()
{
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glTexCoord2f(1, 0); glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord2f(1, 1); glVertex3f(0.5f, 0.5f, -0.5f);
	glTexCoord2f(0, 1); glVertex3f(0.5f, 0.5f, 0.5f);
	glTexCoord2f(0, 0); glVertex3f(0.5f, -0.5f, 0.5f);

	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2f(1, 0); glVertex3f(-0.5f, -0.5f, 0.5f);
	glTexCoord2f(1, 1); glVertex3f(-0.5f, 0.5f, 0.5f);
	glTexCoord2f(0, 1); glVertex3f(-0.5f, 0.5f, -0.5f);

	glNormal3f(0.f, 1.f, 0.f);
	glTexCoord2f(1, 0); glVertex3f(-0.5f, 0.5f, -0.5f);
	glTexCoord2f(1, 1); glVertex3f(-0.5f, 0.5f, 0.5f);
	glTexCoord2f(0, 1); glVertex3f(0.5f, 0.5f, 0.5f);
	glTexCoord2f(0, 0); glVertex3f(0.5f, 0.5f, -0.5f);

	glNormal3f(0, -1, 0);
	glTexCoord2f(1, 0); glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2f(1, 1); glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord2f(0, 1); glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord2f(0, 0); glVertex3f(-0.5f, -0.5f, 0.5f);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0, 0); glVertex3f(-0.5f, -0.5f, 0.5f);
	glTexCoord2f(1, 0); glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord2f(1, 1); glVertex3f(0.5f, 0.5f, 0.5f);
	glTexCoord2f(0, 1); glVertex3f(-0.5f, 0.5f, 0.5f);

	glNormal3f(0, 0, -1);
	glTexCoord2f(1, 0); glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2f(1, 1); glVertex3f(-0.5f, 0.5f, -0.5f);
	glTexCoord2f(0, 1); glVertex3f(0.5f, 0.5f, -0.5f);
	glTexCoord2f(0, 0); glVertex3f(0.5f, -0.5f, -0.5f);
	glEnd();
}

void sprezyna(float grubosc, float srednica, float dlugosc, int zwoje, int slices, int stacks)
{
	float zwojef = zwoje;
	float u_fact = 2.0;
	float t_fact = u_fact * zwojef;
	float t_fact_pi = t_fact * M_PI;
	float u_fact_pi = u_fact * M_PI;

	float dt = t_fact_pi / zwojef / float(stacks);
	float du = u_fact_pi / float(slices);
	float dz = dlugosc / t_fact_pi;
	float x, y, z, xs, ys, zs;

	
	glPushMatrix();
	glTranslatef(srednica, 0, 0);
	gluSphere(quadric, grubosc, slices, stacks);
	glPopMatrix();
	
	for (float t = 0; t < t_fact_pi; t += dt) {
		for (float u = 0; u < u_fact_pi; u += du) {
			glBegin(GL_QUADS);

			x = cos(t) * (srednica + grubosc * cos(u));
			y = sin(t) * (srednica + grubosc * cos(u));
			z = dz * t + grubosc * sin(u);
			xs = cos(t) * srednica;
			ys = sin(t) * srednica;
			zs = dz * t;
			glTexCoord2f(zwojef * (t) / t_fact_pi, (u) / u_fact_pi);
			glNormal3f(x - xs, y - ys, z - zs);
			glVertex3f(x, y, z);

			x = cos(t + dt) * (srednica + grubosc * cos(u));
			y = sin(t + dt) * (srednica + grubosc * cos(u));
			z = dz * (t + dt) + grubosc * sin(u);
			xs = cos(t + dt) * srednica;
			ys = sin(t + dt) * srednica;
			zs = dz * (t + dt);
			glTexCoord2f(zwojef * (t + dt) / t_fact_pi, (u) / u_fact_pi);
			glNormal3f(x - xs, y - ys, z - zs);
			glVertex3f(x, y, z);

			x = cos(t + dt) * (srednica + grubosc * cos(u + du));
			y = sin(t + dt) * (srednica + grubosc * cos(u + du));
			z = dz * (t + dt) + grubosc * sin(u + du);
			xs = cos(t + dt) * srednica;
			ys = sin(t + dt) * srednica;
			zs = dz * (t + dt);
			glTexCoord2f(zwojef * (t + dt) / t_fact_pi, (u + du) / u_fact_pi);
			glNormal3f(x - xs, y - ys, z - zs);
			glVertex3f(x, y, z);

			x = cos(t) * (srednica + grubosc * cos(u + du));
			y = sin(t) * (srednica + grubosc * cos(u + du));
			z = dz * t + grubosc * sin(u + du);
			xs = cos(t) * srednica;
			ys = sin(t) * srednica;
			zs = dz * t;
			glTexCoord2f(zwojef * (t) / t_fact_pi, (u + du) / u_fact_pi);
			glNormal3f(x - xs, y - ys, z - zs);
			glVertex3f(x, y, z);

			glEnd();
		}
	}
	
	glPushMatrix();
	glTranslatef(srednica, 0, dlugosc);
	gluSphere(quadric, grubosc, slices, stacks);
	glPopMatrix();
}

void scena()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(8, 16, 22, 0, 0, 0, 0, 1, 0);
	glRotatef(kam_beta, 1, 0, 0);
	glRotatef(kam_alfa, 0, 1, 0);
	

	float d = sprezyna_dlugosc + amp * sin(t * 2.0 * freq * M_PI);

	glPushMatrix();
	
	glTranslatef(0, 10.5 - sprezyna_grubosc, 0);

	
	glBindTexture(GL_TEXTURE_2D, tekstury[tekstura_drewno]);
	glColor3f(0.8, 0.8, 0.8);
	glPushMatrix();
	glTranslatef(sprezyna_srednica, -d - promien_kuli - sprezyna_grubosc, 0);
	glRotatef(90.0, 1, 0, 0);
	gluSphere(quadric, promien_kuli, fragmenty, fragmenty);
	glPopMatrix();

	
	glBindTexture(GL_TEXTURE_2D, tekstury[tekstura_metal]);
	glColor3f(1.0, 0.8, 0.7);
	glPushMatrix();
	glRotatef(90.0, 1, 0, 0);
	sprezyna(sprezyna_grubosc, sprezyna_srednica, d, sprezyna_zwoje, fragmenty, fragmenty);
	glPopMatrix();

	glPopMatrix();

	
	glColor3f(1.0, 1.0, 1.0);
	
	glPushMatrix();
	glTranslatef(0, -10, 0);
	glScalef(20, 1.0, 20);
	szescian();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-9, 0.5, 0);
	glScalef(1.0, 20, 3.0);
	szescian();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-3.5, 11, 0);
	glScalef(12, 1.0, 3.0);
	szescian();
	glPopMatrix();

	glFlush();
	glutSwapBuffers();
}

GLuint wczytaj_bmp(const char * imagepath) 
{
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int width, height;
	unsigned int imageSize;
	unsigned char * data;
	FILE * file = fopen(imagepath, "rb");
	if (!file)
	{
		printf("Nie mo¿na za³adowaæ tekstury\n");
		exit(0);
	}
	if (fread(header, 1, 54, file) != 54 || header[0] != 'B' || header[1] != 'M')
	{
		printf("Nieprawid³owy plik BMP\n");
		exit(0);
	}
	dataPos = *(unsigned int *)&(header[0x0A]);
	imageSize = *(unsigned int *)&(header[0x22]);
	width = *(unsigned int *)&(header[0x12]);
	height = *(unsigned int *)&(header[0x16]);
	if (imageSize == 0) imageSize = width * height * 3;
	if (dataPos == 0) dataPos = 54;
	data = new unsigned char[imageSize];
	fread(data, 1, imageSize, file);
	fclose(file);

	unsigned char tempRGB;
	for (int i = 0; i < imageSize; i += 3)
	{
		tempRGB = data[i];
		data[i] = data[i + 2];
		data[i + 2] = tempRGB;
	}

	GLuint textureID;
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); 
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID; 
}


void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '+':
		e += de;
		break;
	case '-':
		e -= de;
		break;
	case 13: 
		break;
	case ' ':
		t = 0.0;
		break;
	case 27: 
		exit(1);
		break;
	}

	glutPostRedisplay();
}


void special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		break;
	case GLUT_KEY_RIGHT:
		break;
	case GLUT_KEY_UP:
		break;
	case GLUT_KEY_DOWN:
		break;
	}

	glutPostRedisplay();
}

void timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(dti, timer, 0);
	t += dt;
}

void mouse(int button, int state, int x, int y) 
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN)
		{
			mx = x;
			my = y;
			obracaj = true;
		}
		else if (state == GLUT_UP)
		{
			obracaj = false;
		}
	}
}

void ruch(int x, int y) 
{
	if (obracaj) {
		kam_alfa = (x - mx) * skala_myszy;
		kam_beta = (y - my) * skala_myszy;
	}
}

void init()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)szer / (float)wys, 0.1, 500);
	glMatrixMode(GL_MODELVIEW);

	
	GLfloat global_ambient = 0.1;
	GLfloat global_ambient_v[] = { global_ambient, global_ambient, global_ambient, 1.0 };
	
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient_v);
	
	glEnable(GL_COLOR_MATERIAL);
	
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	
	
	GLfloat ambient0 = 0.0;
	GLfloat ambient0_v[] = { ambient0, ambient0, ambient0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0_v);
	
	GLfloat difuse0 = 1.0;
	GLfloat difuse0_v[] = { difuse0, difuse0, difuse0, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, difuse0_v);
	
	GLfloat specular0 = 0.7;
	GLfloat specular0_v[] = { specular0, specular0, specular0, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular0_v);
	
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 90.0);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 10.0f);

	GLfloat position_l0[] = { 100, 100, 100, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, position_l0);
	GLfloat direction_l0[] = { -1, -1, -1 };
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction_l0);

	glEnable(GL_LIGHT0);
	
	glEnable(GL_LIGHTING);

	
	float mat_ambient = 0.7;
	float mat_diffuse = 0.6;
	float mat_specular = 1.0;
	float mat_shininess = 100;

	float mat_ambient_v[] = { mat_ambient, mat_ambient, mat_ambient, 1.0 };
	float mat_diffuse_v[] = { mat_diffuse, mat_diffuse, mat_diffuse, 1.0 };
	float mat_specular_v[] = { mat_specular, mat_specular, mat_specular, 1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_v);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_v);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_v);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_TEXTURE_2D);

	tekstury[tekstura_drewno] = wczytaj_bmp("Tekstury\\drewno.bmp");
	tekstury[tekstura_metal] = wczytaj_bmp("Tekstury\\metal.bmp");

	quadric = gluNewQuadric();
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluQuadricTexture(quadric, GL_TRUE);      
	gluQuadricNormals(quadric, GLU_SMOOTH);  

}

void destroy()
{
	gluDeleteQuadric(quadric);
	glDeleteTextures(tekstura_ilosc, tekstury);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Kulka");
	glutFullScreen();
	glutKeyboardFunc(keyboard); 
	glutSpecialFunc(special);   
	glutMouseFunc(mouse);		
	glutMotionFunc(ruch);		
	glutDisplayFunc(scena);
	glutTimerFunc(0, timer, 0);
	init();
	glutMainLoop();
	destroy();
	return 0;
}
