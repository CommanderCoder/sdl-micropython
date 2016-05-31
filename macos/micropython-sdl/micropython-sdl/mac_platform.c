//
//  mac_platform.c
//  micropython-sdl
//
//  Created by Everytale on 29/05/2016.
//  Copyright © 2016 Everytale. All rights reserved.
//

#include "mac_platform.h"


#include <unistd.h>



void register_timer(struct timer_wait * tw, unsigned int usec) {
//    unsigned int cur_timer = mmio_read(TIMER_CLO);
//    
//    tw->usec = usec;
//    tw->rollover = 0;
//    tw->trigger_value = 0;
//    
//    if (usec > 0) {
//        tw->trigger_value = cur_timer + usec;
//        if (tw->trigger_value > cur_timer)
//            tw->rollover = 0;
//        else
//            tw->rollover = 1;
//    }
}

int compare_timer(struct timer_wait * tw) {
//    unsigned int cur_timer = mmio_read(TIMER_CLO);
//    
//    if (cur_timer < tw->trigger_value) {
//        if (tw->rollover)
//            tw->rollover = 0;
//    } else if (!tw->rollover) {
//        if (tw->usec > 0) {
//            tw->trigger_value = cur_timer + tw->usec;
//            if (tw->trigger_value > cur_timer)
//                tw->rollover = 0;
//            else
//                tw->rollover = 1;
//        }
//        return 1;
//    }
    
    return 0;
}
