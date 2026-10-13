# MCHOSE GX87 Development Documentation

This document contains technical details and findings from reverse engineering the MCHOSE GX87 keyboard.

## Hardware Specifications

### Microcontroller
- **MCU**: WB32FQ95
- **Architecture**: ARM Cortex-M3
- **Clock Speed**: 96MHz (configured in mcuconf.h)
- **Flash Memory**: 256KB internal. EEPROM emulation via wear-leveling on the
  on-chip embedded flash (`wear_leveling`/`embedded_flash`, sectors at the end
  of flash). The external SPI NOR is no longer used by QMK.

### Matrix Configuration
- **Matrix Size**: 6 rows × 18 columns
- **Diode Direction**: ROW2COL
- **Row Pins**: B15, C6, C7, C8, C9, B14
- **Column Pins**: C1, C2, C3, A0, A1, A2, A3, A4, A5, A6, A7, C4, C5, B0, B1, B2, B10, B11
- **Layout**: TKL (87-key) ANSI layout

### Communication Interfaces
- **USB**: Full-speed USB with VID: 0xFFFE, PID: 0x006F
- **I2C**: Used for LED driver communication
  - Speed: 400kHz
  - Mode: OPMODE_I2C
- **SPI**: Used for external flash memory
  - QSPI interface
  - Pins: SCK (B3), MOSI (B5), MISO (B4)
  - Slave Select: C12
- **UART**: Used for wireless module communication
  - TX Pin: A9
  - RX Pin: A10

### LED System
- **LED Driver**: SNLED27351 (dual controller setup)
  - Address 1: 0b1110100 (0x74)
  - Address 2: 0b1110111 (0x77)
- **LED Count**: 93 total LEDs
  - 87 per-key RGB LEDs
  - 4 status indicator LEDs (indices 88-91)
  - 2 additional indicator LEDs
- **LED Power Control**: Pin A15 (LED_POWER_EN_PIN)

### Wireless Module
- **Wireless Capabilities**:
  - Bluetooth (supports up to 3 devices)
  - 2.4GHz wireless
  - USB (wired mode)
- **Control Pins**:
  - BT_MODE_SW_PIN: C0 (Low when active)
  - RF_MODE_SW_PIN: C15 (Low when active)
- **Battery Management**:
  - BT_CABLE_PIN: B8 (High when charging cable connected)
  - BT_CHARGE_PIN: B9 (Low when charging, High when fully charged)
- **USB Power Control**: Pin A14 (USB_POWER_EN_PIN)

### Power Management
- **Sleep Timeout**: Configured for 30 minutes (1800000ms)
- **Low Power Mode**: Uses a custom implementation in `lowpower.c`
- **Wake Sources**: Matrix key press, UART RX

## Firmware Architecture

### Core Components
1. **Matrix Scanning**: Standard QMK implementation with ROW2COL diode direction
2. **LED Control**: Uses RGB Matrix with custom lighting modes
3. **Wireless Module**: Custom implementation with protocol defined in `wireless.c`
4. **Power Management**: Custom implementation in `lowpower.c` and `lpwr_wb32.c`

### Custom Features
1. **Lighting Modes**:
   - Default: Standard RGB matrix effects
   - Minimal: Backlight off, only indicators
   - Subtle: Subtle white backlight
   - Layers: Layer-based lighting with different colors per layer
   - Mode switching via custom keycode KC_LMOD

2. **Wireless Protocol**:
   - Custom protocol for communicating with wireless module
   - Commands for device selection, pairing, and power management
   - Battery status reporting

3. **Power Management**:
   - Custom low power mode implementation
   - Sleep timeout configurable via LPWR_TIMEOUT
   - Multiple power states: NORMAL, PRESLEEP, STOP, WAKEUP

## Hardware Similarities to Other Keyboards

Based on the hardware configuration, the MCHOSE GX87 appears to be similar to other keyboards in these ways:

1. **MCU**: The WB32FQ95 is a Chinese ARM Cortex-M3 microcontroller that's becoming more common in Chinese mechanical keyboards. It's similar to:
   - STM32F303 (used in many custom keyboards)
   - GD32F303 (Chinese clone of STM32)

2. **LED Driver**: The SNLED27351 is a common LED driver used in many RGB keyboards, including:
   - Keychron Q-series keyboards
   - GMMK Pro
   - Many other Chinese RGB keyboards

3. **Wireless Module**: The custom wireless module appears to be similar to those used in:
   - Keychron K-series wireless keyboards
   - Royal Kludge wireless keyboards
   - Epomaker wireless keyboards

4. **Layout and Design**: Standard TKL (87-key) ANSI layout, common among many keyboards:
   - Ducky One 2
   - Filco Majestouch 2
   - Leopold FC750R

## Sleep/Wake Behavior (RESOLVED in this branch)

The upstream QMK port (jonylee1986) had a wake-from-host-suspend bug: the
keyboard went dead when the computer woke from sleep and needed a replug.
Root causes and fixes, cherry-picked from
`s-kerdel/qmk_firmware_master` branch `mchose_gx87_fix_wireless_modes`:

