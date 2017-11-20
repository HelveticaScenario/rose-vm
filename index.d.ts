/// <reference types="node" />

declare function rosevm(hd?: boolean): rosevm.RosebudJS;

declare namespace rosevm {
  interface Keycodes {
    /**
	 * Placed first to cover the 0 value of the enum.
	 */
    Unknown: 0;

    Backspace: 1;
    Tab: 2;
    Enter: 3;
    Shift: 4;
    Ctrl: 5;
    Alt: 6;
    PauseBreak: 7;
    CapsLock: 8;
    Escape: 9;
    Space: 10;
    PageUp: 11;
    PageDown: 12;
    End: 13;
    Home: 14;
    LeftArrow: 15;
    UpArrow: 16;
    RightArrow: 17;
    DownArrow: 18;
    Insert: 19;
    Delete: 20;

    Key0: 21;
    Key1: 22;
    Key2: 23;
    Key3: 24;
    Key4: 25;
    Key5: 26;
    Key6: 27;
    Key7: 28;
    Key8: 29;
    Key9: 30;

    KeyA: 31;
    KeyB: 32;
    KeyC: 33;
    KeyD: 34;
    KeyE: 35;
    KeyF: 36;
    KeyG: 37;
    KeyH: 38;
    KeyI: 39;
    KeyJ: 40;
    KeyK: 41;
    KeyL: 42;
    KeyM: 43;
    KeyN: 44;
    KeyO: 45;
    KeyP: 46;
    KeyQ: 47;
    KeyR: 48;
    KeyS: 49;
    KeyT: 50;
    KeyU: 51;
    KeyV: 52;
    KeyW: 53;
    KeyX: 54;
    KeyY: 55;
    KeyZ: 56;

    Meta: 57;
    ContextMenu: 58;

    F1: 59;
    F2: 60;
    F3: 61;
    F4: 62;
    F5: 63;
    F6: 64;
    F7: 65;
    F8: 66;
    F9: 67;
    F10: 68;
    F11: 69;
    F12: 70;
    F13: 71;
    F14: 72;
    F15: 73;
    F16: 74;
    F17: 75;
    F18: 76;
    F19: 77;

    NumLock: 78;
    ScrollLock: 79;

    /**
     * Used for miscellaneous characters; it can vary by keyboard.
     * For the US standard keyboard, the ';:' key
     */
    Semicolon: 80;
    /**
     * For any country/region, the '+' key
     * For the US standard keyboard, the '=+' key
     */
    Equal: 81;
    /**
     * For any country/region, the ',' key
     * For the US standard keyboard, the ',<' key
     */
    Comma: 82;
    /**
     * For any country/region, the '-' key
     * For the US standard keyboard, the '-_' key
     */
    Minus: 83;
    /**
     * For any country/region, the '.' key
     * For the US standard keyboard, the '.>' key
     */
    Dot: 84;
    /**
     * Used for miscellaneous characters; it can vary by keyboard.
     * For the US standard keyboard, the '/?' key
     */
    Slash: 85;
    /**
     * Used for miscellaneous characters; it can vary by keyboard.
     * For the US standard keyboard, the '`~' key
     */
    Backtick: 86;
    /**
     * Used for miscellaneous characters; it can vary by keyboard.
     * For the US standard keyboard, the '[{' key
     */
    OpenSquareBracket: 87;
    /**
     * Used for miscellaneous characters; it can vary by keyboard.
     * For the US standard keyboard, the '\|' key
     */
    Backslash: 88;
    /**
     * Used for miscellaneous characters; it can vary by keyboard.
     * For the US standard keyboard, the ']}' key
     */
    CloseSquareBracket: 89;
    /**
     * Used for miscellaneous characters; it can vary by keyboard.
     * For the US standard keyboard, the ''"' key
     */
    Quote: 90;
    /**
     * Used for miscellaneous characters; it can vary by keyboard.
     */
    Oem8: 91;
    /**
     * Either the angle bracket key or the backslash key on the RT 102-key keyboard.
     */
    Oem102: 92;

