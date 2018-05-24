#include "../header/Storage.h"
#include "../header/Instruction.h"
#include "../header/Error.h"
#include <stdio.h>
#include <stdbool.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;


unsigned int fetchDecodeOpCode(unsigned char op1, unsigned char op2){
	return (op1 << 8) | op2;
}

int executeOpCode(unsigned int op){
	//extract the least significant 4 digit
	unsigned char byte1 = op & 15; 
	unsigned char byte2 = (op & 240) >> 4;
	unsigned char byte3 = (op & 3840) >> 8;
	unsigned char byte4 = op >> 12;

	if(byte4 == 0 && byte3 == 0 && byte2 == 0xE && byte1 == 0){
		//opcode 00E0
		_00E0_();
		return 2;
	}else if(byte4 == 0 && byte3 == 0 && byte2 == 0xE && byte1 == 0xE){
		_00EE_();
		incPC();
		return 3;
	}else if(byte4 == 0){
		_0NNN_();
		incPC();
		return 1;
	}else if(byte4 == 1){
		_1NNN_(byte3, byte2, byte1);
		return 4;
	}else if(byte4 == 2){
		_2NNN_(byte3, byte2, byte1);
		return 5;
	}else if(byte4 == 3){
		_3XNN_(byte3, byte2, byte1);
		incPC();
		return 6;
	}else if(byte4 == 4){
		_4XNN_(byte3, byte2, byte1);
		incPC();
		return 7;
	}else if(byte4 == 5 && byte1 == 0){
		_5XY0_(byte3, byte2);
		incPC();
		return 8;
	}else if(byte4 == 6){
		_6XNN_(byte3, byte2, byte1);
		incPC();
		return 9;
	}else if(byte4 == 7){
		_7XNN_(byte3, byte2, byte1);
		incPC();
		return 10;
	}else if(byte4 == 8 && byte1 ==0){
		_8XY0_(byte3, byte2);
		incPC();
		return 11;
	}else if(byte4 == 8 && byte1 ==1){
		_8XY1_(byte3, byte2);
		incPC();
		return 12;
	}else if(byte4 == 8 && byte1 ==2){
		_8XY2_(byte3, byte2);
		incPC();
		return 13;
	}else if(byte4 == 8 && byte1 ==3){
		_8XY3_(byte3, byte2);
		incPC();
		return 14;
	}else if(byte4 == 8 && byte1 ==4){
		_8XY4_(byte3, byte2);
		incPC();
		return 15;
	}else if(byte4 == 8 && byte1 ==5){
		_8XY5_(byte3, byte2);
		incPC();
		return 16;
	}else if(byte4 == 8 && byte1 ==6){
		_8XY6_(byte3, byte2);
		incPC();
		return 17;
	}else if(byte4 == 8 && byte1 ==7){
		_8XY7_(byte3, byte2);
		incPC();
		return 18;
	}else if(byte4 == 8 && byte1 ==0xE){
		_8XYE_(byte3, byte2);
		incPC();
		return 19;
	}else if(byte4 == 9 && byte1 ==0){
		_9XY0_(byte3, byte2);
		incPC();
		return 20;
	}else if(byte4 == 0xA){
		_ANNN_(byte3, byte2, byte1);
		incPC();
		return 21;
	}else if(byte4 == 0xB){
		_BNNN_(byte3, byte2, byte1);
		return 22;
	}else if(byte4 == 0xC){
		_CXNN_(byte3, byte2, byte1);
		incPC();
		return 23;
	}else if(byte4 == 0xD){
		_DXYN_(byte3, byte2, byte1);
		incPC();
		return 24;
	}else if(byte4 == 0xE && byte2 == 9 && byte1 == 0xE){
		_EX9E_(byte3);
		incPC();
		return 25;
	}else if(byte4 == 0xE && byte2 == 0xA && byte1 == 1){
		_EXA1_(byte3);
		incPC();
		return 26;
	}else if(byte4 == 0xF && byte2 == 0 && byte1 == 7){
		_FX07_(byte3);
		incPC();
		return 27;
	}else if(byte4 == 0xF && byte2 == 0 && byte1 == 0xA){
		incPC();
		_FX0A_(byte3);
		return 28;
	}else if(byte4 == 0xF && byte2 == 1 && byte1 == 5){
		_FX15_(byte3);
		incPC();
		return 29; 
	}else if(byte4 == 0xF && byte2 == 1 && byte1 == 8){
		_FX18_(byte3);
		incPC();
		return 30;
	}else if(byte4 == 0xF && byte2 == 1 && byte1 == 0xE){
		_FX1E_(byte3);
		incPC();
		return 31;
	}else if(byte4 == 0xF && byte2 == 2 && byte1 == 9){
		_FX29_(byte3);
		incPC();
		return 32;
	}else if(byte4 == 0xF && byte2 == 3 && byte1 == 3){
		_FX33_(byte3);
		incPC();
		return 33;
	}else if(byte4 == 0xF && byte2 == 5 && byte1 == 5){
		_FX55_(byte3);
		incPC();
		return 34;
	}else if(byte4 == 0xF && byte2 == 6 && byte1 == 5){
		_FX65_(byte3);
		incPC();
		return 35;
	}else{
		// this is bad, no matching opcode
		invalidOpcode();
	}
	return -1;
}

