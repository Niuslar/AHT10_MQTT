#ifndef AHT10_H
#define AHT10_H

#include "driver/i2c.h"


#define AHT10_ADDR (0x38<<1)

//commands 
#define AHT10_INIT_CMD     0xE1 
#define AHT10_SOFT_RESET   0xBA
#define AHT10_TRIG_MEAS    0xAC
#define AHT10_DAT1_CMD     0x33
#define AHT10_DAT2_CMD     0x00

//General 
#define HUMIDITY    0
#define TEMPERATURE 1
#define SEND        0
#define RECEIVE     1

//I2C Config
#define I2C_MASTER_SDA_IO  21
#define I2C_MASTER_SCL_IO  22
#define I2C_MASTER_FREQ_HZ 100000

//APIs 
void aht10_init();
float read_temperature();
float read_humidity();

#endif /*AHT10_H*/