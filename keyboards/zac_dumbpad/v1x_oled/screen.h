#pragma once

#include <string.h>
#include <stdio.h>
#include "quantum.h"

#define NUMBER_OF_SCREENS 3
#define OLED_E_TIMEOUT 300000

extern uint8_t cpu_usage;
extern uint8_t mem_usage;
extern uint8_t gpu_usage;
extern uint16_t process_count;
extern layer_state_t current_layer;
extern uint32_t display_sleep;
extern bool process_macro_key;
extern uint16_t key_pressed;
extern uint16_t macro_keycode;

extern int current_screen;
extern int max_screens;

void reset_timer(void);

bool check_screen_keys(uint16_t keycode, keyrecord_t *record);