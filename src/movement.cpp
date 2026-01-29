#include "../lib/movement.h"
#include "../lib/encoder.h"
#include "../lib/PID.h"

// ==================== WHEEL CLASS IMPLEMENTATION ====================

Wheel::Wheel(float kp, float ki, float kd, 
             int encA, int encB, float diameter, float ppr,
             int pwm, int d1, int d2, int channel) {
    
    // Initialize encoder and PID
    encoder = new Encoder(encA, encB, ppr);
    pid = new PID(kp, ki, kd, -255, 255);
    
    // Store pin assignments
    pwmPin = pwm;
    dir1Pin = d1;
    dir2Pin = d2;
    pwmChannel = channel;
    
    // Initialize state
    currentRPM = 0;
    targetRPM = 0;
}

Wheel::~Wheel() {
    delete encoder;
    delete pid;
}

void Wheel::begin() {
    // Setup motor driver pins
    pinMode(pwmPin, OUTPUT);
    pinMode(dir1Pin, OUTPUT);
    pinMode(dir2Pin, OUTPUT);
    
    // Setup PWM channel (5kHz, 8-bit resolution)
    ledcSetup(pwmChannel, 5000, 8);
    ledcAttachPin(pwmPin, pwmChannel);
    
    // Initialize encoder
    encoder->begin();
    
    Serial.print("Wheel on channel ");
    Serial.print(pwmChannel);
    Serial.println(" initialized");
}

void Wheel::setSpeed(float targetSpeed) {
    targetRPM = targetSpeed;
}

void Wheel::update() {
    // Update encoder to get current RPM
    encoder->update();
    currentRPM = encoder->getRPM();
    
    // Calculate PID correction
    float correctedPWM = pid->calculate(targetRPM, currentRPM);
    
    // Apply PWM to motor
    applyPWM(correctedPWM);
}

void Wheel::applyPWM(float pwmValue) {
    // Constrain to valid PWM range
    pwmValue = constrain(pwmValue, -255, 255);
    
    if (pwmValue >= 0) {
        // Forward direction
        digitalWrite(dir1Pin, HIGH);
        digitalWrite(dir2Pin, LOW);
        ledcWrite(pwmChannel, (int)abs(pwmValue));
    } else {
        // Reverse direction
        digitalWrite(dir1Pin, LOW);
        digitalWrite(dir2Pin, HIGH);
        ledcWrite(pwmChannel, (int)abs(pwmValue));
    }
}

void Wheel::stop() {
    digitalWrite(dir1Pin, LOW);
    digitalWrite(dir2Pin, LOW);
    ledcWrite(pwmChannel, 0);
    targetRPM = 0;
}

float Wheel::getRPM() const {
    return currentRPM;
}

float Wheel::getTargetRPM() const {
    return targetRPM;
}

// ==================== MOVEMENT CLASS IMPLEMENTATION ====================

Movement::Movement(float kp, float ki, float kd,
                   // Front Left
                   int fl_encA, int fl_encB, int fl_pwm, int fl_d1, int fl_d2, int fl_channel,
                   // Front Right
                   int fr_encA, int fr_encB, int fr_pwm, int fr_d1, int fr_d2, int fr_channel,
                   // Rear Left
                   int rl_encA, int rl_encB, int rl_pwm, int rl_d1, int rl_d2, int rl_channel,
                   // Rear Right
                   int rr_encA, int rr_encB, int rr_pwm, int rr_d1, int rr_d2, int rr_channel,
                   // Wheel specs
                   float diameter, float ppr) {
    
    // Create all 4 wheels
    frontLeft = new Wheel(kp, ki, kd, fl_encA, fl_encB, diameter, ppr, 
                          fl_pwm, fl_d1, fl_d2, fl_channel);
    
    frontRight = new Wheel(kp, ki, kd, fr_encA, fr_encB, diameter, ppr,
                           fr_pwm, fr_d1, fr_d2, fr_channel);
    
    rearLeft = new Wheel(kp, ki, kd, rl_encA, rl_encB, diameter, ppr,
                         rl_pwm, rl_d1, rl_d2, rl_channel);
    
    rearRight = new Wheel(kp, ki, kd, rr_encA, rr_encB, diameter, ppr,
                          rr_pwm, rr_d1, rr_d2, rr_channel);
}

