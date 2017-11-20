const KeyCodes = Object.freeze({
  /**
	* Placed first to cover the 0 value of the enum.
	*/
  Unknown: 0,

  Backspace: 1,
  Tab: 2,
  Enter: 3,
  Shift: 4,
  Ctrl: 5,
  Alt: 6,
  PauseBreak: 7,
  CapsLock: 8,
  Escape: 9,
  Space: 10,
  PageUp: 11,
  PageDown: 12,
  End: 13,
  Home: 14,
  LeftArrow: 15,
  UpArrow: 16,
  RightArrow: 17,
  DownArrow: 18,
  Insert: 19,
  Delete: 20,

  Key0: 21,
  Key1: 22,
  Key2: 23,
  Key3: 24,
  Key4: 25,
  Key5: 26,
  Key6: 27,
  Key7: 28,
  Key8: 29,
  Key9: 30,

  KeyA: 31,
  KeyB: 32,
  KeyC: 33,
  KeyD: 34,
  KeyE: 35,
  KeyF: 36,
  KeyG: 37,
  KeyH: 38,
  KeyI: 39,
  KeyJ: 40,
  KeyK: 41,
  KeyL: 42,
  KeyM: 43,
  KeyN: 44,
  KeyO: 45,
  KeyP: 46,
  KeyQ: 47,
  KeyR: 48,
  KeyS: 49,
  KeyT: 50,
  KeyU: 51,
  KeyV: 52,
  KeyW: 53,
  KeyX: 54,
  KeyY: 55,
  KeyZ: 56,

  Meta: 57,
  ContextMenu: 58,

  F1: 59,
  F2: 60,
  F3: 61,
  F4: 62,
  F5: 63,
  F6: 64,
  F7: 65,
  F8: 66,
  F9: 67,
  F10: 68,
  F11: 69,
  F12: 70,
  F13: 71,
  F14: 72,
  F15: 73,
  F16: 74,
  F17: 75,
  F18: 76,
  F19: 77,

  NumLock: 78,
  ScrollLock: 79,

  /**
  * Used for miscellaneous characters, it can vary by keyboard.
  * For the US standard keyboard, the ';:' key
  */
  Semicolon: 80,
  /**
  * For any country/region, the '+' key
  * For the US standard keyboard, the '=+' key
  */
  Equal: 81,
  /**
  * For any country/region, the ',' key
  * For the US standard keyboard, the ',<' key
  */
  Comma: 82,
  /**
  * For any country/region, the '-' key
  * For the US standard keyboard, the '-_' key
  */
  Minus: 83,
  /**
  * For any country/region, the '.' key
  * For the US standard keyboard, the '.>' key
  */
  Dot: 84,
  /**
  * Used for miscellaneous characters, it can vary by keyboard.
  * For the US standard keyboard, the '/?' key
  */
  Slash: 85,
  /**
  * Used for miscellaneous characters, it can vary by keyboard.
  * For the US standard keyboard, the '`~' key
  */
  Backtick: 86,
  /**
  * Used for miscellaneous characters, it can vary by keyboard.
  * For the US standard keyboard, the '[{' key
  */
  OpenSquareBracket: 87,
  /**
  * Used for miscellaneous characters, it can vary by keyboard.
  * For the US standard keyboard, the '\|' key
  */
  Backslash: 88,
  /**
  * Used for miscellaneous characters, it can vary by keyboard.
  * For the US standard keyboard, the ']}' key
  */
  CloseSquareBracket: 89,
  /**
  * Used for miscellaneous characters, it can vary by keyboard.
  * For the US standard keyboard, the ''"' key
  */
  Quote: 90,
  /**
  * Used for miscellaneous characters, it can vary by keyboard.
  */
  Oem8: 91,
  /**
  * Either the angle bracket key or the backslash key on the RT 102-key keyboard.
  */
  Oem102: 92,

  NumPad0: 93, // VK_NUMPAD0, 0x60, Numeric keypad 0 key
  NumPad1: 94, // VK_NUMPAD1, 0x61, Numeric keypad 1 key
  NumPad2: 95, // VK_NUMPAD2, 0x62, Numeric keypad 2 key
  NumPad3: 96, // VK_NUMPAD3, 0x63, Numeric keypad 3 key
  NumPad4: 97, // VK_NUMPAD4, 0x64, Numeric keypad 4 key
  NumPad5: 98, // VK_NUMPAD5, 0x65, Numeric keypad 5 key
  NumPad6: 99, // VK_NUMPAD6, 0x66, Numeric keypad 6 key
  NumPad7: 100, // VK_NUMPAD7, 0x67, Numeric keypad 7 key
  NumPad8: 101, // VK_NUMPAD8, 0x68, Numeric keypad 8 key
  NumPad9: 102, // VK_NUMPAD9, 0x69, Numeric keypad 9 key

  NumPadMultiply: 103, // VK_MULTIPLY, 0x6A, Multiply key
  NumPadAdd: 104, // VK_ADD, 0x6B, Add key
  NumPadSeparator: 105, // VK_SEPARATOR, 0x6C, Separator key
  NumPadSubtract: 106, // VK_SUBTRACT, 0x6D, Subtract key
  NumPadDecimal: 107, // VK_DECIMAL, 0x6E, Decimal key
  NumPadDivide: 108, // VK_DIVIDE, 0x6F,

  /**
  * Cover all key codes when IME is processing input.
  */
  KeyInComposition: 109,

  AbntC1: 110, // Brazilian (ABNT) Keyboard
  AbntC2: 111 // Brazilian (ABNT) Keyboard
});

