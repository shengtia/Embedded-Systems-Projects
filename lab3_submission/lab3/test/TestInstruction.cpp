#include <assert.h>
#include <iostream>
#include <stdio.h>
#include "../header/Storage.h"
#include "../header/Instruction.h"
using namespace std;

int main(){
	gamul8initialize();

	// test1: fectchDecodeOpCode
	unsigned char op1 = 0x6a;
	unsigned char op2 = 0x02;
	assert(0x6a02 == fetchDecodeOpCode(op1, op2));
	cout << "Test1: fetchDecodeOpCode passed!" << "\n\n";

	// test1-2: shiftadd3bytes
	assert(0x123 == shiftAdd3Bytes(0x1,0x2,0x3));
	cout << "Test1-2: shiftAdd3Bytes passed!" << "\n\n";

	// test1-3: shiftadd3bytes
	assert(0x23 == shiftAdd2Bytes(0x2,0x3));
	cout << "Test1-3: shiftAdd2Bytes passed!" << "\n\n";

	// test2: Opcode 0x00E0
	assert(2 == executeOpCode(0x00E0));
	cout << "Test2: 0x00E0 recognition passed!" << "\n\n";

	// test3: Opcode 0x00EE
	SP = 1;
	stack[SP] = 0x230;
	//printf("SP at test file is : %u \n", SP);
	assert(3 == executeOpCode(0x00EE));
	cout << "Test3: 0x00EE recognition passed!" << "\n\n";

	// test4: Opcode 0x00E0
	assert(1 == executeOpCode(0x000));
	cout << "Test4: 0x0NNN recognition passed!" << "\n\n";

	// test5: Opcode 0x1NNN
	assert(4 == executeOpCode(0x1456));
	cout << "Test5: 0x1NNN recognition passed!" << "\n\n";

	// test6: Opcode 0x2NNN
	//SP = 15;
	assert(5 == executeOpCode(0x2333));
	cout << "Test6: 0x2NNN recognition passed!" << "\n\n";

	// test7: Opcode 0x3XNN
	assert(6 == executeOpCode(0x3456));
	cout << "Test7: 0x3XNN recognition passed!" << "\n\n";

	// test8: Opcode 0x4XNN
	assert(7 == executeOpCode(0x4567));
	cout << "Test8: 0x4XNN recognition passed!" << "\n\n";

	// test9-1: Opcode 0x5XY0
	assert(8 == executeOpCode(0x5670));
	cout << "Test9-1: 0x5XY0 recognition passed!" << "\n\n";

	// test10: Opcode 0x6XNN
	assert(9 == executeOpCode(0x6123));
	cout << "Test10: 0x6XNN recognition passed!" << "\n\n";

	// test11: Opcode 0x7XNN
	assert(10 == executeOpCode(0x789A));
	cout << "Test10: 0x7XNN recognition passed!" << "\n\n";

	// test12: Opcode 0x8XY0
	assert(11 == executeOpCode(0x89A0));
	cout << "Test11: 0x8XY0 recognition passed!" << "\n\n";

	// test13: Opcode 0x8XY1
	assert(12 == executeOpCode(0x89A1));
	cout << "Test12: 0x8XY1 recognition passed!" << "\n\n";

	// test14: Opcode 0x8XY2
	assert(13 == executeOpCode(0x89A2));
	cout << "Test13: 0x8XY2 recognition passed!" << "\n\n";

	// test15: Opcode 0x8XY3
	assert(14 == executeOpCode(0x89A3));
	cout << "Test14: 0x8XY3 recognition passed!" << "\n\n";

	// test16: Opcode 0x8XY4
	assert(15 == executeOpCode(0x89A4));
	cout << "Test15: 0x8XY4 recognition passed!" << "\n\n";

	// test17: Opcode 0x8XY5
	assert(16 == executeOpCode(0x89A5));
	cout << "Test16: 0x8XY5 recognition passed!" << "\n\n";

	// test18: Opcode 0x8XY6
	assert(17 == executeOpCode(0x89A6));
	cout << "Test17: 0x8XY6 recognition passed!" << "\n\n";

	// test19: Opcode 0x8XY7
	assert(18 == executeOpCode(0x89A7));
	cout << "Test18: 0x8XY7 recognition passed!" << "\n\n";

	// test20: Opcode 0x8XYE
	assert(19 == executeOpCode(0x89AE));
	cout << "Test19: 0x8XYE recognition passed!" << "\n\n";

	// test21: Opcode 0x9XY0
	assert(20 == executeOpCode(0x9AB0));
	cout << "Test20: 0x9XY0 recognition passed!" << "\n\n";

	// test22: Opcode 0xANNN
	assert(21 == executeOpCode(0xABCD));
	cout << "Test21: 0xANNN recognition passed!" << "\n\n";

	// test23: Opcode 0xBNNN
	assert(22 == executeOpCode(0xBCDF));
	cout << "Test22: 0xBNNN recognition passed!" << "\n\n";

	// test24: Opcode 0xCXNN
	assert(23 == executeOpCode(0xCDEF));
	cout << "Test23: 0xCXNN recognition passed!" << "\n\n";

	// test25: Opcode 0xDXYN
	I = 0;
	assert(24 == executeOpCode(0xD125));
	cout << "Test24: 0xDXYN recognition passed!" << "\n\n";

	// test26: Opcode 0xEX9E
	assert(25 == executeOpCode(0xEF9E));
	cout << "Test25: 0xEX9E recognition passed!" << "\n\n";

	// test27: Opcode 0xEXA1
	assert(26 == executeOpCode(0xEFA1));
	cout << "Test26: 0xEXA1 recognition passed!" << "\n\n";

	// test28: Opcode 0xFX07
	assert(27 == executeOpCode(0xF107));
	cout << "Test27: 0xFX07 recognition passed!" << "\n\n";

	// test29: Opcode 0xFX0A
	cout << "you need to press a key in order to pass this function, press a key and press enter" << "\n\n";
	assert(28 == executeOpCode(0xF10A));
	cout << "Test28: 0xFX0A recognition passed!" << "\n\n";

	// test30: Opcode 0xFX15
	assert(29 == executeOpCode(0xF015));
	cout << "Test29: 0xFX15 recognition passed!" << "\n\n";

	// test31: Opcode 0xFX18
	assert(30 == executeOpCode(0xF018));
	cout << "Test30: 0xFX18 recognition passed!" << "\n\n";

	// test32: Opcode 0xFX1E
	assert(31 == executeOpCode(0xF01E));
	cout << "Test31: 0xF01E recognition passed!" << "\n\n";

	// test33: Opcode 0xFX29
	assert(32 == executeOpCode(0xF029));
	cout << "Test32: 0xFX29 recognition passed!" << "\n\n";

	// test34: Opcode 0xFX33
	assert(33 == executeOpCode(0xF033));
	cout << "Test33: 0xFX33 recognition passed!" << "\n\n";

	// test35: Opcode 0xFX55
	assert(34 == executeOpCode(0xF055));
	cout << "Test34: 0xFX55 recognition passed!" << "\n\n";

	// test36: Opcode 0xFX65
	assert(35 == executeOpCode(0xF065));
	cout << "Test35: 0xFX65 recognition passed!" << "\n\n";


	return 0;
}