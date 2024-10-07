#include "mapping.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "usb_hid_keys.h"
#include "usb_conkbd.h"

#define MOD(x) ((x) << 8)

static __code uint16_t map[128] =
{
    /*0x00*/ KEY_1, KEY_2, KEY_3, KEY_Z, KEY_4, KEY_5, KEY_6, KEY_7,
    /*0x08*/ MOD(KEY_MOD_LMETA) /*Cmd*/, KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_GRAVE,
    /*0x10*/ KEY_X, KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H, KEY_SPACE,
    /*0x18*/ KEY_CAPSLOCK, KEY_TAB, MOD(KEY_MOD_LCTRL), 0, 0, 0, 0, 0,
    /*0x20*/ 0, 0, 0 /*Fn*/, MOD(KEY_MOD_LALT), 0, 0, 0, 0,
    /*0x28*/ 0, 0, 0, 0, KEY_C, KEY_V, KEY_B, KEY_N,
    /*0x30*/ KEY_MINUS, KEY_EQUAL, KEY_BACKSPACE, KEY_HOME /*F1*/, KEY_8, KEY_9, KEY_0, MOD(KEY_MOD_RALT) /*Space2*/,
    /*0x38*/ KEY_LEFTBRACE, KEY_RIGHTBRACE, KEY_BACKSLASH, KEY_END /*F2*/, KEY_U, KEY_I, KEY_O, KEY_P,
    /*0x40*/ KEY_APOSTROPHE, KEY_ENTER, KEY_PAGEUP /*F3*/, 0, KEY_J, KEY_K, KEY_L, KEY_SEMICOLON,
    /*0x48*/ KEY_SLASH, KEY_UP, KEY_PAGEDOWN /*F4*/, 0, KEY_M, KEY_COMMA, KEY_DOT, KEY_INSERT /*Done*/,
    /*0x50*/ MOD(KEY_MOD_RCTRL) /*Del*/, KEY_LEFT, KEY_DOWN, KEY_RIGHT, 0, 0, 0, 0,
    /*0x58*/ MOD(KEY_MOD_LSHIFT), MOD(KEY_MOD_RSHIFT), 0, 0, 0, 0, 0, 0,
};

static __code uint16_t map_fn[128] =
{
    /*0x00*/ KEY_F1, KEY_F2, KEY_F3, 0, KEY_F4, KEY_F5, KEY_F6, KEY_F7,
    /*0x08*/ 0, 0, 0, 0, 0, 0, 0, 0,
    /*0x10*/ 0, 0, 0, 0, 0, 0, 0, 0,
    /*0x18*/ 0, KEY_ESC, 0, 0, 0, 0, 0, 0,
    /*0x20*/ 0, 0, 0, 0, 0, 0, 0, 0,
    /*0x28*/ 0, 0, 0, 0, 0, 0, 0, 0,
    /*0x30*/ KEY_F11, KEY_F12, KEY_DELETE, 0, KEY_F8, KEY_F9, KEY_F10, 0,
    /*0x38*/ 0, 0, 0, 0, 0, 0, 0, 0,
    /*0x40*/ 0, 0, 0, 0, 0, 0, 0, 0,
    /*0x48*/ 0, 0, 0, 0, 0, 0, 0, KEY_SYSRQ,
    /*0x50*/ 0, 0, 0, 0, 0, 0, 0, 0,
    /*0x58*/ 0, 0, 0, 0, 0, 0, 0, 0,
};

#define BITMAP_DEFINE(name, bits) uint8_t name[(bits)/8]
#define bitmap_isset(bitmap, bit) ((bitmap)[(bit)/8] & (1 << ((bit)%8)))
#define bitmap_set(bitmap, bit) do { (bitmap)[(bit)/8] |= (1 << ((bit)%8)); } while (0)
#define bitmap_clear(bitmap, bit) do { (bitmap)[(bit)/8] &= ~(1 << ((bit)%8)); } while (0)
#define bitmap_zero(bitmap) memset((bitmap), 0, sizeof(bitmap) / sizeof((bitmap)[0]))

static uint8_t last_release = 0;
static bool fn_pressed = 0;
static BITMAP_DEFINE(pressed_while_fn, 128);

void MAP_handle(uint8_t key_code)
{
    /* Ignore keyboard ID */
    if (key_code == 0xfa || key_code == 0xfd)
    {
        return;
    }

    uint16_t key_modifier = map[key_code & 0x7f];
            
    if (key_code & 0x80)
    {
        if (last_release == key_code)
        {
            KBD_releaseAll();

            last_release = 0;
            fn_pressed = 0;
            bitmap_zero(pressed_while_fn);
        }
        else if (key_code == (0x22 | 0x80))    // Fn
        {
            fn_pressed = 0;

            last_release = key_code;
        }
        else
        {
            if (bitmap_isset(pressed_while_fn, key_code & 0x7f))
            {
                key_modifier = map_fn[key_code & 0x7f];
                bitmap_clear(pressed_while_fn, key_code & 0x7f);
            }

            KBD_release(key_modifier);

            last_release = key_code;
        }
    }
    else
    {
        last_release = 0;

        if (key_code == 0x22)    // Fn
        {
            fn_pressed = 1;
        }
        else if (fn_pressed)
        {
            uint16_t new_key_modifier = map_fn[key_code];
            if (new_key_modifier != 0)
            {
                bitmap_set(pressed_while_fn, key_code);
                key_modifier = new_key_modifier;
            }

            KBD_press(key_modifier);
        }
        else
        {
            KBD_press(key_modifier);
        }
    }
}
