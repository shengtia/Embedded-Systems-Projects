// opcode 1 0x0NNN
void _0NNN_();

// opcode 2 0x00E0
void _00E0_();

// opcode 3 0x00EE
void _00EE_();

// opcode 4 0x1NNN
void _1NNN_(unsigned char, unsigned char, unsigned char);

// opcode 5 0x2NNN
void _2NNN_(unsigned char , unsigned char , unsigned char);

// opcode 6 0x3XNN
void _3XNN_(unsigned char , unsigned char , unsigned char);

// opcode 7 0x4XNN
void _4XNN_(unsigned char , unsigned char , unsigned char);

// opcode 8 0x5XY0
void _5XY0_(unsigned char , unsigned char);

// opcode 9 0x6XNN
void _6XNN_(unsigned char , unsigned char , unsigned char);

// opcode 10 0x7XNN
void _7XNN_(unsigned char , unsigned char , unsigned char);

// opcode 11 0x8XY0
void _8XY0_(unsigned char , unsigned char);

// opcode 12 0x8XY1
void _8XY1_(unsigned char byte3, unsigned char byte2);

// opcode 13 0x8XY2
void _8XY2_(unsigned char byte3, unsigned char byte2);

// opcode 14 0x8XY3
void _8XY3_(unsigned char byte3, unsigned char byte2);

// opcode 15 0x8XY4
void _8XY4_(unsigned char byte3, unsigned char byte2);

// opcode 16 0x8XY5
void _8XY5_(unsigned char byte3, unsigned char byte2);

// opcode 17 0x8XY6
void _8XY6_(unsigned char byte3, unsigned char byte2);

// opcode 18 0x8XY7
void _8XY7_(unsigned char byte3, unsigned char byte2);

// opcode 19 0x8XYE
void _8XYE_(unsigned char byte3, unsigned char byte2);

// opcode 20 0x9XY0
void _9XY0_(unsigned char byte3, unsigned char byte2);

// opcode 21 0xANNN
void _ANNN_(unsigned char , unsigned char , unsigned char);

// opcode 22 0xBNNN
void _BNNN_(unsigned char , unsigned char , unsigned char);

// opcode 23 0xCXNN
void _CXNN_(unsigned char , unsigned char , unsigned char);

// opcode 24 0xDXYN
void _DXYN_(unsigned char , unsigned char , unsigned char);

// opcode 25 0xEX9E
void _EX9E_(unsigned char);

// opcode 26 0xEXA1
void _EXA1_(unsigned char);

// opcode 27 0xFX07
void _FX07_(unsigned char);

// opcode 28 0xFX0A
void _FX0A_(unsigned char);

// opcode 29 0xFX15
void _FX15_(unsigned char);

// opcode 30 0xFX18
void _FX18_(unsigned char);

// opcode 31 0xFX1E
void _FX1E_(unsigned char);

// opcode 32 0xFX29
void _FX29_(unsigned char);

// opcode 33 0xFX33
void _FX33_(unsigned char);

// opcode 34 0xFX55
void _FX55_(unsigned char);

// opcode 35 0xFX65
void _FX65_(unsigned char);

// fetch && decode, this function translates op1 and op2 into op
unsigned int fetchDecodeOpCode(unsigned char op1, unsigned char op2);

// execute the OpCode, return the opcode number, opcode is numbered from 1 to 35 from 0NNN to ... FX65
int executeOpCode(unsigned int op); 

// clears the screen
void clearScreen();

// shift add 3 bytes, with the first argument has the most significance, each byte is shifted by a multiple of four
unsigned int shiftAdd3Bytes(unsigned char, unsigned char, unsigned char);

//shift add 2 bytes, with the first argument has the most significance, each byte is shifted by a multiple of four
unsigned char shiftAdd2Bytes(unsigned char, unsigned char);

// increment PC by 1
void incPC();

// this function tells whether a key is pressed, it involves the mapping from 
// your keyboard to the simulator's
bool isPressed(unsigned char);

// detects if the pressed button is a key, if so, return it
unsigned char isAKey(unsigned char);