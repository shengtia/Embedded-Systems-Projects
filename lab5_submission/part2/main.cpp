//Balance Bot V4
//Author: Carter Sharer
//Date: 10/13/2016
//ESE519 Lab 6 Part 3 Skeleton Code

/******************************* README USAGE *******************************
* This robot must be powered on while it is laying down flat on a still table 
* This allows the robot to calibrate the IMU (~5 seconds)
* The motors are DISABLED when the robot tilts more then +-45 degrees from
* vertical.  To ENABLE the motors you must lift the robot to < +- 45 degres and
* press the joystick button.  
* To reset the motor positions you must press the josystick button anytime.
******************************************************************************/

//Balance Bot Begin
#include "pin_assignments.h"
#include "I2Cdev.h"
#include "JJ_MPU6050_DMP_6Axis.h"
#include "balance_bot.h"
#include "balance_bot_IMU.h"
#include "stepper_motors.h"
#include "MRF24J40.h"

//Angle Offset is used to set the natural balance point of your robot.  
//You should adjust this offset so that your robots balance point is near 0 
#define ANGLE_OFFSET 105

//For RF Communication
#define JSTICK_H 8
#define JSTICK_V 9
#define SPACE 10
#define KNOB1 11
#define KNOB2 12
#define KNOB3 13
#define KNOB4 14
#define ANGLE 15
#define BUTTON 16
#define JSTICK_OFFSET 100
#define TX_BUFFER_LEN 18
#define TX_ANGLE_OFFSET 100
//Knobs
#define POT1 p17
#define POT2 p18
#define POT3 p16
#define POT4 p15
//JoyStick
#define POTV p19
#define POTH p20

//PID
#define MAX_THROTTLE 100
#define MAX_TARGET_ANGLE 12

//Banlance parameters
#define FORWARD_SPEED -10
#define BACKWARD_SPEED 10

// data to be sent back to the controller
#define COMMUNICATION_FORMAT "Pos_error: %.3f Robot_pos: %.3f Desired_pos: %.3f Angle_error: %.3f  Desired_angle: %.3f"
float pos_error, angle_error, desired_angle;

//PID Default control values from constant definitions
float Kp1;
float Kd1;
float Kp2;
float Kd2;



//Controller Values
float knob1, knob2, knob3, knob4;
float jstick_h, jstick_v;

//Control Variables
float target_angle;
float throttle = 0; //From joystick
float steering = 0; //From joystick
float max_target_angle = MAX_TARGET_ANGLE;
int robot_pos = 0; //Robots position
bool fallen = true;

// joystick and position control
float desiredpos = 0;

// the maximum correction angle the bot can have for position correction
int angle_threshold = 2;

Timer timer;
Timer send_timer;
int timer_value; 
int timer_old; 
int dt;

//Loop Counters 
uint8_t slow_loop_counter;
uint8_t medium_loop_counter;
uint8_t loop_counter;

Serial pc(USBTX, USBRX);

// LEDs
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

//Button
bool button;

float E1 = 0.0;
#include "communication.h"

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================
void init_imu()
{
    pc.printf("\r\r\n\n Start \r\n");

    // Manual MPU initialization... accel=2G, gyro=2000º/s, filter=20Hz BW, output=200Hz
    mpu.setClockSource(MPU6050_CLOCK_PLL_ZGYRO);
    mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_2000);
    mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
    mpu.setDLPFMode(MPU6050_DLPF_BW_10);  //10,20,42,98,188  // Default factor for BROBOT:10
    mpu.setRate(4);   // 0=1khz 1=500hz, 2=333hz, 3=250hz [4=200hz]default
    mpu.setSleepEnabled(false);
    wait_ms(500);

    // load and configure the DMP
    devStatus = mpu.dmpInitialize();
    if(devStatus == 0) {
        mpu.setDMPEnabled(true);
        mpuIntStatus = mpu.getIntStatus();
        dmpReady = true;
    } else {
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        pc.printf("DMP INIT error \r\n");
    }

    //Gyro Calibration
    wait_ms(500);
    pc.printf("Gyro calibration!!  Dont move the robot in 10 seconds... \r\n");
    wait_ms(500);

    // verify connection
    pc.printf(mpu.testConnection() ? "Connection Good \r\n" : "Connection Failed\r\n");

    //Adjust Sensor Fusion Gain
    dmpSetSensorFusionAccelGain(0x20);

    wait_ms(200);
    mpu.resetFIFO();
}

