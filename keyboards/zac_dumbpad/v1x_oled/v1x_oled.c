#include "quantum.h"
#include "v1x_oled.h"
#include "screen.h"
#include "macro.h"
#include "volume.h"
#include "raw_hid.h"
#include "print.h"
#include <string.h>
#include <stdio.h>

bool encoder_update_user(uint8_t index, bool clockwise) {
    reset_timer();
    // Right encoder
    if (index == 0) {
        if (!clockwise) {
            if (encoder_volume_mode) {
                send_volume_packet(true, false);
                get_volume_packet();
                oled_clear();
            } else {
                if (current_layer + 1 <= DYNAMIC_KEYMAP_LAYER_COUNT - 1) {
                    layer_move(current_layer+1);
                } else {
                    layer_move(0);
                }
            }
        } else {
            if (encoder_volume_mode) {
                send_volume_packet(false, false);
                get_volume_packet();
                oled_clear();
            } else {
                if (current_layer - 1 >= 0) {
                    layer_move(current_layer-1);
                } else {
                    layer_move(DYNAMIC_KEYMAP_LAYER_COUNT - 1);
                }
            }
        }
    }
    return false;
}

void keyboard_post_init_user(void) {
    debug_enable=true;
    current_application = malloc(sizeof(char) * 16);
    memset(current_application, 0, 16);
    memcpy(&current_application[0], "no app selected", 15);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    reset_timer();
    if (!check_volume_fn_key(keycode, record)) {
      return false;
    }
    if (!check_screen_keys(keycode, record)) {
      return false;
    }
    if (!check_macro_keys(keycode, record)) {
      return false;
    }
    return true;
}

bool via_command_kb(uint8_t *data, uint8_t length) {
    uint8_t *packet_header = &(data[1]);
    uint8_t packet_data[30];
    memcpy(packet_data, &data[2], sizeof(packet_data));
    switch (*packet_header) {
        case stats_header:
            cpu_usage = packet_data[0];
            mem_usage = packet_data[1];
            process_count = packet_data[3] | (packet_data[2] << 8);
            gpu_usage = packet_data[4];
            oled_clear();
            break;
        case get_volume_header:
            current_volume = packet_data[0];
            if (current_volume == 0xff) {
                encoder_volume_mode = 0;
            }
            for (int i = 1; i < 30; i++) {
                if (packet_data[i] == 0x00) {
                    memcpy(&current_application[0], &packet_data[1], 16); // max 16 chars
                }
            }
            oled_clear();
            break;
        default:
            return false;
    }
    return true;
}