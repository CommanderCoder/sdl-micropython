#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL.h>


#include "../kernel/platform.h"
#include "../kernel/wiring.h"

#include "py/nlr.h"
#include "py/stackctrl.h"
#include "py/compile.h"
#include "py/runtime.h"
#include "py/repl.h"
#include "py/gc.h"
#include "py/mphal.h"
#include "lib/readline.h"
#include "lib/utils/pyexec.h"


#include "genhdr/mpversion.h"

#include "mphalport.h"

#define FONT                    vgafont8
#define BIT_SHIFT               (7 - s_bit_no)

#define CHAR_W                  8
#define CHAR_H                  8

static unsigned char vgafont8[128 * 8]= {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x7e, 0x81, 0xa5, 0x81, 0xbd, 0x99, 0x81, 0x7e,
    0x7e, 0xff, 0xdb, 0xff, 0xc3, 0xe7, 0xff, 0x7e,
    0x6c, 0xfe, 0xfe, 0xfe, 0x7c, 0x38, 0x10, 0x00,
    0x10, 0x38, 0x7c, 0xfe, 0x7c, 0x38, 0x10, 0x00,
    0x38, 0x7c, 0x38, 0xfe, 0xfe, 0x7c, 0x38, 0x7c,
    0x10, 0x10, 0x38, 0x7c, 0xfe, 0x7c, 0x38, 0x7c,
    0x00, 0x00, 0x18, 0x3c, 0x3c, 0x18, 0x00, 0x00,
    0xff, 0xff, 0xe7, 0xc3, 0xc3, 0xe7, 0xff, 0xff,
    0x00, 0x3c, 0x66, 0x42, 0x42, 0x66, 0x3c, 0x00,
    0xff, 0xc3, 0x99, 0xbd, 0xbd, 0x99, 0xc3, 0xff,
    0x0f, 0x07, 0x0f, 0x7d, 0xcc, 0xcc, 0xcc, 0x78,
    0x3c, 0x66, 0x66, 0x66, 0x3c, 0x18, 0x7e, 0x18,
    0x3f, 0x33, 0x3f, 0x30, 0x30, 0x70, 0xf0, 0xe0,
    0x7f, 0x63, 0x7f, 0x63, 0x63, 0x67, 0xe6, 0xc0,
    0x99, 0x5a, 0x3c, 0xe7, 0xe7, 0x3c, 0x5a, 0x99,
    0x80, 0xe0, 0xf8, 0xfe, 0xf8, 0xe0, 0x80, 0x00,
    0x02, 0x0e, 0x3e, 0xfe, 0x3e, 0x0e, 0x02, 0x00,
    0x18, 0x3c, 0x7e, 0x18, 0x18, 0x7e, 0x3c, 0x18,
    0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x66, 0x00,
    0x7f, 0xdb, 0xdb, 0x7b, 0x1b, 0x1b, 0x1b, 0x00,
    0x3e, 0x63, 0x38, 0x6c, 0x6c, 0x38, 0xcc, 0x78,
    0x00, 0x00, 0x00, 0x00, 0x7e, 0x7e, 0x7e, 0x00,
    0x18, 0x3c, 0x7e, 0x18, 0x7e, 0x3c, 0x18, 0xff,
    0x18, 0x3c, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x00,
    0x18, 0x18, 0x18, 0x18, 0x7e, 0x3c, 0x18, 0x00,
    0x00, 0x18, 0x0c, 0xfe, 0x0c, 0x18, 0x00, 0x00,
    0x00, 0x30, 0x60, 0xfe, 0x60, 0x30, 0x00, 0x00,
    0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xfe, 0x00, 0x00,
    0x00, 0x24, 0x66, 0xff, 0x66, 0x24, 0x00, 0x00,
    0x00, 0x18, 0x3c, 0x7e, 0xff, 0xff, 0x00, 0x00,
    0x00, 0xff, 0xff, 0x7e, 0x3c, 0x18, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x30, 0x78, 0x78, 0x30, 0x30, 0x00, 0x30, 0x00,
    0x6c, 0x6c, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x6c, 0x6c, 0xfe, 0x6c, 0xfe, 0x6c, 0x6c, 0x00,
    0x30, 0x7c, 0xc0, 0x78, 0x0c, 0xf8, 0x30, 0x00,
    0x00, 0xc6, 0xcc, 0x18, 0x30, 0x66, 0xc6, 0x00,
    0x38, 0x6c, 0x38, 0x76, 0xdc, 0xcc, 0x76, 0x00,
    0x60, 0x60, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x18, 0x30, 0x60, 0x60, 0x60, 0x30, 0x18, 0x00,
    0x60, 0x30, 0x18, 0x18, 0x18, 0x30, 0x60, 0x00,
    0x00, 0x66, 0x3c, 0xff, 0x3c, 0x66, 0x00, 0x00,
    0x00, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x60,
    0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00,
    0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x80, 0x00,
    0x7c, 0xc6, 0xce, 0xde, 0xf6, 0xe6, 0x7c, 0x00,
    0x30, 0x70, 0x30, 0x30, 0x30, 0x30, 0xfc, 0x00,
    0x78, 0xcc, 0x0c, 0x38, 0x60, 0xcc, 0xfc, 0x00,
    0x78, 0xcc, 0x0c, 0x38, 0x0c, 0xcc, 0x78, 0x00,
    0x1c, 0x3c, 0x6c, 0xcc, 0xfe, 0x0c, 0x1e, 0x00,
    0xfc, 0xc0, 0xf8, 0x0c, 0x0c, 0xcc, 0x78, 0x00,
    0x38, 0x60, 0xc0, 0xf8, 0xcc, 0xcc, 0x78, 0x00,
    0xfc, 0xcc, 0x0c, 0x18, 0x30, 0x30, 0x30, 0x00,
    0x78, 0xcc, 0xcc, 0x78, 0xcc, 0xcc, 0x78, 0x00,
    0x78, 0xcc, 0xcc, 0x7c, 0x0c, 0x18, 0x70, 0x00,
    0x00, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30, 0x00,
    0x00, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30, 0x60,
    0x18, 0x30, 0x60, 0xc0, 0x60, 0x30, 0x18, 0x00,
    0x00, 0x00, 0xfc, 0x00, 0x00, 0xfc, 0x00, 0x00,
    0x60, 0x30, 0x18, 0x0c, 0x18, 0x30, 0x60, 0x00,
    0x78, 0xcc, 0x0c, 0x18, 0x30, 0x00, 0x30, 0x00,
    0x7c, 0xc6, 0xde, 0xde, 0xde, 0xc0, 0x78, 0x00,
    0x30, 0x78, 0xcc, 0xcc, 0xfc, 0xcc, 0xcc, 0x00,
    0xfc, 0x66, 0x66, 0x7c, 0x66, 0x66, 0xfc, 0x00,
    0x3c, 0x66, 0xc0, 0xc0, 0xc0, 0x66, 0x3c, 0x00,
    0xf8, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0xf8, 0x00,
    0xfe, 0x62, 0x68, 0x78, 0x68, 0x62, 0xfe, 0x00,
    0xfe, 0x62, 0x68, 0x78, 0x68, 0x60, 0xf0, 0x00,
    0x3c, 0x66, 0xc0, 0xc0, 0xce, 0x66, 0x3e, 0x00,
    0xcc, 0xcc, 0xcc, 0xfc, 0xcc, 0xcc, 0xcc, 0x00,
    0x78, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00,
    0x1e, 0x0c, 0x0c, 0x0c, 0xcc, 0xcc, 0x78, 0x00,
    0xe6, 0x66, 0x6c, 0x78, 0x6c, 0x66, 0xe6, 0x00,
    0xf0, 0x60, 0x60, 0x60, 0x62, 0x66, 0xfe, 0x00,
    0xc6, 0xee, 0xfe, 0xfe, 0xd6, 0xc6, 0xc6, 0x00,
    0xc6, 0xe6, 0xf6, 0xde, 0xce, 0xc6, 0xc6, 0x00,
    0x38, 0x6c, 0xc6, 0xc6, 0xc6, 0x6c, 0x38, 0x00,
    0xfc, 0x66, 0x66, 0x7c, 0x60, 0x60, 0xf0, 0x00,
    0x78, 0xcc, 0xcc, 0xcc, 0xdc, 0x78, 0x1c, 0x00,
    0xfc, 0x66, 0x66, 0x7c, 0x6c, 0x66, 0xe6, 0x00,
    0x78, 0xcc, 0xe0, 0x70, 0x1c, 0xcc, 0x78, 0x00,
    0xfc, 0xb4, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00,
    0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xfc, 0x00,
    0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0x78, 0x30, 0x00,
    0xc6, 0xc6, 0xc6, 0xd6, 0xfe, 0xee, 0xc6, 0x00,
    0xc6, 0xc6, 0x6c, 0x38, 0x38, 0x6c, 0xc6, 0x00,
    0xcc, 0xcc, 0xcc, 0x78, 0x30, 0x30, 0x78, 0x00,
    0xfe, 0xc6, 0x8c, 0x18, 0x32, 0x66, 0xfe, 0x00,
    0x78, 0x60, 0x60, 0x60, 0x60, 0x60, 0x78, 0x00,
    0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x02, 0x00,
    0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x00,
    0x10, 0x38, 0x6c, 0xc6, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
    0x30, 0x30, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x78, 0x0c, 0x7c, 0xcc, 0x76, 0x00,
    0xe0, 0x60, 0x60, 0x7c, 0x66, 0x66, 0xdc, 0x00,
    0x00, 0x00, 0x78, 0xcc, 0xc0, 0xcc, 0x78, 0x00,
    0x1c, 0x0c, 0x0c, 0x7c, 0xcc, 0xcc, 0x76, 0x00,
    0x00, 0x00, 0x78, 0xcc, 0xfc, 0xc0, 0x78, 0x00,
    0x38, 0x6c, 0x60, 0xf0, 0x60, 0x60, 0xf0, 0x00,
    0x00, 0x00, 0x76, 0xcc, 0xcc, 0x7c, 0x0c, 0xf8,
    0xe0, 0x60, 0x6c, 0x76, 0x66, 0x66, 0xe6, 0x00,
    0x30, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00,
    0x0c, 0x00, 0x0c, 0x0c, 0x0c, 0xcc, 0xcc, 0x78,
    0xe0, 0x60, 0x66, 0x6c, 0x78, 0x6c, 0xe6, 0x00,
    0x70, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00,
    0x00, 0x00, 0xcc, 0xfe, 0xfe, 0xd6, 0xc6, 0x00,
    0x00, 0x00, 0xf8, 0xcc, 0xcc, 0xcc, 0xcc, 0x00,
    0x00, 0x00, 0x78, 0xcc, 0xcc, 0xcc, 0x78, 0x00,
    0x00, 0x00, 0xdc, 0x66, 0x66, 0x7c, 0x60, 0xf0,
    0x00, 0x00, 0x76, 0xcc, 0xcc, 0x7c, 0x0c, 0x1e,
    0x00, 0x00, 0xdc, 0x76, 0x66, 0x60, 0xf0, 0x00,
    0x00, 0x00, 0x7c, 0xc0, 0x78, 0x0c, 0xf8, 0x00,
    0x10, 0x30, 0x7c, 0x30, 0x30, 0x34, 0x18, 0x00,
    0x00, 0x00, 0xcc, 0xcc, 0xcc, 0xcc, 0x76, 0x00,
    0x00, 0x00, 0xcc, 0xcc, 0xcc, 0x78, 0x30, 0x00,
    0x00, 0x00, 0xc6, 0xd6, 0xfe, 0xfe, 0x6c, 0x00,
    0x00, 0x00, 0xc6, 0x6c, 0x38, 0x6c, 0xc6, 0x00,
    0x00, 0x00, 0xcc, 0xcc, 0xcc, 0x7c, 0x0c, 0xf8,
    0x00, 0x00, 0xfc, 0x98, 0x30, 0x64, 0xfc, 0x00,
    0x1c, 0x30, 0x30, 0xe0, 0x30, 0x30, 0x1c, 0x00,
    0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00,
    0xe0, 0x30, 0x30, 0x1c, 0x30, 0x30, 0xe0, 0x00,
    0x76, 0xdc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x10, 0x38, 0x6c, 0xc6, 0xc6, 0xfe, 0x00,
};

