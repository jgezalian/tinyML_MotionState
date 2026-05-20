#ifndef SENSORS_H
#define SENSORS_H

#include "main.h"
#include <stdbool.h>

extern struct SensorCodes sensorCode;

bool Sensors_Init();
bool Sensors_Read_Ready();
uint8_t SHT40_CRC(uint8_t *data);
void CALC_SHT40_AD1B_CRC(uint8_t *data, uint8_t *crc, uint8_t num_bytes);
void Sensor_StatusToString(struct SensorCodes *sensorCode);
float *LSM6DSV16X_ReadData();

#endif