void clearScreen(){
	for (int i = 0; i < 64; i++)
		for (int j = 0; j < 32; j++)
			screen[i][j] = 0;
}

void _0NNN_(){
	// do nothing
}

void _00E0_(){
	clearScreen();
}

void _00EE_(){
	// tested
	PC = stack[SP];
	SP -= 1;
	checkPCUnderflow();
	checkStackUnderflow();
}

void _1NNN_(unsigned char byte3, unsigned char byte2, unsigned char byte1){
	// tested
	PC = shiftAdd3Bytes(byte3, byte2, byte1);
	checkPCUnderflow();
}

void _2NNN_(unsigned char byte3, unsigned char byte2, unsigned char byte1){
	// tested
	// store the current PC
	SP+=1;
	checkStackOverflow();

	stack[SP] = PC;

	// jump to the subroutine
	PC = shiftAdd3Bytes(byte3, byte2, byte1);
	checkPCUnderflow();
}

void _3XNN_(unsigned char byte3, unsigned char byte2, unsigned char byte1){
	//tested
	// compare the value of common register at byte3 with the lower 2 bytes
	if(c_r[byte3] == shiftAdd2Bytes(byte2, byte1)){
		incPC();
		checkPCOverflow();
		checkPCUnderflow();
	}
}

void _4XNN_(unsigned char byte3, unsigned char byte2, unsigned char byte1){
	// tested
	// byte3 indicates the corresponding register
	// byte2 and byte1 represent the comparison number
	if(c_r[byte3] != shiftAdd2Bytes(byte2,byte1)){
		incPC();
		checkPCOverflow();
		checkPCUnderflow();
	}
}

void _5XY0_(unsigned char byte3, unsigned char byte2){
	// tested
	// if VX == VY, then skip the next instruction
	if(c_r[byte3] == c_r[byte2]){
		incPC();
		checkPCOverflow();
		checkPCUnderflow();
	}
}

void _6XNN_(unsigned char byte3, unsigned char byte2, unsigned char byte1){
	// tested
	// sets VX to NN
	//printf("originally register has: %x \n", c_r[byte3]);
	c_r[byte3] = shiftAdd2Bytes(byte2, byte1);
	//printf("now register has: %x \n", c_r[byte3]);
}

void _7XNN_(unsigned char byte3, unsigned char byte2, unsigned char byte1){
	// tested
	// adds NN to VX
	c_r[byte3] += shiftAdd2Bytes(byte2, byte1);
}

void _8XY0_(unsigned char byte3, unsigned char byte2){
	// tested
	// assign VX the value of VY
	c_r[byte3] = c_r[byte2];
}

void _8XY1_(unsigned char byte3, unsigned char byte2){
	// tested
	// assign VX the value VY | VX
	c_r[byte3] |= c_r[byte2];
}

void _8XY2_(unsigned char byte3, unsigned char byte2){
	// tested
	// bitwise and 
	c_r[byte3] &= c_r[byte2];
}

void _8XY3_(unsigned char byte3, unsigned char byte2){
	// tested
	// bitwise xor
	c_r[byte3] ^= c_r[byte2];
}

void _8XY4_(unsigned char byte3, unsigned char byte2){
	// tested
	// adds VY to VX, 
	int r1 = (int) c_r[byte3];
	int r2 = (int)c_r[byte2];
	c_r[byte3] += c_r[byte2];
	if(r1+r2 < 255){
		c_r[0xF] = 0;
		//printf("c_r[0xF] has value 0\n");
	}else{
		c_r[0xF] = 1;
		//printf("c_r[0xF] has value 1\n");
	}
}

void _8XY5_(unsigned char byte3, unsigned char byte2){
	// tested
	// VX-VY < 0, vf set to 0, otherwise 1
	int r1 = (int) c_r[byte3];
	int r2 = (int)c_r[byte2];
	c_r[byte3] -= c_r[byte2];
	if(r1 - r2 < 0){
		c_r[0xF] = 0;
		//printf("c_r[0xF] has value %x \n", c_r[0xF]);
	}else{
		c_r[0xF] = 1;
		//printf("c_r[0xF] has value %x \n", c_r[0xF]);
	}
}

