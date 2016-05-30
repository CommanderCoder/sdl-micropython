//
//  mac_wiring.h
//  micropython-sdl
//
//  Created by Everytale on 29/05/2016.
//  Copyright © 2016 Everytale. All rights reserved.
//

#ifndef mac_wiring_h
#define mac_wiring_h

#define LOW                 0

void pinMode(int pin, int mode);
int  digitalRead(int pin);
void digitalWrite(int pin, int value);

#endif /* mac_wiring_h */
