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
#include <Servo.h>

#include "TankBot.hpp"
#include "TankBotConfig.hpp"

#define DEBUGING 1

// в сантиметрах (distance threshold) Пороги расстояний до препятствия
const int  //
           // Если ближе, то резкий разворот на месте, иначе плавный доворот
    DST_TRH_TURN = 28,  //
    // Если ближе, то стоп и назад
    DST_TRH_BACK = 15;

// Detection distance: 2cm--450cm
const int                      //
    SONIC_DISTANCE_MAX = 450,  //
    SONIC_DISTANCE_MIN = 2;    //

TankBot    //
    Tank;  //
Servo      //
    Huck;  //

void Moving();

void setup() {
  Nanit_Base_Start();
  Tank.Init();
  // // void Nanit_Base_Start 	();
  Serial.begin(9600);  // set up Serial library at 9600 bps
  // if (debug > 1) Serial.println(VERSION);

  pinMode(SONIC_PIN_TRIG, OUTPUT);
  pinMode(SONIC_PIN_ECHO, INPUT);

  pinMode(LEFT_BORT_PIN, INPUT);
  pinMode(RIGHT_BORT_PIN, INPUT);

  pinMode(LIGHT_SENOSR_PIN, INPUT);

  pinMode(BUZZ_PIN, OUTPUT);
#ifdef USED_ACTIVE_BAZZER
  digitalWrite(BUZZ_PIN, HIGH);
#endif

#if DEBUGING
  delay(3000);
#endif
  Huck.attach(P1_1);
  Huck.write(90);
  Tank.setSpeed(120);
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

  long  //
      duration = pulseIn(SONIC_PIN_ECHO, HIGH);
  // Скорость звука 340 м/с или 29 микросекунд на сантиметр.
  // Звук идет вперед и возвращается назад, таким образом время нужно делить на
  // два
  int                            //
      distance = duration / 58;  // = microseconds / 29 / 2

  if (distance < SONIC_DISTANCE_MIN)  // out of range
    return SONIC_DISTANCE_MIN;
  if (distance > SONIC_DISTANCE_MAX)  // out of range
    return SONIC_DISTANCE_MAX;

  return distance;
}

// #define DEBUGING

bool                  //
    IsActive{false},  //
    IsArmed{false};   //

auto                         //
    milliseconds{millis()};  //
void loop() {
  Huck.write(90);
#ifdef DEBUGING
  Serial.println("\n*** new loop() start ***\n");
#endif

  // Активація бота в режим об'їду
  if (!digitalRead(LEFT_BORT_PIN) and !digitalRead(RIGHT_BORT_PIN)) {
    Tank.RunBackward();
    delay(TURN_DELAY);
    IsActive = true;
  }

  if (digitalRead(LIGHT_SENOSR_PIN) and !digitalRead(RIGHT_BORT_PIN)) {
    if (!IsArmed) {
      const short buzz_delay{100};
#ifdef USED_ACTIVE_BAZZER
      digitalWrite(BUZZ_PIN, LOW);
      delay(buzz_delay);
      digitalWrite(BUZZ_PIN, HIGH);
      delay(buzz_delay);
      digitalWrite(BUZZ_PIN, LOW);
      delay(buzz_delay);
      digitalWrite(BUZZ_PIN, HIGH);
      delay(buzz_delay);
      digitalWrite(BUZZ_PIN, LOW);
      delay(buzz_delay);
      digitalWrite(BUZZ_PIN, HIGH);
      delay(buzz_delay);
#else
      const short freg{1000};
      tone(BUZZ_PIN, freg, buzz_delay);
      tone(BUZZ_PIN, freg, buzz_delay);
      tone(BUZZ_PIN, freg, buzz_delay);
      noTone();
#endif
      milliseconds = 15000 + millis();
    }
    IsArmed = true;
  }

  digitalWrite(BUZZ_PIN, HIGH);

  /* if (IsArmed and !IsActive and milliseconds < millis()) {
     //скид
   }*/

  // Бот активний
  if (IsActive) Moving();
}

void Moving() {
  int                                //
      distance = measureDistance(),  // Отримуємо дистанцію до перешкоди
      ch;                            //
#ifdef DEBUGING
  // Вимірювання відстані
  Serial.print("distance = ");
  Serial.println(distance);
#endif
  // Tank.setSpeed(map(distance,150,255,SONIC_DISTANCE_MIN,SONIC_DISTANCE_MAX));
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
      Tank.RotateRight();
    } else {
      Tank.RotateLeft();
    }

    // Раніше повертали заднім ходом на ліво?
    if (Tank.getPrevDirection() == MOTOR_TURN_BACK_LEFT) {
      Tank.RotateRight();
    } else {
      Tank.RotateLeft();
    }

#ifdef DEBUGING
    delay(1000);
#endif

    Tank.RunBackward();
    return;  // ПОчати новий loop()
  }
  // прямо
  if (distance > DST_TRH_TURN) {
    Tank.RunForward();
  } else {
    Tank.Stop();
    randomSeed(millis());
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

  // Під'їздимо до стіни під кутом.
  // Ультрасонік бачить об'єкутів під кутом
  // повертаємо доки не зникне перешкода
  // if (1)
  {
    const long nodelay{2000};
    if (!digitalRead(LEFT_BORT_PIN)) {
      Tank.TurnBackRight();
      long wait{millis() + nodelay};
      while (!digitalRead(LEFT_BORT_PIN))
        if (wait < millis()) break;
      Tank.RunForward();
    }
    if (!digitalRead(RIGHT_BORT_PIN)) {
      Tank.TurnBackLeft();
      long wait{millis() + nodelay};
      while (!digitalRead(RIGHT_BORT_PIN))
        if (wait < millis()) break;
      Tank.RunForward();
    }
  }
}