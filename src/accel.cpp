#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "accel.h"

static constexpr int ADXL345_ADDRESS = 0x53;
static constexpr int ADXL345_POWER_CTL = 0x2D;
static constexpr int ADXL345_DATA_FORMAT = 0x31;
static constexpr int ADXL345_DATAX0 = 0x32;

void accelInit() {
    i2c_init(i2c1, 400 * 1000);
    gpio_set_function(10, GPIO_FUNC_I2C);
    gpio_set_function(11, GPIO_FUNC_I2C);
    gpio_pull_up(10);
    gpio_pull_up(11);
    uint8_t c[2] = {ADXL345_POWER_CTL, 0x08};
    i2c_write_blocking(i2c1, ADXL345_ADDRESS, c, 2, false);
    c[0] = ADXL345_DATA_FORMAT; c[1] = 0x0B;
    i2c_write_blocking(i2c1, ADXL345_ADDRESS, c, 2, false);
}

Point accelRead() {
    uint8_t buffer[6];
    buffer[0] = ADXL345_DATAX0;
    i2c_write_blocking(i2c1, ADXL345_ADDRESS, buffer, 1, true);
    i2c_read_blocking(i2c1, ADXL345_ADDRESS, buffer, 6, false);
    int16_t x = buffer[0] | (buffer[1] << 8);
    int16_t y = buffer[2] | (buffer[3] << 8);
    int16_t z = buffer[4] | (buffer[5] << 8);
    return {float(x) / 256.0f, float(y) / 256.0f, float(z) / 256.0f};
}
