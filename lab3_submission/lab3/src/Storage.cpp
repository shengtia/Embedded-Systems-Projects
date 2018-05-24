#include <stdbool.h>
#include "../header/Storage.h"
#include "../header/Instruction.h"

// the memory of the emulator 0x00 to 0x199 belong to the interpreter
// whereas 0x200 to 0xfff belong to data
unsigned char ram[4096];

// 16 common registers
unsigned char c_r[16];

// index register
unsigned int I;

// PC
unsigned int PC = 0x200;

// stack
unsigned int stack[16];

// stack pointer
unsigned char SP = 0;

// delay and sound timers special regsiters
unsigned char delaytimer = 0;

unsigned char soundtimer = 0;

// pixel state
bool screen[SCREEN_WIDTH][SCREEN_HEIGHT];

// key
bool key_status[4][4];

void gamul8initialize(){
	for(int i = 0; i < FONTSET_SIZE; i++){
		ram[i] = fontset[i];
	}

	for(int i  = 0x200; i < 4096; i++)
		ram[i] = 0;

	for(int i  = 0; i < 16; i++)
		c_r[i] = 0;	

	for (int i = 0; i < 16; i++)
		stack[i] = 0;

	for (int i = 0; i < 64; i++)
		for (int j = 0; j < 32; j++)
			screen[i][j] = 0;
	
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			key_status[i][j] = 0;


	clearScreen();

	delaytimer = 0;

	soundtimer = 0;

	I = 0x00;

	PC = 0x200;

	SP = 0x00;
}