static unsigned char keyNormal_it[] = {
    0x0, 0x0, 0x0, 0x0, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',
    'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '1', '2',
    '3', '4', '5', '6', '7', '8', '9', '0', '\r', 0x0, '\b', '\t', ' ', '\'', 0x0, 0x0,
    '+', '<', 0x0, 0x0, 0x0, '\\', ',', '.', '-', 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, '/', '*', '-', '+', '\r', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', '0', '.', '<', 0x0, 0x0, '='
};

static unsigned char keyShift_it[] = {
    0x0, 0x0, 0x0, 0x0, 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
    'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '!', '"',
    0x0, '$', '%', '&', '/', '(', ')', '=', '\r', 0x0, '\b', '\t', ' ', '?', '^', 0x0,
    '*', '>', 0x0, 0x0, 0x0, '|', ';', ':', '_', 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, '/', '*', '-', '+', '\r', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', '0', '.', '>', 0x0, 0x0, '='
};

#if defined(__cplusplus)
extern "C" {
#endif

__attribute__ ((interrupt ("IRQ"))) void interrupt_irq() {
    SDL_Interrupt_Handler();
}

#if defined(__cplusplus)
}
#endif


static SDL_Rect crect;

void SDL_InitConsole(int w, int h) {
    crect.x = (w - txt_width * CHAR_W) / 2;
    crect.y = (h - txt_height * CHAR_H) / 2;
    crect.w = txt_width * CHAR_W;
    crect.h = txt_height * CHAR_H;

    for (int i = 0; i < txt_width * txt_height; i++) {
        screen[i].c = ' ';
        screen[i].fore.r = 255;
        screen[i].fore.g = 255;
        screen[i].fore.b = 255;
        screen[i].back.r = 98;
        screen[i].back.g = 0;
        screen[i].back.b = 32;
    }

    cur_x = 0;
    cur_y = 0;
}

