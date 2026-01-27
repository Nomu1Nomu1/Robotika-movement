#ifndef WHEEL_H
#define WHEEL_H

#include <Arduino.h>

class Wheel {
  private:
    int _rpwm_pin;
    int _lpwm_pin;
    int _r_en_pin;
    int _l_en_pin;
    
    int _pwm_channel_r;
    int _pwm_channel_l;
    int _frequency;
    int _resolution;

  public:
    Wheel(int rpwm, int lpwm, int r_en, int l_en, int channel_r, int channel_l);

    void begin();

    void drive(int speed);
    
    void stop();
};

#endif