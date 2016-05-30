//
//  mac_wiring.c
//  micropython-sdl
//
//  Created by Everytale on 29/05/2016.
//  Copyright © 2016 Everytale. All rights reserved.
//

#include "mac_wiring.h"

void pinMode(int pin, int mode) {
//    int fSel = gpioToGPFSEL[pin];
//    int shift = gpioToShift[pin];
//    
//    if (mode == OUTPUT) {
//        GPIO->gpfsel[fSel] = (GPIO->gpfsel[fSel] & ~(7 << shift)) | (FSEL_OUTP << shift);
//    }
//    else {
//        GPIO->gpfsel[fSel] = (GPIO->gpfsel[fSel] & ~(7 << shift)) | (FSEL_INPT << shift);
//        
//        if (mode == INPUT_PULLUP) {
//            GPIO->gppud = 2;
//        }
//        else if (mode == INPUT_PULLDOWN) {
//            GPIO->gppud = 1;
//        }
//        else {
//            GPIO->gppud = 0;
//        }
//        usleep(5);
//        GPIO->gppudclk[gpioToPUDCLK[pin]] = 1 << (pin & 31);
//        usleep(5);
//        
//        GPIO->gppud = 0;
//        usleep(5);
//        GPIO->gppudclk[gpioToPUDCLK[pin]] = 0;
//        usleep(5);
//    }
}

int digitalRead(int pin) {
//    if ((GPIO->gplev[gpioToGPLEV[pin]] & (1 << (pin & 31))) != 0) {
//        return HIGH;
//    }
//    else {
        return LOW;
//    }
}

void digitalWrite(int pin, int value) {
//    if (value == LOW) {
//        GPIO->gpclr[gpioToGPCLR[pin]] = 1 << (pin & 31);
//    }
//    else {
//        GPIO->gpset[gpioToGPSET[pin]] = 1 << (pin & 31);
//    }
}