#include "common.h"

const char* rose_keycode_to_string(rose_keycode key) {
    switch (key) {
        case ROSE_KEYCODE_A:
            return "a";
            break;
        case ROSE_KEYCODE_B:
            return "b";
            break;
        case ROSE_KEYCODE_C:
            return "c";
            break;
        case ROSE_KEYCODE_D:
            return "d";
            break;
        case ROSE_KEYCODE_E:
            return "e";
            break;
        case ROSE_KEYCODE_F:
            return "f";
            break;
        case ROSE_KEYCODE_G:
            return "g";
            break;
        case ROSE_KEYCODE_H:
            return "h";
            break;
        case ROSE_KEYCODE_I:
            return "i";
            break;
        case ROSE_KEYCODE_J:
            return "j";
            break;
        case ROSE_KEYCODE_K:
            return "k";
            break;
        case ROSE_KEYCODE_L:
            return "l";
            break;
        case ROSE_KEYCODE_M:
            return "m";
            break;
        case ROSE_KEYCODE_N:
            return "n";
            break;
        case ROSE_KEYCODE_O:
            return "o";
            break;
        case ROSE_KEYCODE_P:
            return "p";
            break;
        case ROSE_KEYCODE_Q:
            return "q";
            break;
        case ROSE_KEYCODE_R:
            return "r";
            break;
        case ROSE_KEYCODE_S:
            return "s";
            break;
        case ROSE_KEYCODE_T:
            return "t";
            break;
        case ROSE_KEYCODE_U:
            return "u";
            break;
        case ROSE_KEYCODE_V:
            return "v";
            break;
        case ROSE_KEYCODE_W:
            return "w";
            break;
        case ROSE_KEYCODE_X:
            return "x";
            break;
        case ROSE_KEYCODE_Y:
            return "y";
            break;
        case ROSE_KEYCODE_Z:
            return "z";
            break;
        case ROSE_KEYCODE_1:
            return "1";
            break;
        case ROSE_KEYCODE_2:
            return "2";
            break;
        case ROSE_KEYCODE_3:
            return "3";
            break;
        case ROSE_KEYCODE_4:
            return "4";
            break;
        case ROSE_KEYCODE_5:
            return "5";
            break;
        case ROSE_KEYCODE_6:
            return "6";
            break;
        case ROSE_KEYCODE_7:
            return "7";
            break;
        case ROSE_KEYCODE_8:
            return "8";
            break;
        case ROSE_KEYCODE_9:
            return "9";
            break;
        case ROSE_KEYCODE_0:
            return "0";
            break;
        case ROSE_KEYCODE_RETURN:
            return "return";
            break;
        case ROSE_KEYCODE_ESCAPE:
            return "escape";
            break;
        case ROSE_KEYCODE_BACKSPACE:
            return "backspace";
            break;
        case ROSE_KEYCODE_TAB:
            return "tab";
            break;
        case ROSE_KEYCODE_SPACE:
            return "space";
            break;
        case ROSE_KEYCODE_MINUS:
            return "minus";
            break;
        case ROSE_KEYCODE_EQUALS:
            return "equals";
            break;
        case ROSE_KEYCODE_LEFTBRACKET:
            return "leftbracket";
            break;
        case ROSE_KEYCODE_RIGHTBRACKET:
            return "rightbracket";
            break;
        case ROSE_KEYCODE_BACKSLASH:
            return "backslash";
            break;
        case ROSE_KEYCODE_NONUSHASH:
            return "nonushash";
            break;
        case ROSE_KEYCODE_SEMICOLON:
            return "semicolon";
            break;
        case ROSE_KEYCODE_APOSTROPHE:
            return "apostrophe";
            break;
        case ROSE_KEYCODE_GRAVE:
            return "grave";
            break;
        case ROSE_KEYCODE_COMMA:
            return "comma";
            break;
        case ROSE_KEYCODE_PERIOD:
            return "period";
            break;
        case ROSE_KEYCODE_SLASH:
            return "slash";
            break;
        case ROSE_KEYCODE_CAPSLOCK:
            return "capslock";
            break;
        case ROSE_KEYCODE_F1:
            return "f1";
            break;
        case ROSE_KEYCODE_F2:
            return "f2";
            break;
        case ROSE_KEYCODE_F3:
            return "f3";
            break;
        case ROSE_KEYCODE_F4:
            return "f4";
            break;
        case ROSE_KEYCODE_F5:
            return "f5";
            break;
        case ROSE_KEYCODE_F6:
            return "f6";
            break;
        case ROSE_KEYCODE_F7:
            return "f7";
            break;
        case ROSE_KEYCODE_F8:
            return "f8";
            break;
        case ROSE_KEYCODE_F9:
            return "f9";
            break;
        case ROSE_KEYCODE_F10:
            return "f10";
            break;
        case ROSE_KEYCODE_F11:
            return "f11";
            break;
        case ROSE_KEYCODE_F12:
            return "f12";
            break;
        case ROSE_KEYCODE_PRINTSCREEN:
            return "printscreen";
            break;
        case ROSE_KEYCODE_SCROLLLOCK:
            return "scrolllock";
            break;
        case ROSE_KEYCODE_PAUSE:
            return "pause";
            break;
        case ROSE_KEYCODE_INSERT:
            return "insert";
            break;
        case ROSE_KEYCODE_HOME:
            return "home";
            break;
        case ROSE_KEYCODE_PAGEUP:
            return "pageup";
            break;
        case ROSE_KEYCODE_DELETE:
            return "delete";
            break;
        case ROSE_KEYCODE_END:
            return "end";
            break;
        case ROSE_KEYCODE_PAGEDOWN:
            return "pagedown";
            break;
        case ROSE_KEYCODE_RIGHT:
            return "right";
            break;
        case ROSE_KEYCODE_LEFT:
            return "left";
            break;
        case ROSE_KEYCODE_DOWN:
            return "down";
            break;
        case ROSE_KEYCODE_UP:
            return "up";
            break;
        case ROSE_KEYCODE_NUMLOCKCLEAR:
            return "numlockclear";
            break;
        case ROSE_KEYCODE_KP_DIVIDE:
            return "kp_divide";
            break;
        case ROSE_KEYCODE_KP_MULTIPLY:
            return "kp_multiply";
            break;
        case ROSE_KEYCODE_KP_MINUS:
            return "kp_minus";
            break;
        case ROSE_KEYCODE_KP_PLUS:
            return "kp_plus";
            break;
        case ROSE_KEYCODE_KP_ENTER:
            return "kp_enter";
            break;
        case ROSE_KEYCODE_KP_1:
            return "kp_1";
            break;
        case ROSE_KEYCODE_KP_2:
            return "kp_2";
            break;
        case ROSE_KEYCODE_KP_3:
            return "kp_3";
            break;
        case ROSE_KEYCODE_KP_4:
            return "kp_4";
            break;
        case ROSE_KEYCODE_KP_5:
            return "kp_5";
            break;
        case ROSE_KEYCODE_KP_6:
            return "kp_6";
            break;
        case ROSE_KEYCODE_KP_7:
            return "kp_7";
            break;
        case ROSE_KEYCODE_KP_8:
            return "kp_8";
            break;
        case ROSE_KEYCODE_KP_9:
            return "kp_9";
            break;
        case ROSE_KEYCODE_KP_0:
            return "kp_0";
            break;
        case ROSE_KEYCODE_KP_PERIOD:
            return "kp_period";
            break;
        case ROSE_KEYCODE_NONUSBACKSLASH:
            return "nonusbackslash";
            break;
        case ROSE_KEYCODE_APPLICATION:
            return "application";
            break;
        case ROSE_KEYCODE_POWER:
            return "power";
            break;
        case ROSE_KEYCODE_KP_EQUALS:
            return "kp_equals";
            break;
        case ROSE_KEYCODE_F13:
            return "f13";
            break;
        case ROSE_KEYCODE_F14:
            return "f14";
            break;
        case ROSE_KEYCODE_F15:
            return "f15";
            break;
        case ROSE_KEYCODE_F16:
            return "f16";
            break;
        case ROSE_KEYCODE_F17:
            return "f17";
            break;
        case ROSE_KEYCODE_F18:
            return "f18";
            break;
        case ROSE_KEYCODE_F19:
            return "f19";
            break;
        case ROSE_KEYCODE_F20:
            return "f20";
            break;
        case ROSE_KEYCODE_F21:
            return "f21";
            break;
        case ROSE_KEYCODE_F22:
            return "f22";
            break;
        case ROSE_KEYCODE_F23:
            return "f23";
            break;
        case ROSE_KEYCODE_F24:
            return "f24";
            break;
        case ROSE_KEYCODE_EXECUTE:
            return "execute";
            break;
        case ROSE_KEYCODE_HELP:
            return "help";
            break;
        case ROSE_KEYCODE_MENU:
            return "menu";
            break;
        case ROSE_KEYCODE_SELECT:
            return "select";
            break;
        case ROSE_KEYCODE_STOP:
            return "stop";
            break;
        case ROSE_KEYCODE_AGAIN:
            return "again";
            break;
        case ROSE_KEYCODE_UNDO:
            return "undo";
            break;
        case ROSE_KEYCODE_CUT:
            return "cut";
            break;
        case ROSE_KEYCODE_COPY:
            return "copy";
            break;
        case ROSE_KEYCODE_PASTE:
            return "paste";
            break;
        case ROSE_KEYCODE_FIND:
            return "find";
            break;
        case ROSE_KEYCODE_MUTE:
            return "mute";
            break;
        case ROSE_KEYCODE_VOLUMEUP:
            return "volumeup";
            break;
        case ROSE_KEYCODE_VOLUMEDOWN:
            return "volumedown";
            break;
        case ROSE_KEYCODE_KP_COMMA:
            return "kp_comma";
            break;
        case ROSE_KEYCODE_KP_EQUALSAS400:
            return "kp_equalsas400";
            break;
        case ROSE_KEYCODE_INTERNATIONAL1:
            return "international1";
            break;
        case ROSE_KEYCODE_INTERNATIONAL2:
            return "international2";
            break;
        case ROSE_KEYCODE_INTERNATIONAL3:
            return "international3";
            break;
        case ROSE_KEYCODE_INTERNATIONAL4:
            return "international4";
            break;
        case ROSE_KEYCODE_INTERNATIONAL5:
            return "international5";
            break;
        case ROSE_KEYCODE_INTERNATIONAL6:
            return "international6";
            break;
        case ROSE_KEYCODE_INTERNATIONAL7:
            return "international7";
            break;
        case ROSE_KEYCODE_INTERNATIONAL8:
            return "international8";
            break;
        case ROSE_KEYCODE_INTERNATIONAL9:
            return "international9";
            break;
        case ROSE_KEYCODE_LANG1:
            return "lang1";
            break;
        case ROSE_KEYCODE_LANG2:
            return "lang2";
            break;
        case ROSE_KEYCODE_LANG3:
            return "lang3";
            break;
        case ROSE_KEYCODE_LANG4:
            return "lang4";
            break;
        case ROSE_KEYCODE_LANG5:
            return "lang5";
            break;
        case ROSE_KEYCODE_LANG6:
            return "lang6";
            break;
        case ROSE_KEYCODE_LANG7:
            return "lang7";
            break;
        case ROSE_KEYCODE_LANG8:
            return "lang8";
            break;
        case ROSE_KEYCODE_LANG9:
            return "lang9";
            break;
        case ROSE_KEYCODE_ALTERASE:
            return "alterase";
            break;
        case ROSE_KEYCODE_SYSREQ:
            return "sysreq";
            break;
        case ROSE_KEYCODE_CANCEL:
            return "cancel";
            break;
        case ROSE_KEYCODE_CLEAR:
            return "clear";
            break;
        case ROSE_KEYCODE_PRIOR:
            return "prior";
            break;
        case ROSE_KEYCODE_RETURN2:
            return "return2";
            break;
        case ROSE_KEYCODE_SEPARATOR:
            return "separator";
            break;
        case ROSE_KEYCODE_OUT:
            return "out";
            break;
        case ROSE_KEYCODE_OPER:
            return "oper";
            break;
        case ROSE_KEYCODE_CLEARAGAIN:
            return "clearagain";
            break;
        case ROSE_KEYCODE_CRSEL:
            return "crsel";
            break;
        case ROSE_KEYCODE_EXSEL:
            return "exsel";
            break;
        case ROSE_KEYCODE_KP_00:
            return "kp_00";
            break;
        case ROSE_KEYCODE_KP_000:
            return "kp_000";
            break;
        case ROSE_KEYCODE_THOUSANDSSEPARATOR:
            return "thousandsseparator";
            break;
        case ROSE_KEYCODE_DECIMALSEPARATOR:
            return "decimalseparator";
            break;
        case ROSE_KEYCODE_CURRENCYUNIT:
            return "currencyunit";
            break;
        case ROSE_KEYCODE_CURRENCYSUBUNIT:
            return "currencysubunit";
            break;
        case ROSE_KEYCODE_KP_LEFTPAREN:
            return "kp_leftparen";
            break;
        case ROSE_KEYCODE_KP_RIGHTPAREN:
            return "kp_rightparen";
            break;
        case ROSE_KEYCODE_KP_LEFTBRACE:
            return "kp_leftbrace";
            break;
        case ROSE_KEYCODE_KP_RIGHTBRACE:
            return "kp_rightbrace";
            break;
        case ROSE_KEYCODE_KP_TAB:
            return "kp_tab";
            break;
        case ROSE_KEYCODE_KP_BACKSPACE:
            return "kp_backspace";
            break;
        case ROSE_KEYCODE_KP_A:
            return "kp_a";
            break;
        case ROSE_KEYCODE_KP_B:
            return "kp_b";
            break;
        case ROSE_KEYCODE_KP_C:
            return "kp_c";
            break;
        case ROSE_KEYCODE_KP_D:
            return "kp_d";
            break;
        case ROSE_KEYCODE_KP_E:
            return "kp_e";
            break;
        case ROSE_KEYCODE_KP_F:
            return "kp_f";
            break;
        case ROSE_KEYCODE_KP_XOR:
            return "kp_xor";
            break;
        case ROSE_KEYCODE_KP_POWER:
            return "kp_power";
            break;
        case ROSE_KEYCODE_KP_PERCENT:
            return "kp_percent";
            break;
        case ROSE_KEYCODE_KP_LESS:
            return "kp_less";
            break;
        case ROSE_KEYCODE_KP_GREATER:
            return "kp_greater";
            break;
        case ROSE_KEYCODE_KP_AMPERSAND:
            return "kp_ampersand";
            break;
        case ROSE_KEYCODE_KP_DBLAMPERSAND:
            return "kp_dblampersand";
            break;
        case ROSE_KEYCODE_KP_VERTICALBAR:
            return "kp_verticalbar";
            break;
        case ROSE_KEYCODE_KP_DBLVERTICALBAR:
            return "kp_dblverticalbar";
            break;
        case ROSE_KEYCODE_KP_COLON:
            return "kp_colon";
            break;
        case ROSE_KEYCODE_KP_HASH:
            return "kp_hash";
            break;
        case ROSE_KEYCODE_KP_SPACE:
            return "kp_space";
            break;
        case ROSE_KEYCODE_KP_AT:
            return "kp_at";
            break;
        case ROSE_KEYCODE_KP_EXCLAM:
            return "kp_exclam";
            break;
        case ROSE_KEYCODE_KP_MEMSTORE:
            return "kp_memstore";
            break;
        case ROSE_KEYCODE_KP_MEMRECALL:
            return "kp_memrecall";
            break;
        case ROSE_KEYCODE_KP_MEMCLEAR:
            return "kp_memclear";
            break;
        case ROSE_KEYCODE_KP_MEMADD:
            return "kp_memadd";
            break;
        case ROSE_KEYCODE_KP_MEMSUBTRACT:
            return "kp_memsubtract";
            break;
        case ROSE_KEYCODE_KP_MEMMULTIPLY:
            return "kp_memmultiply";
            break;
        case ROSE_KEYCODE_KP_MEMDIVIDE:
            return "kp_memdivide";
            break;
        case ROSE_KEYCODE_KP_PLUSMINUS:
            return "kp_plusminus";
            break;
        case ROSE_KEYCODE_KP_CLEAR:
            return "kp_clear";
            break;
        case ROSE_KEYCODE_KP_CLEARENTRY:
            return "kp_clearentry";
            break;
        case ROSE_KEYCODE_KP_BINARY:
            return "kp_binary";
            break;
        case ROSE_KEYCODE_KP_OCTAL:
            return "kp_octal";
            break;
        case ROSE_KEYCODE_KP_DECIMAL:
            return "kp_decimal";
            break;
        case ROSE_KEYCODE_KP_HEXADECIMAL:
            return "kp_hexadecimal";
            break;
        case ROSE_KEYCODE_LCTRL:
            return "lctrl";
            break;
        case ROSE_KEYCODE_LSHIFT:
            return "lshift";
            break;
        case ROSE_KEYCODE_LALT:
            return "lalt";
            break;
        case ROSE_KEYCODE_LGUI:
            return "lgui";
            break;
        case ROSE_KEYCODE_RCTRL:
            return "rctrl";
            break;
        case ROSE_KEYCODE_RSHIFT:
            return "rshift";
            break;
        case ROSE_KEYCODE_RALT:
            return "ralt";
            break;
        case ROSE_KEYCODE_RGUI:
            return "rgui";
            break;
        case ROSE_KEYCODE_MODE:
            return "mode";
            break;
        case ROSE_KEYCODE_AUDIONEXT:
            return "audionext";
            break;
        case ROSE_KEYCODE_AUDIOPREV:
            return "audioprev";
            break;
        case ROSE_KEYCODE_AUDIOSTOP:
            return "audiostop";
            break;
        case ROSE_KEYCODE_AUDIOPLAY:
            return "audioplay";
            break;
        case ROSE_KEYCODE_AUDIOMUTE:
            return "audiomute";
            break;
        case ROSE_KEYCODE_MEDIASELECT:
            return "mediaselect";
            break;
        case ROSE_KEYCODE_WWW:
            return "www";
            break;
        case ROSE_KEYCODE_MAIL:
            return "mail";
            break;
        case ROSE_KEYCODE_CALCULATOR:
            return "calculator";
            break;
        case ROSE_KEYCODE_COMPUTER:
            return "computer";
            break;
        case ROSE_KEYCODE_AC_SEARCH:
            return "ac_search";
            break;
        case ROSE_KEYCODE_AC_HOME:
            return "ac_home";
            break;
        case ROSE_KEYCODE_AC_BACK:
            return "ac_back";
            break;
        case ROSE_KEYCODE_AC_FORWARD:
            return "ac_forward";
            break;
        case ROSE_KEYCODE_AC_STOP:
            return "ac_stop";
            break;
        case ROSE_KEYCODE_AC_REFRESH:
            return "ac_refresh";
            break;
        case ROSE_KEYCODE_AC_BOOKMARKS:
            return "ac_bookmarks";
            break;
        case ROSE_KEYCODE_BRIGHTNESSDOWN:
            return "brightnessdown";
            break;
        case ROSE_KEYCODE_BRIGHTNESSUP:
            return "brightnessup";
            break;
        case ROSE_KEYCODE_DISPLAYSWITCH:
            return "displayswitch";
            break;
        case ROSE_KEYCODE_KBDILLUMTOGGLE:
            return "kbdillumtoggle";
            break;
        case ROSE_KEYCODE_KBDILLUMDOWN:
            return "kbdillumdown";
            break;
        case ROSE_KEYCODE_KBDILLUMUP:
            return "kbdillumup";
            break;
        case ROSE_KEYCODE_EJECT:
            return "eject";
            break;
        case ROSE_KEYCODE_SLEEP:
            return "sleep";
            break;
        case ROSE_KEYCODE_APP1:
            return "app1";
            break;
        case ROSE_KEYCODE_APP2:
            return "app2";
            break;
        default:
            return NULL;
    }
}

