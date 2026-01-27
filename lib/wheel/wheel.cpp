#include "wheel.h"

Wheel::Wheel(int rpwm, int lpwm, int r_en, int l_en, int channel_r, int channel_l) {
  _rpwm_pin = rpwm;
  _lpwm_pin = lpwm;
  _r_en_pin = r_en;
  _l_en_pin = l_en;
  _pwm_channel_r = channel_r;
  _pwm_channel_l = channel_l;
  
  _frequency = 5000;
  _resolution = 8;
}

void Wheel::begin() {
  pinMode(_r_en_pin, OUTPUT);
  pinMode(_l_en_pin, OUTPUT);
  pinMode(_rpwm_pin, OUTPUT);
  pinMode(_lpwm_pin, OUTPUT);

  // Enable the BTS7960 Driver
  digitalWrite(_r_en_pin, HIGH);
  digitalWrite(_l_en_pin, HIGH);

  // Setup ESP32 LEDC (PWM) channels
  ledcSetup(_pwm_channel_r, _frequency, _resolution);
  ledcSetup(_pwm_channel_l, _frequency, _resolution);

  // Attach pins to channels
  ledcAttachPin(_rpwm_pin, _pwm_channel_r);
  ledcAttachPin(_lpwm_pin, _pwm_channel_l);
}

void Wheel::drive(int speed) {
  // Clamp speed between -255 and 255
  if (speed > 255) speed = 255;
  if (speed < -255) speed = -255;

  if (speed > 0) {
    // Forward
    ledcWrite(_pwm_channel_r, speed);
    ledcWrite(_pwm_channel_l, 0);
  } else if (speed < 0) {
    // Backward (convert negative speed to positive for PWM)
    ledcWrite(_pwm_channel_r, 0);
    ledcWrite(_pwm_channel_l, abs(speed));
  } else {
    // Stop
    stop();
  }
}

void Wheel::stop() {
  ledcWrite(_pwm_channel_r, 0);
  ledcWrite(_pwm_channel_l, 0);
}