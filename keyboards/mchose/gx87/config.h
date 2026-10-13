// Copyright 2024 Wind (@yelishang)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// Custom lighting modes
#define LIGHTING_MODE_DEFAULT 0    // Default RGB effects
#define LIGHTING_MODE_MINIMAL 1    // Backlight off, only indicators
#define LIGHTING_MODE_SUBTLE  2    // Subtle white backlight
#define LIGHTING_MODE_LAYERS  3    // Layer-based lighting
#define LIGHTING_MODE_COUNT   4    // Total number of modes

// Status indicator LEDs
#define STATUS_LED_FIRST  88
#define STATUS_LED_SECOND 89
#define STATUS_LED_THIRD  90
#define STATUS_LED_FOURTH 91
#define CAPS_LOCK_LED_INDEX 51

// Fix sleep timeout (30 minutes instead of 60 seconds)
#undef LPWR_TIMEOUT
#define LPWR_TIMEOUT 900000

#undef RGB_MATRIX_TIMEOUT
#define RGB_MATRIX_TIMEOUT 900000

// Prevent USB power management issues
#define NO_SUSPEND_POWER_DOWN

// Completely disable suspend mode
#define NO_SUSPEND_TO_DEEP_SLEEP

// Disable auto sleep
#define NO_AUTO_SLEEP_MODE

// Basic reliability improvements
#undef USB_SUSPEND_WAKEUP_DELAY
#define USB_SUSPEND_WAKEUP_DELAY 1000
#define DEBOUNCE 10

// Improve wireless reliability
#define WIRELESS_DEBOUNCE 10

// Ensure RGB matrix is restored after sleep
#define RGB_MATRIX_SLEEP_KEEPALIVE

#ifdef WIRELESS_ENABLE
//#    define LPWR_TIMEOUT RGB_MATRIX_TIMEOUT
#    define WLS_KEYBOARD_REPORT_KEYS 5
#endif

#define USB_POWER_EN_PIN A14
#define LED_POWER_EN_PIN A15

#  define BT_CABLE_PIN B8 // 充电接入时为高
#  define BT_CHARGE_PIN B9 // 充电时为低，充满时为高

#  define BT_MODE_SW_PIN C0 // 低电平时
#  define RF_MODE_SW_PIN C15 // 低电平时

/* UART */
#define UART_TX_PIN A9
#define UART_RX_PIN A10

/* SPI Config for spi flash*/
#define SPI_DRIVER SPIDQ
#define SPI_SCK_PIN B3
#define SPI_MOSI_PIN B5
#define SPI_MISO_PIN B4
#define SPI_MOSI_PAL_MODE 5

#define EXTERNAL_FLASH_SPI_SLAVE_SELECT_PIN C12
#define WEAR_LEVELING_LOGICAL_SIZE (WEAR_LEVELING_BACKING_SIZE / 2)

/* I2C Config for LED Driver */
#define SNLED27351_I2C_ADDRESS_1 0b1110100
#define SNLED27351_I2C_ADDRESS_2 0b1110111
#define I2C1_OPMODE OPMODE_I2C
#define I2C1_CLOCK_SPEED 400000 /* 400000 */

#define RGB_MATRIX_FRAMEBUFFER_EFFECTS
#define RGB_MATRIX_KEYPRESSES

// Status indicator LEDs identified through testing
// These are the 4 LEDs that make up the status indicator strip
#define STATUS_LED_1    88
#define STATUS_LED_2    89
#define STATUS_LED_3    90
#define STATUS_LED_4    91

// Total number of LEDs in the keyboard
#define LED_COUNT 93