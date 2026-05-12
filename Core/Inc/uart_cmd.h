#ifndef UART_CMD_H
#define UART_CMD_H

#include "main.h"

void UartCmd_Init(void);
void UartCmd_RxCompleteCallback(UART_HandleTypeDef *huart);
void UartCmd_ProcessPending(void);
void UartCmd_PrintStatus(void);
void UartCmd_PrintHelp(void);

#endif /* UART_CMD_H */
