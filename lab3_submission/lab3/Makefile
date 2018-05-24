all: 
	clear
	g++ src/*.cpp -lGL -lGLU -lglut -o bin/emulator
	g++ test/TestInstruction.cpp src/Error.cpp src/Instruction.cpp src/Storage.cpp -o test/TestInstruction
	g++ test/TestFunctionality.cpp src/Error.cpp src/Instruction.cpp src/Storage.cpp -o test/TestFunctionality

	
emulator:
	clear
	g++ src/*.cpp -lGL -lGLU -lglut -o bin/emulator

test:
	clear
	g++ test/TestInstruction.cpp scr/Error.cpp src/Instruction.cpp src/Storage.cpp -o test/TestInstruction

testfunc:
	clear
	g++ test/TestInstruction.cpp src/Error.cpp src/Instruction.cpp src/Storage.cpp -o test/TestFunctionality

clean:
	clear 
	rm bin/emulator test/TestInstruction test/TestFunctionality


run:
	./bin/emulator games/PONG.bin

runtest: 
	./test/TestInstruction

functest:
	./test/TestFunctionality
