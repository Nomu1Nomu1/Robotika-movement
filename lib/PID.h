#ifndef PID_H
#define PID_H



class PID{
    public:
        float kp, ki, kd;
        float prevError, integral;
        PID(float p,float i,float d);
        float calculate(float target, float current);

};

#endif