// Lighting modes header file
// Copyright 2024

#pragma once

#include "quantum.h"

// Function declarations for lighting modes
void apply_lighting_mode_default(void);
void apply_lighting_mode_minimal(void);
void apply_lighting_mode_subtle(void);
void apply_lighting_mode_layers(void);

// Mode management functions
uint8_t get_lighting_mode(void);
void cycle_lighting_mode(void);
void set_lighting_mode(uint8_t mode);
void lighting_init(void);
void apply_current_lighting_mode(void);
