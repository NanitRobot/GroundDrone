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

#include "TankBotConfig.hpp"
/*
Затримки для розворотів потрібно підібрати
 */
const int DELAY_RUN = 2;
const int DELAY_RUN_BACK = TURN_DELAY;
const int DELAY_ROTATE = TURN_DELAY;
const int DELAY_TURN = TURN_DELAY;
const int DELAY_TURN_BACK = TURN_DELAY;

void TankBot::Init() {
  pinMode(MOTOR1_A, OUTPUT);
  pinMode(MOTOR1_B, OUTPUT);
  pinMode(MOTOR2_A, OUTPUT);
  pinMode(MOTOR2_B, OUTPUT);
  pinMode(MOTOR_FAIL, INPUT);
  pinMode(MOTOR_ENABLE, OUTPUT);
  pinMode(ENABLE_DRV_PIN, OUTPUT);
};

void TankBot::RunForward() {
#if DEBUGING
  Serial.println("Forward");
#if (DEBUGING > 5)
  return;
#endif
#endif
  digitalWrite(MOTOR_ENABLE, HIGH);
  digitalWrite(ENABLE_DRV_PIN, HIGH);
  setSpeeds(_speed, _speed);
  delay(DELAY_RUN);
};

void TankBot::RunBackward() {
#if DEBUGING
  Serial.println("Backward");
#if (DEBUGING > 5)
  return;
#endif
#endif
  digitalWrite(MOTOR_ENABLE, HIGH);
  digitalWrite(ENABLE_DRV_PIN, HIGH);
  setSpeeds(-_speed, -_speed);
  delay(DELAY_RUN);
};

void TankBot::RotateRight() {
#if DEBUGING
  Serial.println("RotateRight");
#if (DEBUGING > 5)
  return;
#endif
#endif
  _prev_direction = MOTOR_ROTATE_RIGHT;
  digitalWrite(MOTOR_ENABLE, HIGH);
  digitalWrite(ENABLE_DRV_PIN, HIGH);
  setSpeeds(_speed, -_speed);
  delay(DELAY_ROTATE);
};

void TankBot::TurnRight() {
#if DEBUGING
  Serial.println("TurnRight");
#if (DEBUGING > 5)
  return;
#endif
#endif
  _prev_direction = MOTOR_TURN_RIGHT;
  digitalWrite(MOTOR_ENABLE, HIGH);
  digitalWrite(ENABLE_DRV_PIN, HIGH);
  setSpeeds(_speed, 0);
  delay(DELAY_TURN);
};

void TankBot::TurnBackRight() {
#if DEBUGING
  Serial.println("TurnBackRight");
#if (DEBUGING > 5)
  return;
#endif
#endif
  _prev_direction = MOTOR_TURN_BACK_RIGHT;
  digitalWrite(MOTOR_ENABLE, HIGH);
  digitalWrite(ENABLE_DRV_PIN, HIGH);
  setSpeeds(0, -_speed);
  delay(DELAY_TURN);
}

void TankBot::RotateLeft() {
#if DEBUGING
  Serial.println("RotateLeft");
#if (DEBUGING > 5)
  return;
#endif
#endif
  _prev_direction = MOTOR_ROTATE_LEFT;
  digitalWrite(MOTOR_ENABLE, HIGH);
  digitalWrite(ENABLE_DRV_PIN, HIGH);
  setSpeeds(-_speed, _speed);
  delay(DELAY_ROTATE);
}

void TankBot::TurnLeft() {
#if DEBUGING
  Serial.println("TurnLeft");
#if (DEBUGING > 5)
  return;
#endif
#endif
  _prev_direction = MOTOR_TURN_LEFT;
  digitalWrite(MOTOR_ENABLE, HIGH);
  digitalWrite(ENABLE_DRV_PIN, HIGH);
  setSpeeds(0, _speed);
  delay(DELAY_ROTATE);
}

void TankBot::TurnBackLeft() {
#if DEBUGING
  Serial.println("TurnBackLeft");
#if (DEBUGING > 5)
  return;
#endif
#endif
  _prev_direction = MOTOR_TURN_BACK_LEFT;
  digitalWrite(MOTOR_ENABLE, HIGH);
  digitalWrite(ENABLE_DRV_PIN, HIGH);
  setSpeeds(-_speed, 0);
  delay(DELAY_ROTATE);
}

void TankBot::Stop() {
#if DEBUGING
  Serial.println("Stop");
#if (DEBUGING > 5)
  return;
#endif
#endif
  digitalWrite(MOTOR_ENABLE, LOW);
  digitalWrite(ENABLE_DRV_PIN, LOW);
  setSpeeds(0, 0);
}

#if 0
void StopSlow() {
#if DEBUG
  Serial.println("Stop slow");
#if (DEBUG > 5)
  return;
#endif
#endif
    int speed;
    int diff = SPEED_CURRENT / 3;  // сбрасываем скорость в 3 приема
    const byte steps{3};
    const short diff{_speed / steps};
    for (shspeed = SPEED_CURRENT; speed <= 0; speed -= diff) {
      motorSetSpeed(speed);
      delay(150);
    }
    Stop();
}
// void RunSlow() { setSpeed(255); }
#endif

byte TankBot::getPrevDirection() { return _prev_direction; }
/**
 * @brief Задає швидкість обертання двигунів
 *
 * Функція приймає значення від -255 до 255.
 *
 * Позитивні значення задають пряме обертання двигунів
 * Негативні значення задають реверсне обертання двигунів
 *
 * @param LeftSpeed Швидкість обертання лівого двигуна
 * @param RightSpeed Швидкість обертання правого двигуна
 */

void TankBot::setSpeeds(short LeftSpeed, short RightSpeed) {
#if DEBUGING
  Serial.println("Forward");
#if DEBUGING > 5
  return;
#endif
#endif
  if ((abs(LeftSpeed) <= 255) and (LeftSpeed >= 0)) {
    analogWrite(MOTOR1_A, LeftSpeed);
    digitalWrite(MOTOR1_B, LOW);
  } else {
    analogWrite(MOTOR1_A, LOW);
    digitalWrite(MOTOR1_B, -LeftSpeed);
  }
  if ((abs(RightSpeed) <= 255) and (RightSpeed >= 0)) {
    analogWrite(MOTOR2_A, RightSpeed);
    digitalWrite(MOTOR2_B, LOW);
  } else {
    analogWrite(MOTOR2_A, LOW);
    digitalWrite(MOTOR2_B, -RightSpeed);
  }
}