class KeyCodeStrMap {
  constructor() {
    this._keyCodeToStr = [];
    this._strToKeyCode = Object.create(null);
  }

  define(keyCode, str) {
    this._keyCodeToStr[keyCode] = str;
    this._strToKeyCode[str.toLowerCase()] = keyCode;
  }

  keyCodeToStr(keyCode) {
    return this._keyCodeToStr[keyCode];
  }

  strToKeyCode(str) {
    return this._strToKeyCode[str.toLowerCase()] || KeyCode.Unknown;
  }
}
const uiMap = new KeyCodeStrMap();

function define(keyCodes, uiLabel) {
  uiMap.define(keyCodes, uiLabel);
}

define(KeyCodes.Unknown, 'unknown');

define(KeyCodes.Backspace, 'Backspace');
define(KeyCodes.Tab, 'Tab');
define(KeyCodes.Enter, 'Enter');
define(KeyCodes.Shift, 'Shift');
define(KeyCodes.Ctrl, 'Ctrl');
define(KeyCodes.Alt, 'Alt');
define(KeyCodes.PauseBreak, 'PauseBreak');
define(KeyCodes.CapsLock, 'CapsLock');
define(KeyCodes.Escape, 'Escape');
define(KeyCodes.Space, 'Space');
define(KeyCodes.PageUp, 'PageUp');
define(KeyCodes.PageDown, 'PageDown');
define(KeyCodes.End, 'End');
define(KeyCodes.Home, 'Home');

define(KeyCodes.LeftArrow, 'LeftArrow');
define(KeyCodes.UpArrow, 'UpArrow');
define(KeyCodes.RightArrow, 'RightArrow');
define(KeyCodes.DownArrow, 'DownArrow');
define(KeyCodes.Insert, 'Insert');
define(KeyCodes.Delete, 'Delete');

define(KeyCodes.Key0, '0');
define(KeyCodes.Key1, '1');
define(KeyCodes.Key2, '2');
define(KeyCodes.Key3, '3');
define(KeyCodes.Key4, '4');
define(KeyCodes.Key5, '5');
define(KeyCodes.Key6, '6');
define(KeyCodes.Key7, '7');
define(KeyCodes.Key8, '8');
define(KeyCodes.Key9, '9');

