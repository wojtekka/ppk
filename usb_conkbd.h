// ===================================================================================
// USB HID Consumer Keyboard Functions for CH551, CH552 and CH554
// ===================================================================================

#pragma once
#include <stdint.h>
#include "usb_hid.h"

// Functions
#define KBD_init() HID_init()         // init keyboard
void KBD_press(uint16_t key_modifier);  // LSB - key, MSB - modifier
void KBD_release(uint16_t key_modifier);    // LSB - key, MSB - modifier
void KBD_releaseAll(void);            // release all keys on keyboard

uint8_t KBD_getState(void);           // get keyboard status LEDs

// Keyboard LED states
#define KBD_NUM_LOCK_state            (KBD_getState() & 1)
#define KBD_CAPS_LOCK_state           ((KBD_getState() >> 1) & 1)
#define KBD_SCROLL_LOCK_state         ((KBD_getState() >> 2) & 1)
#define KBD_COMPOSE_state             ((KBD_getState() >> 3) & 1)
#define KBD_KANA_state                ((KBD_getState() >> 4) & 1)

