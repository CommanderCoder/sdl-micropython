//
//  raspi.h
//  micropython-sdl
//
//  Created by Everytale on 10/06/2016.
//  Copyright © 2016 Everytale. All rights reserved.
//

#ifndef raspi_h
#define raspi_h
#include <stdint.h>

int microbit_random(int max);
void microbit_seed_random(void);
void microbit_seed_random(uint32_t seed);
unsigned long system_timer_current_time(void);


#endif /* raspi_h */
