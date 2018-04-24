#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "../common/common.h"
#include "../tty/tty.h"

// Скан-коды стандартной клавиатуры

#define KEY_ESC          0x01 // ESCAPE
    
#define KEY_F1           0x3b // F1
#define KEY_F2           0x3c // F2
#define KEY_F3           0x3d // F3
#define KEY_F4           0x3e // F4
#define KEY_F5           0x3f // F5
#define KEY_F6           0x40 // F6
#define KEY_F7           0x41 // F7
#define KEY_F8           0x42 // F8
#define KEY_F9           0x43 // F9
#define KEY_F10          0x44 // F10
#define KEY_F11          0x45 // F11
#define KEY_F12          0x46 // F12

#define KEY_SYSRQ        // Print Screen
#define KEY_SCROLL_LOCK  // Scroll Lock
#define KEY_PAUSE        // Pause

#define KEY_GRAVE        0x29 // ` и ~
#define KEY_1            0x02 // 1 и !        
#define KEY_2            0x03 // 2 и @
#define KEY_3            0x04 // 3 и #
#define KEY_4            0x05 // 4 и $
#define KEY_5            0x06 // 5 и %
#define KEY_6            0x07 // 6 и ^
#define KEY_7            0x08 // 7 и &
#define KEY_8            0x09 // 8 и *
#define KEY_9            0x0a // 9 и (
#define KEY_0            0x0b // 0 и )

#define KEY_MINUS        0x0c // - и _
#define KEY_EQUAL        0x0d // = и +
#define KEY_BACKSPACE    0x0e // Backspace

#define KEY_TAB          0x0f // Tab
#define KEY_Q            0x10 // q и Q
#define KEY_W            0x11 // w и W
#define KEY_E            0x12 // e и E
#define KEY_R            0x13 // r и R
#define KEY_T            0x14 // t и T
#define KEY_Y            0x15 // y и Y
#define KEY_U            0x16 // u и U
#define KEY_I            0x17 // i и I
#define KEY_O            0x18 // o и O
#define KEY_P            0x19 // p и P
#define KEY_LEFT_BRACE   0x1a // [ и {
#define KEY_RIGHT_BRACE  0x1b // ] и }
#define KEY_BACKSLASH    0x2b // \ и |

#define KEY_CAPSLOCK     0x3a // Caps Lock
#define KEY_A            0x1e // a и A
#define KEY_S            0x1f // s и S
#define KEY_D            0x20 // d и D
#define KEY_F            0x21 // f и F
#define KEY_G            0x22 // g и G
#define KEY_H            0x23 // h и H
#define KEY_J            0x24 // j и J
#define KEY_K            0x25 // k и K
#define KEY_L            0x26 // l и L
#define KEY_SEMICOLON    0x27 // ; и :
#define KEY_APOSTROPHE   0x28 // ' и "
#define KEY_ENTER        0x1c // Return (ENTER)

#define KEY_LEFT_SHIFT   0x2a // Left Shift
#define KEY_Z            0x2c // z и Z
#define KEY_X            0x2d // x и X
#define KEY_C            0x2e // c и C
#define KEY_V            0x2f // v и V
#define KEY_B            0x30 // b и B
#define KEY_N            0x31 // n и N
#define KEY_M            0x32 // m и M    
#define KEY_COMMA        0x33 // , и <
#define KEY_DOT          0x34 // . и >
#define KEY_SLASH        0x35 // / и ?
#define KEY_RIGHT_SHIFT  0x36 // Right Shift

#define KEY_LEFT_CTRL    0x1d // Left Control
#define KEY_LEFT_ALT     0x38 // Left Alt
#define KEY_SPACEBAR     0x39 // Spacebar
#define KEY_RIGHT_ALT    0x38 // Right Alt
#define KEY_RIGHT_CTRL   0x1d // Right Control

enum KEYBOARD_FLAGS
{
    LEFT_SHIFT_PRESSED,
    RIGHT_SHIFT_PRESSED,
    LEFT_CTRL_PRESSED,
    RIGHT_CTRL_PRESSED,
    LEFT_ALT_PRESSED,
    RIGHT_ALT_PRESSED,
    CAPSLOCK_PRESSED
};

void keyboard_dispatcher();

#endif /* _KEYBOARD_H_ */
