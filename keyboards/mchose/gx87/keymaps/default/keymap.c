/* Copyright (C) 2023 jonylee@hfd
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

enum custom_keycodes {
    KC_LMOD = SAFE_RANGE,  // Lighting Mode toggle
};

// Forward declaration of lighting mode functions
extern uint8_t get_lighting_mode(void);
extern void cycle_lighting_mode(void);
// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [0] = LAYOUT_ansi( /* Base */
		KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  G(KC_D), KC_NO,   KC_NO,   KC_NO,
		KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_NO,   KC_HOME, KC_PGUP,
		KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_DEL,  KC_END,  KC_PGDN,
		KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
		KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,                   KC_RSFT,          KC_UP,
		KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,                                      KC_RALT, MO(1),   KC_APP,  KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT),

    [1] = LAYOUT_ansi( /* FN */
		EE_CLR,  KC_BRID, KC_BRIU,G(KC_TAB),KC_MYCM,KC_WSCH,G(S(KC_S)),KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, _______, _______, _______, _______,
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RGB_HUI, RGB_MOD, _______, _______, _______, _______,
		_______, KC_2G4,  KC_BT1,  KC_BT2,  KC_BT3,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
		_______, TO(2),   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                   _______,          RGB_VAI,
		KC_LMOD, GU_TOGG, _______,                            _______,                            _______, _______, _______, _______, RGB_SPD, RGB_VAD, RGB_SPI),

    [2] = LAYOUT_ansi( /* Base */
		KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_F13,  KC_NO,   KC_NO,   KC_NO,
		KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_NO,   KC_HOME, KC_PGUP,
		KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_DEL,  KC_END,  KC_PGDN,
		KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
		KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,                   KC_RSFT,          KC_UP,
		KC_LCTL, KC_LALT, KC_LGUI,                   KC_SPC,                                      KC_RGUI, MO(3),   KC_APP,  KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT),

    [3] = LAYOUT_ansi( /* FN */
		EE_CLR,  KC_BRID, KC_BRIU,G(KC_TAB),KC_MYCM,KC_WSCH,G(S(KC_S)),KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, _______, _______, _______, _______,
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RGB_HUI, RGB_MOD, _______, _______, _______, _______,
		_______, KC_2G4,  KC_BT1,  KC_BT2,  KC_BT3,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
		_______, _______, TO(0),   _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                   _______,          RGB_VAI,
		_______, _______, _______,                            _______,                            _______, _______, _______, _______, RGB_SPD, RGB_VAD, RGB_SPI),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [1] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [2] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [3] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
};
#endif

// Persist the Win/Mac base layer across power cycles.
//
// TO(0)/TO(2) only set the volatile layer state; QMK saves the *default*
// layer byte (EECONFIG_DEFAULT_LAYER) at boot, which stays 0 unless something
// writes it. When the layer state is exactly a base layer (1 = Win, 4 = Mac)
// record it as the default layer so quantum_init() re-selects it after a
// replug.
//
// State 0 is skipped: that is the boot-time call from quantum_init() before
// the saved default layer has propagated into layer_state -- writing there
// would clobber the persisted choice on every boot.

layer_state_t layer_state_set_user(layer_state_t state) {
    if (state == ((layer_state_t)1 << 0) || state == ((layer_state_t)1 << 2)) {
        uint8_t selected = (state == ((layer_state_t)1 << 0)) ? 0 : 2;
        if (((layer_state_t)1 << selected) != eeconfig_read_default_layer()) {
            eeconfig_update_default_layer((layer_state_t)1 << selected);
            default_layer_set((layer_state_t)1 << selected);
        }
    }
    return state;
}

// Process custom keycodes
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_LMOD:
            if (record->event.pressed) {
                // Cycle through lighting modes
                cycle_lighting_mode();
                return false;
            }
            break;
    }
    return true;
}
