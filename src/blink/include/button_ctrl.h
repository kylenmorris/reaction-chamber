
#ifndef BUTTON_CTRL_H
#define BUTTON_CTRL_H

void button_init();
void button_step();
void init_gpio_button(int gpio_pin);
int debounceButton(int i, int reading);

#endif