void SDL_RenderConsole(SDL_Renderer *renderer) {
    int x, y, c_x;
    int index = 0;

    for (y = crect.y; index < txt_width * txt_height; y += CHAR_H) {
        for (c_x = 0, x = crect.x; c_x < txt_width && index < txt_width * txt_height; index++, c_x++, x += CHAR_W) {
            int s_offset = (int) screen[index].c * CHAR_W * CHAR_H;
            for (int f_y = 0; f_y < CHAR_H; f_y++) {
                for (int f_x = 0; f_x < CHAR_W; f_x++) {
                    int s_byte_no = s_offset / 8;
                    int s_bit_no = s_offset % 8;

                    unsigned char s_byte = FONT[s_byte_no];
                    if ((s_byte >> BIT_SHIFT) & 0x1)
                        SDL_SetRenderDrawColor(renderer, screen[index].fore.r, screen[index].fore.g, screen[index].fore.b, 255);
                    else
                        SDL_SetRenderDrawColor(renderer, screen[index].back.r, screen[index].back.g, screen[index].back.b, 255);
                    SDL_RenderDrawPoint(renderer, x + f_x, y + f_y);
                    s_offset++;
                }
            }
        }
    }

    if (cursor_visible) {
        SDL_Rect rect;
        rect.x = crect.x + cur_x * CHAR_W;
        rect.y = crect.y + cur_y * CHAR_H;
        rect.w = CHAR_W;
        rect.h = CHAR_H;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}


SDL_Renderer *renderer;
SDL_Event event;
struct timer_wait tw;
int led_status = LOW;

void flash_cursor()
{
    
    if (compare_timer(&tw)) {
        led_status = led_status == LOW ? HIGH : LOW;
        digitalWrite(16, led_status);
        cursor_visible = cursor_visible ? 0 : 1;
    }
}
void render()
{
    
    SDL_SetRenderDrawColor(renderer, 213, 41, 82, 255);
    SDL_RenderClear(renderer);
    
    SDL_RenderConsole(renderer);
    
    SDL_RenderPresent(renderer);
}
void mp_keyboard_interrupt(void) {
   // MP_STATE_VM(mp_pending_exception) = MP_STATE_PORT(mp_kbd_exception);
}

STATIC int do_repl_sdl(void);

static char *stack_top;

void main() {
    int w, h;
    SDL_Window *screen;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);

    // Default screen resolution (set in config.txt or auto-detected)
    //SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP, &screen, &renderer);

#if !(MACOS_SDLMP)
    // Sets a specific screen resolution
    //  SDL_CreateWindowAndRenderer(32 + 320 + 32, 32 + 200 + 32, SDL_WINDOW_FULLSCREEN, &screen, &renderer);
#else
    // NOTE: It shouldn't be possible to change the window and render in this OS
    SDL_CreateWindowAndRenderer(32 + 320 + 32, 32 + 200 + 32, SDL_WINDOW_RESIZABLE, &screen, &renderer);
#endif
    
    SDL_GetWindowSize(screen, &w, &h);
    SDL_InitConsole(w, h);

    SDL_DrawStringAtA(1, (txt_width - 22) / 2, "**** RASPBERRY-PI ****");
    SDL_DrawStringAtA(3, (txt_width - 30) / 2, "BARE-METAL SDL SYSTEM TEMPLATE\r\n");

    pinMode(16, OUTPUT);
    register_timer(&tw, 250000);

    SDL_DrawStringA("\r\nREADY\r\n");



    int stack_dummy;
    stack_top = (char*)&stack_dummy;
    mp_stack_ctrl_init();
    mp_stack_set_limit(1800); // stack is 2k

    // allocate the heap statically in the bss
    static uint32_t heap[9820 / 4];
    gc_init(heap, (uint8_t*)heap + sizeof(heap));

    
    mp_init();
    mp_hal_init();
    readline_init0();

// MICROPY_REPL_EVENT_DRIVEN
    pyexec_event_repl_init();

// something wrong here - default set to FRIENDLY
    // if (pyexec_mode_kind == PYEXEC_MODE_FRIENDLY_REPL) {
    //     // from microbit import *
    //     mp_import_all(mp_import_name(MP_QSTR_microbit, mp_const_empty_tuple, MP_OBJ_NEW_SMALL_INT(0)));
    // }
#if 1
    bool done = false;
    while(!done) {


        // SORT OUT THE KEYS!

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                switch(event.key.keysym.scancode) {
#if (MACOS_SDLMP)
                        // F12 to quit
                    case SDL_SCANCODE_F12:
                        done = true;
                        break;
#endif

                        
                    case SDL_SCANCODE_UP:
                        if (cur_y > 0)
                            cur_y--;
                        break;
                    case SDL_SCANCODE_DOWN:
                        if (cur_y < txt_height - 1)
                            cur_y++;
                        break;
                    case SDL_SCANCODE_LEFT:
                        if (cur_x > 0)
                            cur_x--;
                        else if (cur_y > 0) {
                            cur_y--;
                            cur_x = txt_width - 1;
                        }
                        break;
                    case SDL_SCANCODE_RIGHT:
                        if (cur_x < txt_width - 1)
                            cur_x++;
                        else if (cur_y < txt_height - 1) {
                            cur_y++;
                            cur_x = 0;
                        }
                        break;
                    case SDL_SCANCODE_HOME:
                        cur_x = 0;
                        break;
                    case SDL_SCANCODE_END:
                        cur_x = txt_width - 1;
                        break;
                    case SDL_SCANCODE_RETURN:
                            SDL_SetChar(keyShift_it[event.key.keysym.scancode]);
                        break;
                    default: {
                        SDL_Keymod mod = SDL_GetModState();
                        if ((mod & (KMOD_LSHIFT | KMOD_RSHIFT)) != 0) {
                            char c = keyShift_it[event.key.keysym.scancode];
                            if (c >= ' ')
                                   SDL_SetChar(c);
                        }
                        else {
                            char c = keyNormal_it[event.key.keysym.scancode];
                            if (c >= ' ')
                                   SDL_SetChar(c);
                        }
                    }
                }


            }
#if (MACOS_SDLMP)
            else if (event.type == SDL_QUIT)
            {
                done = true;
            }
#endif
        }


        // now sort out python
        // MICROPY_REPL_EVENT_DRIVEN
        int ret = 0;
        int interrupt_char = CHAR_CTRL_C;
        int c = mp_hal_stdin_rx_chr();
        if (c == interrupt_char) {
            mp_keyboard_interrupt();
        }
        ret = pyexec_event_repl_process_char(c);
        if (ret & PYEXEC_FORCED_EXIT) {
            done = true;
        }

        // now sort out display
        flash_cursor();
        render();

    }
