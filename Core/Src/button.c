#include "button.h"

#define BOUNCE_MS 10

static GPIO_PinState stable_state;
static GPIO_PinState prev_state;
static uint32_t state_change_t;
static bool pressed_event;

void Button_Init() {
	stable_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
	prev_state = stable_state;
	state_change_t = HAL_GetTick();
	pressed_event = false;
}

void Button_Update() {
	GPIO_PinState cur_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
	if (cur_state != prev_state) {
		state_change_t = HAL_GetTick();
		prev_state = cur_state;
	}

	if (HAL_GetTick() - state_change_t >= BOUNCE_MS) {
		if (stable_state != cur_state) {
			stable_state = cur_state;
			if (stable_state == GPIO_PIN_RESET) {
				pressed_event = true;
			}
		}
	}
}

bool Button_WasPressed() {
	if (pressed_event) {
		pressed_event = false;
		return true;
	}
	return false;
}

GPIO_PinState Button_GetStableState() {
	return stable_state;
}
