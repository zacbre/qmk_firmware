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
    return true;
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

void raw_hid_receive_kb(uint8_t *data, uint8_t length) {
    //dprintf("raw_receive_kb!\n");
    /*for (int i = 0; i < length; i++) {
        dprintf("0x%02X ", data[i]);
    }
    dprintf("\n");*/

    uint8_t *packet_header = &(data[1]);

    uint8_t packet_data[30];
    //dprintf("Received packet!: 0x%02X\n", packet_header);
    memcpy(packet_data, &data[2], sizeof(packet_data));
    //for (int i = 0; i < sizeof(packet_data); i++) {
    //    dprintf("0x%02X ", packet_data[i]);
    //}
    //dprintf("\n");
    switch (*packet_header) {
        case cpu_usage_header:
            // cpu usage
            //dprintf("CPU Usage: %d\n", packet_data[0]);
            cpu_usage = packet_data[0];
            oled_clear();
            break;
        case mem_usage_header:
            //dprintf("Mem Usage: %d\n", packet_data[0]);
            mem_usage = packet_data[0];
            oled_clear();
            break;
        case proc_count_header:
            process_count = packet_data[1] | (packet_data[0] << 8);
            //dprintf("Process Count: %d\n", process_count);
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
        case gpu_utilization_header:
            gpu_usage = packet_data[0];
            oled_clear();
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