#else
    do_repl_sdl();
#endif


    mp_hal_stdout_tx_str("soft reboot\r\n");

    memset(&MP_STATE_PORT(async_data)[0], 0, sizeof(MP_STATE_PORT(async_data)));
    MP_STATE_PORT(async_music_data) = NULL;

    mp_deinit();

}

#if (MACOS_SDLMP)



STATIC void stderr_print_strn(void *env, const char *str, size_t len) {
    (void)env;
    ssize_t dummy = write(STDERR_FILENO, str, len);
    // mp_uos_dupterm_tx_strn(str, len);
    (void)dummy;
}

const mp_print_t mp_stderr_print = {NULL, stderr_print_strn};

#define FORCED_EXIT (0x100)
// If exc is SystemExit, return value where FORCED_EXIT bit set,
// and lower 8 bits are SystemExit value. For all other exceptions,
// return 1.
STATIC int handle_uncaught_exception(mp_obj_base_t *exc) {
    // check for SystemExit
    if (mp_obj_is_subclass_fast(MP_OBJ_FROM_PTR(exc->type), MP_OBJ_FROM_PTR(&mp_type_SystemExit))) {
        // None is an exit value of 0; an int is its value; anything else is 1
        mp_obj_t exit_val = mp_obj_exception_get_value(MP_OBJ_FROM_PTR(exc));
        mp_int_t val = 0;
        if (exit_val != mp_const_none && !mp_obj_get_int_maybe(exit_val, &val)) {
            val = 1;
        }
        return FORCED_EXIT | (val & 255);
    }
    
    // Report all other exceptions
    mp_obj_print_exception(&mp_stderr_print, MP_OBJ_FROM_PTR(exc));
    return 1;
}



