#########################################
#            Names and Emails           #
#########################################
Name: Shengtian Zhou
Email: shengtia@seas.upenn.edu

Name: Boya Li
Email: boyali@seas.upenn.edu

#########################################
#   List of all submitted source files  #
#########################################
# Header files include:

1. Error.h

2. GUI.h

3. Instruction.h

4. Storage.h

# Source files

1. emulator.cpp   

2. Error.cpp      

3. GUI.cpp       

4. Instruction.cpp 

5. Storage.cpp   

# Test files

1. TestFunctionality.cpp 

2. TestInstruction.cpp  

#########################################
#     OverView of work accomplished     #
#########################################
# Everything Required is Implemented #

1. Memory and Resiters Implemented & Tested

2. Keys Mapping Implemented, Tested:

  |1|2|3|C|		=>		|1|2|3|4|
  |4|5|6|D|		=>		|Q|W|E|R|
  |7|8|9|E|		=>		|A|S|D|F|
  |A|0|B|F|		=>		|Z|X|C|V|

3. All Instructions Implemented & Tested

4. Grpahical Display Implemented & Tested

5. Sound Implemented & Tested

6. Error Handlings Implemented & Tested

7. Sprites overflow: should Wrap Screen Implemented & Tested

##########################################
#		Compilation instructions         #
##########################################
# The following instructions need to be typed in the main directory

1. make: generates the emulator and tests  

2. make run: runs the emulator with the PONG game

3. make emulator: generates only the emulator

4. make test: generates a test for opcodes accessability

5. make runtest: runs the opcodes accessability test

6. make testfunc: generates a test for opcode functionality

7. make functest: runs the opcode functionality test

8. make clean: cleans all generated files

###########################################
#     Description of Code and Layout      #
###########################################
# Source file descriptions:

# Main Control Flow Layout:

1. Check for valid input
2. Initialize Memory and Registers
3. Initialize GUI
4. Execute Delay operations if any exists
5. Fetch/Decode/Execute/PC-Update
6. Update Screen
7. Go back to 4

# Source files, within the folder /src

1. emulator.cpp    : is the main file that contains the control flow of the program

2. Error.cpp       : contains common error functions when executing instructions

3. GUI.cpp         : contains everything related to GLUT and Key-press/-release handlers

4. Instruction.cpp : contains all instruction implementations and key mapping 

5. Storage.cpp     : contains memory and register definition and initialization function

# Test files, wihtin the folder /test

1. TestFunctionality.cpp : tests the functionality of each instruction (commented out, since this file is only used during development)

2. TestInstruction.cpp   : tests if an input opcode can get to the actual opcode.

# Note: all my functions and variables have proper comments above them, please refer to the actual file,
# it is redundant to copy and paste them here since I want to keep my ReadMe clean.

###########################################
#          Customization of Specs	      #
###########################################

1. Instead of using the given files, I combined the starter GUI code into GUI.h/.cpp
2. Since render partly meant execution of 1 cycle, I took it out of GUI and put it in emulator.cpp
3. No other customization, screen size is the same as in the starter code.



###########################################
#		       Other things               #
###########################################

    ---   Types of Errors   ---

1. Non-existent/wrong ROM files
2. Unknown opcodes
3. ROM size constraint
4. Stack Pointer underflow
5. Stack Pointer overflow
6. PC overflow
7. PC underflow
8. Index Register overflow
9. DXYN opcode: Invalid input of coordinates
10. EX9E, EXA1: None-existent key 
11. FX29: no sprite in interpreter space other than 0x0-0xF

    ---  Choice of Language  ---

1. C++ and C

	--- OpCode design choice  ---

1. Each Opcode is a function
2. This enables modularization and easy testing
3. An opcode is determined by the executeOpcode function in Instruction.cpp

	---   GUI design choice   ---

1. I put together the starter code into one GUI file
2. For modularization and easy testing
3. Easy replacement when using other GUI

    ---        Storage        ---

1. All the hardware, memory, registers are emulated in Storage
2. All the variables representing memory and registers are
3. The Stack is designed using an extra Array rather than the Ram, so the Data space becomes 0xFFF - 0x200 - 0xF


    ---        Sound          ---
1. The beep.aiff file is put in the main directory.
2. If you want to run other games than PONG in the bin folder, you have to copy and paste this beep file in the bin folder for the system to find it.
3. Since the PONG game is ran through "make run" in the main directory, it can successfully enable the sound.