#include <Arduino.h>
#include "../lib/movement.h"

// Single Movement object controls all 4 wheels
Movement* robot;

// Pin definitions
// Front Left Wheel
#define FL_ENC_A 34
#define FL_ENC_B 35
#define FL_PWM 25
#define FL_DIR1 26
#define FL_DIR2 27
#define FL_CHANNEL 0

// Front Right Wheel
#define FR_ENC_A 36
#define FR_ENC_B 39
#define FR_PWM 32
#define FR_DIR1 33
#define FR_DIR2 14
#define FR_CHANNEL 1

// Rear Left Wheel
#define RL_ENC_A 4
#define RL_ENC_B 2
#define RL_PWM 12
#define RL_DIR1 13
#define RL_DIR2 15
#define RL_CHANNEL 2

// Rear Right Wheel
#define RR_ENC_A 16
#define RR_ENC_B 17
#define RR_PWM 18
#define RR_DIR1 19
#define RR_DIR2 21
#define RR_CHANNEL 3

// Constants
const float WHEEL_DIAMETER = 6.0;  // cm
const float ENCODER_PPR = 360.0;   // Pulses per revolution
const float KP = 1.0;
const float KI = 0.1;
const float KD = 0.05;

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("=================================");
    Serial.println("  Robot Movement System v2.0");
    Serial.println("=================================");
    
    // Create movement system with all wheel configurations
    robot = new Movement(
        KP, KI, KD,
        // Front Left
        FL_ENC_A, FL_ENC_B, FL_PWM, FL_DIR1, FL_DIR2, FL_CHANNEL,
        // Front Right
        FR_ENC_A, FR_ENC_B, FR_PWM, FR_DIR1, FR_DIR2, FR_CHANNEL,
        // Rear Left
        RL_ENC_A, RL_ENC_B, RL_PWM, RL_DIR1, RL_DIR2, RL_CHANNEL,
        // Rear Right
        RR_ENC_A, RR_ENC_B, RR_PWM, RR_DIR1, RR_DIR2, RR_CHANNEL,
        // Wheel specs
        WHEEL_DIAMETER, ENCODER_PPR
    );
    
    // Initialize the system
    robot->begin();
    
    Serial.println("\nSetup complete! Starting movement...\n");
}

void loop() {
    // Example 1: Simple movement patterns
    // moveForwardBackward();
    
    // Example 2: Strafe left and right
    // strafeDemo();
    
    // Example 3: Rotation
    // rotationDemo();
    
    // Example 4: Complex holonomic movement
    holonomicDemo();
    
    // Example 5: Individual wheel control
    // individualWheelDemo();
}

// === DEMO FUNCTIONS ===

void moveForwardBackward() {
    Serial.println("Moving Forward...");
    robot->moveForward(50);
    robot->update();
    delay(3000);
    
    Serial.println("Stopping...");
    robot->stopAll();
    delay(1000);
    
    Serial.println("Moving Backward...");
    robot->moveBackward(50);
    robot->update();
    delay(3000);
    
    Serial.println("Stopping...");
    robot->stopAll();
    delay(1000);
}

void strafeDemo() {
    Serial.println("Strafing Left...");
    robot->strafeLeft(50);
    robot->update();
    delay(3000);
    
    Serial.println("Stopping...");
    robot->stopAll();
    delay(1000);
    
    Serial.println("Strafing Right...");
    robot->strafeRight(50);
    robot->update();
    delay(3000);
    
    Serial.println("Stopping...");
    robot->stopAll();
    delay(1000);
}

void rotationDemo() {
    Serial.println("Turning Left...");
    robot->turnLeft(50);
    robot->update();
    delay(3000);
    
    Serial.println("Stopping...");
    robot->stopAll();
    delay(1000);
    
    Serial.println("Turning Right...");
    robot->turnRight(50);
    robot->update();
    delay(3000);
    
    Serial.println("Stopping...");
    robot->stopAll();
    delay(1000);
}

void holonomicDemo() {
    Serial.println("Holonomic Movement: Forward + Strafe Right + Turn Right");
    robot->drive(100, 50, 30);
    robot->update();
    delay(3000);
    
    Serial.println("Stopping...");
    robot->stopAll();
    delay(1000);
    
    Serial.println("Holonomic Movement: Backward + Strafe Left + Turn Left");
    robot->drive(-100, -50, -30);
    robot->update();
    delay(3000);
    
    Serial.println("Stopping...");
    robot->stopAll();
    delay(1000);
}

void individualWheelDemo() {
    Serial.println("Testing individual wheels...");
    
    // Test Front Left
    Serial.println("Front Left");
    robot->setWheelSpeed(0, 50);
    robot->update();
    delay(2000);
    robot->stopAll();
    delay(1000);
    
    // Test Front Right
    Serial.println("Front Right");
    robot->setWheelSpeed(1, 50);
    robot->update();
    delay(2000);
    robot->stopAll();
    delay(1000);
    
    // Test Rear Left
    Serial.println("Rear Left");
    robot->setWheelSpeed(2, 50);
    robot->update();
    delay(2000);
    robot->stopAll();
    delay(1000);
    
    // Test Rear Right
    Serial.println("Rear Right");
    robot->setWheelSpeed(3, 50);
    robot->update();
    delay(2000);
    robot->stopAll();
    delay(1000);
}

// === MONITORING FUNCTION ===

void printWheelSpeeds() {
    Serial.print("FL: ");
    Serial.print(robot->getWheelRPM(0));
    Serial.print(" | FR: ");
    Serial.print(robot->getWheelRPM(1));
    Serial.print(" | RL: ");
    Serial.print(robot->getWheelRPM(2));
    Serial.print(" | RR: ");
    Serial.println(robot->getWheelRPM(3));
}