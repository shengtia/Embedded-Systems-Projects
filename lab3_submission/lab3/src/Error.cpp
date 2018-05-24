#include <iostream>
#include <cstdlib>
#include "../header/Storage.h"
#include "../header/Error.h"
using namespace std;

// prints an error message in the console when stack overflows
void checkStackOverflow(){
	if(SP >= 16){
		cerr << "Error : Stack overflow, Stack Pointer cannot be larger than or equal to 16!" << "\n";
		exit(0);
	}
}

// prints an error message in the console when stack underflows
void checkStackUnderflow(){
	if (SP == 255){
		cerr << "Error: Stack Pointer out of bounds, Stack Pointer cannot be negative!" << "\n";
		exit(0);	
	}
}

// prints an error message in the console when PC overflows
void checkPCOverflow(){
	if(PC > 4095){
		cerr << "Error : PC overflows" << endl;
		exit(0);
	}
}

void checkPCUnderflow(){
	if(PC < 0x200){
		cerr << "Error : PC underflows: accessing protected meory" << endl;
		exit(0);
	}
}

// prints an error message in the console when index overflows
void checkIndexOverflow(){
	if(I > 0xFFF){
		cerr << "Error : the index register has a value larger than 0xFFF" << "\n";
		exit(0);
	}
}

void invalidOpcode(){
	cerr << "Error : invalid opcode detected." << "\n";
	exit(0);
}