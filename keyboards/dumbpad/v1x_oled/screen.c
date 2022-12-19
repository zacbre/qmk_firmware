#include "screen.h"
#include "volume.h"
#include "raw_hid.h"
#include "print.h"
#include <string.h>
#include <stdio.h>

uint8_t       cpu_usage     = 0;
uint8_t       mem_usage     = 0;
uint8_t       gpu_usage     = 0;
uint16_t      process_count = 0;
layer_state_t current_layer;
uint32_t      display_sleep     = 0;
bool          process_macro_key = false;
uint16_t      key_pressed       = 0;
uint16_t      macro_keycode     = 0;

int current_screen = 0;
int max_screens    = 2;

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_0;
}

void reset_timer(void) {
    oled_on();
    display_sleep = timer_read32();
}

bool determine_sleep(void) {
    if (timer_elapsed32(display_sleep) > OLED_E_TIMEOUT) {
        oled_off();
        return true;
    }
    return false;
}

layer_state_t default_layer_state_set_kb(layer_state_t state) {
    current_layer = get_highest_layer(state);
    return state;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    reset_timer();
    current_layer = get_highest_layer(state);
    return state;
}

void draw_layer(void) {
    oled_set_cursor(16, 0);
    char layer[] = {current_layer + '0', '\0'};
    oled_write_P(PSTR("L:  "), false);
    oled_write(&layer[0], false);
}

void draw_macro_usage(void) {
    oled_set_cursor(16, 1);
    char macro[4];
    switch (macro_keycode) {
        case KC_F20:
            strncpy(macro, "F20\0", 4);
            break;
        case KC_F21:
            strncpy(macro, "F21\0", 4);
            break;
        case KC_F22:
            strncpy(macro, "F22\0", 4);
            break;
    }
    oled_write_P(PSTR("M:"), false);
    if (process_macro_key) {
        oled_write(macro, false);
    } else {
        oled_write_P(PSTR(" NO"), false);
    }
}

void draw_cpu_usage(void) {
    oled_set_cursor(0, 0);
    char cpu[2];
    memset(cpu, '\0', 2);
    itoa(cpu_usage, cpu, 10);
    oled_write_P(PSTR("CPU Usage: "), false);
    oled_write(&cpu[0], false);
    oled_write_P(PSTR("%"), false);
}

void draw_mem_usage(void) {
    oled_set_cursor(0, 1);
    char mem[2];
    memset(mem, '\0', 2);
    itoa(mem_usage, mem, 10);
    oled_write_P(PSTR("Mem Usage: "), false);
    oled_write(&mem[0], false);
    oled_write_P(PSTR("%"), false);
}

void draw_gpu_usage(void) {
    oled_set_cursor(0, 2);
    char gpu[2];
    memset(gpu, '\0', 2);
    itoa(gpu_usage, gpu, 10);
    oled_write_P(PSTR("GPU Usage: "), false);
    oled_write(&gpu[0], false);
    oled_write_P(PSTR("%"), false);
}

void draw_process_count(void) {
    oled_set_cursor(0, 3);
    char proc_count[2];
    memset(proc_count, '\0', 2);
    itoa(process_count, proc_count, 10);
    oled_write_P(PSTR("Processes: "), false);
    oled_write(&proc_count[0], false);
}

void draw_volume_mode(void) {
    oled_set_cursor(0, 0);
    oled_write_P(PSTR("Vol Mode: "), false);
    if (encoder_volume_mode) {
        oled_write_P(PSTR("On"), false);
    } else {
        oled_write_P(PSTR("Off"), false);
    }
    oled_set_cursor(0, 1);

    /*if (strlen(current_application) > 13) {
        // truncate what we output.
        char temp_app[16];
        memcpy(&temp_app[0], &current_application[0], 13);
        memcpy(&temp_app[13], &"...", 3);
        oled_write(temp_app, false);
    } else {

    }*/
    oled_write(current_application, false);
    oled_set_cursor(0, 2);
    if (current_volume != 0xFF) {
        char current_vol[2];
        memset(current_vol, '\0', 2);
        itoa(current_volume, current_vol, 10);
        oled_write(&current_vol[0], false);
        oled_write_P(PSTR("%"), false);
    } else if (current_volume == 0) {
        oled_write_P(PSTR("Muted!"), false);
    } else {
        oled_write_P(PSTR("Not running!"), false);
    }
}

// which screen to display.
int  last_screen = 0;
void draw_screen(void) {
    // oled_clear();
    if (last_screen != current_screen) {
        // dprintf("Screen is new, clear old!\n");
        oled_clear();
        last_screen = current_screen;
    }
    switch (current_screen) {
        case 0:
            // stats
            draw_cpu_usage();
            draw_mem_usage();
            draw_gpu_usage();
            draw_process_count();
            break;
        case 1:
            // volume mode
            draw_volume_mode();
            break;
    }
    draw_macro_usage();
    draw_layer();
}

bool oled_task_user(void) {
    if (!determine_sleep()) {
        draw_screen();
    }
    return false;
}

bool check_screen_keys(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_F13:
            if (record->event.pressed) {
                if (encoder_volume_mode) {
                    // mute or unmute application
                    if (current_volume == 0) {
                        send_volume_packet(true, true);
                        get_volume_packet();
                        oled_clear();
                    } else {
                        send_volume_packet(false, true);
                        get_volume_packet();
                        oled_clear();
                    }
                } else {
                    if (current_screen + 1 >= max_screens) {
                        current_screen = 0;
                    } else {
                        current_screen++;
                    }
                }
            }
            return false;
        default:
            return true;
    }
}