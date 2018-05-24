//**** LCD Pong Lab
/* This sample main file gives some example functions which can be implemented for the pong game
Following are the criterion of the game :
a. Two paddles along Y axis. Paddle height between 8-12 pixels. Width ~2pixel
b. Gridlines for the visible LCD region/game area
c. Ball < 3pixel radius, preferably originating at center of display
d. Score keeping for each player
e. Paddles move on touch screen input. Keep in mind to have bounded input region for each for simplicity
f. Ball follows proper game dynamics. Starts with random direction and speed
g. Generate sounds in game : when balls hits surfaces, hits paddles or misses paddles
h. Reset game if a player loses
These are not the exhaustive limits of the game. There can be more features to make the game interesting 
as long as these criterion are met.
Example functions and variables are provided to get you started. You structure your code independently from that.
*/

#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <util/delay.h>
#include "lcd.h"
#include "uart.h"

#define BAUD 9600
#define HIGH 1
#define LOW 0
//*** Size of the buffer = 128 bytes * 8pages
#define BUFFER 1024
//*** Use for colour
#define BLACK 0x000001

//Define Screen size variables
//Define paddle parameter variables
//Define ball parameter variables
unsigned int raw_x = 150;
unsigned int raw_y = 150;
unsigned int raw_ac = 0;
unsigned char x;
unsigned char y;

char ball_x = 63;
char ball_y = 31;

unsigned char upaddle_x = 125;
unsigned char upaddle_y = 26;

unsigned char cpaddle_x = 1;
unsigned char cpaddle_y = 26;

const unsigned char defalutcx = 1;
const unsigned char defaultcy = 26;
const unsigned char defaultux = 125;
const unsigned char defaultuy = 26;
const unsigned char defualtballx = 63;
const unsigned char defualtbally = 31;

const unsigned paddle_width = 1;
const unsigned paddle_length = 9;

const unsigned cscore_x = 53;
const unsigned uscore_x = 68;
const unsigned score_y = 0;
unsigned char cscore = 0;
unsigned char uscore = 0;

unsigned int upper_bound = 0;
unsigned int lower_bound = 1000;

//array
unsigned int ac[20]; 

unsigned char collision_condition = 0; // there are 

bool cdirection = 0; // 0 indicates going up, 1 indicates going down
bool udirection = 0;
bool reset = 1; // when the ball hits either vertical boundaries

unsigned char ballspeed = 3; // the speed of the ball
unsigned char balldirection = 0; // the ball stays the ball can go to 6 different direction

void drawball(unsigned char* buff, unsigned int x, unsigned int y){
	//setpixel(buff, ball_x, ball_y,1);
	fillrect(buff, x-1, y-2, 2, 4, 1);
	fillrect(buff, x-2, y-1, 4, 2, 1);
}

//**** Example function to generate different game tones
void beep_tone(){
	// horizontal bounds
	DDRB |= (1 << DDB1); 
	if(collision_condition == 9 || collision_condition == 10){
		OCR1A = 399;
		_delay_ms(50);
	}else if(collision_condition == 1 || collision_condition ==2){
		OCR1A = 199;
		_delay_ms(50);
	}else if(collision_condition == 0){
		// do nothing
	}else{
		OCR1A = 299;
		_delay_ms(50);
	}
	DDRB &= ~(1 << DDB1);
}

//***** Example function checks for Touchscreen inputs and changes the paddle positions  *****//
void checkInput(){

	readX();
	//_delay_us(250);
	readY();
	readAc();
	// calibration
	//float A = -0.0006, B = 0.1615, C = -20.2898, D = -0.1174, E = -0.0006 , F = 100.379;
	float A = 0.1874, B = 0.0043, C = -30.1983, D = -0.0064, E = -0.1296 , F = 111.6902;
	x = A * raw_x + B * raw_y + C;
	y = D * raw_x + E * raw_y + F; 
}

void updatebounds(){
//upperbound
//lowerbound
	
	for (int i = 0; i < 50; i++){
		readAc();
		if(raw_ac < lower_bound){
			lower_bound = raw_ac;
		}
		if(raw_ac > upper_bound){
			upper_bound = raw_ac;
		}
		
	}	
}
// ball direction = 1;
void left(){
	ball_x -= ballspeed;
}
// ball direction = 2
void right(){
	ball_x +=ballspeed;
}

// ball direction = 3
void leftup(){
	left();
	ball_y -= ballspeed;
}

// ball direction = 4
void leftdown(){
	left();
	ball_y += ballspeed;
}

// ball direction = 5
void rightup(){
	right();
	ball_y -= ballspeed;
}

