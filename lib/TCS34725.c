#include "tcs34725.h"
#include "i2c.h"
//#include <math.h>

// -----------------------------------------------------------------------------
// Low-level I2C helpers using your _I2C1_* functions
// -----------------------------------------------------------------------------
static void _TCS34725_Write8(uint8_t addr, uint8_t reg, uint8_t val)
{
    uint8_t data[2] = { TCS34725_COMMAND_BIT | reg, val };
    _I2C1_StartWrite(addr, 2, data, _I2C_AutoEnd_Hardware);
}

static uint8_t _TCS34725_Read8(uint8_t addr, uint8_t reg)
{
    uint8_t cmd = TCS34725_COMMAND_BIT | reg;
    uint8_t val;
    _I2C1_StartWrite(addr, 1, &cmd, _I2C_AutoEnd_Software);
    _I2C1_StartRead(addr, 1, &val, _I2C_AutoEnd_Hardware);
    return val;
}

static uint16_t _TCS34725_Read16(uint8_t addr, uint8_t reg)
{
    uint8_t cmd = TCS34725_COMMAND_BIT | reg;
    uint8_t buf[2];
    _I2C1_StartWrite(addr, 1, &cmd, _I2C_AutoEnd_Software);
    _I2C1_StartRead(addr, 2, buf, _I2C_AutoEnd_Hardware);
    return (uint16_t)(buf[1] << 8 | buf[0]);
}

// -----------------------------------------------------------------------------
// Public functions
// -----------------------------------------------------------------------------
bool _TCS34725_Init(TCS34725_t *dev)
{
    dev->address = TCS34725_ADDRESS;
    dev->integrationTime = 0xEB; // default 50 ms
    dev->gain = 0x01;            // 4x gain

    uint8_t id = _TCS34725_Read8(dev->address, TCS34725_ID);
    if (id == 0x00 || id == 0xFF)
        return false;  // sensor not found

    dev->initialized = true;

    _TCS34725_Write8(dev->address, TCS34725_ATIME, dev->integrationTime);
    _TCS34725_Write8(dev->address, TCS34725_CONTROL, dev->gain);
    _TCS34725_Enable(dev);

    return true;
}

void _TCS34725_Enable(TCS34725_t *dev)
{
    _TCS34725_Write8(dev->address, TCS34725_ENABLE, TCS34725_ENABLE_PON);
    for (volatile int i = 0; i < 10000; i++); // ~3ms delay
    _TCS34725_Write8(dev->address, TCS34725_ENABLE, TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN);
}

void _TCS34725_GetRawData(TCS34725_t *dev, uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c)
{
    *c = _TCS34725_Read16(dev->address, TCS34725_CDATAL);
    *r = _TCS34725_Read16(dev->address, TCS34725_RDATAL);
    *g = _TCS34725_Read16(dev->address, TCS34725_GDATAL);
    *b = _TCS34725_Read16(dev->address, TCS34725_BDATAL);
}

uint16_t _TCS34725_CalculateLux(uint16_t r, uint16_t g, uint16_t b)
{
    float lux = (-0.32466f * r) + (1.57837f * g) + (-0.73191f * b);
    if (lux < 0) lux = 0;
    return (uint16_t)lux;
}

uint16_t _TCS34725_CalculateColorTemp(uint16_t r, uint16_t g, uint16_t b)
{
    float X = (-0.14282f * r) + (1.54924f * g) + (-0.95641f * b);
    float Y = (-0.32466f * r) + (1.57837f * g) + (-0.73191f * b);
    float Z = (-0.68202f * r) + (0.77073f * g) + (0.56332f * b);

    float xc = X / (X + Y + Z);
    float yc = Y / (X + Y + Z);
    float n = (xc - 0.3320f) / (0.1858f - yc);

    float cct = (449.0f * n * n * n) + (3525.0f * n * n) + (6823.3f * n) + 5520.33f;
    if (cct < 0) cct = 0;
    return (uint16_t)cct;
}
