#include "TCS34725.h"

//static void tcs34725_write8(TCS34725_HandleTypeDef *sensor, uint8_t reg, uint8_t value) {
//    uint8_t data[2] = { TCS34725_COMMAND_BIT | reg, value };
//    HAL_I2C_Master_Transmit(sensor->i2c, TCS34725_ADDRESS, data, 2, HAL_MAX_DELAY);
//}

//static uint8_t tcs34725_read8(TCS34725_HandleTypeDef *sensor, uint8_t reg) {
//    uint8_t tx = TCS34725_COMMAND_BIT | reg;
//    uint8_t rx;
//    HAL_I2C_Master_Transmit(sensor->i2c, TCS34725_ADDRESS, &tx, 1, HAL_MAX_DELAY);
//    HAL_I2C_Master_Receive(sensor->i2c, TCS34725_ADDRESS, &rx, 1, HAL_MAX_DELAY);
//    return rx;
//}

//static uint16_t tcs34725_read16(TCS34725_HandleTypeDef *sensor, uint8_t reg) {
//    uint8_t tx = TCS34725_COMMAND_BIT | reg;
//    uint8_t rx[2];
//    HAL_I2C_Master_Transmit(sensor->i2c, TCS34725_ADDRESS, &tx, 1, HAL_MAX_DELAY);
//    HAL_I2C_Master_Receive(sensor->i2c, TCS34725_ADDRESS, rx, 2, HAL_MAX_DELAY);
//    return (rx[1] << 8) | rx[0];
//}

//static void tcs34725_enable(TCS34725_HandleTypeDef *sensor) {
//    tcs34725_write8(sensor, TCS34725_ENABLE, TCS34725_ENABLE_PON);
//    HAL_Delay(3);
//    tcs34725_write8(sensor, TCS34725_ENABLE, TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN);
//    HAL_Delay(50);
//}

//static void tcs34725_disable(TCS34725_HandleTypeDef *sensor) {
//    uint8_t reg = tcs34725_read8(sensor, TCS34725_ENABLE);
//    tcs34725_write8(sensor, TCS34725_ENABLE, reg & ~(TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN));
//}

//HAL_StatusTypeDef tcs34725_init(TCS34725_HandleTypeDef *sensor, I2C_HandleTypeDef *hi2c) {
//    sensor->i2c = hi2c;
//    sensor->initialized = 0;

//    uint8_t id = tcs34725_read8(sensor, TCS34725_ID);
//    if (id != 0x44 && id != 0x10) {
//        return HAL_ERROR;
//    }

//    sensor->initialized = 1;
//    tcs34725_setIntegrationTime(sensor, TCS34725_INTEGRATIONTIME_50MS);
//    tcs34725_setGain(sensor, TCS34725_GAIN_4X);
//    tcs34725_enable(sensor);
//    return HAL_OK;
//}

//void tcs34725_setIntegrationTime(TCS34725_HandleTypeDef *sensor, uint8_t time) {
//    if (!sensor->initialized) return;
//    tcs34725_write8(sensor, TCS34725_ATIME, time);
//}

//void tcs34725_setGain(TCS34725_HandleTypeDef *sensor, uint8_t gain) {
//    if (!sensor->initialized) return;
//    tcs34725_write8(sensor, TCS34725_CONTROL, gain);
//}

//void tcs34725_getRawData(TCS34725_HandleTypeDef *sensor, uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c) {
//    if (!sensor->initialized) return;

//    *c = tcs34725_read16(sensor, TCS34725_*
