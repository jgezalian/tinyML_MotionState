#ifndef LED_H
#define LED_H

#include <stdint.h>
#include "main.h"

typedef enum {
	LED_OFF, LED_ON, LED_BLINK_SLOW, LED_BLINK_FAST, LED_MODE_COUNT = 4
} LedMode;

void Led_Init();
void Led_SetMode(LedMode mode);
void Led_NextMode();
LedMode Led_GetMode();
void Led_SetBlinkInterval(uint32_t interval_ms);
uint32_t Led_GetBlinkInterval();
void Led_TimerTick();
const char *Led_ModeToString(LedMode mode);
uint32_t Led_GetBlinkCounterMs();

#endif
