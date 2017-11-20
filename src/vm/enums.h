#pragma once

enum rose_vm_error {
  rose_vm_FUN_NOT_FOUND,
  rose_vm_NO_ERR,
  rose_vm_CRITICAL_ERR
};

// enum rose_fs_error {
//     ROSE_FS_NO_ERR,
//     ROSE_FS_CANT_READ_DIR_ERR,
//     ROSE_FS_FILE_NOT_ON_DISK,
//     ROSE_FS_FILE_REMOVED,
//     ROSE_FS_CRITICAL_ERR
// };

// enum rose_file_type {
//     ROSE_INVALID_FILE,
//     ROSE_INFO_FILE,
//     ROSE_CODE_FILE,
//     ROSE_DATA_FILE,
//     ROSE_DIRECTORY,
//     ROSE_CART_DIRECTORY
// };

enum rose_keycode {
  /**
   * Placed first to cover the 0 value of the enum.
   */
  ROSE_KEYCODE_Unknown = 0,

  ROSE_KEYCODE_Backspace = 1,
  ROSE_KEYCODE_Tab = 2,
  ROSE_KEYCODE_Enter = 3,
  ROSE_KEYCODE_Shift = 4,
  ROSE_KEYCODE_Ctrl = 5,
  ROSE_KEYCODE_Alt = 6,
  ROSE_KEYCODE_PauseBreak = 7,
  ROSE_KEYCODE_CapsLock = 8,
  ROSE_KEYCODE_Escape = 9,
  ROSE_KEYCODE_Space = 10,
  ROSE_KEYCODE_PageUp = 11,
  ROSE_KEYCODE_PageDown = 12,
  ROSE_KEYCODE_End = 13,
  ROSE_KEYCODE_Home = 14,
  ROSE_KEYCODE_LeftArrow = 15,
  ROSE_KEYCODE_UpArrow = 16,
  ROSE_KEYCODE_RightArrow = 17,
  ROSE_KEYCODE_DownArrow = 18,
  ROSE_KEYCODE_Insert = 19,
  ROSE_KEYCODE_Delete = 20,

  ROSE_KEYCODE_KEY_0 = 21,
  ROSE_KEYCODE_KEY_1 = 22,
  ROSE_KEYCODE_KEY_2 = 23,
  ROSE_KEYCODE_KEY_3 = 24,
  ROSE_KEYCODE_KEY_4 = 25,
  ROSE_KEYCODE_KEY_5 = 26,
  ROSE_KEYCODE_KEY_6 = 27,
  ROSE_KEYCODE_KEY_7 = 28,
  ROSE_KEYCODE_KEY_8 = 29,
  ROSE_KEYCODE_KEY_9 = 30,

  ROSE_KEYCODE_KEY_A = 31,
  ROSE_KEYCODE_KEY_B = 32,
  ROSE_KEYCODE_KEY_C = 33,
  ROSE_KEYCODE_KEY_D = 34,
  ROSE_KEYCODE_KEY_E = 35,
  ROSE_KEYCODE_KEY_F = 36,
  ROSE_KEYCODE_KEY_G = 37,
  ROSE_KEYCODE_KEY_H = 38,
  ROSE_KEYCODE_KEY_I = 39,
  ROSE_KEYCODE_KEY_J = 40,
  ROSE_KEYCODE_KEY_K = 41,
  ROSE_KEYCODE_KEY_L = 42,
  ROSE_KEYCODE_KEY_M = 43,
  ROSE_KEYCODE_KEY_N = 44,
  ROSE_KEYCODE_KEY_O = 45,
  ROSE_KEYCODE_KEY_P = 46,
  ROSE_KEYCODE_KEY_Q = 47,
  ROSE_KEYCODE_KEY_R = 48,
  ROSE_KEYCODE_KEY_S = 49,
  ROSE_KEYCODE_KEY_T = 50,
  ROSE_KEYCODE_KEY_U = 51,
  ROSE_KEYCODE_KEY_V = 52,
  ROSE_KEYCODE_KEY_W = 53,
  ROSE_KEYCODE_KEY_X = 54,
  ROSE_KEYCODE_KEY_Y = 55,
  ROSE_KEYCODE_KEY_Z = 56,

  ROSE_KEYCODE_Meta = 57,
  ROSE_KEYCODE_ContextMenu = 58,

  ROSE_KEYCODE_F1 = 59,
  ROSE_KEYCODE_F2 = 60,
  ROSE_KEYCODE_F3 = 61,
  ROSE_KEYCODE_F4 = 62,
  ROSE_KEYCODE_F5 = 63,
  ROSE_KEYCODE_F6 = 64,
  ROSE_KEYCODE_F7 = 65,
  ROSE_KEYCODE_F8 = 66,
  ROSE_KEYCODE_F9 = 67,
  ROSE_KEYCODE_F10 = 68,
  ROSE_KEYCODE_F11 = 69,
  ROSE_KEYCODE_F12 = 70,
  ROSE_KEYCODE_F13 = 71,
  ROSE_KEYCODE_F14 = 72,
  ROSE_KEYCODE_F15 = 73,
  ROSE_KEYCODE_F16 = 74,
  ROSE_KEYCODE_F17 = 75,
  ROSE_KEYCODE_F18 = 76,
  ROSE_KEYCODE_F19 = 77,

  ROSE_KEYCODE_NumLock = 78,
  ROSE_KEYCODE_ScrollLock = 79,

