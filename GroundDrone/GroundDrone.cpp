/**
 * @file GroundDrone.cpp
 * @author Sam4uk
 * @brief
 * @version 0.1
 * @date 2023-05-31
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "GroundDrone.hpp"

#include "GroundDroneConfig.hpp"
/*
Затримки для розворотів потрібно підібрати

На затримки впливають
  - рівень зараяду батареї
  - напруга живлевлення
  - внтуршіній опір джерела живлення

Константи підбирається експерементальним шляхои
 */
const int DELAY_RUN = 2;
const int DELAY_RUN_BACK = 400;
const int DELAY_ROTATE = 200;
const int DELAY_TURN = 400;
const int DELAY_TURN_BACK = 400;

void GroundDrone::Init() {
  pinMode(MOTOR1_A, OUTPUT);
  pinMode(MOTOR1_B, OUTPUT);
  pinMode(MOTOR2_A, OUTPUT);
  pinMode(MOTOR2_B, OUTPUT);
  pinMode(MOTOR_FAIL, INPUT);
  pinMode(MOTOR_ENABLE, OUTPUT);
  pinMode(ENABLE_DRV_PIN, OUTPUT);
};

void GroundDrone::setSpeed(short value) { _speed = value; }

void GroundDrone::RunForward() {
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

void GroundDrone::RunBackward() {
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

void GroundDrone::RotateRight() {
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

void GroundDrone::TurnRight() {
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

void GroundDrone::TurnBackRight() {
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

void GroundDrone::RotateLeft() {
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

void GroundDrone::TurnLeft() {
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

void GroundDrone::TurnBackLeft() {
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

void GroundDrone::Stop() {
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

/**
 * @todo Методи потребують доопрацювання
 * 
 */
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

byte GroundDrone::getPrevDirection() { return _prev_direction; }
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

void GroundDrone::setSpeeds(short LeftSpeed, short RightSpeed) {
#if DEBUGING
  Serial.println("Forward");
#if DEBUGING > 5
  return;
#endif
#endif
  // Ігнорувати значення більші від 255 для уникнення переповенення
  if (abs(LeftSpeed) <= 255)
    if (LeftSpeed >= 0) 
    // пряме обертання
    {
      analogWrite(MOTOR1_A, LeftSpeed);
      digitalWrite(MOTOR1_B, LOW);
    } else
    // реверсне обертання
    {
      digitalWrite(MOTOR1_A, LOW);
      analogWrite(MOTOR1_B, -LeftSpeed);
    }

  // Ігнорувати значення більші від 255 для уникнення переповенення
  if (abs(RightSpeed) <= 255)
    if (RightSpeed >= 0)
    // пряме обертання
    {
      analogWrite(MOTOR2_A, RightSpeed);
      digitalWrite(MOTOR2_B, LOW);
    } else
    // реверсне обертання
    {
      analogWrite(MOTOR2_A, LOW);
      digitalWrite(MOTOR2_B, -RightSpeed);
    }
}