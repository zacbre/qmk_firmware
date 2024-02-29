#include "volume.h"
#include "raw_hid.h"
#include <stdlib.h>

bool encoder_volume_mode = false;
char *current_application = "";
uint16_t selected_app = 0x0000;

int current_volume = 0;

void get_volume_packet(void) {
    dprintf("Sending get volume packet!\n");
    uint8_t packet_data[32];
    memset(packet_data, '\0', sizeof(packet_data));
    packet_data[0] = get_volume_header;
    memcpy(&packet_data[1], &selected_app, sizeof(uint16_t));
    dprintf("Packet: ");
    for (int i = 0; i < sizeof(packet_data); i++) {
      dprintf("0x%02X ", packet_data[i]);
    }
    dprintf("\n");
    raw_hid_send(packet_data, sizeof(packet_data));
}

void send_volume_packet(bool vol_up, bool force) {
    uint8_t packet_data[32];
    memset(packet_data, '\0', sizeof(packet_data));
    if (force) {
      packet_data[0] = force_set_volume_header;
    } else {
      packet_data[0] = set_volume_header;
    }
    memcpy(&packet_data[1], &selected_app, sizeof(uint16_t));
    packet_data[3] = 0x00;
    if (vol_up) {
      packet_data[3] = 0x01;
    }
    raw_hid_send(packet_data, sizeof(packet_data));
}

bool check_volume_fn_key(uint16_t keycode, keyrecord_t *record) {
    switch(keycode) {
        case KC_FN0:
        case KC_FN1:
        case KC_FN2:
        case KC_FN3:
        case KC_FN4:
        case KC_FN5:
        case KC_FN6:
        case KC_FN7:
        case KC_FN8:
        case KC_FN9:
        case KC_FN10:
        case KC_FN11:
        case KC_FN12:
        case KC_FN13:
        case KC_FN14:
        case KC_FN15:
        case KC_FN16:
        case KC_FN17:
        case KC_FN18:
        case KC_FN19:
        case KC_FN20:
        case KC_FN21:
        case KC_FN22:
        case KC_FN23:
        case KC_FN24:
        case KC_FN25:
        case KC_FN26:
        case KC_FN27:
        case KC_FN28:
        case KC_FN29:
        case KC_FN30:
        case KC_FN31:
            if (record->event.pressed) {
                if(selected_app != keycode) {
                    encoder_volume_mode = true;
                } else {
                    encoder_volume_mode = !encoder_volume_mode;
                }
                selected_app = keycode;
                get_volume_packet();
                oled_clear();
                return false;
            }
            return false;
        default:
            return true;
    }
}