/**
 * @file TankBot.cpp
 * @author Sam4uk
 * @brief 
 * @version 0.1
 * @date 2023-05-31
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "TankBot.hpp"

/*
Затримки для розворотів потрібно підібрати
 */
const int DELAY_RUN = 2;
const int DELAY_RUN_BACK = 500;
const int DELAY_ROTATE = 500;
const int DELAY_TURN = 500;
const int DELAY_TURN_BACK = 500;


void TankBot::Init() {
  pinMode(MOTOR1_A, OUTPUT);
  pinMode(MOTOR1_B, OUTPUT);
  pinMode(MOTOR2_A, OUTPUT);
  pinMode(MOTOR2_B, OUTPUT);
  pinMode(MOTOR_FAIL, INPUT);
  pinMode(MOTOR_ENABLE, OUTPUT);
  // pinMode(MOTOR_ENABLE,HIGH);
};

void TankBot::RunForward() {
#if DEBUGING
  Serial.println("Forward");
#if DEBUGING>5
  return;
#endif
#endif
  digitalWrite(MOTOR_ENABLE, HIGH);
  setSpeeds(_speed, _speed);
  delay(DELAY_RUN);
};

void TankBot::RunBackward() {
  digitalWrite(MOTOR_ENABLE, HIGH);
  setSpeeds(-_speed, -_speed);
  delay(DELAY_RUN);
};

void TankBot::RotateRight() {
  _prev_direction = MOTOR_ROTATE_RIGHT;
  digitalWrite(MOTOR_ENABLE, HIGH);
  setSpeeds(_speed, -_speed);
  delay(DELAY_ROTATE);
};

void TankBot::TurnRight() {
  _prev_direction = MOTOR_TURN_RIGHT;
  digitalWrite(MOTOR_ENABLE, HIGH);
  setSpeeds(_speed, 0);
  delay(DELAY_TURN);
};

void TankBot::TurnBackRight() {
  _prev_direction = MOTOR_TURN_BACK_RIGHT;
  digitalWrite(MOTOR_ENABLE, HIGH);
  setSpeeds(0, -_speed);
  delay(DELAY_TURN);
}

void TankBot::RotateLeft() {
  _prev_direction = MOTOR_ROTATE_LEFT;
  digitalWrite(MOTOR_ENABLE, HIGH);
  setSpeeds(-_speed, _speed);
  delay(DELAY_ROTATE);
}

void TankBot::TurnLeft() {
  _prev_direction = MOTOR_TURN_LEFT;
  digitalWrite(MOTOR_ENABLE, HIGH);
  setSpeeds(0, _speed);
  delay(DELAY_ROTATE);
}

void TankBot::TurnBackLeft() {
  _prev_direction = MOTOR_TURN_BACK_LEFT;
  digitalWrite(MOTOR_ENABLE, HIGH);
  setSpeeds(-_speed, 0);
  delay(DELAY_ROTATE);
}

void TankBot::Stop() {
  digitalWrite(MOTOR_ENABLE, LOW);
  setSpeeds(0, 0);
}

// {
//   digitalWrite(MOTOR_ENABLE, LOW);
//   setSpeeds(0, 0);
// }
// void StopSlow() {
//   //    if (debug > 1) Serial.println("Stop slow");
//   // if (debug > 5) return;
//   // int speed;
//   // int diff = SPEED_CURRENT / 3;  // сбрасываем скорость в 3 приема
//   const byte steps{3};
//   const short diff{_speed / steps};
//   // for (shspeed = SPEED_CURRENT; speed <= 0; speed -= diff) {
//   //   motorSetSpeed(speed);
//   //   delay(150);
//   // }
//   Stop();
// }
// void RunSlow() { setSpeed(255); }



// void setSpeed(byte value) {
//     //     // скорость мотора 0--255
//     // if (speed > 255) speed = 255;
//     // if (speed < 0) speed = 0;
//     // if (debug) {
//     //   Serial.print("Motor set Speed = ");
//     //   Serial.println(speed);
//     // }
//     // if (debug > 5) return;
//     // motorFrontLeft.setSpeed(speed);
//     // motorFrontRight.setSpeed(speed);
//     // motorRearLeft.setSpeed(speed);
//     // motorRearRight.setSpeed(speed);
//     // // запоминаем текущую скорость

//     _speed = value;
//   }

byte TankBot::getPrevDirection() { return _prev_direction; }

void TankBot::setSpeeds(short LeftSpeed, short RightSpeed) {
  if (LeftSpeed >= 0) {
    analogWrite(MOTOR1_A, LeftSpeed);
    digitalWrite(MOTOR1_B, LOW);
  } else {
    analogWrite(MOTOR1_A, LOW);
    digitalWrite(MOTOR1_B, -LeftSpeed);
  }
  if (RightSpeed >= 0) {
    analogWrite(MOTOR2_A, RightSpeed);
    digitalWrite(MOTOR2_B, LOW);
  } else {
    analogWrite(MOTOR2_A, LOW);
    digitalWrite(MOTOR2_B, -RightSpeed);
  }
}