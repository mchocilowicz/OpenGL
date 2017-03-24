#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h> 
#define _USE_MATH_DEFINES
#include <math.h> 
#include "freeglut.h"




float PI = 3.141564;


float angleL = 0;

float angleR = 0;


float angleCenter = 0;


float dist = 0.0;

int licznik = -50;


int krok = 0;

bool oddalanie = true;

bool zatrzymanie = false;


void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float ang, float dirx, float diry)
{

	float s = sin(ang);
	float c = cos(ang);


	x1 += dirx * dist;
	x2 += dirx * dist;
	x3 += dirx * dist;

	y1 += diry * dist;
	y2 += diry * dist;
	y3 += diry * dist;


	float xr1 = x1 * c - y1 * s;
	float yr1 = x1 * s + y1 * c;

	float xr2 = x2 * c - y2 * s;
	float yr2 = x2 * s + y2 * c;

	float xr3 = x3 * c - y3 * s;
	float yr3 = x3 * s + y3 * c;



	int centerX = 5;
	int centerY = 5;


	glBegin(GL_TRIANGLES);
	glVertex3f(xr1 + centerX, yr1 + centerY, 0.0);
	glVertex3f(xr2 + centerX, yr2 + centerY, 0.0);
	glVertex3f(xr3 + centerX, yr3 + centerY, 0.0);
	glEnd();


	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(xr1 + centerX, yr1 + centerY, 0.0);
	glVertex3f(xr2 + centerX, yr2 + centerY, 0.0);
	glVertex3f(xr3 + centerX, yr3 + centerY, 0.0);
	glEnd();
}


