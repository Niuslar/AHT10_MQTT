
#include "aht10.h"
#include <stdint.h>
#include <math.h>

static float read_sensor(uint8_t temp_or_hum);
static void i2c_master_transmission(uint8_t send_or_rec, uint8_t *buf, uint8_t len, uint8_t addr);
static void i2c_master_receive(uint8_t *rx_buf, uint8_t len, uint8_t addr);
static void i2c_master_send(uint8_t *data, uint8_t len, uint8_t addr);

//Commands to trigger measurement 
uint8_t trig_cmd[3] = {AHT10_TRIG_MEAS, AHT10_DAT1_CMD, AHT10_DAT2_CMD};
//Calibration commands
uint8_t calib_cmd[3] = {0xE1, 0x08, 0x00};

int i2c_master_port = 0;

uint8_t data_len;

//Set I2C configuration (pins, mode, speed)

i2c_config_t conf = {
    .mode = I2C_MODE_MASTER,
    .sda_io_num = I2C_MASTER_SDA_IO,         // SDA = GPIO 21
    .sda_pullup_en = GPIO_PULLUP_ENABLE,
    .scl_io_num = I2C_MASTER_SCL_IO,         // SCL = GPIO 22
    .scl_pullup_en = GPIO_PULLUP_ENABLE,
    .master.clk_speed = I2C_MASTER_FREQ_HZ,  // 100kHz 
};


void aht10_init()
{

    //Config
    i2c_param_config(i2c_master_port, &conf);

    //Install I2C Driver 
    i2c_driver_install(i2c_master_port, conf.mode, 0, 0, 0);
    
    //Calibrate 
    data_len = 3;
    i2c_master_send(calib_cmd, data_len, AHT10_ADDR);

}

float read_humidity()
{
    return read_sensor(HUMIDITY);
}

float read_temperature()
{
    return read_sensor(TEMPERATURE);
}

static float read_sensor(uint8_t temp_or_hum)
{
        uint8_t rx_buf[6];
        data_len = 3;
        i2c_master_send(trig_cmd, data_len, AHT10_ADDR);
        
        //The sensor needs 75 ms before reading the data
        vTaskDelay(75 / portTICK_PERIOD_MS);
        
        data_len = 6;
        i2c_master_receive(rx_buf, data_len, AHT10_ADDR);
        
        //check if the device is busy and try to read data again
        
        while(rx_buf[0] & (1 << 7))
        {
            i2c_master_receive(rx_buf, data_len, AHT10_ADDR);
        }            
                
        uint32_t humidity_reading = ((uint32_t)rx_buf[1] << 12) | ((uint16_t)rx_buf[2] << 4) | (rx_buf[3] >> 4) ;
        
        uint32_t temp_reading = (((uint32_t)rx_buf[3] & 0x0F) << 16) | ((uint16_t)rx_buf[4] << 8) | (rx_buf[5]) ;
        
        float humidity = (((float)humidity_reading) / (pow(2,20))) * 100;
        
        float temperature = ((((float)temp_reading) / (pow(2,20))) * 200) - 50;
        
        if(temp_or_hum == HUMIDITY)
        {
            return humidity;
        }
        else if(temp_or_hum == TEMPERATURE)
        {
            return temperature;
        }
        
        return 999.0f; //return unrealistic value 
        
}

static void i2c_master_send(uint8_t *data, uint8_t len, uint8_t addr)
{
    i2c_master_transmission(SEND, data, len, addr);
}

static void i2c_master_receive(uint8_t *rx_buf, uint8_t len, uint8_t addr)
{
    i2c_master_transmission(RECEIVE, rx_buf, len, addr);
}

static void i2c_master_transmission(uint8_t send_or_rec, uint8_t *buf, uint8_t len, uint8_t addr)
{
    //Create command link 
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    //Populate cmd
    i2c_master_start(cmd);
    if(send_or_rec == SEND)
    {
        i2c_master_write_byte(cmd, addr, 0);
        i2c_master_write(cmd, buf, len, 0);
    }
    else
    {
        i2c_master_write_byte(cmd, (addr|1), 0);
        i2c_master_read(cmd, buf, len, 0);
    }
    i2c_master_stop(cmd);

    //Send commands and then destroy commands link
    i2c_master_cmd_begin(i2c_master_port, cmd, 1000000);
    i2c_cmd_link_delete(cmd);
}

