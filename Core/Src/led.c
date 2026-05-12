#include "led.h"

#define DEFAULT_BLINK_INTERVAL_MS 100U
#define SLOW_BLINK_INTERVAL_MS 200U
#define FAST_BLINK_INTERVAL_MS 50U

static uint32_t blink_interval_ms;
static uint32_t blink_counter_ms;
static LedMode current_mode;

void Led_Init() {
	current_mode = LED_OFF;
	blink_interval_ms = DEFAULT_BLINK_INTERVAL_MS;
	blink_counter_ms = 0;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

}

void Led_SetMode(LedMode mode) {
	current_mode = mode;
	blink_counter_ms = 0;
	switch (mode) {
	case LED_OFF:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		break;
	case LED_ON:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		break;
	case LED_BLINK_SLOW:
		blink_interval_ms = SLOW_BLINK_INTERVAL_MS;
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		break;
	case LED_BLINK_FAST:
		blink_interval_ms = FAST_BLINK_INTERVAL_MS;
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		break;
	default:
		current_mode = LED_OFF;
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		break;
	}

}

void Led_NextMode() {
	LedMode mode = (current_mode + 1) % LED_MODE_COUNT;
	Led_SetMode(mode);
}

LedMode Led_GetMode() {
	return current_mode;
}

void Led_SetBlinkInterval(uint32_t interval_ms) {
	blink_interval_ms = interval_ms;
	blink_counter_ms = 0;
}

uint32_t Led_GetBlinkInterval() {
	return blink_interval_ms;
}

void Led_TimerTick() {
	if (current_mode != LED_BLINK_SLOW && current_mode != LED_BLINK_FAST) {
		return;
	}

	blink_counter_ms += 50;
	if (blink_counter_ms >= blink_interval_ms) {
		blink_counter_ms = 0;
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}
}

uint32_t Led_GetBlinkCounterMs() {
	return blink_counter_ms;
}

const char* Led_ModeToString(LedMode mode) {
	switch (mode) {
	case LED_OFF:
		return "LED OFF";
	case LED_ON:
		return "LED ON";
	case LED_BLINK_SLOW:
		return "LED BLINK SLOW";
	case LED_BLINK_FAST:
		return "LED BLINK FAST";
	default:
		return "UNKNOWN";
	}
}
