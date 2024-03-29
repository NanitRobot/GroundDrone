/**
 * @file GroundDrone.ino
 * @author Sam4uk
 * @brief
 * @version 0.1
 * @date 2023-05-31
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <Servo.h>

#include "GroundDrone.hpp"
#include "GroundDroneConfig.hpp"

#define DEBUGING 1

// в сантиметрах (distance threshold) Пороги расстояний до препятствия
const int  //
           // Если ближе, то резкий разворот на месте, иначе плавный доворот
    DST_TRH_TURN = 28,  //
    // Если ближе, то стоп и назад
    DST_TRH_BACK = 15;

// Detection distance: 2cm--450cm
const int  //
    TURN_DELAY = 152,          //
    SONIC_DISTANCE_MAX = 450,  //
    SONIC_DISTANCE_MIN = 2,    //
    DroneSpeed = 120,          ///< Швидкість дрона
    HuckClosed = 160,          ///< Гак закрито
    HuckOpened = 0,            ///< Гак відкрито
    MovingTime = 15000,  ///< Час активного пошуку в мілісекундах
    JobTime = 15000  ///< Час на виеонання завдання
    ;
GroundDrone    //
    Drone;  ///< Drone
Servo      //
    Huck;  ///< Виконавчий механізм

bool detected{false}, PIRDetected{};

bool                  //
    IsActive{false},  ///< Прапор активності машинки в режимі пошуку
    IsArmed{false};  ///< Заряджено. Виконання завдання
long                 //
    ListenTime,  ///< Змінна рахунку часу виконнання завдання
    ActiveTime{millis()};  ///< Змінна рахунку часу пошуку

void Moving();  ///< Оголошення функції яка описує алгоритм руху дрона
void Drop();  ///< Оголошення функції яка виконує завдання

#if NOT_USED_CODE
void PIR_INT() {
  if (ActiveTime > millis())
    if (IsArmed and !IsActive) PIRDetected = true;
};
#endif

void setup()  ///< Функція налаштувань дрона
{
  Nanit_Base_Start();
  Drone.Init();  ///< Об'єкт танк

#ifdef DEBUGING
  Serial.begin(9600);  // set up Serial library at 9600 bps
#endif

  // Піни ульразвукового далекоміру
  pinMode(SONIC_PIN_TRIG, OUTPUT);
  pinMode(SONIC_PIN_ECHO, INPUT);
  // Піни дачиків "стіни" на бортах
  pinMode(LEFT_BORT_PIN, INPUT);
  pinMode(RIGHT_BORT_PIN, INPUT);
  // "кнопка"
  pinMode(LIGHT_SENOSR_PIN, INPUT);
  // Детектор руху
  pinMode(PIR_SENSOR_PIN, INPUT);
  // Базер
  pinMode(BUZZ_PIN, OUTPUT);
#ifdef USED_ACTIVE_BAZZER
  // код для пасивного базера
  digitalWrite(BUZZ_PIN, HIGH);
#endif

#if DEBUGING
  delay(3000);
#endif
  // attachInterrupt(digitalPinToInterrupt(PIR_SENSOR_PIN), PIR_INT, CHANGE);
  Huck.attach(P1_1);
  Huck.write(HuckClosed);
  Drone.setSpeed(DroneSpeed);
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

void loop() {
#ifdef DEBUGING
  Serial.println("\n*** new loop() start ***\n");
#endif

  // Активація бота в режим об'їду
  if (!digitalRead(LEFT_BORT_PIN) and !digitalRead(RIGHT_BORT_PIN)) {
    Drone.RunBackward();
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
      ActiveTime = MovingTime + millis();
      Huck.write(HuckClosed);
    }
    IsArmed = true;
  }

  digitalWrite(BUZZ_PIN, HIGH);

  /* if (IsArmed and !IsActive and IsActive < millis()) {
     //скид
   }*/
  if (IsActive) {
    PIRDetected = false;
    Moving();
    if (IsArmed and (millis() > ActiveTime)) {
      IsActive = false;
      ListenTime = JobTime + millis();
    }

  } else if (IsArmed) {
    Drone.Stop();
    if (millis() > ListenTime) {
      IsActive = true;
      ActiveTime = MovingTime + millis();
    }
    if (digitalRead(PIR_SENSOR_PIN)) {
      Drop();
      Drone.setSpeed(0xFF);
      for (;;) Moving();
    }
  };
}

void Moving() {
  int                                //
      distance = measureDistance();  // Отримуємо дистанцію до перешкоди

#ifdef DEBUGING
  Serial.print("distance = ");
  Serial.println(distance);
#endif
  // Drone.setSpeed(map(distance,150,255,SONIC_DISTANCE_MIN,SONIC_DISTANCE_MAX));
  // Вимірювання відстані
  if (distance <= DST_TRH_BACK)  // Якщо дуже близько портібно їхати назад
  {
#ifdef DEBUGING
    Serial.println("ALARM! Distance too small!!!");
#endif
    Drone.Stop();  // Зупиняємось
#ifdef DEBUGING
    delay(1000);
#endif

    if (Drone.getPrevDirection() == MOTOR_TURN_BACK_LEFT) {
      Drone.RotateRight();
    } else {
      Drone.RotateLeft();
    }

    // Раніше повертали заднім ходом на ліво?
    if (Drone.getPrevDirection() == MOTOR_TURN_BACK_LEFT) {
      Drone.RotateRight();
    } else {
      Drone.RotateLeft();
    }

#ifdef DEBUGING
    delay(1000);
#endif

    Drone.RunBackward();
    return;  // ПОчати новий loop()
  }
  // прямо
  if (distance > DST_TRH_TURN) {
    Drone.RunForward();
  } else {
    Drone.Stop();
    randomSeed(millis());
    if (random(1, 10) < 5) {
#ifdef DEBUGING
      delay(500);
#endif
      Drone.TurnLeft();
    } else {
#ifdef DEBUGING
      delay(500);
#endif
      Drone.TurnRight();
    }
  }

  // Під'їздимо до стіни під кутом.
  // Ультрасонік бачить об'єкутів під кутом
  // повертаємо доки не зникне перешкода
  {
    // Визначаємо час на виконання повороту
    const long nodelay{2000};
    // Якщо підїхали бортом до стіни
    if (!digitalRead(LEFT_BORT_PIN)) {
      // трохи повертаємо
      Drone.TurnBackRight();
      long wait{millis() + nodelay};
      // повертаємо доки не відїдемо від стіни
      while (!digitalRead(LEFT_BORT_PIN))
        // якщо поворот не вдалий протягом визначеного часу
        if (wait < millis())
          // забуксували
          // припиняємо поворот
          break;
      // їдемо далі
      Drone.RunForward();
    }
    // теж заме для правого борту
    if (!digitalRead(RIGHT_BORT_PIN)) {
      Drone.TurnBackLeft();
      long wait{millis() + nodelay};
      while (!digitalRead(RIGHT_BORT_PIN))
        if (wait < millis()) break;
      Drone.RunForward();
    }
  }
}

void Drop() {
  /// Виконуємо корисну роботу
  Huck.write(HuckOpened);
  // Затримка на відкриття
  delay(100);
}