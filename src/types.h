#ifndef ROSE_TYPES_H
#define ROSE_TYPES_H

#include <cstdint>
#include <cstdbool>
#include <cstdlib>
#include <cerrno>
#include <ctime>
#include <cstdint>
#include <cstring>
#include <cassert>
#include <iostream>
#include <string>
#include <zlib.h>
#include <archive.h>
#include <archive_entry.h>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <map>

#include "libplatform/libplatform.h"
#include "v8.h"
using namespace std;
using namespace v8;


enum rose_api_error{
    ROSE_API_ERR_NONE,
    ROSE_API_ERR_OUT_OF_BOUNDS
};

enum rose_screenmode { ROSE_GAMEMODE, ROSE_EDITORMODE };

struct rose_color{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

// ***** fs

enum rose_fs_error {
    ROSE_FS_NO_ERR,
    ROSE_FS_CANT_READ_DIR_ERR,
    ROSE_FS_FILE_NOT_ON_DISK,
    ROSE_FS_FILE_REMOVED,
    ROSE_FS_CRITICAL_ERR
};

enum rose_file_type { ROSE_INVALID_FILE, ROSE_CODE_FILE, ROSE_DATA_FILE, ROSE_DIRECTORY, ROSE_CART_DIRECTORY };

struct rose_file {
    rose_file_type type;
    string name;
    bool on_disk;
    struct rose_file* parent;
    vector<rose_file*> contents;
    bool in_mem;
    uint8_t* buffer;
    size_t buffer_len;
    time_t last_modification;
    bool removed;
};

typedef rose_fs_error (*rose_fs_hook_read_file)(rose_file* file);
typedef rose_fs_error (*rose_fs_hook_write_file)(rose_file* file);
typedef rose_fs_error (*rose_fs_hook_shutdown)();
typedef rose_fs_error (*rose_fs_hook_get_base_path)(char** path);

struct rose_fs {
    rose_file* cart;
    rose_file* root;
    rose_file* pwd;
    rose_fs_hook_read_file read_file;
    rose_fs_hook_write_file write_file;
    rose_fs_hook_shutdown shutdown;
    rose_fs_hook_get_base_path get_base_path;
};

// **** js


struct rose_js {
    rose_fs* fs;
    Isolate* isolate;
    Isolate::CreateParams create_params;
    Global<ObjectTemplate> global_template;
    Global<Context> context;
    vector<rose_file*> include_path;
    Global<v8::Map> module_cache;
};

// **** rt


enum rose_rt_error{
    ROSE_RT_FUN_NOT_FOUND,
    ROSE_RT_NO_ERR,
    ROSE_RT_CRITICAL_ERR
};

enum rose_keycode {
    ROSE_KEYCODE_A = 0,
    ROSE_KEYCODE_B = 1,
    ROSE_KEYCODE_C = 2,
    ROSE_KEYCODE_D = 3,
    ROSE_KEYCODE_E = 4,
    ROSE_KEYCODE_F = 5,
    ROSE_KEYCODE_G = 6,
    ROSE_KEYCODE_H = 7,
    ROSE_KEYCODE_I = 8,
    ROSE_KEYCODE_J = 9,
    ROSE_KEYCODE_K = 10,
    ROSE_KEYCODE_L = 11,
    ROSE_KEYCODE_M = 12,
    ROSE_KEYCODE_N = 13,
    ROSE_KEYCODE_O = 14,
    ROSE_KEYCODE_P = 15,
    ROSE_KEYCODE_Q = 16,
    ROSE_KEYCODE_R = 17,
    ROSE_KEYCODE_S = 18,
    ROSE_KEYCODE_T = 19,
    ROSE_KEYCODE_U = 20,
    ROSE_KEYCODE_V = 21,
    ROSE_KEYCODE_W = 22,
    ROSE_KEYCODE_X = 23,
    ROSE_KEYCODE_Y = 24,
    ROSE_KEYCODE_Z = 25,
    ROSE_KEYCODE_1 = 26,
    ROSE_KEYCODE_2 = 27,
    ROSE_KEYCODE_3 = 28,
    ROSE_KEYCODE_4 = 29,
    ROSE_KEYCODE_5 = 30,
    ROSE_KEYCODE_6 = 31,
    ROSE_KEYCODE_7 = 32,
    ROSE_KEYCODE_8 = 33,
    ROSE_KEYCODE_9 = 34,
    ROSE_KEYCODE_0 = 35,
    ROSE_KEYCODE_RETURN = 36,
    ROSE_KEYCODE_ESCAPE = 37,
    ROSE_KEYCODE_BACKSPACE = 38,
    ROSE_KEYCODE_TAB = 39,
    ROSE_KEYCODE_SPACE = 40,
    ROSE_KEYCODE_MINUS = 41,
    ROSE_KEYCODE_EQUALS = 42,
    ROSE_KEYCODE_LEFTBRACKET = 43,
    ROSE_KEYCODE_RIGHTBRACKET = 44,
    ROSE_KEYCODE_BACKSLASH = 45,
    ROSE_KEYCODE_NONUSHASH = 46,
    ROSE_KEYCODE_SEMICOLON = 47,
    ROSE_KEYCODE_APOSTROPHE = 48,
    ROSE_KEYCODE_GRAVE = 49,
    ROSE_KEYCODE_COMMA = 50,
    ROSE_KEYCODE_PERIOD = 51,
    ROSE_KEYCODE_SLASH = 52,
    ROSE_KEYCODE_CAPSLOCK = 53,
    ROSE_KEYCODE_F1 = 54,
    ROSE_KEYCODE_F2 = 55,
    ROSE_KEYCODE_F3 = 56,
    ROSE_KEYCODE_F4 = 57,
    ROSE_KEYCODE_F5 = 58,
    ROSE_KEYCODE_F6 = 59,
    ROSE_KEYCODE_F7 = 60,
    ROSE_KEYCODE_F8 = 61,
    ROSE_KEYCODE_F9 = 62,
    ROSE_KEYCODE_F10 = 63,
    ROSE_KEYCODE_F11 = 64,
    ROSE_KEYCODE_F12 = 65,
    ROSE_KEYCODE_PRINTSCREEN = 66,
    ROSE_KEYCODE_SCROLLLOCK = 67,
    ROSE_KEYCODE_PAUSE = 68,
    ROSE_KEYCODE_INSERT = 69,
    ROSE_KEYCODE_HOME = 70,
    ROSE_KEYCODE_PAGEUP = 71,
    ROSE_KEYCODE_DELETE = 72,
    ROSE_KEYCODE_END = 73,
    ROSE_KEYCODE_PAGEDOWN = 74,
    ROSE_KEYCODE_RIGHT = 75,
    ROSE_KEYCODE_LEFT = 76,
    ROSE_KEYCODE_DOWN = 77,
    ROSE_KEYCODE_UP = 78,
    ROSE_KEYCODE_NUMLOCKCLEAR = 79,
    ROSE_KEYCODE_KP_DIVIDE = 80,
    ROSE_KEYCODE_KP_MULTIPLY = 81,
    ROSE_KEYCODE_KP_MINUS = 82,
    ROSE_KEYCODE_KP_PLUS = 83,
    ROSE_KEYCODE_KP_ENTER = 84,
    ROSE_KEYCODE_KP_1 = 85,
    ROSE_KEYCODE_KP_2 = 86,
    ROSE_KEYCODE_KP_3 = 87,
    ROSE_KEYCODE_KP_4 = 88,
    ROSE_KEYCODE_KP_5 = 89,
    ROSE_KEYCODE_KP_6 = 90,
    ROSE_KEYCODE_KP_7 = 91,
    ROSE_KEYCODE_KP_8 = 92,
    ROSE_KEYCODE_KP_9 = 93,
    ROSE_KEYCODE_KP_0 = 94,
    ROSE_KEYCODE_KP_PERIOD = 95,
    ROSE_KEYCODE_NONUSBACKSLASH = 96,
    ROSE_KEYCODE_APPLICATION = 97,
    ROSE_KEYCODE_POWER = 98,
    ROSE_KEYCODE_KP_EQUALS = 99,
    ROSE_KEYCODE_F13 = 100,
    ROSE_KEYCODE_F14 = 101,
    ROSE_KEYCODE_F15 = 102,
    ROSE_KEYCODE_F16 = 103,
    ROSE_KEYCODE_F17 = 104,
    ROSE_KEYCODE_F18 = 105,
    ROSE_KEYCODE_F19 = 106,
    ROSE_KEYCODE_F20 = 107,
    ROSE_KEYCODE_F21 = 108,
    ROSE_KEYCODE_F22 = 109,
    ROSE_KEYCODE_F23 = 110,
    ROSE_KEYCODE_F24 = 111,
    ROSE_KEYCODE_EXECUTE = 112,
    ROSE_KEYCODE_HELP = 113,
    ROSE_KEYCODE_MENU = 114,
    ROSE_KEYCODE_SELECT = 115,
    ROSE_KEYCODE_STOP = 116,
    ROSE_KEYCODE_AGAIN = 117,
    ROSE_KEYCODE_UNDO = 118,
    ROSE_KEYCODE_CUT = 119,
    ROSE_KEYCODE_COPY = 120,
    ROSE_KEYCODE_PASTE = 121,
    ROSE_KEYCODE_FIND = 122,
    ROSE_KEYCODE_MUTE = 123,
    ROSE_KEYCODE_VOLUMEUP = 124,
    ROSE_KEYCODE_VOLUMEDOWN = 125,
    ROSE_KEYCODE_KP_COMMA = 126,
    ROSE_KEYCODE_KP_EQUALSAS400 = 127,
    ROSE_KEYCODE_INTERNATIONAL1 = 128,
    ROSE_KEYCODE_INTERNATIONAL2 = 129,
    ROSE_KEYCODE_INTERNATIONAL3 = 130,
    ROSE_KEYCODE_INTERNATIONAL4 = 131,
    ROSE_KEYCODE_INTERNATIONAL5 = 132,
    ROSE_KEYCODE_INTERNATIONAL6 = 133,
    ROSE_KEYCODE_INTERNATIONAL7 = 134,
    ROSE_KEYCODE_INTERNATIONAL8 = 135,
    ROSE_KEYCODE_INTERNATIONAL9 = 136,
    ROSE_KEYCODE_LANG1 = 137,
    ROSE_KEYCODE_LANG2 = 138,
    ROSE_KEYCODE_LANG3 = 139,
    ROSE_KEYCODE_LANG4 = 140,
    ROSE_KEYCODE_LANG5 = 141,
    ROSE_KEYCODE_LANG6 = 142,
    ROSE_KEYCODE_LANG7 = 143,
    ROSE_KEYCODE_LANG8 = 144,
    ROSE_KEYCODE_LANG9 = 145,
    ROSE_KEYCODE_ALTERASE = 146,
    ROSE_KEYCODE_SYSREQ = 147,
    ROSE_KEYCODE_CANCEL = 148,
    ROSE_KEYCODE_CLEAR = 149,
    ROSE_KEYCODE_PRIOR = 150,
    ROSE_KEYCODE_RETURN2 = 151,
    ROSE_KEYCODE_SEPARATOR = 152,
    ROSE_KEYCODE_OUT = 153,
    ROSE_KEYCODE_OPER = 154,
    ROSE_KEYCODE_CLEARAGAIN = 155,
    ROSE_KEYCODE_CRSEL = 156,
    ROSE_KEYCODE_EXSEL = 157,
    ROSE_KEYCODE_KP_00 = 158,
    ROSE_KEYCODE_KP_000 = 159,
    ROSE_KEYCODE_THOUSANDSSEPARATOR = 160,
    ROSE_KEYCODE_DECIMALSEPARATOR = 161,
    ROSE_KEYCODE_CURRENCYUNIT = 162,
    ROSE_KEYCODE_CURRENCYSUBUNIT = 163,
    ROSE_KEYCODE_KP_LEFTPAREN = 164,
    ROSE_KEYCODE_KP_RIGHTPAREN = 165,
    ROSE_KEYCODE_KP_LEFTBRACE = 166,
    ROSE_KEYCODE_KP_RIGHTBRACE = 167,
    ROSE_KEYCODE_KP_TAB = 168,
    ROSE_KEYCODE_KP_BACKSPACE = 169,
    ROSE_KEYCODE_KP_A = 170,
    ROSE_KEYCODE_KP_B = 171,
    ROSE_KEYCODE_KP_C = 172,
    ROSE_KEYCODE_KP_D = 173,
    ROSE_KEYCODE_KP_E = 174,
    ROSE_KEYCODE_KP_F = 175,
    ROSE_KEYCODE_KP_XOR = 176,
    ROSE_KEYCODE_KP_POWER = 177,
    ROSE_KEYCODE_KP_PERCENT = 178,
    ROSE_KEYCODE_KP_LESS = 179,
    ROSE_KEYCODE_KP_GREATER = 180,
    ROSE_KEYCODE_KP_AMPERSAND = 181,
    ROSE_KEYCODE_KP_DBLAMPERSAND = 182,
    ROSE_KEYCODE_KP_VERTICALBAR = 183,
    ROSE_KEYCODE_KP_DBLVERTICALBAR = 184,
    ROSE_KEYCODE_KP_COLON = 185,
    ROSE_KEYCODE_KP_HASH = 186,
    ROSE_KEYCODE_KP_SPACE = 187,
    ROSE_KEYCODE_KP_AT = 188,
    ROSE_KEYCODE_KP_EXCLAM = 189,
    ROSE_KEYCODE_KP_MEMSTORE = 190,
    ROSE_KEYCODE_KP_MEMRECALL = 191,
    ROSE_KEYCODE_KP_MEMCLEAR = 192,
    ROSE_KEYCODE_KP_MEMADD = 193,
    ROSE_KEYCODE_KP_MEMSUBTRACT = 194,
    ROSE_KEYCODE_KP_MEMMULTIPLY = 195,
    ROSE_KEYCODE_KP_MEMDIVIDE = 196,
    ROSE_KEYCODE_KP_PLUSMINUS = 197,
    ROSE_KEYCODE_KP_CLEAR = 198,
    ROSE_KEYCODE_KP_CLEARENTRY = 199,
    ROSE_KEYCODE_KP_BINARY = 200,
    ROSE_KEYCODE_KP_OCTAL = 201,
    ROSE_KEYCODE_KP_DECIMAL = 202,
    ROSE_KEYCODE_KP_HEXADECIMAL = 203,
    ROSE_KEYCODE_LCTRL = 204,
    ROSE_KEYCODE_LSHIFT = 205,
    ROSE_KEYCODE_LALT = 206,
    ROSE_KEYCODE_LGUI = 207,
    ROSE_KEYCODE_RCTRL = 208,
    ROSE_KEYCODE_RSHIFT = 209,
    ROSE_KEYCODE_RALT = 210,
    ROSE_KEYCODE_RGUI = 211,
    ROSE_KEYCODE_MODE = 212,
    ROSE_KEYCODE_AUDIONEXT = 213,
    ROSE_KEYCODE_AUDIOPREV = 214,
    ROSE_KEYCODE_AUDIOSTOP = 215,
    ROSE_KEYCODE_AUDIOPLAY = 216,
    ROSE_KEYCODE_AUDIOMUTE = 217,
    ROSE_KEYCODE_MEDIASELECT = 218,
    ROSE_KEYCODE_WWW = 219,
    ROSE_KEYCODE_MAIL = 220,
    ROSE_KEYCODE_CALCULATOR = 221,
    ROSE_KEYCODE_COMPUTER = 222,
    ROSE_KEYCODE_AC_SEARCH = 223,
    ROSE_KEYCODE_AC_HOME = 224,
    ROSE_KEYCODE_AC_BACK = 225,
    ROSE_KEYCODE_AC_FORWARD = 226,
    ROSE_KEYCODE_AC_STOP = 227,
    ROSE_KEYCODE_AC_REFRESH = 228,
    ROSE_KEYCODE_AC_BOOKMARKS = 229,
    ROSE_KEYCODE_BRIGHTNESSDOWN = 230,
    ROSE_KEYCODE_BRIGHTNESSUP = 231,
    ROSE_KEYCODE_DISPLAYSWITCH = 232,
    ROSE_KEYCODE_KBDILLUMTOGGLE = 233,
    ROSE_KEYCODE_KBDILLUMDOWN = 234,
    ROSE_KEYCODE_KBDILLUMUP = 235,
    ROSE_KEYCODE_EJECT = 236,
    ROSE_KEYCODE_SLEEP = 237,
    ROSE_KEYCODE_APP1 = 238,
    ROSE_KEYCODE_APP2 = 239,
    ROSE_KEYCODE_UNKNOWN = 240
};


struct rose_mousestate {
    int16_t x;
    int16_t y;
    bool left_btn_down;
    bool middle_btn_down;
    bool right_btn_down;
    bool x1_btn_down;
    bool x2_btn_down;
    int16_t wheel_x;
    int16_t wheel_y;
    bool wheel_inverted;
};

typedef uint8_t* rose_memory_iterator;

struct rose_memory_range {
    rose_memory_iterator begin;
    rose_memory_iterator end;
};

struct rose_rt {
    rose_js* js;
    uint8_t* mem;
    uint32_t mem_size;
    rose_memory_range* screen;
    rose_memory_range* schema;
    rose_memory_range* palette;
    rose_memory_range* palette_filter; // TODO: rename this to something not shit
    rose_memory_range* palette_transparency;
    rose_memory_range* clipping_region;
    rose_memory_iterator pen_color_addr;
    rose_memory_range* print_cursor;
    rose_memory_range* camera_offset;
    rose_memory_range* pointer_positions;
    rose_memory_range* btn_states;
    rose_memory_range* prev_btn_states;
    rose_memory_range* mouse_wheel;
    rose_memory_range* key_states;
    rose_memory_range* prev_key_states;
    rose_fs* fs;
};


// *** game

enum rose_game_error {
    ROSE_GAME_FUN_NOT_FOUND,
    ROSE_GAME_NO_ERR,
    ROSE_GAME_CRITICAL_ERR
};

struct rose_game {
    rose_rt* rt;
};

struct rose_editor_instance {
    rose_rt* rt;
};

struct rose_editor {
    vector<rose_editor_instance*> editors;

};

#endif