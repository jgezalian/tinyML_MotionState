#ifndef UART_CMD_H
#define UART_CMD_H

#include "main.h"
#include "sensors.h"

void UartCmd_Init(void);
void UartCmd_RxCompleteCallback(UART_HandleTypeDef *huart);
void UartCmd_ProcessPending(void);
void UartCmd_PrintGeneral(char *str);
void UartCmd_PrintStatus(void);
void UartCmd_PrintHelp(void);
void UartCmd_PrintSensorStatus(const char **SensorStatuses);
void UartCmd_PrintSensorInitOk();
void UartCmd_PrintSensorReadReady(char *sensor_name);
void UartCmd_PrintLSM6DSV16XData(float *data);
void UartCmd_PrintLSM6DSV16XDataCSV(const LSM6DSV16X_Sample *LSM6DSV16X_Sample);

#endif /* UART_CMD_H */
