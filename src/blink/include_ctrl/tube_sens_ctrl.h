#ifndef TUBE_SENS_CTRL_H
#define TUBE_SENS_CTRL_H

void tube_sens_ctrl_init(void);

// Detects the insertion of tubes and might do other stuff later
void tube_sens_ctrl_step(void);

#endif