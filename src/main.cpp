#include <Arduino.h>
#include <../lib/movement.h>


Movement *fl, *fr, *rl, *rr;

void move(int ly, int lx, int rx);
void setMotorSpeed();

void setup() {
  Serial.begin(115200);

  fl = new Movement(1.0, 0.1, 0.05, 34, 35, 6.0, 360);
  fl->begin();

  fr = new Movement(1.0, 0.1, 0.05, 36, 39, 6.0, 360);
  fr->begin();

  rl = new Movement(1.0, 0.1, 0.05, 4, 2, 6.0, 360);
  rl->begin();

  rr = new Movement(1.0, 0.1, 0.05, 16, 17, 6.0, 360);
  rr->begin();
}

void loop() {
  fr->update(10); //output di kirim ke motordriver sebagai pwm
  // put your main code here, to run repeatedly:
  
}

void move(int forward, int strafe, int turn){
  int vfl = forward + strafe + turn; //milai ideal
  int vfr = forward - strafe - turn; 
  int vbl = forward - strafe + turn;
  int vbr = forward + strafe - turn;

  float max_val = std::max({abs(vfl), abs(vfr), abs(vbl), abs(vbr)});

 if (max_val > 255) {
    vfl = (vfl / max_val) * 255;
    vfr = (vfr / max_val) * 255;
    vbl = (vbl / max_val) * 255;
    vbr = (vbr / max_val) * 255;
}
}