void drawTriangleR(float x1, float y1, float x2, float y2, float x3, float y3, float ang, float dirx, float diry)
{

	x1 += dirx * dist;
	x2 += dirx * dist;
	x3 += dirx * dist;

	y1 += diry * dist;
	y2 += diry * dist;
	y3 += diry * dist;



	float xC = (x1 + x2 + x3) / 3;
	float yC = (y1 + y2 + y3) / 3;


	float s2 = sin(angleCenter);
	float c2 = cos(angleCenter);


	float xr1 = xC + (x1 - xC) * c2 - (y1 - yC) * s2;
	float yr1 = yC + (x1 - xC) * s2 + (y1 - yC) * c2;

	float xr2 = xC + (x2 - xC) * c2 - (y2 - yC) * s2;
	float yr2 = yC + (x2 - xC) * s2 + (y2 - yC) * c2;

	float xr3 = xC + (x3 - xC) * c2 - (y3 - yC) * s2;
	float yr3 = yC + (x3 - xC) * s2 + (y3 - yC) * c2;


	float s = sin(ang);
	float c = cos(ang);





	float xres1 = xr1 * c - yr1 * s;
	float yres1 = xr1 * s + yr1 * c;

	float xres2 = xr2 * c - yr2 * s;
	float yres2 = xr2 * s + yr2 * c;

	float xres3 = xr3 * c - yr3 * s;
	float yres3 = xr3 * s + yr3 * c;




	int screenCenterX = 5;
	int screenCenterY = 5;




	glBegin(GL_TRIANGLES);
	glVertex3f(xres1 + screenCenterX, yres1 + screenCenterY, 0.0);
	glVertex3f(xres2 + screenCenterX, yres2 + screenCenterY, 0.0);
	glVertex3f(xres3 + screenCenterX, yres3 + screenCenterY, 0.0);
	glEnd();


	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(xres1 + screenCenterX, yres1 + screenCenterY, 0.0);
	glVertex3f(xres2 + screenCenterX, yres2 + screenCenterY, 0.0);
	glVertex3f(xres3 + screenCenterX, yres3 + screenCenterY, 0.0);
	glEnd();

}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);


	glColor3f(0.0, 0.0, 1.0);
	drawTriangleR(0, 0, 0, 1, -1, 0, angleL, -1, 1);
	glColor3f(0.0, 0.0, 1.0);
	drawTriangleR(0, 0, 0, -1, -1, 0, angleL, -1, -1);
	glColor3f(0.0, 0.0, 1.0);
	drawTriangleR(0, 0, 0, 1, 1, 0, angleL, 1, 1);
	glColor3f(0.0, 0.0, 1.0);
	drawTriangleR(0, 0, 0, -1, 1, 0, angleL, 1, -1);


	// lewe 1
	glColor3f(0.0, 1.0, 0.0);
	drawTriangle(-1, 0, -1, 1, -2, 0, angleR, -1, 1);
	glColor3f(1.0, 0.0, 0.0);
	drawTriangle(-1, 0, -1, -1, -2, 0, angleR, -1, -1);

	// lewe 2
	glColor3f(1.0, 0.0, 0.0);
	drawTriangle(-2, 0, -2, 1, -3, 0, angleL, -1, 1);
	glColor3f(1.0, 0.0, 1.0);
	drawTriangle(-2, 0, -2, -1, -3, 0, angleL, -1, -1);


	// prawe 1
	glColor3f(1.0, 0.0, 0.0);
	drawTriangle(1, 0, 1, 1, 2, 0, angleR, 1, 1);
	glColor3f(0.0, 1.0, 0.0);
	drawTriangle(1, 0, 1, -1, 2, 0, angleR, 1, -1);

	// prawe 2
	glColor3f(1.0, 0.0, 1.0);
	drawTriangle(2, 0, 2, 1, 3, 0, angleL, 1, 1);
	glColor3f(1.0, 0.0, 0.0);
	drawTriangle(2, 0, 2, -1, 3, 0, angleL, 1, -1);

	// gorne 1
	glColor3f(1.0, 0.0, 0.0);
	drawTriangle(0, 1, 0, 2, -1, 1, angleR, -1, 1);
	glColor3f(0.0, 1.0, 0.0);
	drawTriangle(0, 1, 0, 2, 1, 1, angleR, 1, 1);

	// gorne 2
	glColor3f(1.0, 0.0, 1.0);
	drawTriangle(0, 2, 0, 3, -1, 2, angleL, -1, 1);
	glColor3f(1.0, 0.0, 0.0);
	drawTriangle(0, 2, 0, 3, 1, 2, angleL, 1, 1);

	// dolne 1
	glColor3f(0.0, 1.0, 0.0);
	drawTriangle(0, -1, 0, -2, -1, -1, angleR, -1, -1);
	glColor3f(1.0, 0.0, 0.0);
	drawTriangle(0, -1, 0, -2, 1, -1, angleR, 1, -1);

	// dolne 2
	glColor3f(1.0, 0.0, 0.0);
	drawTriangle(0, -2, 0, -3, -1, -2, angleL, -1, -1);
	glColor3f(1.0, 0.0, 1.0);
	drawTriangle(0, -2, 0, -3, 1, -2, angleL, 1, -1);

	// zolte
	glColor3f(1.0, 1.0, 0.0);
	drawTriangle(1, -1, 1, -2, 2, -1, angleL, 1, -1);
	glColor3f(1.0, 1.0, 0.0);
	drawTriangle(-1, -1, -1, -2, -2, -1, angleL, -1, -1);
	glColor3f(1.0, 1.0, 0.0);
	drawTriangle(1, 1, 1, 2, 2, 1, angleL, 1, 1);
	glColor3f(1.0, 1.0, 0.0);
	drawTriangle(-1, 1, -1, 2, -2, 1, angleL, -1, 1);

	glFlush();
	glutSwapBuffers();
}


void timerFunction(int timerID)
{
	if (licznik == 1850) licznik = 0;

	licznik++;

	if (licznik > 0 && licznik <= 1800)
	{
		if (licznik % 900 == 0)
		{
			oddalanie = !oddalanie;
		}

		float p = 2 * PI;
		if (oddalanie) krok++;
		else krok--;

		// aktualizacja katow

		// (poprzednie wartosci obrotow)
		//angleL += p / 60.0f;
		//angleR -= p / 180.0f;


		angleL += p / 450.0f;
		angleR -= p / 450.0f;
		angleCenter += p / 450.f;


		dist = krok * (3.0 / 10.0) / 180.0f;
	}



	glutTimerFunc(20, timerFunction, 0);
	glutPostRedisplay();
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Trojkaty");
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	glOrtho(0.0, 10.0, 0.0, 10.0, -1.0, 1.0);


	glutDisplayFunc(display);

	glutTimerFunc(30, timerFunction, 0);

	glutMainLoop();
	return 0;
}