void setdesiredposition(){
    if(jstick_h > 0 ){
     //backward   
     desiredpos = 3;
    }else if(jstick_h < 0){
     //forward   
     desiredpos = -3;
    }
}
// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================
int main()
{
    //Set the Channel. 0 is default, 15 is max
    uint8_t channel = 0;
    mrf.SetChannel(channel);

    pc.baud(115200);
    pc.printf("Start\r\n");
    init_imu();
    timer.start();
    //timer
    timer_value = timer.read_us();
    send_timer.start();
    //Init Stepper Motors
    //Attach Timer Interupts (Tiker)
    timer_M1.attach_us(&ISR1, ZERO_SPEED);
    timer_M2.attach_us(&ISR2, ZERO_SPEED);
    step_M1 = 1;
    dir_M1 = 1;
    enable = DISABLE; //Disable Motors

    //Attach Interupt for IMU
    checkpin.rise(&dmpDataReady);

    //Used to set angle upon startup, filter
    bool FILTER_DISABLE = true;

    //Enable Motors
    enable = ENABLE;
    
    float E_1 = 0;
    float PE_1 = 0;
    while(1) {
        //Led 4 to indicate if robot it STANDING or FALLEN
        led4 = !fallen;

        //Led 2 to indicate a button press
        led2 = button;

        //If button is pressed reset motor position
        if(button) {
            pos_M1 = 0; //Reset position of Motor 1
            pos_M2 = 0; //Reset position of motor 2
            fallen = false; //Reset fallen flag
        }

        //This is the main while loop, all your code goes here
        while(!mpuInterrupt) {
            //Timer
            timer_value = timer.read_us();

            //Set gainz with knobs
            Kp1 = knob1;
            Kd1 = knob2;
            Kp2 = knob3;
            Kd2 = knob4;

            //Joystick control
            throttle = jstick_v;
            steering = jstick_h;

            /**** Update Values DO NOT MODIFY ********/
            loop_counter++;
            slow_loop_counter++;
            medium_loop_counter++;
            dt = (timer_value - timer_old);
            timer_old = timer_value;
            angle_old = angle;

            /*****************************************/

            //STANDING: Motor Control Enabled
            if(((angle < 45) && (angle > -45)) && (fallen == false)) {
                
                //Enable Motor
                enable = ENABLE;
                
                /* This is where you want to implement your controllers 
                * Start off with a simple P controller.
                *
                */
                
                //position control froward and backwards
                //detecting moving forward or backwards
                
                
                
                desiredpos += steering * dt/21000000;
               
                pos_error = robot_pos - desiredpos;
                desired_angle = Kp2 * pos_error + Kd2*25000000/dt * (pos_error - PE_1);
                desired_angle = CAP(desired_angle, MAX_ANGLE);
                angle_error = angle - desired_angle;
                
                //angle control
                motor1 = -Kp1 * angle_error - Kd1*20000000/dt * (angle_error - E_1); 
                motor2 = Kp1 * angle_error + Kd1*20000000/dt * (angle_error - E_1); 
                
                // getting feedback
                robot_pos = (pos_M1-pos_M2) / 300;
                E_1 = angle_error;
                PE_1 = pos_error;
                
                //float E = angle - 5; //should be balanced at 0
                //motor1 = (E * Kp1 + Kd1 / dt * (E - E1))* -1; 
                //motor2 = (E * Kp1 + Kd1 / dt * (E - E1));    
                //E1 = E;
                //Calculate motor inputs
                
                
                //motor1 += int16_t(throttle/2 + steering/8);
                //motor2 += int16_t(throttle/2 - steering/8);
                    
                
                if(jstick_v > 0 && jstick_h == 0){
                    //turn left
                    motor1 += -10;
                    motor2 += -10;
                }else if(jstick_v < 0 && jstick_h == 0){
                    // turn right   
                    motor1 += 10;
                    motor2 += 10;
                }else{
                    
                }
                
                
                //Cap the max and min values [-100, 100]
                motor1 = CAP(motor1, MAX_CONTROL_OUTPUT);
                motor2 = CAP(motor2, MAX_CONTROL_OUTPUT);
                
               
                
                //Set Motor Speed here
                setMotor1Speed(motor1);
                setMotor2Speed(motor2);

            } else { //FALLEN: Motor Control Disabled
                //Disable Motors
                enable = DISABLE;

                //Set fallen flag
                fallen = true;
            }
            
            /* Here are some loops that trigger at different intervals, this 
            * will allow you to do things at a slower rate, thus saving speed
            * it is important to keep this fast so we dont miss IMU readings */
            
            //Fast Loop: Good for printing to serial monitor
            if(loop_counter >= 5) {
                loop_counter = 0;
                //pc.printf("angle:%d Kp1:%0.3f Kd1:%0.2f Kp2:%0.2f Kd2:%0.3f pos_M1:%d pos_M2:%d \r\n", (int)angle, Kp1, Kd1, Kp2, Kd2, pos_M1, pos_M2, robot_pos);
            }
            
            //Meduim Loop: Good for sending and receiving 
            if (medium_loop_counter >= 10) {
                medium_loop_counter = 0; // Read  status
               
                //Recieve Data
                rxLen = rf_receive(rxBuffer, 128);
                if(rxLen > 0) {
                    led1 = led1^1;
                    //Process data with our protocal
                    communication_protocal(rxLen);
                }

            }  // End of medium loop

            //Slow Loop: Good for sending if speed is not an issue 
            if(send_timer.read_ms() >= 500) {
                 send_timer.reset();
                 led3 = led3^1;
                 sprintf(txBuffer, COMMUNICATION_FORMAT, pos_error, robot_pos, desiredpos, angle_error, desired_angle);
                 rf_send(txBuffer, strlen(txBuffer) + 1);
                /* Send Data To Controller goes here *
                 *                                   */
            } //End of Slow Loop

            //Reattach interupt
            checkpin.rise(&dmpDataReady);
        } //END WHILE


        /********************* All IMU Handling DO NOT MODIFY *****************/
        //Disable IRQ
        checkpin.rise(NULL);

        //reset interrupt flag and get INT_STATUS byte
        mpuInterrupt = false;
        mpuIntStatus = mpu.getIntStatus();

        //get current FIFO count
        fifoCount = mpu.getFIFOCount();

        // check for overflow (this should never happen unless our code is too inefficient)
        if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
            // reset so we can continue cleanly
            mpu.resetFIFO();
            pc.printf("FIFO overflow!");

            //otherwise, check for DMP data ready interrupt (this should happen frequently)
        } else if (mpuIntStatus & 0x02) {
            //wait for correct available data length, should be a VERY short wait
            while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

            //read a packet from FIFO
            mpu.getFIFOBytes(fifoBuffer, packetSize);

            //track FIFO count here in case there is > 1 packet available
            //(this lets us immediately read more without waiting for an interrupt)
            fifoCount -= packetSize;

            //Read new angle from IMU
            new_angle = (float)(dmpGetPhi() - ANGLE_OFFSET);
            dAngle = new_angle - angle;

            //Filter out angle readings larger then MAX_ANGLE_DELTA
            if( ((dAngle < 15) && (dAngle > -15)) || FILTER_DISABLE) {
                angle = new_angle;
                FILTER_DISABLE = false; //turn of filter disabler
            } 
        }
        /********************* All IMU Handling DO NOT MODIFY *****************/

    } //end main loop
} //End Main()