void _8XY6_(unsigned char byte3, unsigned char byte2){
	//printf("c_r[0xF] is originally : %x\n", c_r[0xF]);
	//printf("c_r[byte3] is originally : %x\n", c_r[byte3]);
	c_r[0xF] = c_r[byte3] & 0x1;
	c_r[byte3] = c_r[byte3] >> 1;
	//printf("c_r[0xF] is now : %x\n", c_r[0xF]);
	//printf("c_r[byte3] is now : %x\n", c_r[byte3]);
}

void _8XY7_(unsigned char byte3, unsigned char byte2){
	// tested
	c_r[byte3] = c_r[byte2] - c_r[byte3];
	if((int)c_r[byte2] - (int)c_r[byte3] < 0){
		c_r[0xF] = 0;
		//printf("c_r[0xF] has value %x \n", c_r[0xF]);
	}else{
		c_r[0xF] = 1;
		//printf("c_r[0xF] has value %x \n", c_r[0xF]);
	}
}

void _8XYE_(unsigned char byte3, unsigned char byte2){
	// tested
	//printf("c_r[0xF] is originally : %x\n", c_r[0xF]);
	//printf("c_r[byte3] is originally : %x\n", c_r[byte3]);
	c_r[0xF] = (c_r[byte3] & 128) >> 7;
	c_r[byte3] = c_r[byte3] << 1;
	//printf("c_r[0xF] is now : %x\n", c_r[0xF]);
	//printf("c_r[byte3] is now : %x\n", c_r[byte3]);
}

void _9XY0_(unsigned char byte3, unsigned char byte2){
	// tested
	// PC = 4;
	if(c_r[byte3] != c_r[byte2]){
		incPC();
		checkPCOverflow();
		checkPCUnderflow();
	}
	//printf("PC is :%d\n", PC);
}

void _ANNN_(unsigned char byte3, unsigned char byte2, unsigned char byte1){
	// tested
	//printf("I was : %x \n", I);
	I = shiftAdd3Bytes(byte3,byte2,byte1);
	//printf("I is now: %x \n", I);
}

void _BNNN_(unsigned char byte3, unsigned char byte2, unsigned char byte1){
	// gauranteed
	PC = shiftAdd3Bytes(byte3,byte2,byte1);
	PC += c_r[0];
	checkPCOverflow();
	checkPCUnderflow();
}

void _CXNN_(unsigned char byte3, unsigned char byte2, unsigned char byte1){
	// tested
	// generate the random number
	srand(time(NULL));
	unsigned char random = rand() % 255 + 1;
	//printf("the random number is %x \n", random);
	// bitwise operation
	random = shiftAdd2Bytes(byte2,byte1) & random;
	// assigment
	c_r[byte3] = random;
	//printf("the value stored in r_c[byte3] is %x\n", c_r[byte3]);
}

void _DXYN_(unsigned char byte3, unsigned char byte2, unsigned char byte1){
	// tested 
	unsigned char startx = c_r[byte3];
	unsigned char starty = c_r[byte2];
	unsigned char line;
	bool pixel[8];
	c_r[0xF] = 0;

	if(startx >= 64 || starty >= 32){
		cerr << "Error: DXYN coordinate out of bounds" << endl;
		exit(0);
	}

	for(int y = 0; y < byte1; y++){
		line = ram[I+y];
		// copy the pixels, from the most significant bit to the least
		pixel[0] = line >> 7;
		pixel[1] = (line & 64) >> 6;
		pixel[2] = (line & 32) >> 5;
		pixel[3] = (line & 16) >> 4;
		pixel[4] = (line & 8) >> 3;
		pixel[5] = (line & 4) >> 2;
		pixel[6] = (line & 2) >> 1;
		pixel[7] = (line & 1);

		// variables used for wrapping around drawings
		int minusx = 0;
		int minusy = 0;
		for(int x = 0; x < 8; x++){ 
			
			if(pixel[x] != 0){
				// if y overflows and x overflows
				if((y + starty >= 32) && (x+startx >= 64)){
					minusx = -64; minusy = -32;
				// if y overflows but x does not
				}else if(y + starty >= 32){
					minusx = 0; minusy = -32;
				// if x overflows but y does not
				}else if(x+startx >= 64){
					minusx = -64; minusy = 0;
				// neither overflows
				}else{
					minusx = 0; minusy = 0;
				}
				if(screen[x+startx+minusx][y+starty+minusy] == 1){
						c_r[0xF] = 1;
					}
				screen[x+startx+minusx][y+starty+minusy] ^= 1;
			}
		}
	}
}