1. `WB32_USB_HOST_WAKEUP_DURATION` was 2 ms in `mcuconf.h` -- too short for
   many hosts to register the remote-wakeup resume signal. Restored to the
   10 ms chibios-contrib default.
2. `USB_POWER_EN_PIN` (external D+ pull-up, active low) was driven in
   `keyboard_post_init_kb()`, after `init_usb_driver()` had already connected
   the bus; cold boot enumerated against a disabled pull-up. Moved to
   `keyboard_pre_init_kb()`.
3. `usb_remote_wakeup()` in `transport.c` had no recovery path when USB never
   reached `USB_ACTIVE`. A non-blocking recovery FSM now restarts the USB
   stack automatically (2 s grace, up to 20 attempts), replicating the old
   manual mode-slider workaround.
4. `wireless_send_nkro()` busy-waited on `smsg_is_busy()` without a
   connected-check, freezing the main loop when the wireless module was not
   ACKing. The override was removed; the QMK default in `wireless.c` handles
   NKRO correctly.
5. `smsg_retry` was reset in the wrong state transition, so stuck packets
   never dropped.
6. EEPROM moved from the external SPI NOR to on-chip embedded flash -- the
   SPI-backed wear-leveling was resetting settings on every boot.

Verified on hardware (2026-10): sleep/wake recovers without replug.

## Win/Mac Base Layer Persistence

The VIA keymap stores `TO(0)` (Fn+S) / `TO(2)` (Fn+A) as the base-layer
switch. These were volatile: every replug booted back to Windows.
`process_record_user()` now intercepts `QK_TO` for layers 0/2 and calls
`set_single_persistent_default_layer()`, which writes
`EECONFIG_DEFAULT_LAYER` -- the byte `quantum_init()` restores at boot.
Note that a `layer_state_set_user()` hook is the wrong place for this: it
fires at boot before the default layer is folded into the layer state and
interferes with the action layer cache during a switch.

Also note: `SMART_LAYER_INDICATOR` must OR `default_layer_state` into its
layer lookup -- after boot the base layer lives only in
`default_layer_state`, not in `layer_state`.

Do NOT attempt to persist the base layer by writing the EEPROM byte from a
layer-state hook; it clobbers itself at boot (see git history,
commit 1bfce78b9f).

## Known Issues and Quirks

1. **Sleep/Wake Issues**:
   - The keyboard may have issues with sleep/wake cycles
   - Multiple power management systems conflict with each other
   - Wireless connection may be unstable after waking from sleep

2. **Battery Management**:
   - Battery status reporting is limited
   - No proper battery level indication to the host

3. **Wireless Stability**:
   - Bluetooth and 2.4GHz connections may be unstable
   - Custom wireless protocol may have reliability issues

## Potential Improvements

1. **Power Management**:
   - Consolidate power management code to avoid conflicts
   - Improve sleep/wake reliability
   - Add user-configurable sleep timeouts

2. **Wireless Stability**:
   - Improve wireless reconnection after sleep
   - Better handling of connection switching

3. **Battery Reporting**:
   - Implement proper battery level reporting to host
   - Add visual battery level indicators using LEDs

4. **Firmware Optimization**:
   - Reduce code complexity in wireless handling
   - Improve documentation of the custom wireless protocol
   - Better integration with standard QMK features

## Flashing Instructions

The keyboard uses the WB32 DFU bootloader (in ROM, cannot be bricked from
software). Build:

```
make mchose/gx87:default
```

Enter the bootloader by holding **Esc** (top-left key, bootmagic) while
plugging in USB, then:

```
make mchose/gx87:default:flash
# or directly:
wb32-dfu-updater_cli -s 0x08000000 -D mchose_gx87_default.bin
```

On macOS the toolchain via brew needs an explicit PATH for the keg-only
cross-compiler:

```
PATH="/opt/homebrew/Cellar/arm-none-eabi-gcc@8/8.5.0_2/bin:/opt/homebrew/Cellar/arm-none-eabi-binutils/2.41/bin:$PATH" \
  qmk compile -kb mchose/gx87 -km default
```

After the first flash of this branch, EEPROM storage moves from the external
SPI flash to on-chip flash: VIA layers/RGB settings reset to defaults once.
Re-import the VIA JSON (`gx87_via.json`) and re-set lighting preferences.

## Conclusion

The MCHOSE GX87 appears to be a Chinese-manufactured TKL keyboard with wireless capabilities. Its hardware is similar to many other keyboards on the market, particularly those from Keychron, Royal Kludge, and other Chinese manufacturers. The main distinguishing feature is the custom wireless implementation and power management system, which may be the source of some stability issues.

The keyboard uses standard components (WB32 MCU, SNLED27351 LED driver) that are well-supported by QMK, but the custom wireless implementation adds complexity and potential reliability issues. With some firmware improvements, particularly in power management and wireless handling, this could be a solid wireless TKL option.