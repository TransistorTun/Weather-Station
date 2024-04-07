#ifndef DHT_H
#define DHT_H

#include <stdint.h>
#include <stdbool.h>
#include "driver/gpio.h"
#include "esp_err.h"

enum dht11_status
{
    DHT_CRC_ERROR = -1,
    DHT_TIMEOUT_ERROR,
    DHT_OK
};

typedef struct {
    float temperature; 
    float humidity;    
} dht11_data_t;


esp_err_t DHT11_Init(dht11_data_t *dht11_data, gpio_num_t io_pin);
uint8_t DHT11_Get_Data(dht11_data_t *dht11_data);

#endif /* DHT_H */