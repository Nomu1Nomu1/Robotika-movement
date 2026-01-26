#include <Arduino.h>
#include "../lib/movement.h"


Movement Fl*, *Fr, Rl*, Rr*;

void setup() {
  Fl = new Movement(1,1,1,1,1,1,1);  
  Fl->begin();
  Fr = new Movement(1,1,1,1,1,1,1);
  Fr->begin();
  Rl = new Movement(1,1,1,1,1,1,1);
  Rl->begin();
  Rr = new Movement(1,1,1,1,1,1,1);
  Rr->begin();
}

void loop() {
  Fr->update(10); //output di kirim ke motordriver sebagai pwm
  // put your main code here, to run repeatedly:
  
}

void move(int ly, int lx, int rx){
    int frontLeft = ly + lx + rx;
    int frontRight = ly - lx - rx;
    int rearLeft = ly - lx + rx;
    int rearRight = ly + lx - rx;

float max_val = std::max({abs(frontLeft), abs(frontRight), abs(rearLeft), abs(rearRight)});

if (max_val > 255) {
    frontLeft = (frontLeft / max_val) * 255;
    frontRight = (frontRight / max_val) * 255;
    rearLeft = (rearLeft / max_val) * 255;
    rearRight = (rearRight / max_val) * 255;
}

//untuk menghubungkan hasil hitungan ke masing masing objek
float outputfrontLeft = fl->update(frontLeft);
float outputfrontRight = fr->update(frontRight);
float outputrearLeft = rl->update(rearLeft);
float outputrearRight = rr->update(rearRight);

//mengirim hasil akhir (pwm) ke motor driver
//sepertinya masih ada kekurangan bagian pin pwm
analogWrite(pinPWM_FL, outputfrontLeft);
analogWrite(pinPWM_FR, outputfrontRight);
analogWrite(pinPWM_RL, outputrearLeft);
analogWrite(pinPWM_RR, outputrearRight);
}