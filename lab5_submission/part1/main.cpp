//ESE519 Lab6 Controller Part 1 (INCOMPLETE IMPLIMENTATION)
//Author: Carter Sharer
//Date: 10/18/2016
//Sources: ESE350 Whack-A-Mole: https://developer.mbed.org/users/mlab4/code/ESE350-Whack-a-Mole/
//This is a simple working templete for sending and receving between two MRF24J40 Modules
//Contains a communication protocol for sending a receiving 

#include "mbed.h"
#include "MRF24J40.h"
#include <string>
#include "Joystick.h"
#include <stdlib.h>
#include <string.h>

#define SEND        //Uncomment if you want to transmit data
#define RECEIVE     //Uncomment if you want to receive data

#define NONE 250

//============================
//==    Pin Assignments     ==
//============================
//Knobs
#define POT1 p17  //Knob1
#define POT2 p18  //Knob2
#define POT3 p16  //Knob3
#define POT4 p15  //Knob4
//JoyStick
#define POTV p19 //Vertial
#define POTH p20 //Horizontal
//MRF24J
#define SDI p11
#define SDO p12
#define SCK p13
#define CS p7
#define RESET p8
//Button
#define BUTTON1 p21
#define COMMUNICATION_FORMAT "Jstick_h: %0.0f Jstick_v: %0.0f Knob1 %0.2f Knob2 %0.2f Knob3 %0.2f Knob4 %0.2f Button: %d "
#define RECEIVE_FORMAT "Pos_error: %.3f Robot_pos: %.3f Desired_pos: %.3f Angle_error: %.3f  Desired_angle: %.3f "
// receive data
float pos_error, angle_error, desired_angle, motor1, motor2, desired_pos, robot_pos;


//============================
//==        Objects         ==
//============================
//Knobs
AnalogIn pot1(POT1);
AnalogIn pot2(POT2);
AnalogIn pot3(POT3);
AnalogIn pot4(POT4);
float knob1, knob2, knob3, knob4;
int receive_num = 0;
int send_num = 0;
//Joystick
Joystick jstick(POTV, POTH);
float jstick_h, jstick_v;

//variable from the bot
int test = 0;

//MRF24J40
PinName mosi(SDI);
PinName miso(SDO);
PinName sck(SCK);
PinName cs(CS);
PinName reset(RESET);
// RF tranceiver
MRF24J40 mrf(mosi, miso, sck, cs, reset);

//Button
DigitalIn Button(BUTTON1);
bool button;

// LEDs
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

// Timer
Timer timer;

// Serial port for showing RX data.
Serial pc(USBTX, USBRX);

// Send / receive buffers.
// IMPORTANT: The MRF24J40 is intended as zigbee tranceiver; it tends
// to reject data that doesn't have the right header. So the first
// 8 bytes in txBuffer look like a valid header. The remaining 120
// bytes can be used for anything you like.
char txBuffer[128];
char rxBuffer[128];
int rxLen;


//***************** Do not change these methods (please) *****************//
/**
* Receive data from the MRF24J40.
*
* @param data A pointer to a char array to hold the data
* @param maxLength The max amount of data to read.
*/
int rf_receive(char *data, uint8_t maxLength)
{
    uint8_t len = mrf.Receive((uint8_t *)data, maxLength);
    uint8_t header[8]= {1, 8, 0, 0xA1, 0xB2, 0xC3, 0xD4, 0x00};

    if(len > 10) {
        //Remove the header and footer of the message
        for(uint8_t i = 0; i < len-2; i++) {
            if(i<8) {
                //Make sure our header is valid first
                if(data[i] != header[i])
                    return 0;
            } else {
                data[i-8] = data[i];
            }
        }
        //pc.printf("Received: %s length:%d\r\n", data, ((int)len)-10);
    }
    return ((int)len)-10;
}

