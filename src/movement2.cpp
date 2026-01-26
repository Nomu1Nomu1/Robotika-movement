#include "../movement robotik/movement.h"
#include "../movement robotik/encoder.h"
#include "../movement robotik/PID.h"

Movement::Movement(float Kp, float Ki, float Kd, int chanelA, int ChanelB, float diameter, float ppr){
    encoder = new Encoder(chanelA,ChanelB,ppr,diameter);
    pid = new PID(Kp,Ki,Kd);

}

float Movement::update(float target){
    encoder->update();
    return pid->calculate(target, encoder->getRPM()); // output kecepatan setiap roda setelah dikoreksi
}

void Movement::begin(){
    encoder -> begin();
}