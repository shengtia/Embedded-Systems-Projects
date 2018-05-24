// all this functions started with "check" will result in program exit if conditions are met.
// the functions without "check" will exit the program directly after displaying the proper error msg

// prints an error message in the console when stack overflows
void checkStackOverflow();

// prints an error message in the console when stack underflows
void checkStackUnderflow();

// prints an error message in the console when PC overflows
void checkPCOverflow();

// prints an error message in the console when the PC underflows
void checkPCUnderflow();

// prints an error message in the console when index overflows
void checkIndexOverflow();

// prints an error message in the console when there is an invalid opcode
void invalidOpcode();