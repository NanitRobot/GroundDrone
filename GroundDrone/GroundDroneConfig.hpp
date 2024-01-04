/**
 * @file GroundDroneConfig.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-06-12
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef _GROND_DRONE_CONFIG_HPP_
#define _GROND_DRONE_CONFIG_HPP_

/* пины для подключения HC-SR04 Ultrasonic Module Distance Measuring
 * 13, 2 цифровые пины
 * 14, 15 аналоговые пины A0 и A1 соответственно
 */

/* Закоментувати якщо використовуются пасивний базер*/
#define USED_ACTIVE_BAZZER

// PORT 1
#define GAS_SENSOR_PIN   (P1_1)  ///< Використовується АЦП
#define LIGHT_SENOSR_PIN (P1_2)  ///< Використовується АЦП
// PORT 4
#define SERVO_MOTOR_PIN (P4_1)///< ШІМ модуляція для рівня яскравості
#define RED_LED_PIN     (P4_2)///< ШІМ модуляція для рівня яскравості
#define GREEN_LED_PIN   (P4_3)///< ШІМ модуляція для рівня яскравості
#define BLUE_LED_PIN    (P4_4)///< ШІМ модуляція для повороту механізму
// PORT 6
#define SONIC_PIN_ECHO (P6_1)
#define SONIC_PIN_TRIG (P6_4)
#define LEFT_BORT_PIN  (P6_2)
#define RIGHT_BORT_PIN (P6_3)
// PORT 9
#define BUZZ_PIN       (P9_4) ///< ШИМ для базера
#define PIR_SENSOR_PIN (P9_3)  ///< INT 4 переривання для дачика PIR
// PORT 12
#define HUMIDITY_SENSOR_PIN (P12_1) ///< Датчик вологості та температури
#define ENABLE_DRV_PIN      (P12_2) ///< Увімкнути зовнішній драйвер двигуна

#endif