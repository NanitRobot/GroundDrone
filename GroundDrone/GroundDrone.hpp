/**
 * @file GroundDrone.hpp
 * @author Sam4uk
 * @brief
 * @version 0.1
 * @date 2023-05-31
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#include <NanitLib.h>

/*
 * Види поворотів
 */

const byte                   //
    MOTOR_ROTATE_RIGHT = 0,  //  розворот праворуч на місці
    MOTOR_TURN_RIGHT = 1,    // плавний поворот праворуч
    MOTOR_ROTATE_LEFT = 2,  // розворот ліоруч на місці
    MOTOR_TURN_LEFT = 3,    //  плавний поворот ліпоруч
    MOTOR_TURN_BACK_RIGHT = 4,  // поворот праворуч заднім ходом
    MOTOR_TURN_BACK_LEFT = 5;  // поворот ліворуч заднім ходом

class GroundDrone {
 public:
  void Init();           ///< Ініціалізація моторів
  void setSpeed(short);  ///< Вказати швидкість руху
  void RunForward();     ///< Рухаємось вперед прямо
  void RunBackward();    ///< Назад прямо
  void RotateRight();    ///< Правий розворот на місці
  void TurnRight();      ///< Правий поворт
  void TurnBackRight();  ///< Правий поворт заднім ходом
  void RotateLeft();     ///< Лівий розворот на місці
  void TurnLeft();       ///< Лівий поворот
  void TurnBackLeft();   ///< Лівий поворт заднім ходом
  void Stop();           ///< Зупика
  byte getPrevDirection();  ///<

 private:
  void setSpeeds(short LeftSpeed, short RightSpeed);
  byte                  //
      _curent_speed,    //
      _prev_direction;  //
  short                 //
      _speed{255};      //
};
