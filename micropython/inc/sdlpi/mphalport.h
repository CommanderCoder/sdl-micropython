/*
 * This file is part of the Micro Python project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Damien P. George
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef __MICROPY_INCLUDED_MICROBIT_MPHALPORT_H__
#define __MICROPY_INCLUDED_MICROBIT_MPHALPORT_H__


#include <SDL.h>


#ifdef __cplusplus
extern "C" {
#endif

void mp_hal_init(void);

// provide these since we don't assume VT100 support
void mp_hal_move_cursor_back(unsigned int pos);
void mp_hal_erase_line_from_cursor(unsigned int n_chars);

void mp_hal_display_string(const char*);


#define txt_width  64
#define txt_height 48

extern int cur_x;
extern int cur_y;
extern int cursor_visible;

struct _colour {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};


struct _screen {
    unsigned char c;
    struct _colour fore;
    struct _colour back;
};

    extern struct _colour cursor;
    extern struct _colour mainback;
    extern struct _colour linefore;
    extern struct _colour lineback;
    
    extern SDL_Renderer *renderer;
    

extern struct _screen screen[txt_width * txt_height];

void SDL_DrawCharA(char c);
void SDL_DrawStringA(const char *s);
void SDL_DrawStringAtA(int y, int x, const char *s);

void SDL_SetChar(char a);
    
#ifdef __cplusplus
}
#endif

#endif // __MICROPY_INCLUDED_MICROBIT_MPHALPORT_H__