/**
* Send data to another MRF24J40.
*
* @param data The string to send
* @param maxLength The length of the data to send.
*                  If you are sending a null-terminated string you can pass strlen(data)+1
*/
void rf_send(char *data, uint8_t len)
{
    //We need to prepend the message with a valid ZigBee header
    uint8_t header[8]= {1, 8, 0, 0xA1, 0xB2, 0xC3, 0xD4, 0x00};
    uint8_t *send_buf = (uint8_t *) malloc( sizeof(uint8_t) * (len+8) );

    for(uint8_t i = 0; i < len+8; i++) {
        //prepend the 8-byte header
        send_buf[i] = (i<8) ? header[i] : data[i-8];
    }
    //pc.printf("Sent: %s\r\n", send_buf+8);

    mrf.Send(send_buf, len+8);
    free(send_buf);
}
//***************** You can start coding here *****************//

//Returns true if c is a letter (upper or lower case), false otherwise
bool isLetter(char c) {
    
    /*
    * YOUR IMPLIMENTATION HERE
    *
    *
    *
    */
    if(((64 < c) && (c < 91)) || ((96 < c) && (c < 123))) return true;
    
    return false;
}

//Returns true if c is a number character (0-9), false otherwise
bool isNumber(char c) {
    
    /*
    * YOUR IMPLIMENTATION HERE
    *
    *
    *
    */
    if(47 < c && c < 58) return true;
    return false;
}

//Pulls data out of rxBuffer and updates global variables accordingly 
//Len is the length of the rxBuffer we are going to scan 
void communication_protocal(int len)
{
    bool found_name = false;
    bool found_num = false;
    bool complete_name = false;
    bool complete_num = false;
    uint8_t name_start = NONE; uint8_t name_end = NONE;
    uint8_t num_start = NONE; uint8_t num_end = NONE;
    
    //Loop through all charaters in rxBuffer
    for(uint8_t i = 0; i <= rxLen; i++) {
        char c = rxBuffer[i];
        //pc.printf("Indexed char '%c'\r\n", c);

        //Is it the start of a name? (Check if its a letter)
        if(isLetter(c) & name_start==NONE) { //if a num
            //If We havent found a name yet, this is start of a name
            if(found_name == false) {
                //pc.printf("found name start at: '%c'\r\n", c);
                name_start = i;
                found_name = true;
            }
        }
        //Is is a 'end of name' charater? Check for ' ', ':', '-'
        else if(((c == ' ') | (c == ':') | (c == '-')) & found_name & !complete_name) {// found end name character
            if(found_name) {
                complete_name = true;
                name_end = i;
                //pc.printf("found end of name at: '%c'\r\n", txBuffer[name_end]);
            }
        }

        //Is it a 'start of a number' charater? Check if its a number, or '-', or a '.'
        else if( (isNumber(c) | (c=='-') | (c=='.')) & complete_name & num_start==NONE) {
            if(found_num == false) {
                //pc.printf("found num start at: '%c'\r\n",c);
                num_start = i;
                found_num = true;
            }
        }
        //Is it a 'end of number' character? Check if its a ' ', ':', '-', or a letter
        else if( (((c==' ')|(c==':')|(c=='-')) | isLetter(c)) & found_num & complete_name) {
            if(found_num) {
                complete_num = true;
                num_end = i;
                //pc.printf("found end of num at: '%c' \r\n", txBuffer[num_end]);
            }
        }
        
        //If we have a complete name AND number value (ie. start and end of each != NONE)
        if(found_name & found_num & complete_name & complete_num) {
            //pc.printf("Found MATCH\r\n");
            //Reset flags
            found_name = false;
            found_num = false;
            complete_name = false;
            complete_num = false;
            
            //Set name
            uint8_t nameLen = uint8_t((name_end-name_start) + 1);
            char * name[nameLen];
            *name = &rxBuffer[name_start];
            rxBuffer[name_end] = '\0';
            
            //Set num
            uint8_t numLen = uint8_t((num_end-num_start) + 1);
            char * num[numLen];
            *num = &rxBuffer[num_start];
            rxBuffer[num_end] = '\0';
            
            //Now that we have isolated a name and its number value
            //we want to set the corresponding value to this number.
            //Ex: if name is 'Knob4' and num is '0.34', we want to the the 
            // variable name knob4 to the value 0.34.  
            //Do this for all variable names in COMMUNICATION_FORMAT
            //HINT: look up strcmp, and atof
            /*
            * YOUR IMPLIMENTATION HERE
            *
            *
            *
            */
            if(strcmp("Knob4", *name) == 0) knob4 = atof(*num);
            else if(strcmp("Knob3", *name) == 0) knob3 = atof(*num);
            else if(strcmp("Knob2", *name) == 0) knob2 = atof(*num);
            else if(strcmp("Knob1", *name) == 0) knob1 = atof(*num);
            else if(strcmp("Jstick_h", *name) == 0) jstick_h = atof(*num);
            else if(strcmp("Jstick_v", *name) == 0) jstick_v = atof(*num);
            else if(strcmp("Button", *name) == 0) button = atoi(*num);
            else if(strcmp("Pos_error", *name) == 0) pos_error = atof(*num);
            else if(strcmp("Angle_error", *name) == 0) angle_error = atof(*num);
            else if(strcmp("Desired_angle", *name) == 0) desired_angle = atof(*num);
            else if(strcmp("Robot_pos", *name) == 0) robot_pos = atof(*num);
            else if(strcmp("Desired_pos", *name) == 0) desired_pos = atof(*num);
            
            
            //Reset flags
            name_start = NONE;
            name_end = NONE;
            num_start = NONE;
            num_end = NONE;
        }
    }
}

