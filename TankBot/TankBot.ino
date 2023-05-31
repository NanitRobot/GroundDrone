/**
 * @file TankBot.ino
 * @author Sam4uk 
 * @brief 
 * @version 0.1
 * @date 2023-05-31
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "TankBot.hpp"

#define DEBUGING 1


// в сантиметрах (distance threshold) Пороги расстояний до препятствия
// Если ближе, то резкий разворот на месте, иначе плавный доворот
const int DST_TRH_TURN = 28;
// Если ближе, то стоп и назад
const int DST_TRH_BACK = 15;

/* пины для подключения HC-SR04 Ultrasonic Module Distance Measuring
 * 13, 2 цифровые пины
 * 14, 15 аналоговые пины A0 и A1 соответственно
 */
#define SONIC_PIN_TRIG (P6_4)
#define SONIC_PIN_ECHO (P6_1)
// Detection distance: 2cm--450cm
const int SONIC_DISTANCE_MAX = 450;
const int SONIC_DISTANCE_MIN = 2;

TankBot Tank;

void setup() {
  Nanit_Base_Start();
  Tank.Init();
  // // void Nanit_Base_Start 	();
  Serial.begin(9600);  // set up Serial library at 9600 bps
  // if (debug > 1) Serial.println(VERSION);

  pinMode(SONIC_PIN_TRIG, OUTPUT);
  pinMode(SONIC_PIN_ECHO, INPUT);

#if DEBUGING
  delay(3000);
#endif
}

// Возвращает расстояние до препятствия в сантиметрах
int measureDistance() {
#if DEBUGING > 5
  if (debug > 5) return random(SONIC_DISTANCE_MIN, 50);
#endif
  /* Для запуска передатчика нужно подать на Trig сигнал, длительностью 10мкс.
   * Передатчик который посылает 8 коротких импульсов с частотой 40kHz.
   * Приемник получает отраженный сигнал и на входе Echo генерируется сигнал,
   * длительность которого равна времени прохождения звукового сигнала.
   */
  digitalWrite(SONIC_PIN_TRIG, LOW);  // инициализация перед замером
  delayMicroseconds(5);               // 3
  digitalWrite(SONIC_PIN_TRIG, HIGH);
  delayMicroseconds(12);  // 10
  digitalWrite(SONIC_PIN_TRIG, LOW);

  long duration = pulseIn(SONIC_PIN_ECHO, HIGH);
  // Скорость звука 340 м/с или 29 микросекунд на сантиметр.
  // Звук идет вперед и возвращается назад, таким образом время нужно делить на
  // два
  int distance = duration / 58;  // = microseconds / 29 / 2

  if (distance < SONIC_DISTANCE_MIN)  // out of range
    return SONIC_DISTANCE_MIN;
  if (distance > SONIC_DISTANCE_MAX)  // out of range
    return SONIC_DISTANCE_MAX;

  return distance;
}

void loop() {
#ifdef DEBUGING
  Serial.println("\n*** new loop() start ***\n");
#endif

  int                                //
      distance = measureDistance(),  // Отримуємо дистанцію до перешкоди
      ch;                            //
#ifdef DEBUGING
  // Вимірювання відстані
  Serial.print("distance = ");
  Serial.println(distance);
#endif
  //   // препятствие так близко что надо ехать назад ?
  //
  if (distance <= DST_TRH_BACK)  // Якщо дуже близько портібно їхати назад
  {
#ifdef DEBUGING
    Serial.println("ALARM! Distance too small!!!");
#endif
    Tank.Stop();  // Зупиняємось
#ifdef DEBUGING
    delay(1000);
#endif

    if (Tank.getPrevDirection() == MOTOR_TURN_BACK_LEFT) {
      Tank.TurnBackRight();
    } else {
      Tank.TurnBackLeft();
    }

    // Раніше повертали заднім ходом на ліво?
    if (Tank.getPrevDirection() == MOTOR_TURN_BACK_LEFT) {
      Tank.TurnBackRight();
    } else {
      Tank.TurnBackLeft();
    }
#ifdef DEBUGING
    delay(1000);
#endif
    //     // motorRunBack();
    Tank.RunBackward();
    return;  // начать новый loop()
  }
  // прямо
  if (distance > DST_TRH_TURN) {
    Tank.RunForward();
  } else {
    Tank.Stop();
    if (random(1, 10) > 5) {
#ifdef DEBUGING
      delay(500);
#endif
      Tank.TurnLeft();
    } else {
#ifdef DEBUGING
      delay(500);
#endif
      Tank.TurnRight();
    }
  }
}