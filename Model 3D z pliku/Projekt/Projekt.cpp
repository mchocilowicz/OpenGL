#include "glew.h"
#include "freeglut.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <vector>
#include <iostream>

#include "utils.h"
#include "object.h"

#define VIEWPORT_WIDTH 640
#define VIEWPORT_HEIGHT 480

using namespace std;

int w = 640;
int h = 480;
float alfa = 0.0f;

GLuint TextureID;

vector<glm::vec3> vertices;
vector<glm::vec3> normals;
vector<glm::vec2> uvs;

object car, wheels[4];

void init() {

	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	
    
    utils::load_obj("car.obj", vertices, uvs, normals);
    car = object(vertices, uvs);
    car.textureID = utils::load_bmp("car.bmp");
    car.posZ = 0.0f;
    car.posY = 0.1f;
    car.rotY = 0.0f;

	vertices.clear(); uvs.clear(); normals.clear();
	
    utils::load_obj("wheel.obj", vertices, uvs, normals);
    GLuint wheel_texture = utils::load_bmp("wheel.bmp");
	for (int i = 0; i < 4; i++) {
		wheels[i] = object(vertices, uvs);
		wheels[i].textureID = wheel_texture;
		wheels[i].scaleX = 3.0f;
		wheels[i].scaleY = 3.0f;
		wheels[i].scaleZ = 3.0f;
		wheels[i].rotY = 90.0f;
		wheels[i].posY = 0.75f;
	}
    wheels[0].posX = 1.95f;
    wheels[0].posZ = 3.37f;
    wheels[1].posX = -1.95f;
    wheels[1].posZ = 3.37f;
    wheels[2].posX = 1.95f;
    wheels[2].posZ = -3.1f;
    wheels[3].posX = -1.95f;
    wheels[3].posZ = -3.1f;


  
    glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
}

void onIdle() {
	alfa += 2.0f;
	glutPostRedisplay();
}

void renderObject(object obj) {

	glPushMatrix();
	glTranslatef(obj.posX, obj.posY, obj.posZ);
	glRotatef(obj.rotX, 1.0f, 0.0f, 0.0f);
    glRotatef(obj.rotY, 0.0f, 1.0f, 0.0f);
    glRotatef(obj.rotZ, 0.0f, 0.0f, 1.0f);
	glScalef(obj.scaleX, obj.scaleY, obj.scaleZ);

	glBindTexture(GL_TEXTURE_2D, obj.textureID);

	glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, obj.vertexBuffer);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, obj.uvBuffer);
    glTexCoordPointer(2, GL_FLOAT, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, obj.vertices.size());

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	
 
	
	glPopMatrix();

}

void onKeyboard(unsigned char key, int x, int y) {

    switch (key) {

      
        case 27:
            exit(1);
        break;
    }
}

void onDisplay() {

   
    glViewport(0, 0, w, h);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (float)VIEWPORT_WIDTH / (float)VIEWPORT_HEIGHT, 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();		
	gluLookAt(0.0f, 5.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	glRotatef(alfa, 0,1,0);
	
	renderObject(car);
	for (int i = 0; i < 4; i++) {
		wheels[i].rotX = alfa;
		renderObject(wheels[i]);
	}

	glFlush();
    glutSwapBuffers();
};


void onReshape(int newW, int newH) {
    w = newW;
    h = newH;
}

int main(int argc, char* argv[]) {
  
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(VIEWPORT_WIDTH, VIEWPORT_HEIGHT);


    glutCreateWindow("Car");

    glutReshapeFunc(onReshape);

   
    GLenum glew_status = glewInit();
    if (glew_status != GLEW_OK) {
        cout << "Error:\n" << glewGetErrorString(glew_status);
        return 1;
    }


    if (!GLEW_VERSION_2_0) {
        cout << "Error: your graphic card does not support OpenGL 2.0\n";
        return 1;
    }


    init();


    glutDisplayFunc(onDisplay);
    glutIdleFunc(onIdle);


    glutKeyboardFunc(onKeyboard);


    glutMainLoop();

    return 0;
}
