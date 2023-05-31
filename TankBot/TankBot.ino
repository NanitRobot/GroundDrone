#include <NanitLib.h>


#define DEBUGING 1

/*
 * Види поворотів
 */

const byte                   //
    MOTOR_ROTATE_RIGHT = 0,  //  розворот праворуч на місці
    MOTOR_TURN_RIGHT = 1,    // плавний поворот праворуч
    MOTOR_ROTATE_LEFT = 2,  // розворот ліоруч на місці
    MOTOR_TURN_LEFT = 3,        //  плавний поворот ліпоруч
    MOTOR_TURN_BACK_RIGHT = 4,  // поворот праворуч заднім ходом
    MOTOR_TURN_BACK_LEFT = 5;  // поворот ліворуч заднім ходом


/* 
Затримки для розворотів потрібно підібрати
 */
const int DELAY_RUN = 2;
const int DELAY_RUN_BACK = 500;
const int DELAY_ROTATE = 500;
const int DELAY_TURN = 500;
const int DELAY_TURN_BACK = 500;

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


class Bot {
 public:
  void Init()  ///< Ініціалізація моторів
  {
    pinMode(MOTOR1_A, OUTPUT);
    pinMode(MOTOR1_B, OUTPUT);
    pinMode(MOTOR2_A, OUTPUT);
    pinMode(MOTOR2_B, OUTPUT);
    pinMode(MOTOR_FAIL, INPUT);
    pinMode(MOTOR_ENABLE, OUTPUT);
    // pinMode(MOTOR_ENABLE,HIGH);
  };
  void RunForward()  ///< Рухаємось вперед прямо
  {
    //     if (debug > 1) Serial.println("Forward");
    // if (debug > 5) return;
    digitalWrite(MOTOR_ENABLE, HIGH);
    setSpeeds(_speed, _speed);
    delay(DELAY_RUN);
  };
  void RunBackward()  ///< Назад прямо
  {
    digitalWrite(MOTOR_ENABLE, HIGH);
    setSpeeds(-_speed, -_speed);
    delay(DELAY_RUN);
  };
  void RotateRight()  ///< Правий розворот на місці
  {
    _prev_direction = MOTOR_ROTATE_RIGHT;
    digitalWrite(MOTOR_ENABLE, HIGH);
    setSpeeds(_speed, -_speed);
    delay(DELAY_ROTATE);
  };
  void TurnRight() ///< Правий поворт
  
   {
    _prev_direction = MOTOR_TURN_RIGHT;
    digitalWrite(MOTOR_ENABLE, HIGH);
    setSpeeds(_speed, 0);
    delay(DELAY_TURN);
  };
  void TurnBackRight() ///< Правий поворт заднім ходом
   {
    _prev_direction = MOTOR_TURN_BACK_RIGHT;
    digitalWrite(MOTOR_ENABLE, HIGH);
    setSpeeds(0, _speed);
    delay(DELAY_TURN);
  }
  void RotateLeft() ///< Лівий розворот на місці
  {
    _prev_direction = MOTOR_ROTATE_LEFT;
    digitalWrite(MOTOR_ENABLE, HIGH);
    setSpeeds(-_speed, _speed);
    delay(DELAY_ROTATE);
  }
  void TurnLeft()///< Лівий поворот
   {
    _prev_direction = MOTOR_TURN_LEFT;
    digitalWrite(MOTOR_ENABLE, HIGH);
    setSpeeds(0, _speed);
    delay(DELAY_ROTATE);
  }
  void TurnBackLeft()///< Лівий поворт заднім ходом
   {
    _prev_direction = MOTOR_TURN_BACK_LEFT;
    digitalWrite(MOTOR_ENABLE, HIGH);
    setSpeeds(-_speed, 0);
    delay(DELAY_ROTATE);
  }
  void Stop()  ///< Зупика
  {
    digitalWrite(MOTOR_ENABLE, LOW);
    setSpeeds(0, 0);
  }
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
  void setSpeed(byte value) {
    //     // скорость мотора 0--255
    // if (speed > 255) speed = 255;
    // if (speed < 0) speed = 0;
    // if (debug) {
    //   Serial.print("Motor set Speed = ");
    //   Serial.println(speed);
    // }
    // if (debug > 5) return;
    // motorFrontLeft.setSpeed(speed);
    // motorFrontRight.setSpeed(speed);
    // motorRearLeft.setSpeed(speed);
    // motorRearRight.setSpeed(speed);
    // // запоминаем текущую скорость

    _speed = value;
  }
  byte getPrevDirection() { return _prev_direction; }

 private:
  void setSpeeds(short LeftSpeed, short RightSpeed) {
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
  byte                  //
      _curent_speed,    //
      _prev_direction;  //
  short                 //
      _speed{150};      //

} Tank;

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