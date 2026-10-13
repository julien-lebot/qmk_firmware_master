// Declare custom effects using the RGB_MATRIX_EFFECT macro
RGB_MATRIX_EFFECT(SMART_LAYER_INDICATOR)

// Define effects inside the RGB_MATRIX_CUSTOM_EFFECT_IMPLS ifdef block
#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#include "quantum.h"
#include "../../config.h"

// Layer colors - variables for easier customization
static uint8_t base_layer_color[3] = {0x10, 0x10, 0x40};  // Subtle blue
static uint8_t mac_layer_color[3] = {0x30, 0x00, 0x30};   // Subtle purple
static uint8_t fn_key_color[3] = {0xFF, 0x80, 0x00};      // Bright orange
static uint8_t caps_lock_color[3] = {0xFF, 0x00, 0x00};   // Bright red
static uint8_t status_led_color[3] = {0x00, 0x00, 0xFF};  // Blue

// Matrix dimensions
#define MATRIX_ROWS 6
#define MATRIX_COLS 18

// Get the corresponding matrix position for an LED index
// This uses the actual mapping from keyboard.json
static bool get_matrix_from_led(uint8_t led_index, uint8_t *row, uint8_t *col) {
    // LEDs 0-86 correspond to actual keys with matrix positions
    // LEDs 88+ are indicator LEDs without matrix positions
    if (led_index >= 88) return false;
    
    // This is the actual mapping from keyboard.json
    // Each entry maps an LED index to its matrix position [row, col]
    // The index in this array corresponds to the LED index
    static const uint8_t led_matrix_map[][2] = {
        {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}, 
        {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12}, {0, 13}, {0, 14}, {0, 15}, 
        {0, 16}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, 
        {1, 7}, {1, 8}, {1, 9}, {1, 10}, {1, 11}, {1, 12}, {1, 13}, {1, 14}, 
        {1, 15}, {1, 16}, {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, 
        {2, 6}, {2, 7}, {2, 8}, {2, 9}, {2, 10}, {2, 11}, {2, 12}, {2, 13}, 
        {2, 14}, {2, 15}, {2, 16}, {3, 0}, {3, 1}, {3, 2}, {3, 3}, {3, 4}, 
        {3, 5}, {3, 6}, {3, 7}, {3, 8}, {3, 9}, {3, 10}, {3, 11}, {3, 13}, 
        {4, 0}, {4, 1}, {4, 2}, {4, 3}, {4, 4}, {4, 5}, {4, 6}, {4, 7}, 
        {4, 8}, {4, 9}, {4, 10}, {4, 13}, {4, 15}, {5, 0}, {5, 1}, {5, 2}, 
        {5, 5}, {5, 9}, {5, 10}, {5, 11}, {5, 13}, {5, 14}, {5, 15}, {5, 16}
    };
    
    // Get the matrix position from the mapping
    *row = led_matrix_map[led_index][0];
    *col = led_matrix_map[led_index][1];
    
    return true;
}

// Function to check if a keycode is a letter (A-Z)
static bool is_alpha_keycode(uint16_t keycode) {
    return (keycode >= KC_A && keycode <= KC_Z);
}

// Function to check if a key is different between layers
static bool is_key_overridden(uint8_t row, uint8_t col, uint8_t base_layer, uint8_t active_layer) {
    if (base_layer == active_layer) return false;
    
    uint16_t base_keycode = keymap_key_to_keycode(base_layer, (keypos_t){.row = row, .col = col});
    uint16_t active_keycode = keymap_key_to_keycode(active_layer, (keypos_t){.row = row, .col = col});
    
    // If the keycode is different between layers and not KC_TRNS, it's overridden
    return (base_keycode != active_keycode) && (active_keycode != KC_TRNS);
}

// Main effect implementation
bool SMART_LAYER_INDICATOR(effect_params_t* params) {
    // Don't use the LED limits for this effect - we want to process all LEDs every time
    uint8_t led_min = 0;
    uint8_t led_max = RGB_MATRIX_LED_COUNT;
    
    // Step 1: Get the current active layer
    uint8_t active_layer = get_highest_layer(layer_state);
    uint8_t base_layer = 0;  // Default base layer
    bool is_fn_active = false;
    
    // Determine which base layer we're using and if Fn is active
    if (active_layer == 1) {
        base_layer = 0;  // Normal base layer
        is_fn_active = true;
    } else if (active_layer == 3) {
        base_layer = 2;  // Mac base layer
        is_fn_active = true;
    } else {
        base_layer = active_layer;  // Current layer is the base layer
    }
    
    // Check if Caps Lock is active
    bool caps_lock_on = host_keyboard_led_state().caps_lock;
    
    // Step 2: Apply base color based on the base layer (not the active layer)
    uint8_t *base_color;
    switch (base_layer) {
        case 0:  // Normal base layer
            base_color = base_layer_color;
            break;
        case 2:  // Mac base layer
            base_color = mac_layer_color;
            break;
        default:
            base_color = base_layer_color;
    }
    
    // Apply colors to each LED
    for (uint8_t i = led_min; i < led_max; i++) {
        // Apply the base layer color to all keys
        rgb_matrix_set_color(i, base_color[0], base_color[1], base_color[2]);
        
        // Step 3: Highlight keys that are overridden in the current layer
        if (is_fn_active) {  // Only when Fn is active
            uint8_t row, col;
            if (get_matrix_from_led(i, &row, &col)) {
                // Check if this key is overridden in the active layer
                if (is_key_overridden(row, col, base_layer, active_layer)) {
                    rgb_matrix_set_color(i, fn_key_color[0], fn_key_color[1], fn_key_color[2]);
                }
            }
        }
        
        // Step 4: Highlight letter keys when Caps Lock is active
        if (caps_lock_on) {
            uint8_t row, col;
            if (get_matrix_from_led(i, &row, &col)) {
                // Get the keycode for this position in the current layer
                uint16_t keycode = keymap_key_to_keycode(active_layer, (keypos_t){.row = row, .col = col});
                
                // Check if it's a letter key (A-Z)
                if (is_alpha_keycode(keycode)) {
                    rgb_matrix_set_color(i, caps_lock_color[0], caps_lock_color[1], caps_lock_color[2]);
                }
                
                // Highlight the Caps Lock LED itself
                if (i == CAPS_LOCK_LED_INDEX) {
                    rgb_matrix_set_color(i, caps_lock_color[0], caps_lock_color[1], caps_lock_color[2]);
                }
            }
        }
        
    }
    
    // Set status indicator LEDs explicitly
    rgb_matrix_set_color(STATUS_LED_FIRST, status_led_color[0], status_led_color[1], status_led_color[2]);
    rgb_matrix_set_color(STATUS_LED_SECOND, status_led_color[0], status_led_color[1], status_led_color[2]);
    rgb_matrix_set_color(STATUS_LED_THIRD, status_led_color[0], status_led_color[1], status_led_color[2]);
    rgb_matrix_set_color(STATUS_LED_FOURTH, status_led_color[0], status_led_color[1], status_led_color[2]);
    
    return false;
}


#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
