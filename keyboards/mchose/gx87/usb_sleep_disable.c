#include QMK_KEYBOARD_H

#ifdef WIRELESS_ENABLE
#include "wireless.h"
#endif

// Function to check if we're in USB mode
bool is_usb_mode(void) {
#ifdef WIRELESS_ENABLE
    return (wireless_get_current_devs() == DEVS_USB);
#else
    return true;  // Always USB mode if wireless isn't enabled
#endif
}

// Override the suspend wakeup condition function to prevent sleep when in USB mode
bool suspend_wakeup_condition(void) {
    // If we're in USB mode, always return true to prevent sleep
    if (is_usb_mode()) {
        return true;
    }

    // Otherwise, use the default behavior for wireless modes
    matrix_power_up();
    matrix_scan();
    matrix_power_down();
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
        if (matrix_get_row(r)) return true;
    }
    return false;
}
