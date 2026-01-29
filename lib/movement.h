#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <Arduino.h>

// Forward declarations
class Encoder;
class PID;

// Wheel class - handles individual wheel control with encoder and PID
class Wheel {
private:
    Encoder* encoder;
    PID* pid;
    
    // Motor driver pins
    int pwmPin;
    int dir1Pin;
    int dir2Pin;
    int pwmChannel;
    
    // Current state
    float currentRPM;
    float targetRPM;

public:
    /**
     * Constructor for Wheel
     * @param kp, ki, kd: PID constants
     * @param encA, encB: Encoder pins
     * @param diameter: Wheel diameter in cm
     * @param ppr: Encoder pulses per revolution
     * @param pwm: PWM output pin
     * @param d1, d2: Direction control pins
     * @param channel: PWM channel (0-15 for ESP32)
     */
    Wheel(float kp, float ki, float kd, 
          int encA, int encB, float diameter, float ppr,
          int pwm, int d1, int d2, int channel);
    
    ~Wheel();
    
    // Initialize wheel (setup pins, encoder, PWM)
    void begin();
    
    // Set target speed in RPM
    void setSpeed(float targetSpeed);
    
    // Update PID and apply correction (call this in loop)
    void update();
    
    // Apply direct PWM value (-255 to 255)
    void applyPWM(float pwmValue);
    
    // Stop the wheel
    void stop();
    
    // Get current RPM from encoder
    float getRPM() const;
    
    // Get target RPM
    float getTargetRPM() const;
};

// Movement class - coordinates all 4 wheels for holonomic movement
class Movement {
private:
    Wheel* frontLeft;
    Wheel* frontRight;
    Wheel* rearLeft;
    Wheel* rearRight;
    
    // Normalize speeds if any exceed maxSpeed
    void normalizeSpeed(int& vfl, int& vfr, int& vrl, int& vrr, int maxSpeed = 255);

public:
    /**
     * Constructor - creates all 4 wheels with their configurations
     * PID values
     * @param kp, ki, kd: PID constants (same for all wheels)
     * 
     * Front Left pins
     * @param fl_encA, fl_encB: Front left encoder pins
     * @param fl_pwm, fl_d1, fl_d2: Front left motor pins
     * @param fl_channel: Front left PWM channel
     * 
     * Front Right pins
     * @param fr_encA, fr_encB: Front right encoder pins
     * @param fr_pwm, fr_d1, fr_d2: Front right motor pins
     * @param fr_channel: Front right PWM channel
     * 
     * Rear Left pins
     * @param rl_encA, rl_encB: Rear left encoder pins
     * @param rl_pwm, rl_d1, rl_d2: Rear left motor pins
     * @param rl_channel: Rear left PWM channel
     * 
     * Rear Right pins
     * @param rr_encA, rr_encB: Rear right encoder pins
     * @param rr_pwm, rr_d1, rr_d2: Rear right motor pins
     * @param rr_channel: Rear right PWM channel
     * 
     * Wheel specs
     * @param diameter: Wheel diameter in cm
     * @param ppr: Encoder pulses per revolution
     */
    Movement(float kp, float ki, float kd,
             // Front Left
             int fl_encA, int fl_encB, int fl_pwm, int fl_d1, int fl_d2, int fl_channel,
             // Front Right
             int fr_encA, int fr_encB, int fr_pwm, int fr_d1, int fr_d2, int fr_channel,
             // Rear Left
             int rl_encA, int rl_encB, int rl_pwm, int rl_d1, int rl_d2, int rl_channel,
             // Rear Right
             int rr_encA, int rr_encB, int rr_pwm, int rr_d1, int rr_d2, int rr_channel,
             // Wheel specs
             float diameter, float ppr);
    
    ~Movement();
    
    // Initialize all wheels
    void begin();
    
    // Update all wheels (call this in loop)
    void update();
    
    // Holonomic movement control
    void drive(int forward, int strafe, int turn);
    
    // Simple movement patterns
    void moveForward(float speed);
    void moveBackward(float speed);
    void strafeLeft(float speed);
    void strafeRight(float speed);
    void turnLeft(float speed);
    void turnRight(float speed);
    
    // Individual wheel control
    void setWheelSpeed(int wheelIndex, float speed); // 0=FL, 1=FR, 2=RL, 3=RR
    
    // Stop all wheels
    void stopAll();
    
    // Get RPM of specific wheel
    float getWheelRPM(int wheelIndex) const;
};

#endif