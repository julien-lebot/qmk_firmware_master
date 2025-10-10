// Lighting mode manager
// Copyright 2024

#include "quantum.h"
#include "../config.h"
#include "lighting_modes.h"

// Current lighting mode (saved to EEPROM)
static uint8_t current_lighting_mode = LIGHTING_MODE_DEFAULT;

// Get the current lighting mode
uint8_t get_lighting_mode(void) {
    return current_lighting_mode;
}

// Set a specific lighting mode
void set_lighting_mode(uint8_t mode) {
    if (mode < LIGHTING_MODE_COUNT) {
        current_lighting_mode = mode;
        eeconfig_update_user(current_lighting_mode);  // Save to EEPROM
    }
}

// Cycle to the next lighting mode
void cycle_lighting_mode(void) {
    current_lighting_mode = (current_lighting_mode + 1) % LIGHTING_MODE_COUNT;
    eeconfig_update_user(current_lighting_mode);  // Save to EEPROM
}

// Initialize lighting mode from EEPROM
void lighting_init(void) {
    // Load lighting mode from EEPROM
    current_lighting_mode = eeconfig_read_user() % LIGHTING_MODE_COUNT;
}

// Apply the current lighting mode
void apply_current_lighting_mode(void) {
    // Apply the current lighting mode
    switch (current_lighting_mode) {
        case LIGHTING_MODE_MINIMAL:
            apply_lighting_mode_minimal();
            break;
        case LIGHTING_MODE_SUBTLE:
            apply_lighting_mode_subtle();
            break;
        case LIGHTING_MODE_LAYERS:
            apply_lighting_mode_layers();
            break;
        case LIGHTING_MODE_DEFAULT:
        default:
            apply_lighting_mode_default();
            break;
    }
}
