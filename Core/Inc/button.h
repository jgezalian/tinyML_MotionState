#ifndef BUTTON_H
#define BUTTON_H

#include <stdbool.h>
#include "main.h"

void Button_Init(void);
void Button_Update(void);
bool Button_WasPressed(void);
GPIO_PinState Button_GetStableState(void);

#endif
