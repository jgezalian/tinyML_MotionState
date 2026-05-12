#include "uart_cmd.h"
#include "led.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart2;

static uint8_t rx_byte;
static volatile uint8_t pending_cmd;
static volatile bool cmd_ready;

void UartCmd_Init() {
	rx_byte = 0;
	pending_cmd = 0;
	cmd_ready = false;
	HAL_UART_Receive_IT(&huart2, &rx_byte, 1);
}

void UartCmd_RxCompleteCallback(UART_HandleTypeDef *huart) {
	pending_cmd = rx_byte;
	cmd_ready = true;
	HAL_UART_Receive_IT(&huart2, &rx_byte, 1);
}

void UartCmd_ProcessPending(void) {
	if (!cmd_ready) {
		return;
	}
	uint8_t cmd = pending_cmd;
	cmd_ready = false;
	switch (cmd) {
	case '0':
		Led_SetMode(LED_OFF);
		UartCmd_PrintStatus();
		break;
	case '1':
		Led_SetMode(LED_ON);
		UartCmd_PrintStatus();
		break;
	case '2':
		Led_SetMode(LED_BLINK_SLOW);
		UartCmd_PrintStatus();
		break;
	case '3':
		Led_SetMode(LED_BLINK_FAST);
		UartCmd_PrintStatus();
		break;
	case 'f':
		uint32_t cur_blink_interval = Led_GetBlinkInterval();
		if (cur_blink_interval > 50)
			Led_SetBlinkInterval(cur_blink_interval - 50);
		UartCmd_PrintStatus();
		break;
	case 's':
		Led_SetBlinkInterval(Led_GetBlinkInterval() + 50);
		UartCmd_PrintStatus();
		break;
	case 'p':
		UartCmd_PrintStatus();
		break;
	case '?':
	case 'h':
		UartCmd_PrintHelp();
		break;
	case '\r':
	case '\n':
		break;
	default:
		UartCmd_PrintHelp();
		break;
	}
}

void UartCmd_PrintStatus() {
	char tx_buffer[96];
	int n = snprintf(tx_buffer, sizeof(tx_buffer),
			"mode: %s, blink_interval: %lu\r\n", Led_ModeToString(Led_GetMode()),
			Led_GetBlinkInterval());
	HAL_UART_Transmit(&huart2, (uint8_t*) tx_buffer, (uint16_t) n, 100);
}

void UartCmd_PrintHelp(void) {
	const char *help = "\r\nCommands: \r\n"
			"0: LED off\r\n"
			"1: LED on\r\n"
			"2: LED slow blink\r\n"
			"3: Led fast blink\r\n"
			"f: Increase blink rate\r\n"
			"s: Decrease blink rate\r\n"
			"p: Print status\r\n"
			"h/?: help\r\n";

	HAL_UART_Transmit(&huart2, (uint8_t*) help, (uint16_t) strlen(help), 100);
}
