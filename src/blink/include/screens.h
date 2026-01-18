#ifndef SCREENS_H
#define SCREENS_H

void draw_idle_menu(void);
void draw_results_screen(void);
void draw_test_running_screen(void);
void draw_heating_screen(void);
void draw_results_history(void);

void print_to_display(int x, int y, char* char_ptr);

#endif