// Command line options, with their defaults
STATIC bool compile_only = false;
STATIC uint emit_opt = MP_EMIT_OPT_NONE;

// Returns standard error codes: 0 for success, 1 for all other errors,
// except if FORCED_EXIT bit is set then script raised SystemExit and the
// value of the exit is in the lower 8 bits of the return value
STATIC int execute_from_lexer(mp_lexer_t *lex, mp_parse_input_kind_t input_kind, bool is_repl) {
    if (lex == NULL) {
        printf("MemoryError: lexer could not allocate memory\n");
        return 1;
    }
    
    mp_hal_set_interrupt_char(CHAR_CTRL_C);
    
    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        qstr source_name = lex->source_name;
        
#if MICROPY_PY___FILE__
        if (input_kind == MP_PARSE_FILE_INPUT) {
            mp_store_global(MP_QSTR___file__, MP_OBJ_NEW_QSTR(source_name));
        }
#endif
        
        mp_parse_tree_t parse_tree = mp_parse(lex, input_kind);
        
        /*
         printf("----------------\n");
         mp_parse_node_print(parse_tree.root, 0);
         printf("----------------\n");
         */
        
        mp_obj_t module_fun = mp_compile(&parse_tree, source_name, emit_opt, is_repl);
        
        if (!compile_only) {
            // execute it
            mp_call_function_0(module_fun);
            // check for pending exception
            if (MP_STATE_VM(mp_pending_exception) != MP_OBJ_NULL) {
                mp_obj_t obj = MP_STATE_VM(mp_pending_exception);
                MP_STATE_VM(mp_pending_exception) = MP_OBJ_NULL;
                nlr_raise(obj);
            }
        }
        
        mp_hal_set_interrupt_char(-1);
        nlr_pop();
        return 0;
        
    } else {
        // uncaught exception
        mp_hal_set_interrupt_char(-1);
        return handle_uncaught_exception(nlr.ret_val);
    }
}


