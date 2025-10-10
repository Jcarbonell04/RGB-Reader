#ifndef TCS34725_H
#define TCS34725_H

#include <stdint.h>
#include <stdbool.h>

// I2C address
#define TCS34725_ADDRESS           0x29
#define TCS34725_COMMAND_BIT       0x80

// Registers
#define TCS34725_ENABLE            0x00
#define TCS34725_ATIME             0x01
#define TCS34725_CONTROL           0x0F
#define TCS34725_ID                0x12
#define TCS34725_CDATAL            0x14
#define TCS34725_RDATAL            0x16
#define TCS34725_GDATAL            0x18
#define TCS34725_BDATAL            0x1A

// Enable bits
#define TCS34725_ENABLE_AIEN       0x10
#define TCS34725_ENABLE_WEN        0x08
#define TCS34725_ENABLE_AEN        0x02
#define TCS34725_ENABLE_PON        0x01

typedef struct {
    uint8_t address;
    uint8_t integrationTime;
    uint8_t gain;
    bool initialized;
} TCS34725_t;

// API
bool _TCS34725_Init(TCS34725_t *dev);
void _TCS34725_Enable(TCS34725_t *dev);
void _TCS34725_GetRawData(TCS34725_t *dev, uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c);
uint16_t _TCS34725_CalculateLux(uint16_t r, uint16_t g, uint16_t b);
uint16_t _TCS34725_CalculateColorTemp(uint16_t r, uint16_t g, uint16_t b);

#endif




