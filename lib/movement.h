#ifndef MOVEMENT_H
#define MOVEMENT_H

//menggabungkan movement

#include <Arduino.h>
class Encoder;
class PID;

class Movement{
    private:
        Encoder* encoder;
        PID* pid;
    public:
        Movement(float Kp, float Ki, float Kd, int chanelA, int ChanelB, float diameter, float ppr);
        float update(float target);
        void begin();


};


#endif