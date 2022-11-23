/* Copyright 2020 imchipwood
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

#define KC_COPY   LCTL(KC_C)
#define KC_CUT    LCTL(KC_X)
#define KC_PASTE  LCTL(KC_V)
#define KC_UNDO  LCTL(KC_Z)
#define KC_REDO  LCTL(KC_Y)
#define KC_OREDO  LCTL(LSFT(KC_Z))

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
            BASE LAYER
    /-----------------------------------------------------`
    |             |    7    |    8    |    9    |  Bkspc  |
    |             |---------|---------|---------|---------|
    |             |    4    |    5    |    6    |   Esc   |
    |             |---------|---------|---------|---------|
    |             |    1    |    2    |    3    |   Tab   |
    |-------------|---------|---------|---------|---------|
    | Play Pause  |  TT(1)  |    0    |    .    |  Enter  |
    \-----------------------------------------------------'
    */
    [0] = LAYOUT(
                    KC_CUT,   KC_COPY,   KC_PASTE, _______,
                    KC_UNDO,  KC_REDO,   KC_OREDO, KC_6,
                    KC_TAB,   KC_1,      KC_2,     KC_EQL,
        KC_MUTE,    _______,  KC_LOCK,   KC_0,     KC_LSHIFT
    ),
    /*
            SUB LAYER
    /-----------------------------------------------------`
    |             |         |         |         |  Reset  |
    |             |---------|---------|---------|---------|
    |             |         |         |         |    +    |
    |             |---------|---------|---------|---------|
    |             |         |         |         |    -    |
    |-------------|---------|---------|---------|---------|
    |    LOCK     |         |         |         |    =    |
    \-----------------------------------------------------'
    */
    [1] = LAYOUT(
                    KC_F20 ,     KC_F21 ,     KC_F22 ,      _______,
                    _______,     _______,     _______,      KC_KP_PLUS,
                    _______,     _______,     _______,      KC_KP_MINUS,
        _______,    _______,     _______,     _______,      KC_EQL
    ),

    [2] = LAYOUT(
                    _______,     _______,     _______,      _______,
                    _______,     _______,     _______,      _______,
                    _______,     _______,     _______,      _______,
        _______,    _______,     _______,     _______,      _______
    ),

    [3] = LAYOUT(
                    _______,     _______,     _______,      _______,
                    _______,     _______,     _______,      _______,
                    _______,     _______,     _______,      _______,
        _______,    _______,     _______,     _______,      _______
    ),
    [4] = LAYOUT(
                   KC_MS_BTN2,   KC_MS_UP,    KC_MS_BTN1,   KC_MS_WH_UP,
                   KC_MS_LEFT,   KC_MS_DOWN,  KC_MS_RIGHT,  KC_MS_WH_DOWN,
                   _______,      KC_UP,       _______,      _______,
        _______,   KC_LEFT,      KC_DOWN,     KC_RIGHT,     _______
        ),
};
