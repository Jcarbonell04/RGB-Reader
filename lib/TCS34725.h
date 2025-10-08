/* USER CODE BEGIN 0 */
#include <stdint.h>

#define TCS34725_ADDRESS          (0x29 << 1) /* I2C address */
/* Datasheet is at https://cdn-shop.adafruit.com/datasheets/TCS34725.pdf */
#define TCS34725_COMMAND_BIT      (0x80)      /* Command bit */
#define TCS34725_ENABLE           (0x00)      /* Enable register */
#define TCS34725_ENABLE_AEN       (0x02)      /* RGBC Enable */
#define TCS34725_ENABLE_PON       (0x01)      /* Power on */
#define TCS34725_ATIME            (0x01)      /* Integration time */
#define TCS34725_CONTROL          (0x0F)      /* Set the gain level */
#define TCS34725_ID               (0x12)
/* 0x44 = TCS34721/TCS34725, 0x4D = TCS34723/TCS34727 */
#define TCS34725_CDATAL           (0x14)      /* Clear channel data */
#define TCS34725_CDATAH           (0x15)
#define TCS34725_RDATAL           (0x16)      /* Red channel data */
#define TCS34725_RDATAH           (0x17)
#define TCS34725_GDATAL           (0x18)      /* Green channel data */
#define TCS34725_GDATAH           (0x19)
#define TCS34725_BDATAL           (0x1A)      /* Blue channel data */
#define TCS34725_BDATAH           (0x1B)

/* set the correct delay time in void getRawData() for TCS34725_INTEGRATIONTIME */
#define TCS34725_INTEGRATIONTIME_50MS   0xEB  /* 50ms  - 20 cycles */
#define TCS34725_GAIN_4X                0x01  /* 4x gain  */

uint8_t _tcs34725Initialised = 0;
int red, green, blue;

void write8 (uint8_t reg, uint32_t value);
uint8_t read8(uint8_t reg);
uint16_t read16(uint8_t reg);
void enable(void);
void disable(void);
void setIntegrationTime(uint8_t it);
void setGain(uint8_t gain);
void tcs3272_init( void );
void getRawData (uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c);
void getRGB(int *R, int *G, int *B);

// write functions
/* Writes a register and an 8 bit value over I2C */
void write8 (uint8_t reg, uint32_t value)
{
    uint8_t txBuffer[2];
    txBuffer[0] = (TCS34725_COMMAND_BIT | reg);
    txBuffer[1] = (value & 0xFF);
    //HAL_I2C_Master_Transmit(&hi2c1, TCS34725_ADDRESS, txBuffer, 2, 100);
}




// oldie

//#ifndef TCS34725_H
//#define TCS34725_H

//#include "stm32g031xx.h" 

//// Sensor I2C address
//#define TCS34725_ADDRESS          (0x29 << 1)

//// Register addresses and settings
//#define TCS34725_COMMAND_BIT      (0x80)
//#define TCS34725_ENABLE           (0x00)
//#define TCS34725_ENABLE_AEN       (0x02)
//#define TCS34725_ENABLE_PON       (0x01)
//#define TCS34725_ATIME            (0x01)
//#define TCS34725_CONTROL          (0x0F)
//#define TCS34725_ID               (0x12)

//#define TCS34725_CDATAL           (0x14)
//#define TCS34725_CDATAH           (0x15)
//#define TCS34725_RDATAL           (0x16)
//#define TCS34725_RDATAH           (0x17)
//#define TCS34725_GDATAL           (0x18)
//#define TCS34725_GDATAH           (0x19)
//#define TCS34725_BDATAL           (0x1A)
//#define TCS34725_BDATAH           (0x1B)

//// Integration time
//#define TCS34725_INTEGRATIONTIME_50MS   0xEB
//#define TCS34725_GAIN_4X                0x01

//typedef struct {
//    I2C_HandleTypeDef *i2c;
//    uint8_t initialized;
//} TCS34725_HandleTypeDef;

//HAL_StatusTypeDef tcs34725_init(TCS34725_HandleTypeDef *sensor, I2C_HandleTypeDef *hi2c);
//void tcs34725_setIntegrationTime(TCS34725_HandleTypeDef *sensor, uint8_t time);
//void tcs34725_setGain(TCS34725_HandleTypeDef *sensor, uint8_t gain);
//void tcs34725_getRawData(TCS34725_HandleTypeDef *sensor, uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c);
//void tcs34725_getRGB(TCS34725_HandleTypeDef *sensor, int *r, int *g, int *b);

//#endif // TCS34725_H
