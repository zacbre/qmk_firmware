#include "macro.h" 
uint32_t unsend_key(uint32_t trigger_time, void *cb_arg) {
  uint16_t keycode = *(uint16_t*)cb_arg;
  unregister_code(keycode);
  free(cb_arg);
  return 0;
}

uint32_t send_key(uint32_t trigger_time, void *cb_arg) {
  uint16_t converted_key = *(uint16_t*)cb_arg;
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

  //dprintf("0x%04X send_macro\n", *(uint16_t*)cb_arg);

  switch (*(uint16_t*)cb_arg) {
  case KC_F20:
    /*dprintf("0x%04X send_macro UP\n", KC_MS_UP);
    press_key(KC_MS_UP, 10);
    dprintf("0x%04X send_macro DOWN\n", KC_MS_DOWN);
    press_key(KC_MS_DOWN, 20);*/
    //dprintf("0x%04X send_macro LEFT\n", KC_MS_LEFT);
    press_key(KC_MS_LEFT, 10);
    //dprintf("0x%04X send_macro RIGHT\n", KC_MS_RIGHT);
    press_key(KC_MS_RIGHT, 20);
    break;
  case KC_F21:
    //dprintf("0x%04X send_macro LCTL\n", KC_LCTL);
    press_key(KC_LCTL, 1);
    //dprintf("0x%04X send_macro SPC\n", KC_SPC);
    press_key(KC_SPC, 1);
    break;
  }
  return 1000;
}

bool check_macro_keys(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_F20:
        case KC_F21:
        case KC_F22:
        case KC_F23:
        case KC_F24:
            //dprintf("0x%04X process_record_user\n", keycode);
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
            return true;
    }
}
