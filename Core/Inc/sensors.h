#ifndef SENSORS_H
#define SENSORS_H

#include "main.h"
#include <stdbool.h>

extern struct SensorCodes sensorCode;

typedef struct
{
    uint32_t timestamp;
    float ax_g;
    float ay_g;
    float az_g;
    float dps_x;
    float dps_y;
    float dps_z;
} LSM6DSV16X_Sample;

bool Sensors_Init();
bool LSM6DSV16X_Read_Ready();
uint8_t SHT40_CRC(uint8_t *data);
void CALC_SHT40_AD1B_CRC(uint8_t *data, uint8_t *crc, uint8_t num_bytes);
void Sensor_StatusToString(struct SensorCodes *sensorCode);
LSM6DSV16X_Sample *LSM6DSV16X_ReadData();

#endif
