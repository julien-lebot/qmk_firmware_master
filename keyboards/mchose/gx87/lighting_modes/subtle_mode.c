// Subtle white backlight
// Copyright 2024

#include "quantum.h"
#include "../config.h"

void apply_lighting_mode_subtle(void) {
    // Set all LEDs to subtle white
    for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
        rgb_matrix_set_color(i, 0x30, 0x30, 0x35);  // Subtle cool white
    }
    
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
