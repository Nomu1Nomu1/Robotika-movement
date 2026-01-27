#include <Arduino.h>
#include "../lib/movement.h"


Movement* Fl;
Movement* Fr;
Movement* Rl;
Movement* Rr;

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
  Fl->update(10);
  Fr->update(10);
  Rl->update(10);
  Rr->update(10);
  //output di kirim ke motordriver sebagai pwm
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
float outputfrontLeft = Fl->update(frontLeft);
float outputfrontRight = Fr->update(frontRight);
float outputrearLeft = Rl->update(rearLeft);
float outputrearRight = Rr->update(rearRight);

//mengirim hasil akhir (pwm) ke motor driver
//sepertinya masih ada kekurangan bagian pin pwm
analogWrite(pinPWM_FL, outputfrontLeft);
analogWrite(pinPWM_FR, outputfrontRight);
analogWrite(pinPWM_RL, outputrearLeft);
analogWrite(pinPWM_RR, outputrearRight);
}