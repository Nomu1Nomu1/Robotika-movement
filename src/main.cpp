#include <Arduino.h>
#include <../lib/movement.h>


Movement* Fr;

void setup() {
  Fr = new Movement(1,1,1,1,1,1,1);
  Fr->begin();
}

void loop() {
  Fr->update(10); //output di kirim ke motordriver sebagai pwm
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