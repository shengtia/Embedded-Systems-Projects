#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include "../header/Storage.h"
#include "../header/Instruction.h"
#include "../header/GUI.h"
#include "../header/Error.h"

using namespace std;
// struct necessary to get the length of the input file in bytes
struct stat results;

// stores the file_length
int file_length = 0;

/**
* emulator.cpp contains sthe main function
* 
* Author: Stanley
**/

// update and render logic, update the display essentially, 1 cycle of execution, size is the file size
void renderExecute1Cycle();

// setup gui, run execution, run game, *app is the bin file turned into a char array
void run(int argc, char* argv[],  char *app);

int main(int argc, char* argv[]){
	
	if(argc != 2){
		cerr << "Argument error: Need exactly one input file path\n";
		return 0;
	}

	cout << "The input file path is : "<< argv[1] << "\n";

	// parse the bin file
	ifstream myFile (argv[1], ios::in | ios::binary);

	if(!myFile){
		cerr << "The input file does not exist "<< "\n";
	}

	// get the size of the bin file
	if (stat(argv[1], &results) == 0){
        // The size of the file in bytes is in
         file_length = results.st_size;
	}
    else{
        // An error occurred
        cerr << "Error : unable to get the size of the file, or the input is not a file "<< "\n";
        return 0;
    }

    // check if the size of the file is appropriate
    if(file_length % 2 != 0){
    	cerr << "Error : The file length is supposed to be an even number"<< "\n";
    	return 0;
    }else if(file_length == 0){
    	cerr << "Error : The file length is 0"<< "\n";
    	return 0;
    }else if(file_length > (4096 - 0x200 -16)){  // -16 because we reserve the area for stack
    	cerr << "Error : Application is too large" << "\n";
    	return 0;
    }

    //load application code into a buffer
	char buffer1[file_length];
	myFile.read (buffer1, file_length);

	// main control flow 
	run(argc, argv, buffer1);
 
	return 0;
}

void renderExecute1Cycle(){
	// one cycle of execution, assuming 1 ins / cycle

	// update timers
	if(delaytimer > 0){
		delaytimer--;
	}
	
	if(soundtimer > 0){
		system("paplay beep.aiff &> /dev/null &");
		soundtimer--;
	}

	// fetch decode execute instruction
	executeOpCode(fetchDecodeOpCode(ram[PC], ram[PC+1]));

	// check if pc + 1 leads to overflow
	checkPCOverflow();

	// clears screen
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	
	glLoadIdentity();

	// essentially the drawings are indicated by screen[]
	for(int w = 0; w < SCREEN_WIDTH; w++){
		for(int h = 0; h < SCREEN_WIDTH; h++){
			if(screen[w][h] == 1){
				draw_square((float)(w * 10), (float)(h * 10));
			}
		}
	}
	// swap buffers, allows for smooth animation
	glutSwapBuffers();
}

void run(int argc, char* argv[], char *app){
	// now initilize memory and registers
	gamul8initialize();  

	// loading the application into ram
	for(int i = 0; i < file_length; i++){
		ram[0x200+i] = (unsigned char)app[i];
	}

	// seed random variable for use in emulation
	srand(time(NULL));

	// initialize GLUT
	glutInit(&argc, argv);

	// initialize display and window
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("ESE 519 - Gamul8r");

	// initialize orthographic 2D view, among other things
	initGL();
	 
	// handle key presses and releases
	glutKeyboardFunc(your_key_press_handler);
	glutKeyboardUpFunc(your_key_release_handler);

	// GLUT draw function
	glutDisplayFunc(renderExecute1Cycle);

	// GLUT idle function, causes screen to redraw
	glutIdleFunc(idle);

	// main loop, all events processed here
	glutMainLoop();

}