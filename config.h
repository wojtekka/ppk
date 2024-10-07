// ===================================================================================
// User Configurations for CH552E USB
// ===================================================================================

#pragma once

// USB device descriptor
#define USB_VENDOR_ID       0x6666      // VID
#define USB_PRODUCT_ID      0x7777      // PID
#define USB_DEVICE_VERSION  0x0100      // v1.0 (BCD-format)

// USB configuration descriptor
#define USB_MAX_POWER_mA    50          // max power in mA 

// USB descriptor strings
#define MANUFACTURER_STR    't','o','x','y','g','e','n','.','n','e','t'
#define PRODUCT_STR         'p','p','k'
