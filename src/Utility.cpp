#include "Utility.h"

std::string GetStringFromKeycode(int32_t _key) {
    std::vector<std::string> table(128, "");
    table = {"ESC",
             "1",
             "2",
             "3",
             "4",
             "5",
             "6",
             "7",
             "8",
             "9",
             "0",
             "-",
             "=",
             "Backspace",
             "Tab",
             "Q",
             "W",
             "E",
             "R",
             "T",
             "Y",
             "U",
             "I",
             "O",
             "P",
             "Left Bracket",
             "Right Bracket",
             "Enter",
             "LCtrl",
             "A",
             "S",
             "D",
             "F",
             "G",
             "H",
             "J",
             "K",
             "L",
             ";",
             "\"",
             "~",
             "LShift",
             "\\",
             "Z",
             "X",
             "C",
             "V",
             "B",
             "N",
             "M",
             ",",
             ".",
             "/",
             "RShift",
             "NUM *",
             "LAlt",
             "Spacebar",
             "Caps Lock",
             "F1",
             "F2",
             "F3",
             "F4",
             "F5",
             "F6",
             "F7",
             "F8",
             "F9",
             "F10",
             "NUM Lock",
             "Scroll Lock",
             "NUM 7",
             "NUM 8",
             "NUM 9",
             "NUM -",
             "NUM 4",
             "NUM 5",
             "NUM 6",
             "NUM +",
             "NUM1",
             "NUM 2",
             "NUM 3",
             "NUM 0",
             "NUM .",
             "F11",
             "F12",
             "NUM Enter",
             "RCtrl",
             "NUM /",
             "SysRq / PtrScr",
             "RAlt",
             "Pause",
             "Home",
             "Up Arrow",
             "PgUp",
             "Left Arrow",
             "Right Arrow",
             "End",
             "Down Arrow",
             "PgDown",
             "Insert",
             "Delete",
             "LMB",
             "RMB",
             "M/W MB",
             "MB3",
             "MB4",
             "MB5",
             "MB6",
             "MB7",
             "MW Up",
             "MW Down",
             "DPAD_UP",
             "DPAD_DOWN",
             "DPAD_LEFT",
             "DPAD_RIGHT",
             "Start",
             "Back",
             "LThumb",
             "RThumb",
             "LShoulder",
             "RShoulder",
             "A",
             "B",
             "X",
             "Y",
             "LT",
             "RT",
             ""};

    uint32_t result;
    if (_key == -1)
        result = 127;
    else if (_key < 84)
        result = _key - 1;
    else if (_key < 89)
        result = _key - 4;
    else if (_key < 158)
        result = _key - 71;
    else if (_key == 181)
        result = _key - 94;
    else if (_key < 185)
        result = _key - 95;
    else if (_key == 197)
        result = _key - 107;
    else if (_key < 202)
        result = _key - 108;
    else if (_key == 203)
        result = _key - 109;
    else if (_key == 205)
        result = _key - 110;
    else if (_key < 212)
        result = _key - 111;
    else
        result = _key - 155;

    return table[result];
}