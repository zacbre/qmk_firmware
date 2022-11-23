#include "v1x_oled.h"
#include "raw_hid.h"
#include "print.h"
#include <string.h>
#include <stdio.h>

#define ENABLE_MACRO_SPAM
#define HID_ENABLE

uint8_t cpu_usage = 0;
uint8_t mem_usage = 0;
uint16_t process_count = 0;

#ifdef OLED_ENABLE
uint32_t display_sleep = 0;
bool process_macro_key = false;
uint16_t key_pressed = 0;
uint16_t macro_keycode = 0;

void reset_timer(void) {
    oled_on();
    display_sleep = timer_read32();
}

bool determine_sleep(void) {
    if (timer_elapsed32(display_sleep) > OLED_TIMEOUT) {
        oled_off();
        return true;
    }
    return false;
}
layer_state_t current_layer;
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
    oled_set_cursor(0, 0);
    char layer[] = {current_layer+'0','\0'};
    oled_write_P(PSTR("L:  "), false);
    oled_write(&layer[0], false);
}

void draw_macro_usage(void) {
    oled_set_cursor(0, 1);
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
    oled_set_cursor(0, 2);
    char cpu[2];
    memset(cpu, '\0', 2);
    itoa(cpu_usage,cpu,10);
    if (cpu_usage >= 100) {
        oled_write_P(PSTR("C:"), false);
    } else if (cpu_usage >= 10) {
        oled_write_P(PSTR("C: "), false);
    } else {
        oled_write_P(PSTR("C:  "), false);
    }
    oled_write(&cpu[0], false);
}

void draw_mem_usage(void) {
    oled_set_cursor(0, 3);
    char mem[2];
    memset(mem, '\0', 2);
    itoa(mem_usage,mem,10);
    if (mem_usage >= 100) {
        oled_write_P(PSTR("M:"), false);
    } else if (mem_usage >= 10) {
        oled_write_P(PSTR("M: "), false);
    } else {
        oled_write_P(PSTR("M:  "), false);
    }
    oled_write(&mem[0], false);
}

void draw_process_count(void) {
    oled_set_cursor(0, 4);
    char proc_count[2];
    memset(proc_count, '\0', 2);
    itoa(process_count,proc_count,10);
    if (process_count >= 100) {
        oled_write_P(PSTR("P:"), false);
    } else if (process_count >= 10) {
        oled_write_P(PSTR("P: "), false);
    } else {
        oled_write_P(PSTR("P:  "), false);
    }
    oled_write(&proc_count[0], false);
}

void draw_screen(void) {
    draw_layer();
    draw_macro_usage();
    draw_cpu_usage();
    draw_mem_usage();
    draw_process_count();
}

uint8_t current_layer = 0;

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}

bool oled_task_user(void) {
    if (!determine_sleep()) {
        draw_screen();
    }
    return false;
}
#endif

#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
#ifdef OLED_ENABLE
    reset_timer();
#endif
    // Right encoder
    if (index == 0) {
        if (!clockwise) {
#ifdef OLED_ENABLE
            if (current_layer + 1 <= DYNAMIC_KEYMAP_LAYER_COUNT - 1) {
                layer_move(current_layer+1);
            } else {
                layer_move(0);
            }
#else
            tap_code(KC_VOLU);
#endif
        } else {
#ifdef OLED_ENABLE
            if (current_layer - 1 >= 0) {
                layer_move(current_layer-1);
            } else {
                layer_move(DYNAMIC_KEYMAP_LAYER_COUNT - 1);
            }
#else
            tap_code(KC_VOLU);
#endif
        }
    }
    return true;
}
#endif

void keyboard_post_init_user(void) {
    debug_enable=true;
    //debug_matrix=true;
    //debug_keyboard=true;
    //debug_mouse=true;
}

#ifdef ENABLE_MACRO_SPAM
uint32_t unsend_key(uint32_t trigger_time, void *cb_arg) {
    uint16_t keycode = *(uint16_t*)cb_arg;
    dprintf("0x%04X unregister cb_arg\n", cb_arg);
    dprintf("0x%04X unregister keycode\n", keycode);
    unregister_code(keycode);
    free(cb_arg);
    return 0;
}