char rose_keycode_to_printable_char(rose_keycode key, bool shift) {
    switch (key) {
        case ROSE_KEYCODE_A:
            return shift ? 'A' : 'a';
            break;
        case ROSE_KEYCODE_B:
            return shift ? 'B' : 'b';
            break;
        case ROSE_KEYCODE_C:
            return shift ? 'C' : 'c';
            break;
        case ROSE_KEYCODE_D:
            return shift ? 'D' : 'd';
            break;
        case ROSE_KEYCODE_E:
            return shift ? 'E' : 'e';
            break;
        case ROSE_KEYCODE_F:
            return shift ? 'F' : 'f';
            break;
        case ROSE_KEYCODE_G:
            return shift ? 'G' : 'g';
            break;
        case ROSE_KEYCODE_H:
            return shift ? 'H' : 'h';
            break;
        case ROSE_KEYCODE_I:
            return shift ? 'I' : 'i';
            break;
        case ROSE_KEYCODE_J:
            return shift ? 'J' : 'j';
            break;
        case ROSE_KEYCODE_K:
            return shift ? 'K' : 'k';
            break;
        case ROSE_KEYCODE_L:
            return shift ? 'L' : 'l';
            break;
        case ROSE_KEYCODE_M:
            return shift ? 'M' : 'm';
            break;
        case ROSE_KEYCODE_N:
            return shift ? 'N' : 'n';
            break;
        case ROSE_KEYCODE_O:
            return shift ? 'O' : 'o';
            break;
        case ROSE_KEYCODE_P:
            return shift ? 'P' : 'p';
            break;
        case ROSE_KEYCODE_Q:
            return shift ? 'Q' : 'q';
            break;
        case ROSE_KEYCODE_R:
            return shift ? 'R' : 'r';
            break;
        case ROSE_KEYCODE_S:
            return shift ? 'S' : 's';
            break;
        case ROSE_KEYCODE_T:
            return shift ? 'T' : 't';
            break;
        case ROSE_KEYCODE_U:
            return shift ? 'U' : 'u';
            break;
        case ROSE_KEYCODE_V:
            return shift ? 'V' : 'v';
            break;
        case ROSE_KEYCODE_W:
            return shift ? 'W' : 'w';
            break;
        case ROSE_KEYCODE_X:
            return shift ? 'X' : 'x';
            break;
        case ROSE_KEYCODE_Y:
            return shift ? 'Y' : 'y';
            break;
        case ROSE_KEYCODE_Z:
            return shift ? 'Z' : 'z';
            break;
        case ROSE_KEYCODE_1:
            return shift ? '!' : '1';
            break;
        case ROSE_KEYCODE_2:
            return shift ? '@' : '2';
            break;
        case ROSE_KEYCODE_3:
            return shift ? '#' : '3';
            break;
        case ROSE_KEYCODE_4:
            return shift ? '$' : '4';
            break;
        case ROSE_KEYCODE_5:
            return shift ? '%' : '5';
            break;
        case ROSE_KEYCODE_6:
            return shift ? '^' : '6';
            break;
        case ROSE_KEYCODE_7:
            return shift ? '&' : '7';
            break;
        case ROSE_KEYCODE_8:
            return shift ? '*' : '8';
            break;
        case ROSE_KEYCODE_9:
            return shift ? '(' : '9';
            break;
        case ROSE_KEYCODE_0:
            return shift ? ')' : '0';
            break;
        case ROSE_KEYCODE_RETURN:
            return '\n';
            break;
        case ROSE_KEYCODE_TAB:
            return ' ';
            break;
        case ROSE_KEYCODE_SPACE:
            return ' ';
            break;
        case ROSE_KEYCODE_MINUS:
            return shift ? '_' : '-';
            break;
        case ROSE_KEYCODE_EQUALS:
            return shift ? '+' : '=';
            break;
        case ROSE_KEYCODE_LEFTBRACKET:
            return shift ? '{' : '[';
            break;
        case ROSE_KEYCODE_RIGHTBRACKET:
            return shift ? '}' : ']';
            break;
        case ROSE_KEYCODE_BACKSLASH:
            return (const char) (shift ? '|' : '\\');
            break;
        case ROSE_KEYCODE_SEMICOLON:
            return shift ? ':' : ';';
            break;
        case ROSE_KEYCODE_APOSTROPHE:
            return (const char) (shift ? '"' : '\'');
            break;
        case ROSE_KEYCODE_GRAVE:
            return shift ? '~' : '`';
            break;
        case ROSE_KEYCODE_COMMA:
            return shift ? '<' : ',';
            break;
        case ROSE_KEYCODE_PERIOD:
            return shift ? '>' : '.';
            break;
        case ROSE_KEYCODE_SLASH:
            return shift ? '?' : '/';
            break;
        case ROSE_KEYCODE_KP_DIVIDE:
            return '/';
            break;
        case ROSE_KEYCODE_KP_MULTIPLY:
            return '*';
            break;
        case ROSE_KEYCODE_KP_MINUS:
            return '-';
            break;
        case ROSE_KEYCODE_KP_PLUS:
            return '+';
            break;
        case ROSE_KEYCODE_KP_ENTER:
            return '\n';
            break;
        case ROSE_KEYCODE_KP_1:
            return '1';
            break;
        case ROSE_KEYCODE_KP_2:
            return '2';
            break;
        case ROSE_KEYCODE_KP_3:
            return '3';
            break;
        case ROSE_KEYCODE_KP_4:
            return '4';
            break;
        case ROSE_KEYCODE_KP_5:
            return '5';
            break;
        case ROSE_KEYCODE_KP_6:
            return '6';
            break;
        case ROSE_KEYCODE_KP_7:
            return '7';
            break;
        case ROSE_KEYCODE_KP_8:
            return '8';
            break;
        case ROSE_KEYCODE_KP_9:
            return '9';
            break;
        case ROSE_KEYCODE_KP_0:
            return '0';
            break;
        case ROSE_KEYCODE_KP_PERIOD:
            return '.';
            break;
        case ROSE_KEYCODE_KP_EQUALS:
            return '=';
            break;
        case ROSE_KEYCODE_KP_COMMA:
            return ',';
            break;
        case ROSE_KEYCODE_KP_LEFTPAREN:
            return '(';
            break;
        case ROSE_KEYCODE_KP_RIGHTPAREN:
            return ')';
            break;
        case ROSE_KEYCODE_KP_LEFTBRACE:
            return '{';
            break;
        case ROSE_KEYCODE_KP_RIGHTBRACE:
            return '}';
            break;
        case ROSE_KEYCODE_KP_TAB:
            return ' ';
            break;
        case ROSE_KEYCODE_KP_PERCENT:
            return '%';
            break;
        case ROSE_KEYCODE_KP_LESS:
            return '<';
            break;
        case ROSE_KEYCODE_KP_GREATER:
            return '>';
            break;
        case ROSE_KEYCODE_KP_AMPERSAND:
            return '&';
            break;
        case ROSE_KEYCODE_KP_VERTICALBAR:
            return '|';
            break;
        case ROSE_KEYCODE_KP_COLON:
            return ':';
            break;
        case ROSE_KEYCODE_KP_HASH:
            return '#';
            break;
        case ROSE_KEYCODE_KP_SPACE:
            return ' ';
            break;
        default:
            return 0;
    }
}