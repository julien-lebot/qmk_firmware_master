// Default RGB mode - uses QMK's built-in RGB matrix effects
// Copyright 2024

#include "quantum.h"
#include "../config.h"

void apply_lighting_mode_default(void) {
    // Just let the default RGB matrix effects run
    // Only set the status indicator LEDs and Caps Lock
    
    // Set status indicator LEDs to blue
    rgb_matrix_set_color(STATUS_LED_FIRST, 0x00, 0x00, 0xFF);
    rgb_matrix_set_color(STATUS_LED_SECOND, 0x00, 0x00, 0xFF);
    rgb_matrix_set_color(STATUS_LED_THIRD, 0x00, 0x00, 0xFF);
    rgb_matrix_set_color(STATUS_LED_FOURTH, 0x00, 0x00, 0xFF);
    
    // Set Caps Lock indicator
    if (host_keyboard_led_state().caps_lock) {
        rgb_matrix_set_color(CAPS_LOCK_LED_INDEX, 0xFF, 0x00, 0x00);  // Bright red
    }
}
