/*
Copyright 2020 imchipwood
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once


#define KC_FN0 0x00C0
#define KC_FN1 0x00C1
#define KC_FN2 0x00C2
#define KC_FN3 0x00C3
#define KC_FN4 0x00C4
#define KC_FN5 0x00C5
#define KC_FN6 0x00C6
#define KC_FN7 0x00C7
#define KC_FN8 0x00C8
#define KC_FN9 0x00C9
#define KC_FN10 0x00CA
#define KC_FN11 0x00CB
#define KC_FN12 0x00CC
#define KC_FN13 0x00CD
#define KC_FN14 0x00CE
#define KC_FN15 0x00CF
#define KC_FN16 0x00D0
#define KC_FN17 0x00D1
#define KC_FN18 0x00D2
#define KC_FN19 0x00D3
#define KC_FN20 0x00D4
#define KC_FN21 0x00D5
#define KC_FN22 0x00D6
#define KC_FN23 0x00D7
#define KC_FN24 0x00D8
#define KC_FN25 0x00D9
#define KC_FN26 0x00DA
#define KC_FN27 0x00DB
#define KC_FN28 0x00DC
#define KC_FN29 0x00DD
#define KC_FN30 0x00DE
#define KC_FN31 0x00DF

#define DYNAMIC_KEYMAP_LAYER_COUNT 5

/* Column/Row IO definitions */
/* //vertical
#define MATRIX_ROWS 5
#define MATRIX_COLS 4
#define MATRIX_ROW_PINS { C6, B6, B2, B3, B1 }
#define MATRIX_COL_PINS { F7, F6, F5, F4 }*/

//horizontal
#define MATRIX_ROWS 4
#define MATRIX_COLS 5
#define MATRIX_ROW_PINS { F4, F5, F6, F7 }
#define MATRIX_COL_PINS { C6, B1, B3, B2, B6 }

//#define UNUSED_PINS

/* Dual rotary encoders */
#define ENCODERS_PAD_A { B4, E6 }
#define ENCODERS_PAD_B { B5, D7 }

/* Column/Row IO definitions */
#define DIODE_DIRECTION COL2ROW

/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCE 5

/* Reduce tapdance required taps from 5 to 2 */
#define TAPPING_TOGGLE 2