    Numpad0: 93; // VK_NUMPAD0, 0x60, Numeric keypad 0 key
    Numpad1: 94; // VK_NUMPAD1, 0x61, Numeric keypad 1 key
    Numpad2: 95; // VK_NUMPAD2, 0x62, Numeric keypad 2 key
    Numpad3: 96; // VK_NUMPAD3, 0x63, Numeric keypad 3 key
    Numpad4: 97; // VK_NUMPAD4, 0x64, Numeric keypad 4 key
    Numpad5: 98; // VK_NUMPAD5, 0x65, Numeric keypad 5 key
    Numpad6: 99; // VK_NUMPAD6, 0x66, Numeric keypad 6 key
    Numpad7: 100; // VK_NUMPAD7, 0x67, Numeric keypad 7 key
    Numpad8: 101; // VK_NUMPAD8, 0x68, Numeric keypad 8 key
    Numpad9: 102; // VK_NUMPAD9, 0x69, Numeric keypad 9 key

    NumpadMultiply: 103; // VK_MULTIPLY, 0x6A, Multiply key
    NumpadAdd: 104; // VK_ADD, 0x6B, Add key
    NumpadSeparator: 105; // VK_SEPARATOR, 0x6C, Separator key
    NumpadSubtract: 106; // VK_SUBTRACT, 0x6D, Subtract key
    NumpadDecimal: 107; // VK_DECIMAL, 0x6E, Decimal key
    NumpadDivide: 108; // VK_DIVIDE, 0x6F,

    /**
     * Cover all key codes when IME is processing input.
     */
    KeyInComposition: 109;

    AbntC1: 110; // Brazilian (ABNT) Keyboard
    AbntC2: 111; // Brazilian (ABNT) Keyboard
  }

  interface Key {
    (idx: number): boolean;
    codes: Keycodes;
    codeFromString(str: string): number;
    codeToString(code: number): string;
  }

  interface RosebudJS {
    cartSize: number;
    memory: Buffer;

    screen: Buffer;
    palette: Buffer;
    screenWidth: number;
    screenHeight: number;

    updateMousePos(x: number, y: number): void;
    updateBtnState(btn: number, pressed: boolean): void;
    updateWheelState(deltaX: number, deltaY: number, inverted: boolean): void;
    updateKeyState(keycode: number, pressed: boolean): void;

    resetSystemMemory(): void;

    pset(x: number, y: number): void;
    pset(x: number, y: number, color: number): void;
    pget(x: number, y: number): number;

    palset(idx: number, red: number, green: number, blue: number): void;
    palget(idx: number): [number, number, number];

    line(x0: number, y0: number, x1: number, y1: number): void;
    line(x0: number, y0: number, x1: number, y1: number, color: number): void;

    rect(x1: number): void;
    rect(x1: number, y1: number): void;
    rect(x0: number, y0: number, x1: number): void;
    rect(x0: number, y0: number, x1: number, y1: number): void;
    rect(x0: number, y0: number, x1: number, y1: number, color: number): void;

    rectfill(x1: number): void;
    rectfill(x1: number, y1: number): void;
    rectfill(x0: number, y0: number, x1: number): void;
    rectfill(x0: number, y0: number, x1: number, y1: number): void;
    rectfill(x0: number, y0: number, x1: number, y1: number, color: number): void;

    circ(x0: number, y0: number): void;
    circ(x0: number, y0: number, radius: number): void;
    circ(x0: number, y0: number, radius: number, color: number): void;

    circfill(x0: number, y0: number): void;
    circfill(x0: number, y0: number, radius: number): void;
    circfill(x0: number, y0: number, radius: number, color: number): void;

    tri(x0: number, y0: number, x1: number, y1: number, x2: number, y2: number): void;
    tri(x0: number, y0: number, x1: number, y1: number, x2: number, y2: number, color: number): void;

    trifill(x0: number, y0: number, x1: number, y1: number, x2: number, y2: number): void;
    trifill(x0: number, y0: number, x1: number, y1: number, x2: number, y2: number, color: number): void;

    cls(): void;

    print(str: string): void;
    print(str: string, x: number, y: number): void;
    print(str: string, x: number, y: number, color: number): void;

    color(idx: number): void;

    mouse(): [number, number];

    btn(idx: number): boolean;

    btnp(idx: number): boolean;

    wheel(inverted?: boolean): [number, number];

    key: Key;

    keyp: Key;

    poke(idx: number, val: number): void;

    peek(idx: number): number;

    memcpy(destAddr: number, srcAddr: number, length: number): void;

    memset(destAddr: number, val: number, length: number): void;
  }
}

export = rosevm;