#endif

#if MICROPY_USE_READLINE == 1
#include "lib/mp-readline/readline.h"
#else
STATIC char *strjoin(const char *s1, int sep_char, const char *s2) {
    int l1 = strlen(s1);
    int l2 = strlen(s2);
    char *s = malloc(l1 + l2 + 2);
    memcpy(s, s1, l1);
    if (sep_char != 0) {
        s[l1] = sep_char;
        l1 += 1;
    }
    memcpy(s + l1, s2, l2);
    s[l1 + l2] = 0;
    return s;
}
#endif


char *prompt(char *p) {
#if MICROPY_USE_READLINE == 1
    // MicroPython supplied readline
    vstr_t vstr;
    vstr_init(&vstr, 16);
    mp_hal_stdio_mode_raw();
    int ret = readline(&vstr, p);
    mp_hal_stdio_mode_orig();
    if (ret != 0) {
        vstr_clear(&vstr);
        if (ret == CHAR_CTRL_D) {
            // EOF
            printf("\n");
            return NULL;
        } else {
            printf("\n");
            char *line = malloc(1);
            line[0] = '\0';
            return line;
        }
    }
    vstr_null_terminated_str(&vstr);
    char *line = malloc(vstr.len + 1);
    memcpy(line, vstr.buf, vstr.len + 1);
    vstr_clear(&vstr);
#elif MICROPY_USE_READLINE == 2
    // GNU readline
    char *line = readline(p);
    if (line) {
        add_history(line);
    }
#else
    // simple read string
    static char buf[256];
    fputs(p, stdout);
    char *s = fgets(buf, sizeof(buf), stdin);
    if (!s) {
        return NULL;
    }
    int l = strlen(buf);
    if (buf[l - 1] == '\n') {
        buf[l - 1] = 0;
    } else {
        l++;
    }
    char *line = malloc(l);
    memcpy(line, buf, l);
#endif
    return line;
}