// ball direction = 6
void rightdown(){
	right();
	ball_y += ballspeed;
}

//***** Example function to update game parameters
void update(){
	
	//update ball
	
	// update user paddle
	if( x > 110 && x < 128){
		if(y > 54){
			y = 54;
		}else{
			upaddle_y = y;
		}
	}
	
	// or if there is an ac input;
	if(raw_ac > upper_bound){
		if(upaddle_y <= 52){
			upaddle_y += 2;
		}
	}else if(raw_ac < lower_bound){
		if(upaddle_y >= 2){
			upaddle_y -= 2;
		}
	}
	
	// update computer paddle
	if(cpaddle_y <= 4 ){
		cdirection = 1;
	}else if(cpaddle_y >= 54){
		cdirection = 0;
	}
	cpaddle_y = cdirection ? cpaddle_y+4 : cpaddle_y-4;
	
	// update ball
	switch(collision_condition){
		case 0:
			if(balldirection == 1) left();
			else if (balldirection == 2) right();
			else if (balldirection == 3) leftup();
			else if (balldirection == 4) leftdown();
			else if (balldirection == 5) rightup();
			else if (balldirection == 6) rightdown();
			break;
		case 1:
			reset = 1;
			uscore++;
			break;
		case 2:
			reset = 1;
			cscore++;
			break;
		case 3:
			rightdown();
			balldirection = 6;
			break;
		case 4:
			rightup();
			balldirection = 5;
			break;
		case 5:
			leftdown();
			balldirection = 4;
			break;
		case 6:
			leftup();
			balldirection = 3;
			break;
		case 7:
			
			if(balldirection == 3) {rightup(); balldirection = 5;}
			if(balldirection == 4) {rightdown(); balldirection = 6;}
			if(balldirection == 1 && !cdirection) {rightup(); balldirection = 5;}
			if(balldirection == 1 && cdirection) {rightdown(); balldirection = 6;}
			break;
		case 8:
			
			if(balldirection == 5) {leftup(); balldirection = 3;}
			if(balldirection == 6) {leftdown(); balldirection = 4;}
			if(balldirection == 2) {left(); balldirection = 1;}
			break;
		case 9:
			if(balldirection == 3) {balldirection = 4; leftdown();}
			if(balldirection == 5) {balldirection = 6; rightdown();}
			break;
		case 10:
			if(balldirection == 4) {balldirection = 3; leftup();}
			if(balldirection == 6) {balldirection = 5; rightup();}
			break;
	}
	// reset everything if reset is true
	if(reset){
		// update score
		if(uscore == 10 || cscore == 10) { uscore = 0; cscore = 0; PORTB |= 0x01; _delay_ms(500); PORTB &= ~0x05; PORTB |= 0x00;}
		// update everything else
		cpaddle_x = defalutcx;
		cpaddle_y = defaultcy;
		upaddle_x = defaultux;
		upaddle_y = defaultuy;
		ball_x = defualtballx;
		ball_y = defualtbally;
		ballspeed = rand()%2 + 2;		
		balldirection = rand()%6 + 1;
		reset = 0;
	}
}

//**** Example draw function to draw all the game sprites/graphics
void draw(){
	clear_buffer(buff);
	
	// draw the game board
	drawrect(buff, 0, 0,  127, 63, 0);
	for(int i = 0; i <= 10; i++) if(i != 5) fillrect(buff, 63, i*6, 0, 3, 1);
	
	//drawupaddle
	fillrect(buff, upaddle_x, upaddle_y, paddle_width, paddle_length, 1);
	
	//drawcpaddle
	fillrect(buff, cpaddle_x, cpaddle_y, paddle_width, paddle_length, 1);
	//drawball
	drawball(buff,ball_x,ball_y);
	
	//draw user scores
	drawchar(buff, uscore_x, score_y, uscore+48);
	
	//draw computer scores
	drawchar(buff, cscore_x, score_y, cscore+48);
	write_buffer(buff);
}

