# MCHOSE GX87 Development Documentation

This document contains technical details and findings from reverse engineering the MCHOSE GX87 keyboard.

## Hardware Configuration

### LED System

The keyboard uses a sophisticated LED setup with multiple types of indicators:

1. **Main Backlight LEDs**
- Driver: SNLED27351 (dual controller setup)
  - Address 1: 0b1110100 (0x74)
  - Address 2: 0b1110111 (0x77)
- Configuration in `keyboard.json`: `rgb_matrix.driver: "snled27351"`
- Per-key RGB LEDs (marked with `flags: 4` in layout)
- Total of 87 individually addressable RGB LEDs

2. **Special Indicator LEDs** (`flags: 1` in layout)
These LEDs form a multi-function indicator strip:
- Tab key area: `[2, 0]`
- Caps Lock area: `[3, 0]`
- Left Shift area: `[4, 0]`
- Bottom row modifiers: 
  - Left side: `[5, 0]`, `[5, 1]`, `[5, 2]`
  - Right side: `[5, 9]`, `[5, 10]`, `[5, 11]`
- Arrow key area: `[5, 13]`, `[5, 14]`, `[5, 15]`, `[5, 16]`

3. **Dedicated Status LEDs** (`flags: 2` in layout)
Four LEDs at coordinates:
- (181, 42)
- (193, 42)
- (206, 42)
- (218, 42)
Used for: Caps Lock, Num Lock, Scroll Lock, and Wireless/Battery status

### Battery & Power Management

1. **Hardware Pins**
```c
#define BT_CABLE_PIN B8   // High when charging cable connected
#define BT_CHARGE_PIN B9  // Low when charging, High when fully charged
```

2. **Power Control Pins**
```c
#define USB_POWER_EN_PIN A14
#define LED_POWER_EN_PIN A15
```

3. **Mode Switch Pins**
```c
#define BT_MODE_SW_PIN C0  // Low when active
#define RF_MODE_SW_PIN C15 // Low when active
```

### Wireless Module Interface

The keyboard uses a custom wireless module with the following features:

1. **Connection Modes**
- USB
- Bluetooth (up to 3 devices)
- 2.4GHz wireless

2. **Battery Commands**
- Send Commands:
  ```c
  MD_SND_CMD_DEVCTRL_CHARGING      = 0x64  // Battery power control
  MD_SND_CMD_DEVCTRL_CHARGING_STOP = 0x65  // Battery power control
  MD_SND_CMD_DEVCTRL_CHARGING_DONE = 0x66  // Battery power control
  ```
- Receive Commands:
  ```c
  MD_REV_CMD_DEVCTRL_BAT_LOW    = 0x21  // Currently unused
  MD_REV_CMD_DEVCTRL_BAT_NORMAL = 0x23  // Currently unused
  MD_REV_CMD_BATVOL             = 0x5C  // Battery voltage
  ```

3. **Power Management Features**
- Auto sleep after 30 minutes in BT/2.4G modes
- Commands for sleep control:
  ```c
  MD_SND_CMD_DEVCTRL_SLEEP_BT_EN   = 0x55  // 30min timeout in BT mode
  MD_SND_CMD_DEVCTRL_SLEEP_BT_DIS  = 0x56
  MD_SND_CMD_DEVCTRL_SLEEP_2G4_EN  = 0x57  // 30min timeout in 2.4G mode
  MD_SND_CMD_DEVCTRL_SLEEP_2G4_DIS = 0x58
  ```

## Current Implementation Notes

1. **Battery Status**
- Hardware pins (BT_CABLE_PIN, BT_CHARGE_PIN) are defined but not currently used in the QMK firmware
- Battery status is handled through wireless module commands
- Current behavior:
  - Shows green when fully charged
  - Blinks same color as matrix LEDs when not fully charged

2. **Layer Functionality**
- Layer switching works correctly
- Mac OS Alt/Win key swapping functional

3. **VIA Support**
- VIA support has been implemented in the `via` keymap
- Custom keycodes for Bluetooth and 2.4GHz functionality are included
- Four layers are available for customization
- RGB matrix controls are accessible through VIA

## Known Issues

1. **Battery Indication**
- Battery level indication could be improved by using both hardware pins and module commands
- Current indication (blinking same as matrix) is not very useful for battery level monitoring

2. **Fn Key Functions**
- Many Fn key combinations from stock firmware are not implemented in QMK

## Future Improvements

1. **Battery Status**
- Implement proper battery level reading using both hardware pins and module commands
- Create distinct LED patterns for different battery states
- Add battery level reporting to host system

2. **LED Indicators**
- Implement custom LED patterns for different keyboard states
- Better utilize the multi-function indicator strip

3. **Power Management**
- Implement more granular power management settings
- Add user-configurable sleep timeouts

## VIA Implementation and Testing

VIA support has been implemented directly in the default keymap:

1. **Configuration Files**:
   - `keyboards/mchose/gx87/keymaps/default/rules.mk`:
     ```
     VIA_ENABLE = yes
     LTO_ENABLE = yes
     ```
   - `keyboards/mchose/gx87/keymaps/default/config.h`:
     ```c
     #define DYNAMIC_KEYMAP_LAYER_COUNT 4
     #define VIA_CUSTOM_LIGHTING_ENABLE
     ```

2. **JSON Definition**: Using the existing `gx87_via.json` file

### Testing VIA Support

After flashing the firmware with the default keymap:

1. Open [usevia.app](https://usevia.app) in a web browser
2. Click "Authorize device via WebHID"
3. Select the MCHOSE GX87 from the device list
4. The keyboard layout should appear and be fully customizable
5. If the layout doesn't appear automatically, try uploading the `gx87_via.json` file manually

Note: If you're using the VIA desktop app, you may need to add the `gx87_via.json` file to the app's keyboard definitions folder.

To flash the firmware, simply use:
```
make mchose/gx87:default:flash
```

The default keymap now includes VIA support, so there's no need for a separate via target.