void _EX9E_(unsigned char byte3){
	if(c_r[byte3] > 0xF){
		cerr << "Error: The keypad does not contain such key" << endl;
		exit(0);
	}

	if(isPressed(c_r[byte3]))
		incPC();
}

void _EXA1_(unsigned char byte3){
	if(c_r[byte3] > 0xF){
		cerr << "Error: The keypad does not contain such key" << endl;
		exit(0);
	}
	
	if(!isPressed(c_r[byte3])){
		incPC();
	}
	
}

void _FX07_(unsigned char byte3){
	// garanteed
	// sets vx to the value of the delay timer
	c_r[byte3] = delaytimer;
}

void _FX0A_(unsigned char byte3){
	// tested
	unsigned char c = 0xFF;
	while(isAKey(c) == 0xFF){
		c = getchar();
	}
	c_r[byte3] = isAKey(c);
}

void _FX15_(unsigned char byte3){
	// garanteed
	// sets the delay timer to vx
	delaytimer = c_r[byte3];
}

void _FX18_(unsigned char byte3){
	// garanteed
	// sets the sound timer to vx
	soundtimer = c_r[byte3];
}

void _FX1E_(unsigned char byte3){
	// guaranteed
	// adds vx to I
	I = I+c_r[byte3];
	checkIndexOverflow();
}

void _FX29_(unsigned char byte3){
	// guaranteed
	if(c_r[byte3] > 0xF){
		cerr << "Error FX29: there is no such sprite in the memory" << endl;
	}
	I = c_r[byte3] * 5;
}

void _FX33_(unsigned char byte3){
	// tested
	// BCD hundreds digit, stored in I, tens digit stored in I + 1, ones digit stored in I + 2
	I += 2;
	checkIndexOverflow();
	I -= 2;
	ram[I] = c_r[byte3]/100;
	//printf("hundreds digit: %d\n", ram[I]);
	ram[I+1] = (c_r[byte3]%100) / 10;
	//printf("tens digit: %d\n", ram[I+1]);
	ram[I+2] = c_r[byte3] % 10;
	//printf("ones digit: %d\n", ram[I+2]);
}

void _FX55_(unsigned char byte3){
	// tested
	// stores v0 to vx in memory starting at address I
	c_r[4] = 5;
	I += byte3;
	checkIndexOverflow();
	I -= byte3;
	for(int i = 0; i <= byte3; i++){
		ram[I+i] = c_r[i];
	}
}

void _FX65_(unsigned char byte3){
	// fills v0 to vx with values from memory starting at address I
	I += byte3;
	checkIndexOverflow();
	I -= byte3;
	for(int i = 0; i <= byte3; i++){
		c_r[i] = ram[I+i];
	}
}

unsigned int shiftAdd3Bytes(unsigned char byte3, unsigned char byte2, unsigned char byte1){
	return (byte3 << 8) + (byte2 << 4) + byte1;
}

unsigned char shiftAdd2Bytes(unsigned char byte2, unsigned char byte1){
	return (byte2 << 4) + byte1;
}

void incPC(){
	PC += 2;
}

bool isPressed(unsigned char key){
	
	switch(key){
		case 0x1:
			return key_status[0][0];
		case 0x2:
			return key_status[1][0];
		case 0x3:
			return key_status[2][0];
		case 0xc:
			return key_status[3][0];
		case 0x4:
			return key_status[0][1];
		case 0x5:
			return key_status[1][1];
		case 0x6:
			return key_status[2][1];
		case 0xd:
			return key_status[3][1];
		case 0x7:
			return key_status[0][2];
		case 0x8:
			return key_status[1][2];
		case 0x9:
			return key_status[2][2];
		case 0xe:
			return key_status[3][2];
		case 0xa:
			return key_status[0][3];
		case 0x0:
			return key_status[1][3];
		case 0xb:
			return key_status[2][3];
		case 0xf:
			return key_status[3][3];

	}
}

unsigned char isAKey(unsigned char key){
	switch(key){
		case '1':
			return 0x1;
		case '2':
			return 0x2;
		case '3':
			return 0x3;
		case '4':
			return 0xc;
		case 'q':
			return 0x4;
		case 'w':
			return 0x5;
		case 'e':
			return 0x6;
		case 'r':
			return 0xd;
		case 'a':
			return 0x7;
		case 's':
			return 0x8;
		case 'd':
			return 0x9;
		case 'f':
			return 0xe;
		case 'z':
			return 0xa;
		case 'x':
			return 0x0;
		case 'c':
			return 0xb;
		case 'v':
			return 0xf;
	}
	return 0xFF;
}