define(KeyCodes.KeyA, 'A');
define(KeyCodes.KeyB, 'B');
define(KeyCodes.KeyC, 'C');
define(KeyCodes.KeyD, 'D');
define(KeyCodes.KeyE, 'E');
define(KeyCodes.KeyF, 'F');
define(KeyCodes.KeyG, 'G');
define(KeyCodes.KeyH, 'H');
define(KeyCodes.KeyI, 'I');
define(KeyCodes.KeyJ, 'J');
define(KeyCodes.KeyK, 'K');
define(KeyCodes.KeyL, 'L');
define(KeyCodes.KeyM, 'M');
define(KeyCodes.KeyN, 'N');
define(KeyCodes.KeyO, 'O');
define(KeyCodes.KeyP, 'P');
define(KeyCodes.KeyQ, 'Q');
define(KeyCodes.KeyR, 'R');
define(KeyCodes.KeyS, 'S');
define(KeyCodes.KeyT, 'T');
define(KeyCodes.KeyU, 'U');
define(KeyCodes.KeyV, 'V');
define(KeyCodes.KeyW, 'W');
define(KeyCodes.KeyX, 'X');
define(KeyCodes.KeyY, 'Y');
define(KeyCodes.KeyZ, 'Z');

define(KeyCodes.Meta, 'Meta');
define(KeyCodes.ContextMenu, 'ContextMenu');

define(KeyCodes.F1, 'F1');
define(KeyCodes.F2, 'F2');
define(KeyCodes.F3, 'F3');
define(KeyCodes.F4, 'F4');
define(KeyCodes.F5, 'F5');
define(KeyCodes.F6, 'F6');
define(KeyCodes.F7, 'F7');
define(KeyCodes.F8, 'F8');
define(KeyCodes.F9, 'F9');
define(KeyCodes.F10, 'F10');
define(KeyCodes.F11, 'F11');
define(KeyCodes.F12, 'F12');
define(KeyCodes.F13, 'F13');
define(KeyCodes.F14, 'F14');
define(KeyCodes.F15, 'F15');
define(KeyCodes.F16, 'F16');
define(KeyCodes.F17, 'F17');
define(KeyCodes.F18, 'F18');
define(KeyCodes.F19, 'F19');

define(KeyCodes.NumLock, 'NumLock');
define(KeyCodes.ScrollLock, 'ScrollLock');

define(KeyCodes.Semicolon, ';', ';', 'OEM_1');
define(KeyCodes.Equal, '=', '=', 'OEM_PLUS');
define(KeyCodes.Comma, ',', ',', 'OEM_COMMA');
define(KeyCodes.Minus, '-', '-', 'OEM_MINUS');
define(KeyCodes.Dot, '.', '.', 'OEM_PERIOD');
define(KeyCodes.Slash, '/', '/', 'OEM_2');
define(KeyCodes.Backtick, '`', '`', 'OEM_3');
define(KeyCodes.AbntC1, 'ABNT_C1');
define(KeyCodes.AbntC2, 'ABNT_C2');
define(KeyCodes.OpenSquareBracket, '[', '[', 'OEM_4');
define(KeyCodes.Backslash, '\\', '\\', 'OEM_5');
define(KeyCodes.CloseSquareBracket, ']', ']', 'OEM_6');
define(KeyCodes.Quote, '\'', '\'', 'OEM_7');
define(KeyCodes.Oem8, 'OEM_8');
define(KeyCodes.Oem102, 'OEM_102');

define(KeyCodes.NumPad0, 'NumPad0');
define(KeyCodes.NumPad1, 'NumPad1');
define(KeyCodes.NumPad2, 'NumPad2');
define(KeyCodes.NumPad3, 'NumPad3');
define(KeyCodes.NumPad4, 'NumPad4');
define(KeyCodes.NumPad5, 'NumPad5');
define(KeyCodes.NumPad6, 'NumPad6');
define(KeyCodes.NumPad7, 'NumPad7');
define(KeyCodes.NumPad8, 'NumPad8');
define(KeyCodes.NumPad9, 'NumPad9');

define(KeyCodes.NumPadMultiply, 'NumPadMultiply');
define(KeyCodes.NumPadAdd, 'NumPadAdd');
define(KeyCodes.NumPadSeparator, 'NumPadSeparator');
define(KeyCodes.NumPadSubtract, 'NumPadSubtract');
define(KeyCodes.NumPadDecimal, 'NumPadDecimal');
define(KeyCodes.NumPadDivide, 'NumPadDivide');

exports.Keycodes = KeyCodes

exports.keycodeToString = function keycodeToString(keyCode) {
  return uiMap.keyCodeToStr(keyCode);
}
exports.keycodeFromString =  function keycodeFromString(key) {
  return uiMap.strToKeyCode(key);
}