int main (void){
   
    //Set the Channel. 0 is default, 15 is max
    uint8_t channel = 0; //channel = (Group# - 1)
    mrf.SetChannel(channel);

    //Set Baud rate (9600-115200 is ideal)
    pc.baud(115200);
    pc.printf("\r\n Start! \r\n");

    //Start Timer
    timer.start();

    //Scale Joystick Values, range[-100, 100]
    jstick.setScale(-100, 100);

    while(1) {
        
        //(1) Read Joystick Values, round to int8_t presision
        jstick_h = (int8_t)jstick.horizontal();
        jstick_v = (int8_t)jstick.vertical();
        //pc.printf("H: %0.2f V:%0.2f \r\n", jstick.horizontal(), jstick.vertical());

        //(2) Read Pot Values, Scale, and round to precision
        knob1 = (uint8_t)(pot1.read() * 100); //rounded to uint8_t
        knob2 = (pot2.read() * 100);
        knob3 = (pot3.read());
        knob4 = (int)(pot4.read() * 100);  //rounded to float

        //(3)Read Button Val, Add to buffer
        button = !Button.read(); //button is active low

#ifdef RECEIVE
        //RECEIVE DATA: Try to receive some data
        
        rxLen = rf_receive(rxBuffer, 128);
        if(rxLen > 0) {
            //Toggle the Led
            led1 = led1^1;
           
            //(4) Process data with our protocal 
            communication_protocal(rxLen);
            
            //Print values once we recieve and process data
            //pc.printf("Received| %d \r\n", receive_num++);
            pc.printf(RECEIVE_FORMAT, pos_error, robot_pos, desired_pos, angle_error, desired_angle);
        }//main if
#endif

#ifdef SEND    
        //SEND DATA: Send some data every 1/2 second
        if(timer.read_ms() >= 500) {
            //Reset the timer to 0
            timer.reset();
            // Toggle LED 2.
            led2 = led2^1;

            //(5) Add all values to buffer to be sent
            sprintf(txBuffer, COMMUNICATION_FORMAT, jstick_h, jstick_v, knob1, knob2, knob3, knob4, button);
            
            //(6) Send the buffer
            rf_send(txBuffer, strlen(txBuffer) + 1);
            //pc.printf("send | %d \r\n", send_num++);
            pc.printf("Sent| %s\r\n", txBuffer);
        }   
#endif

    } //end loop
}//end main