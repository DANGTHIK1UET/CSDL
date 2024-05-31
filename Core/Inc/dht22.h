#ifndef __DHT_H
#define __DHT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"


#define DHT22_PORT GPIOB
#define DHT22_PIN GPIO_PIN_9
uint8_t RH1, RH2, TC1, TC2, SUM, CHECK;
uint32_t pMillis, cMillis;
float tCelsius = 0;
float tFahrenheit = 0;
float RH = 0;

uint8_t DHT22_Read (void);
void microDelay (uint16_t delay);
uint8_t DHT22_Start (void);

#ifdef __cplusplus

}
#endif

#endif /* __DHT_H */