  /**
   * Used for miscellaneous characters; it can vary by keyboard.
   * For the US standard keyboard, the ';:' key
   */
  ROSE_KEYCODE_US_SEMICOLON = 80,
  /**
   * For any country/region, the '+' key
   * For the US standard keyboard, the '=+' key
   */
  ROSE_KEYCODE_US_EQUAL = 81,
  /**
   * For any country/region, the ',' key
   * For the US standard keyboard, the ',<' key
   */
  ROSE_KEYCODE_US_COMMA = 82,
  /**
   * For any country/region, the '-' key
   * For the US standard keyboard, the '-_' key
   */
  ROSE_KEYCODE_US_MINUS = 83,
  /**
   * For any country/region, the '.' key
   * For the US standard keyboard, the '.>' key
   */
  ROSE_KEYCODE_US_DOT = 84,
  /**
   * Used for miscellaneous characters; it can vary by keyboard.
   * For the US standard keyboard, the '/?' key
   */
  ROSE_KEYCODE_US_SLASH = 85,
  /**
   * Used for miscellaneous characters; it can vary by keyboard.
   * For the US standard keyboard, the '`~' key
   */
  US_BACKTICK = 86,
  /**
   * Used for miscellaneous characters; it can vary by keyboard.
   * For the US standard keyboard, the '[{' key
   */
  ROSE_KEYCODE_US_OPEN_SQUARE_BRACKET = 87,
  /**
   * Used for miscellaneous characters; it can vary by keyboard.
   * For the US standard keyboard, the '\|' key
   */
  ROSE_KEYCODE_US_BACKSLASH = 88,
  /**
   * Used for miscellaneous characters; it can vary by keyboard.
   * For the US standard keyboard, the ']}' key
   */
  ROSE_KEYCODE_US_CLOSE_SQUARE_BRACKET = 89,
  /**
   * Used for miscellaneous characters; it can vary by keyboard.
   * For the US standard keyboard, the ''"' key
   */
  ROSE_KEYCODE_US_QUOTE = 90,
  /**
   * Used for miscellaneous characters; it can vary by keyboard.
   */
  ROSE_KEYCODE_OEM_8 = 91,
  /**
   * Either the angle bracket key or the backslash key on the RT 102-key
   * keyboard.
   */
  ROSE_KEYCODE_OEM_102 = 92,

  ROSE_KEYCODE_NUMPAD_0 = 93,   // VK_NUMPAD0, 0x60, Numeric keypad 0 key
  ROSE_KEYCODE_NUMPAD_1 = 94,   // VK_NUMPAD1, 0x61, Numeric keypad 1 key
  ROSE_KEYCODE_NUMPAD_2 = 95,   // VK_NUMPAD2, 0x62, Numeric keypad 2 key
  ROSE_KEYCODE_NUMPAD_3 = 96,   // VK_NUMPAD3, 0x63, Numeric keypad 3 key
  ROSE_KEYCODE_NUMPAD_4 = 97,   // VK_NUMPAD4, 0x64, Numeric keypad 4 key
  ROSE_KEYCODE_NUMPAD_5 = 98,   // VK_NUMPAD5, 0x65, Numeric keypad 5 key
  ROSE_KEYCODE_NUMPAD_6 = 99,   // VK_NUMPAD6, 0x66, Numeric keypad 6 key
  ROSE_KEYCODE_NUMPAD_7 = 100,  // VK_NUMPAD7, 0x67, Numeric keypad 7 key
  ROSE_KEYCODE_NUMPAD_8 = 101,  // VK_NUMPAD8, 0x68, Numeric keypad 8 key
  ROSE_KEYCODE_NUMPAD_9 = 102,  // VK_NUMPAD9, 0x69, Numeric keypad 9 key

  ROSE_KEYCODE_NUMPAD_MULTIPLY = 103,   // VK_MULTIPLY, 0x6A, Multiply key
  ROSE_KEYCODE_NUMPAD_ADD = 104,        // VK_ADD, 0x6B, Add key
  ROSE_KEYCODE_NUMPAD_SEPARATOR = 105,  // VK_SEPARATOR, 0x6C, Separator key
  ROSE_KEYCODE_NUMPAD_SUBTRACT = 106,   // VK_SUBTRACT, 0x6D, Subtract key
  ROSE_KEYCODE_NUMPAD_DECIMAL = 107,    // VK_DECIMAL, 0x6E, Decimal key
  ROSE_KEYCODE_NUMPAD_DIVIDE = 108,     // VK_DIVIDE, 0x6F,

  /**
   * Cover all key codes when IME is processing input.
   */
  ROSE_KEYCODE_KEY_IN_COMPOSITION = 109,

  ROSE_KEYCODE_ABNT_C1 = 110,  // Brazilian (ABNT) Keyboard
  ROSE_KEYCODE_ABNT_C2 = 111,  // Brazilian (ABNT) Keyboard

  /**
   * Placed last to cover the length of the enum.
   * Please do not depend on this value!
   */
  ROSE_KEYCODE_MAX_VALUE
};

enum rose_api_error { ROSE_API_ERR_NONE, ROSE_API_ERR_OUT_OF_BOUNDS };

enum rose_screenmode {
  ROSE_STARTUPMODE,
  ROSE_GAMEMODE,
  ROSE_CONSOLEMODE,
  ROSE_EDITORMODE
};