/// <reference types="node" />

declare function rosevm(hd?: boolean): rosevm.RosebudJS;

declare namespace rosevm {
  interface Keycodes {
    A: 0;
    B: 1;
    C: 2;
    D: 3;
    E: 4;
    F: 5;
    G: 6;
    H: 7;
    I: 8;
    J: 9;
    K: 10;
    L: 11;
    M: 12;
    N: 13;
    O: 14;
    P: 15;
    Q: 16;
    R: 17;
    S: 18;
    T: 19;
    U: 20;
    V: 21;
    W: 22;
    X: 23;
    Y: 24;
    Z: 25;
    Number1: 26;
    Number2: 27;
    Number3: 28;
    Number4: 29;
    Number5: 30;
    Number6: 31;
    Number7: 32;
    Number8: 33;
    Number9: 34;
    Number0: 35;
    Return: 36;
    Escape: 37;
    Backspace: 38;
    Tab: 39;
    Space: 40;
    Minus: 41;
    Equals: 42;
    Leftbracket: 43;
    Rightbracket: 44;
    Backslash: 45;
    Nonushash: 46;
    Semicolon: 47;
    Apostrophe: 48;
    Grave: 49;
    Comma: 50;
    Period: 51;
    Slash: 52;
    Capslock: 53;
    F1: 54;
    F2: 55;
    F3: 56;
    F4: 57;
    F5: 58;
    F6: 59;
    F7: 60;
    F8: 61;
    F9: 62;
    F10: 63;
    F11: 64;
    F12: 65;
    Printscreen: 66;
    Scrolllock: 67;
    Pause: 68;
    Insert: 69;
    Home: 70;
    Pageup: 71;
    Delete: 72;
    End: 73;
    Pagedown: 74;
    Right: 75;
    Left: 76;
    Down: 77;
    Up: 78;
    Numlockclear: 79;
    KpDivide: 80;
    KpMultiply: 81;
    KpMinus: 82;
    KpPlus: 83;
    KpEnter: 84;
    Keypad1: 85;
    Keypad2: 86;
    Keypad3: 87;
    Keypad4: 88;
    Keypad5: 89;
    Keypad6: 90;
    Keypad7: 91;
    Keypad8: 92;
    Keypad9: 93;
    Keypad0: 94;
    KpPeriod: 95;
    Nonusbackslash: 96;
    Application: 97;
    Power: 98;
    KpEquals: 99;
    F13: 100;
    F14: 101;
    F15: 102;
    F16: 103;
    F17: 104;
    F18: 105;
    F19: 106;
    F20: 107;
    F21: 108;
    F22: 109;
    F23: 110;
    F24: 111;
    Execute: 112;
    Help: 113;
    Menu: 114;
    Select: 115;
    Stop: 116;
    Again: 117;
    Undo: 118;
    Cut: 119;
    Copy: 120;
    Paste: 121;
    Find: 122;
    Mute: 123;
    Volumeup: 124;
    Volumedown: 125;
    KpComma: 126;
    KpEqualsas400: 127;
    International1: 128;
    International2: 129;
    International3: 130;
    International4: 131;
    International5: 132;
    International6: 133;
    International7: 134;
    International8: 135;
    International9: 136;
    Lang1: 137;
    Lang2: 138;
    Lang3: 139;
    Lang4: 140;
    Lang5: 141;
    Lang6: 142;
    Lang7: 143;
    Lang8: 144;
    Lang9: 145;
    Alterase: 146;
    Sysreq: 147;
    Cancel: 148;
    Clear: 149;
    Prior: 150;
    Return2: 151;
    Separator: 152;
    Out: 153;
    Oper: 154;
    Clearagain: 155;
    Crsel: 156;
    Exsel: 157;
    Kp00_158;
    Kp000_159;
    Thousandsseparator: 160;
    Decimalseparator: 161;
    Currencyunit: 162;
    Currencysubunit: 163;
    KpLeftparen: 164;
    KpRightparen: 165;
    KpLeftbrace: 166;
    KpRightbrace: 167;
    KpTab: 168;
    KpBackspace: 169;
    KpA: 170;
    KpB: 171;
    KpC: 172;
    KpD: 173;
    KpE: 174;
    KpF: 175;
    KpXor: 176;
    KpPower: 177;
    KpPercent: 178;
    KpLess: 179;
    KpGreater: 180;
    KpAmpersand: 181;
    KpDblampersand: 182;
    KpVerticalbar: 183;
    KpDblverticalbar: 184;
    KpColon: 185;
    KpHash: 186;
    KpSpace: 187;
    KpAt: 188;
    KpExclam: 189;
    KpMemstore: 190;
    KpMemrecall: 191;
    KpMemclear: 192;
    KpMemadd: 193;
    KpMemsubtract: 194;
    KpMemmultiply: 195;
    KpMemdivide: 196;
    KpPlusminus: 197;
    KpClear: 198;
    KpClearentry: 199;
    KpBinary: 200;
    KpOctal: 201;
    KpDecimal: 202;
    KpHexadecimal: 203;
    Lctrl: 204;
    Lshift: 205;
    Lalt: 206;
    Lgui: 207;
    Rctrl: 208;
    Rshift: 209;
    Ralt: 210;
    Rgui: 211;
    Mode: 212;
    Audionext: 213;
    Audioprev: 214;
    Audiostop: 215;
    Audioplay: 216;
    Audiomute: 217;
    Mediaselect: 218;
    Www: 219;
    Mail: 220;
    Calculator: 221;
    Computer: 222;
    AcSearch: 223;
    AcHome: 224;
    AcBack: 225;
    AcForward: 226;
    AcStop: 227;
    AcRefresh: 228;
    AcBookmarks: 229;
    Brightnessdown: 230;
    Brightnessup: 231;
    Displayswitch: 232;
    Kbdillumtoggle: 233;
    Kbdillumdown: 234;
    Kbdillumup: 235;
    Eject: 236;
    Sleep: 237;
    App1: 238;
    App2: 239;
    Unknown: 240;
  }

  interface Key {
    (idx: number): boolean;
    codes: Keycodes;
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
