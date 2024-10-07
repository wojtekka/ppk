#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "mapping.h"
#include "usb_hid_keys.h"

bool KBD_releaseAll_called;
uint16_t KBD_press_called;
uint16_t KBD_release_called;

void KBD_releaseAll(void)
{
    printf("KBD_releaseAll()\n");
    if (KBD_releaseAll_called)
    {
        printf("called twice\n");
        abort();
    }
    KBD_releaseAll_called = true;
}

void KBD_press(uint16_t key_modifier)
{
    printf("KBD_press(0x%04x)\n", key_modifier);
    if (KBD_press_called != 0)
    {
        printf("called twice\n");
        abort();
    }
    KBD_press_called = key_modifier;
}

void KBD_release(uint16_t key_modifier)
{
    printf("KBD_release(0x%04x)\n", key_modifier);
    if (KBD_release_called != 0)
    {
        printf("called twice\n");
        abort();
    }
    KBD_release_called = key_modifier;
}

#define EXPECT_RELEASE_ALL(key_code) expect_release_all(key_code, __LINE__)

void expect_release_all(uint8_t key_code, int line)
{
    printf("sending 0x%02x\n", key_code);
    MAP_handle(key_code);

    if (!KBD_releaseAll_called || KBD_press_called != 0 || KBD_release_called != 0)
    {
        printf("expected KBD_releaseAll() in %s:%d\n", __FILE__, line);
        abort();
    }
    KBD_releaseAll_called = false;
}

#define EXPECT_RELEASE(key_code, key_modifier) expect_release(key_code, key_modifier, __LINE__)

void expect_release(uint8_t key_code, uint16_t key_modifier, int line)
{
    printf("sending 0x%02x\n", key_code);
    MAP_handle(key_code);

    if (KBD_release_called != key_modifier || KBD_press_called != 0 || KBD_releaseAll_called)
    {
        printf("expected KBD_release(0x%04x) in %s:%d\n", key_modifier, __FILE__, line);
        abort();
    }
    KBD_release_called = 0;
}

#define EXPECT_PRESS(key_code, key_modifier) expect_press(key_code, key_modifier, __LINE__)

void expect_press(uint8_t key_code, uint16_t key_modifier, int line)
{
    printf("sending 0x%02x\n", key_code);
    MAP_handle(key_code);

    if (KBD_press_called != key_modifier || KBD_release_called != 0 || KBD_releaseAll_called)
    {
        printf("expected KBD_press(0x%04x) in %s:%d\n", key_modifier, __FILE__, line);
        abort();
    }
    KBD_press_called = 0;
}

#define EXPECT_NOTHING(key_code) expect_nothing(key_code, __LINE__)

void expect_nothing(uint8_t key_code, int line)
{
    printf("sending 0x%02x\n", key_code);
    MAP_handle(key_code);

    if (KBD_press_called != 0 || KBD_release_called != 0 || KBD_releaseAll_called)
    {
        printf("expected nothing in %s:%d\n", __FILE__, line);
        abort();
    }
}

int main(void)
{
    /* 1 */
    EXPECT_PRESS(0x00, KEY_1);
    EXPECT_RELEASE(0x80 | 0x00, KEY_1);
    EXPECT_RELEASE_ALL(0x80 | 0x00);

    /* Fn+1 -> F1 */
    EXPECT_NOTHING(0x22);
    EXPECT_PRESS(0x00, KEY_F1);
    EXPECT_RELEASE(0x80 | 0x00, KEY_F1);
    EXPECT_NOTHING(0x80 | 0x22);
    EXPECT_RELEASE_ALL(0x80 | 0x22);

    /* Fn+1 -> F1, Fn released first */
    EXPECT_NOTHING(0x22);
    EXPECT_PRESS(0x00, KEY_F1);
    EXPECT_NOTHING(0x80 | 0x22);
    EXPECT_RELEASE(0x80 | 0x00, KEY_F1);
    EXPECT_RELEASE_ALL(0x80 | 0x00);

    /* 1 after releasing Fn */
    EXPECT_PRESS(0x00, KEY_1);
    EXPECT_RELEASE(0x80 | 0x00, KEY_1);
    EXPECT_RELEASE_ALL(0x80 | 0x00);

    /* Fn+1, Fn+2 -> F1, F2 */
    EXPECT_NOTHING(0x22);
    EXPECT_PRESS(0x00, KEY_F1);
    EXPECT_RELEASE(0x80 | 0x00, KEY_F1);
    EXPECT_PRESS(0x01, KEY_F2);
    EXPECT_RELEASE(0x80 | 0x01, KEY_F2);
    EXPECT_NOTHING(0x80 | 0x22);
    EXPECT_RELEASE_ALL(0x80 | 0x22);

    /* Fn+1+2 -> F1, F2, together */
    EXPECT_NOTHING(0x22);
    EXPECT_PRESS(0x00, KEY_F1);
    EXPECT_PRESS(0x01, KEY_F2);
    EXPECT_RELEASE(0x80 | 0x01, KEY_F2);
    EXPECT_RELEASE(0x80 | 0x00, KEY_F1);
    EXPECT_NOTHING(0x80 | 0x22);
    EXPECT_RELEASE_ALL(0x80 | 0x22);

    /* Fn+1+2 -> F1, F2, released in different order */
    EXPECT_NOTHING(0x22);
    EXPECT_PRESS(0x00, KEY_F1);
    EXPECT_PRESS(0x01, KEY_F2);
    EXPECT_RELEASE(0x80 | 0x00, KEY_F1);
    EXPECT_RELEASE(0x80 | 0x01, KEY_F2);
    EXPECT_NOTHING(0x80 | 0x22);
    EXPECT_RELEASE_ALL(0x80 | 0x22);
}
