// ===================================================================================
// USB HID Consumer Keyboard Functions for CH551, CH552 and CH554
// ===================================================================================

#include "usb_conkbd.h"
#include "usb_hid.h"
#include "usb_handler.h"

#define KBD_sendReport()  HID_sendReport(KBD_report, sizeof(KBD_report))
#define CON_sendReport()  HID_sendReport(CON_report, sizeof(CON_report))

// ===================================================================================
// Keyboard HID report
// ===================================================================================
__xdata uint8_t  KBD_report[9] = {1,0,0,0,0,0,0,0,0};
__xdata uint8_t  CON_report[9] = {2,0,0,0,0,0,0,0,0};

// ===================================================================================
// Press a key on keyboard
// ===================================================================================
void KBD_press(uint16_t key_modifier) {
  uint8_t i;
  uint8_t key = key_modifier & 255;
  uint8_t modifier = (key_modifier >> 8) & 255;

  KBD_report[1] |= modifier;

  // Check if key is already present in report
  for(i=3; i<9; i++) {
    if(KBD_report[i] == key) return;            // return if already in report
  }

  // Find an empty slot, insert key and transmit report
  for(i=3; i<9; i++) {
    if(KBD_report[i] == 0) {                    // empty slot?
      KBD_report[i] = key;                      // insert key
      KBD_sendReport();                         // send report
      return;                                   // and return
    }
  }
}

// ===================================================================================
// Release a key on keyboard
// ===================================================================================
void KBD_release(uint16_t key_modifier) {
  uint8_t i;
  uint8_t key = key_modifier & 255;
  uint8_t modifier = (key_modifier >> 8) & 255;

  KBD_report[1] &= ~modifier;
  
  // Delete key in report
  for(i=3; i<9; i++) {
    if(KBD_report[i] == key) KBD_report[i] = 0; // delete key in report
  }
  KBD_sendReport();                             // send report
}

// ===================================================================================
// Release all keys on keyboard
// ===================================================================================
void KBD_releaseAll(void) {
  uint8_t i;
  for(i=8; i; i--) KBD_report[i] = 0;           // delete all keys in report
  KBD_sendReport();                             // send report
}

// ===================================================================================
// Get keyboard status LEDs
// ===================================================================================
uint8_t KBD_getState(void) {
  return EP2_buffer[0];
}
