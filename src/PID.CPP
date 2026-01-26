#include "../lib/PID.h"

    PID::PID(float p,float i,float d): kp(p), ki(i), kd(d), prevError(0), integral(0){};
    float PID::calculate(float target, float current){
        float error = target - current;
        integral += error;
        float derivative = error - prevError;
        prevError = error;
        //kecepatan pwm roda 
        return (error*kp) + (integral*ki)+(derivative*kd);
        }