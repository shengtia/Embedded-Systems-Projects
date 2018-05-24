#ifndef _Joystick_h
#define _Joystick_h

#include "mbed.h"

#define DEAD_ZONE 2 //where values will be set to zero. [-2,+2]

class Joystick 
{
    public: //Function and variables go here
        Joystick(PinName pinA, PinName pinB); //Constructor
        float horizontal(void); //Reads horizontal value of joystick
        float vertical(void);  //Reads vertical value of joystick
        void setScale(float min, float max); //Set the scale of values
        
        
    private:
        
        AnalogIn horiz; //horizontal pot in joystick
        AnalogIn vert; //Vertical pot in joystick
        
        float rawV_values[5];
        float rawH_values[5];
        float _min, _max; //Min and Max for scaling
        float rawMinH, rawMaxH, rawMinV, rawMaxV; //Max/Min raw values we have seen so far
        float raw_hc, raw_vc; //Raw Center values
        
        float vavr();
        float havr();
        void updateh(float h);
        void updatev(float v);
};

#endif