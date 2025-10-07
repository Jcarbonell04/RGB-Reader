#ifndef TCS34725_H
#define TCS34725_H

#include "stm32g031xx.h" 

// Sensor I2C address
#define TCS34725_ADDRESS          (0x29 << 1)

// Register addresses and settings
#define TCS34725_COMMAND_BIT      (0x80)
#define TCS34725_ENABLE           (0x00)
#define TCS34725_ENABLE_AEN       (0x02)
#define TCS34725_ENABLE_PON       (0x01)
#define TCS34725_ATIME            (0x01)
#define TCS34725_CONTROL          (0x0F)
#define TCS34725_ID               (0x12)

#define TCS34725_CDATAL           (0x14)
#define TCS34725_CDATAH           (0x15)
#define TCS34725_RDATAL           (0x16)
#define TCS34725_RDATAH           (0x17)
#define TCS34725_GDATAL           (0x18)
#define TCS34725_GDATAH           (0x19)
#define TCS34725_BDATAL           (0x1A)
#define TCS34725_BDATAH           (0x1B)

// Integration time
#define TCS34725_INTEGRATIONTIME_50MS   0xEB
#define TCS34725_GAIN_4X                0x01

typedef struct {
    I2C_HandleTypeDef *i2c;
    uint8_t initialized;
} TCS34725_HandleTypeDef;

HAL_StatusTypeDef tcs34725_init(TCS34725_HandleTypeDef *sensor, I2C_HandleTypeDef *hi2c);
void tcs34725_setIntegrationTime(TCS34725_HandleTypeDef *sensor, uint8_t time);
void tcs34725_setGain(TCS34725_HandleTypeDef *sensor, uint8_t gain);
void tcs34725_getRawData(TCS34725_HandleTypeDef *sensor, uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c);
void tcs34725_getRGB(TCS34725_HandleTypeDef *sensor, int *r, int *g, int *b);

#endif // TCS34725_H