STATIC int do_repl_sdl(void) {
    mp_hal_stdout_tx_str("MicroPython " MICROPY_GIT_TAG " on " MICROPY_BUILD_DATE "; "
                         MICROPY_PY_SYS_PLATFORM " version\nUse Ctrl-D to exit, Ctrl-E for paste mode\n");
    
#if MICROPY_USE_READLINE == 1
    
    // use MicroPython supplied readline
    
    vstr_t line;
    vstr_init(&line, 16);
    for (;;) {
        mp_hal_stdio_mode_raw();
        
    input_restart:
        vstr_reset(&line);
        int ret = readline(&line, ">>> ");
        mp_parse_input_kind_t parse_input_kind = MP_PARSE_SINGLE_INPUT;
        
        if (ret == CHAR_CTRL_C) {
            // cancel input
            mp_hal_stdout_tx_str("\r\n");
            goto input_restart;
        } else if (ret == CHAR_CTRL_D) {
            // EOF
            printf("\n");
            mp_hal_stdio_mode_orig();
            vstr_clear(&line);
            return 0;
        } else if (ret == CHAR_CTRL_E) {
            // paste mode
            mp_hal_stdout_tx_str("\npaste mode; Ctrl-C to cancel, Ctrl-D to finish\n=== ");
            vstr_reset(&line);
            for (;;) {
                char c = mp_hal_stdin_rx_chr();
                if (c == CHAR_CTRL_C) {
                    // cancel everything
                    mp_hal_stdout_tx_str("\n");
                    goto input_restart;
                } else if (c == CHAR_CTRL_D) {
                    // end of input
                    mp_hal_stdout_tx_str("\n");
                    break;
                } else {
                    // add char to buffer and echo
                    vstr_add_byte(&line, c);
                    if (c == '\r') {
                        mp_hal_stdout_tx_str("\n=== ");
                    } else {
                        mp_hal_stdout_tx_strn(&c, 1);
                    }
                }
            }
            parse_input_kind = MP_PARSE_FILE_INPUT;
        } else if (line.len == 0) {
            if (ret != 0) {
                printf("\n");
            }
            goto input_restart;
        } else {
            // got a line with non-zero length, see if it needs continuing
            while (mp_repl_continue_with_input(vstr_null_terminated_str(&line))) {
                vstr_add_byte(&line, '\n');
                ret = readline(&line, "... ");
                if (ret == CHAR_CTRL_C) {
                    // cancel everything
                    printf("\n");
                    goto input_restart;
                } else if (ret == CHAR_CTRL_D) {
                    // stop entering compound statement
                    break;
                }
            }
        }
        
        mp_hal_stdio_mode_orig();
        
        mp_lexer_t *lex = mp_lexer_new_from_str_len(MP_QSTR__lt_stdin_gt_, line.buf, line.len, false);
        ret = execute_from_lexer(lex, parse_input_kind, true);
        if (ret & FORCED_EXIT) {
            return ret;
        }
    }
    
#else
    
    // use GNU or simple readline
    
    for (;;) {
        char *line = prompt(">>> ");
        if (line == NULL) {
            // EOF
            return 0;
        }
        while (mp_repl_continue_with_input(line)) {
            char *line2 = prompt("... ");
            if (line2 == NULL) {
                break;
            }
            char *line3 = strjoin(line, '\n', line2);
            free(line);
            free(line2);
            line = line3;
        }
        
        mp_lexer_t *lex = mp_lexer_new_from_str_len(MP_QSTR__lt_stdin_gt_, line, strlen(line), false);
        int ret = execute_from_lexer(lex, MP_PARSE_SINGLE_INPUT, true);
        if (ret & FORCED_EXIT) {
            return ret;
        }
        free(line);
        
        
        flash_cursor();
        render();
    }
    
#endif
}





