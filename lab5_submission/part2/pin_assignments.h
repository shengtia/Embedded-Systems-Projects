//pin_assignments.h
//Contains all the pin assignments for this project
//Author: Carter Sharer 
//Date: 11/8/2016

//HARDWARE
//The main processor is an mbed LPC1768
//There are two motor drivers A4988(1) and A4988(2)
//There is one 6 axis IMU MP6050
//There is one RF transciever MRF24J40

/****************** Wiring Diagram ********************************\

                    -------------
                    |GND |_|VOUT|  --- MPU6050|MRF24J40 VCC (3.3v)
                    |VIN     VU |
                    |VB      IF-|
                    |nR      IF+|
                    |p5   L  RD-|
                    |p6   P  RD+|
 MRF24J40 CS   ---  |p7   C  TD-|
 MRF24J40 RESET --  |p8   1  TD+|
                    |p9   7  D- |
                    |p10  6  D+ |
 MRF24J40 SDI  ---  |p11  8  p30|
 MRF24J40 SDO  ---  |p12     p29|  --- MPU6050 INT (CHECKPIN)
 MRF24J40 SCK  ---  |p13     p28|  --- MPU6050 SDA
                    |p14     p27|  --- MPU6050 SCL
 A4988(2) Step ---  |p15     p26|
 A4988(2) Dir  ---  |p16     p25|
                    |p17     p24|
 A4988(1,2) ENABLE  |p18     p23|
 A4988(1) Step ---  |p19     p22|
 A4988(1) Dir  ---  |p20     p21|
                    -------------

******************************************************************/


//Pin Declarations

//Motor Driver Pins
#define MOTOR1_STEP p19
#define MOTOR1_DIR p20
#define MOTOR2_STEP p15
#define MOTOR2_DIR p17
#define MOTOR_ENABLE p18  //For both motor divers 

//MPU6050 SPI PINS
#define I2C_SDA_1 p28
#define I2C_SCL_1 p27
#define CHECKPIN p29  //for IMU interupt pin (INT)

//MRF24J40
#define SDI p11
#define SDO p12
#define SCK p13
#define CS p7
#define RESET p8