//**** Example function checks for Horizontal and Vertical collisions of the Ball
void checkCollisions(){
	
	
	if(ball_x - 2 <= 0 && (ball_y+2 < cpaddle_y || ball_y-2 > cpaddle_y+9)){
		// hit left bound
		collision_condition = 1;
	}else if(ball_x + 2 >= 127 && (ball_y+2 < upaddle_y || ball_y > upaddle_y+9)){
		// hit the right bound
		collision_condition = 2;
	}else if (ball_x -2  <= 2 && !( ball_y+2 < cpaddle_y || ball_y-2 > cpaddle_y+9) && ball_y - 2 <= 2){
		// paddle c + upper bound
		collision_condition = 3;
	}else if(ball_x -2  <= 2 && !( ball_y+2 < cpaddle_y || ball_y-2 > cpaddle_y+9) && ball_y +2 >= 63){
		// paddle c + lower bound
		collision_condition = 4;
	}else if(ball_x + 2 >= 125 && !(ball_y+2 < upaddle_y || ball_y > upaddle_y+9) && ball_y - 2 <= 2){
		// paddle u + upper bound	
		collision_condition = 5;
	}else if(ball_x + 2 >= 125 && !(ball_y+2 < upaddle_y || ball_y > upaddle_y+9) && ball_y +2 >= 63){
		// paddle u + lower bound
		collision_condition = 6;
	}else if(ball_x -2  <= 2 && !( ball_y+2 < cpaddle_y || ball_y-2 > cpaddle_y+9)){
		// paddle c
		collision_condition = 7;
	}else if(ball_x + 2 >= 125 && !(ball_y+2 < upaddle_y || ball_y > upaddle_y+9)){
		// paddle u	
		collision_condition = 8;
	}else if(ball_y - 2 <= 2){
		// upper bound
		collision_condition = 9;
	}else if(ball_y +2 >= 63){
		// lower bound
		collision_condition  = 10;
	}else{
		collision_condition = 0;
	}
	
}

//**** Example function which reads the X coordinate for the touchscreen 
int readX(){
	// put x- and x+ in digital mode
	// x- = ddc3, x+ = ddc1
	DDRC |= (1 << DDC3) | (1 << DDC1);
	DDRC &= ~(1 << DDC0);
	DDRC &= ~(1 << DDC2);
	
	// set x- high, x+ low
	PORTC |= (1 << DDC3);
	PORTC &= ~(1 << DDC1);
	
	// ADC input select to be y-
	ADMUX &= ~((1 << MUX0) | (1 << MUX1) | (1 << MUX2) | (1 << MUX3));
	
	ADCSRA |= (1 << ADSC); // start an conversion
	while(ADCSRA & (1 << ADSC));
	raw_x = ADC;
}

//**** Example function which reads the Y coordinate for the touchscreen 
int readY(){
	// put y- and y+ in digital mode
	// y- = ddc0, y+ = ddc2
	DDRC |= (1 << DDC0) | (1 << DDC2);
	DDRC &= ~(1 << DDC1);
	DDRC &= ~(1 << DDC3);
	
	// set y- high, y+ low
	PORTC |= (1 << DDC0);
	PORTC &= ~(1 << DDC2);

	// ADC input select to be x-
	ADMUX |= (1 << MUX0);
	ADMUX |= (1 << MUX1);
	
	ADCSRA |= (1 << ADSC); // start an conversion
	while(ADCSRA & (1 << ADSC));
	raw_y = ADC;
}


int readAc(){
	ADMUX &= ~(1 << MUX0);
	ADMUX &= ~(1 << MUX1);
	ADMUX |= (1 << MUX2);
	
	ADCSRA |= (1 << ADSC); // start an conversion
	while(ADCSRA & (1 << ADSC));
	raw_ac = ADC;	
}

int main(void)
{

	uart_init();
	srand(time(NULL));
	
	//** Setup the backlight of the LCD - Red, Blue and Green - all on
	DDRD |= 0x80; 
	PORTD &= ~0x80;
	PORTD |= 0x00;
	
	DDRB |= 0x05;
	PORTB &= ~0x05;
	PORTB |= 0x00;

	// enable global interrupt
	sei();
	
	// initialize the LCD
	lcd_init();
	lcd_command(CMD_DISPLAY_ON);
	lcd_set_brightness(0x18);
	write_buffer(buff);
	_delay_ms(1500);
	clear_buffer(buff);
	write_buffer(buff);
	
	// setup ADC
	ADCSRA |= (1 << ADPS1) | (1 << ADPS0); // prescale to 1/32
	ADMUX |= (1 << REFS0);
	ADMUX &= ~(1 << REFS1); // reference voltage is set to 5V
	ADCSRA |= (1 << ADEN); // enable ADC
	//ADCSRA |= (1 << ADATE); // enable free running mode
	
	_delay_us(250); 
	
	
	//setup pwm
	TCCR1A |= 0x40;
	TCCR1B |= (1 << WGM12);
	TCCR1B |= 0x02;
	
	updatebounds();
	
	while (1)
	{	
		checkInput();
  		checkCollisions(); 
  		update();          
 		draw();
		beep_tone();
	}
}