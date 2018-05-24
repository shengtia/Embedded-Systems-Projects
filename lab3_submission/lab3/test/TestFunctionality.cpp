#include <assert.h>
#include <iostream>
#include <stdio.h>
#include "../header/Storage.h"
#include "../header/Instruction.h"
using namespace std;


int main(){
	gamul8initialize();

	// test 6xnn
	// executeOpCode(0x61FF);

	// test 7xnn
	//executeOpCode(0x71FF);

	// test 8xy4
	//executeOpCode(0x8124);
	//c_r[1] = 12;
	//c_r[2] = 245;
	//executeOpCode(0x8124);

	// test 8xy5
	//executeOpCode(0x8125);
	//c_r[1] = 243;
	//c_r[2] = 245;
	//executeOpCode(0x8125);

	// test 8xy6
	//c_r[1] = 6;
	//executeOpCode(0x8126);

	// test 8xy7
	//executeOpCode(0x8127);
	//c_r[1] = 5;
	//c_r[2] = 4;
	//executeOpCode(0x8127);

	// test 8xyE
	//c_r[1] = 128;
	//executeOpCode(0x812E);

	// test 9xy0
	//executeOpCode(0x9120);

	// test ANNN
	//executeOpCode(0xA056);

	// test CXNN
	//executeOpCode(0xC0FF);

	// test FX55
	//I = 0xF00;
	//executeOpCode(0xF455);

	// test FX33
	//c_r[1] = 234;
	//executeOpCode(0xF133);

	// test DXYN
	//c_r[1] = 65;
	//c_r[2] = 30;
	//executeOpCode(0xD125);
	 
}