Movement::~Movement() {
    delete frontLeft;
    delete frontRight;
    delete rearLeft;
    delete rearRight;
}

void Movement::begin() {
    Serial.println("Initializing Movement System...");
    frontLeft->begin();
    frontRight->begin();
    rearLeft->begin();
    rearRight->begin();
    Serial.println("Movement System Ready!");
}

void Movement::update() {
    // Update all wheels with PID control
    frontLeft->update();
    frontRight->update();
    rearLeft->update();
    rearRight->update();
}

void Movement::normalizeSpeed(int& vfl, int& vfr, int& vrl, int& vrr, int maxSpeed) {
    // Find maximum absolute value
    int maxVal = max(max(abs(vfl), abs(vfr)), max(abs(vrl), abs(vrr)));
    
    // Normalize if any speed exceeds max
    if (maxVal > maxSpeed) {
        float scale = (float)maxSpeed / maxVal;
        vfl = (int)(vfl * scale);
        vfr = (int)(vfr * scale);
        vrl = (int)(vrl * scale);
        vrr = (int)(vrr * scale);
    }
}

void Movement::drive(int forward, int strafe, int turn) {
    // Mecanum wheel kinematics
    int vfl = forward + strafe + turn;  // Front Left
    int vfr = forward - strafe - turn;  // Front Right
    int vrl = forward - strafe + turn;  // Rear Left
    int vrr = forward + strafe - turn;  // Rear Right
    
    // Normalize speeds
    normalizeSpeed(vfl, vfr, vrl, vrr);
    
    // Set target speeds
    frontLeft->setSpeed(vfl);
    frontRight->setSpeed(vfr);
    rearLeft->setSpeed(vrl);
    rearRight->setSpeed(vrr);
}

void Movement::moveForward(float speed) {
    frontLeft->setSpeed(speed);
    frontRight->setSpeed(speed);
    rearLeft->setSpeed(speed);
    rearRight->setSpeed(speed);
}

void Movement::moveBackward(float speed) {
    frontLeft->setSpeed(-speed);
    frontRight->setSpeed(-speed);
    rearLeft->setSpeed(-speed);
    rearRight->setSpeed(-speed);
}

void Movement::strafeLeft(float speed) {
    frontLeft->setSpeed(-speed);
    frontRight->setSpeed(speed);
    rearLeft->setSpeed(speed);
    rearRight->setSpeed(-speed);
}

void Movement::strafeRight(float speed) {
    frontLeft->setSpeed(speed);
    frontRight->setSpeed(-speed);
    rearLeft->setSpeed(-speed);
    rearRight->setSpeed(speed);
}

void Movement::turnLeft(float speed) {
    frontLeft->setSpeed(-speed);
    frontRight->setSpeed(speed);
    rearLeft->setSpeed(-speed);
    rearRight->setSpeed(speed);
}

void Movement::turnRight(float speed) {
    frontLeft->setSpeed(speed);
    frontRight->setSpeed(-speed);
    rearLeft->setSpeed(speed);
    rearRight->setSpeed(-speed);
}

void Movement::setWheelSpeed(int wheelIndex, float speed) {
    switch(wheelIndex) {
        case 0: frontLeft->setSpeed(speed); break;
        case 1: frontRight->setSpeed(speed); break;
        case 2: rearLeft->setSpeed(speed); break;
        case 3: rearRight->setSpeed(speed); break;
        default: 
            Serial.println("Invalid wheel index! Use 0-3");
            break;
    }
}

void Movement::stopAll() {
    frontLeft->stop();
    frontRight->stop();
    rearLeft->stop();
    rearRight->stop();
}

float Movement::getWheelRPM(int wheelIndex) const {
    switch(wheelIndex) {
        case 0: return frontLeft->getRPM();
        case 1: return frontRight->getRPM();
        case 2: return rearLeft->getRPM();
        case 3: return rearRight->getRPM();
        default: 
            Serial.println("Invalid wheel index! Use 0-3");
            return 0;
    }
}