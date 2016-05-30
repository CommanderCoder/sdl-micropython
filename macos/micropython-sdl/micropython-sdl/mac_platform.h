//
//  mac_platform.h
//  micropython-sdl
//
//  Created by Everytale on 29/05/2016.
//  Copyright © 2016 Everytale. All rights reserved.
//

#ifndef mac_platform_h
#define mac_platform_h


#include <unistd.h>
#include <stdint.h>

struct timer_wait {
    unsigned int usec;
    unsigned int trigger_value;
    int          rollover;
};

void register_timer(struct timer_wait * tw, unsigned int usec);
int compare_timer(struct timer_wait * tw);

#endif /* mac_platform_h */
