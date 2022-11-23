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

#include "config_common.h"

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
