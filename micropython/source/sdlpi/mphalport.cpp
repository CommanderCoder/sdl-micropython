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

//#include "MicroBit.h"

extern "C" {

#include "py/mpstate.h"
#include "py/mphal.h"
#include <SDL.h>




int cur_x;
int cur_y;
int cursor_visible;
struct _screen screen[txt_width * txt_height];

void SDL_DrawStringA(const char *s) {
    char c;

    while (*s) {
        c = *s++;
        if (c == '\r') {
            cur_x = 0;
        }
        else if (c == '\b') {
            if (cur_x > 0)
                cur_x--;
            else if (cur_y > 0) {
                cur_y--;
                cur_x = txt_width - 1;
            }
        }
        else if (c == '\n') {
            cur_y++;
            if (cur_y >= txt_height) {
                memcpy(&screen[0], &screen[txt_width], sizeof(screen) - sizeof(struct _screen) * txt_width);
                for (int i = txt_width * (txt_height - 1); i < txt_width * txt_height; i++) {
                    screen[i].c = ' ';
                    screen[i].fore.r = 23;
                    screen[i].fore.g = 98;
                    screen[i].fore.b = 35;
                    screen[i].back.r = 30;
                    screen[i].back.g = 41;
                    screen[i].back.b = 51;
                }
                cur_y--;
            }
        }
        else {
            screen[cur_y * txt_width + cur_x].c = c;
            cur_x++;
            if (cur_x >= txt_width) {
                cur_x = 0;
                cur_y++;
                if (cur_y >= txt_height) {
                    memcpy(&screen[0], &screen[txt_width], sizeof(screen) - sizeof(struct _screen) * txt_width);
                    for (int i = txt_width * (txt_height - 1); i < txt_width * txt_height; i++) {
                        screen[i].c = ' ';
                        screen[i].fore.r = 23;
                        screen[i].fore.g = 98;
                        screen[i].fore.b = 35;
                        screen[i].back.r = 30;
                        screen[i].back.g = 41;
                        screen[i].back.b = 51;
                    }
                    cur_y--;
                }
            }
        }
    }
    cursor_visible = 1;
}

void SDL_DrawStringAtA(int y, int x, const char *s) {
    cur_x = x % txt_width;
    cur_y = y % txt_height;
    SDL_DrawStringA(s);
}

void SDL_DrawCharA(char c) {
    if (c == '\r') {
        cur_x = 0;
    }
    else if (c == '\b') {
        if (cur_x > 0)
            cur_x--;
        else if (cur_y > 0) {
            cur_y--;
            cur_x = txt_width - 1;
        }
    }
    else if (c == '\n') {
        cur_y++;
        if (cur_y >= txt_height) {
            memcpy(&screen[0], &screen[txt_width], sizeof(screen) - sizeof(struct _screen) * txt_width);
            for (int i = txt_width * (txt_height - 1); i < txt_width * txt_height; i++) {
                screen[i].c = ' ';
                screen[i].fore.r = 23;
                screen[i].fore.g = 98;
                screen[i].fore.b = 35;
                screen[i].back.r = 30;
                screen[i].back.g = 41;
                screen[i].back.b = 51;
            }
            cur_y--;
        }
    }
    else {
        screen[cur_y * txt_width + cur_x].c = c;
        cur_x++;
        if (cur_x >= txt_width) {
            cur_x = 0;
            cur_y++;
            if (cur_y >= txt_height) {
                memcpy(&screen[0], &screen[txt_width], sizeof(screen) - sizeof(struct _screen) * txt_width);
                for (int i = txt_width * (txt_height - 1); i < txt_width * txt_height; i++) {
                    screen[i].c = ' ';
                    screen[i].fore.r = 23;
                    screen[i].fore.g = 98;
                    screen[i].fore.b = 35;
                    screen[i].back.r = 30;
                    screen[i].back.g = 41;
                    screen[i].back.b = 51;
                }
                cur_y--;
            }
        }
    }
    cursor_visible = 1;
}

void SDL_DrawCharAtA(int y, int x, char c) {
    cur_x = x % txt_width;
    cur_y = y % txt_height;
    SDL_DrawCharA(c);
}


unsigned char sdl_char = 0;

void SDL_SetChar(char a)
{
    sdl_char=a;
}

// Receive single character
int mp_hal_stdin_rx_chr(void) {
    unsigned char c = sdl_char;
    sdl_char = 0;
    return c;
}
    
// Send string of given length
void mp_hal_stdout_tx_strn(const char *str, size_t len) {
    for (; len > 0; --len) {
        SDL_DrawCharA(*str++);
    }
}

// Send string of given length
void mp_hal_stdout_tx_char(const char c) {
   SDL_DrawCharA(c);
}

void mp_hal_stdout_tx_strn_cooked(const char *str, size_t len) {
    while (len--) {
        if (*str == '\n') {
            mp_hal_stdout_tx_char('\r');
        }
        mp_hal_stdout_tx_char(*str++);
    }
}
    

void mp_hal_stdout_tx_str(const char *str) {
    mp_hal_stdout_tx_strn(str, strlen(str));
}

mp_uint_t mp_hal_ticks_ms(void) { return 0; }

void mp_hal_set_interrupt_char(int c) {}


STATIC void mp_hal_print_many(const char chrs[8], unsigned int total) {
    while (total > 0) {
        unsigned int n = total;
        if (n > 8) {
            n = 8;
        }
        total -= n;
        mp_hal_stdout_tx_strn(chrs, n);
    }
}

void mp_hal_move_cursor_back(unsigned int pos) {
    mp_hal_print_many("\b\b\b\b\b\b\b\b", pos);
}

void mp_hal_erase_line_from_cursor(unsigned int n_chars) {
    mp_hal_print_many("        ", n_chars);
    mp_hal_move_cursor_back(n_chars);
}

void mp_hal_display_string(const char *str) {
    mp_hal_stdout_tx_str(str);
}

void mp_hal_init()
{

}

}