uint32_t send_key(uint32_t trigger_time, void *cb_arg) {
    uint16_t converted_key = *(uint16_t*)cb_arg;
    dprintf("0x%04X register keycode\n", converted_key);
    register_code(converted_key);

    defer_exec(10, unsend_key, cb_arg);
    return 0;
}

void press_key(uint16_t key, uint32_t defer_time) {
    uint16_t *allocated_key = malloc(sizeof(uint16_t));
    allocated_key[0] = key;
    defer_exec(defer_time, send_key, allocated_key);
}

uint32_t send_macro(uint32_t trigger_time, void *cb_arg) {
    if (!process_macro_key) {
        free(cb_arg);
        return 0;
    }

    dprintf("0x%04X send_macro\n", *(uint16_t*)cb_arg);

    switch (*(uint16_t*)cb_arg) {
        case KC_F20:
            /*dprintf("0x%04X send_macro UP\n", KC_MS_UP);
            press_key(KC_MS_UP, 10);
            dprintf("0x%04X send_macro DOWN\n", KC_MS_DOWN);
            press_key(KC_MS_DOWN, 20);*/
            dprintf("0x%04X send_macro LEFT\n", KC_MS_LEFT);
            press_key(KC_MS_LEFT, 10);
            dprintf("0x%04X send_macro RIGHT\n", KC_MS_RIGHT);
            press_key(KC_MS_RIGHT, 20);
            break;
        case KC_F21:
            dprintf("0x%04X send_macro LCTL\n", KC_LCTL);
            press_key(KC_LCTL, 1);
            dprintf("0x%04X send_macro SPC\n", KC_SPC);
            press_key(KC_SPC, 1);
            break;
    }
    return 1000;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef OLED_ENABLE
    reset_timer();
#endif
    switch (keycode) {
        case KC_F20:
        case KC_F21:
        case KC_F22:
        case KC_F23:
        case KC_F24:
            dprintf("0x%04X process_record_user\n", keycode);
            if (record->event.pressed) {
                process_macro_key = !process_macro_key;
                if (process_macro_key) {
                    macro_keycode = keycode;
                    uint16_t *pressed_key = malloc(sizeof(uint16_t));
                    pressed_key[0] = keycode;
                    defer_exec(1, send_macro, pressed_key);
                } else {
                    macro_keycode = 0;
                }
            }
            return false;
        default:
            return true; // Process all other keycodes normally
    }
}
#endif

#ifdef HID_ENABLE
float get_f64(uint8_t *data, uint32_t index) {
    float fl;
    memcpy(&fl, &data[index], sizeof(float));
    return fl;
}

void raw_hid_receive_kb(uint8_t *data, uint8_t length) {
    dprintf("raw_receive_kb!\n");
    for (int i = 0; i < length; i++) {
        dprintf("0x%02X ", data[i]);
    }
    dprintf("\n");

    uint8_t *packet_header = &(data[1]);

    uint8_t packet_data[30];
    memcpy(packet_data, &data[2], sizeof(packet_data));
    for (int i = 0; i < sizeof(packet_data); i++) {
        dprintf("0x%02X ", packet_data[i]);
    }
    dprintf("\n");
    switch (*packet_header) {
        case 0x01:
            // cpu usage
            dprintf("CPU Usage: %d\n", packet_data[0]);
            cpu_usage = packet_data[0];
            break;
        case 0x02:
            dprintf("Mem Usage: %d\n", packet_data[0]);
            mem_usage = packet_data[0];
            break;
        case 0x03:
            process_count = packet_data[1] | (packet_data[0] << 8);
            dprintf("Process Count: %d\n", process_count);
            break;
    }
    /*dprintf("\n");
    char xxd[32];
    truncate_first_bytes(data, length, xxd);
    dprintf("Actual recv: '%s'\n", xxd);
    char c[32] = "ayy lma0";
    for (int i = 0; i < sizeof(c); i++) {
        dprintf("0x%02X ", c[i]);
    }
    dprintf("\n");
    dprintf("Actual send: '%s'\n", c);
    uint8_t *u = (uint8_t *)&c;
    raw_hid_send(u, sizeof(c));*/
}
#endif