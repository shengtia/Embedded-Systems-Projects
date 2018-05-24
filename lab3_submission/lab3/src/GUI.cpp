#include <GL/glut.h>
#include <stdio.h>
#include <cstdlib>
#include "../header/GUI.h"
#include "../header/Storage.h"

// method to draw a single pixel on the screen
void draw_square(float x_coord, float y_coord){
	// draws a white 10 x 10 square with the coordinates passed

	glBegin(GL_QUADS);  //GL_QUADS treats the following group of 4 vertices 
						//as an independent quadrilateral

		glColor3f(1.0f, 1.0f, 1.0f); 	//color in RGB
										//values between 0 & 1
										//E.g. Pure Red = (1.0f, 0.0f, 0.0f)
		glVertex2f(x_coord, y_coord);			//vertex 1
		glVertex2f(x_coord + 10, y_coord);		//vertex 2
		glVertex2f(x_coord + 10, y_coord + 10);	//vertex 3
		glVertex2f(x_coord, y_coord + 10);		//vertex 4

		//glVertex3f lets you pass X, Y and Z coordinates to draw a 3D quad
		//only if you're interested

	glEnd();
}

// idling function for animation, called by glutIdleFunc
void idle(){
	// gives the call to redraw the screen
	glutPostRedisplay();
}

// initializes GL 2D mode and other settings
void initGL(){
	// sets up GLUT window for 2D drawing
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// clears screen color (RGBA)
	glClearColor(0.f, 0.f, 0.f, 1.f);
}

// function to handle user keyboard input (when pressed) 
void your_key_press_handler(unsigned char key, int x, int y){
	updateKey(key, 1);
}

// function to handle key release
void your_key_release_handler(unsigned char key, int x, int y){
	updateKey(key, 0);
}

void updateKey(unsigned char key, bool b){
	switch(key){
		case '1':
			key_status[0][0] = b;
			break;
		case '2':
			key_status[1][0] = b;
			break;
		case '3':
			key_status[2][0] = b;
			break;
		case '4':
			key_status[3][0] = b;
			break;
		case 'q':
			key_status[0][1] = b;
			break;
		case 'w':
			key_status[1][1] = b;
			break;
		case 'e':
			key_status[2][1] = b;
			break;
		case 'r':
			key_status[3][1] = b;
			break;
		case 'a':
			key_status[0][2] = b;
			break;
		case 's':
			key_status[1][2] = b;
			break;
		case 'd':
			key_status[2][2] = b;
			break;
		case 'f':
			key_status[3][2] = b;
			break;
		case 'z':
			key_status[0][3] = b;
			break;
		case 'x':
			key_status[1][3] = b;
			break;
		case 'c':
			key_status[2][3] = b;
			break;
		case 'v':
			key_status[3][3] = b;
			break;
	}
}

