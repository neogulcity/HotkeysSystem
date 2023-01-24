#include "Input.h"

#include <imgui.h>

#include "Config.h"
#include "EquipsetManager.h"
#include "WidgetHandler.h"


#include "Actor.h"

InputHandler::InputHandler() {
    // Key: scan_code
    // Value: ImGuiKey
    keyMap[1] = ImGuiKey_Escape;
    keyMap[2] = ImGuiKey_1;
    keyMap[3] = ImGuiKey_2;
    keyMap[4] = ImGuiKey_3;
    keyMap[5] = ImGuiKey_4;
    keyMap[6] = ImGuiKey_5;
    keyMap[7] = ImGuiKey_6;
    keyMap[8] = ImGuiKey_7;
    keyMap[9] = ImGuiKey_8;
    keyMap[10] = ImGuiKey_9;
    keyMap[11] = ImGuiKey_0;
    keyMap[12] = ImGuiKey_Minus;
    keyMap[13] = ImGuiKey_Equal;
    keyMap[14] = ImGuiKey_Backspace;
    keyMap[15] = ImGuiKey_Tab;
    keyMap[16] = ImGuiKey_Q;
    keyMap[17] = ImGuiKey_W;
    keyMap[18] = ImGuiKey_E;
    keyMap[19] = ImGuiKey_R;
    keyMap[20] = ImGuiKey_T;
    keyMap[21] = ImGuiKey_Y;
    keyMap[22] = ImGuiKey_U;
    keyMap[23] = ImGuiKey_I;
    keyMap[24] = ImGuiKey_O;
    keyMap[25] = ImGuiKey_P;
    keyMap[26] = ImGuiKey_LeftBracket;
    keyMap[27] = ImGuiKey_RightBracket;
    keyMap[28] = ImGuiKey_Enter;
    keyMap[29] = ImGuiKey_ModCtrl;
    keyMap[30] = ImGuiKey_A;
    keyMap[31] = ImGuiKey_S;
    keyMap[32] = ImGuiKey_D;
    keyMap[33] = ImGuiKey_F;
    keyMap[34] = ImGuiKey_G;
    keyMap[35] = ImGuiKey_H;
    keyMap[36] = ImGuiKey_J;
    keyMap[37] = ImGuiKey_K;
    keyMap[38] = ImGuiKey_L;
    keyMap[39] = ImGuiKey_Semicolon;
    keyMap[40] = ImGuiKey_Apostrophe;
    keyMap[41] = ImGuiKey_GraveAccent;
    keyMap[42] = ImGuiKey_ModShift;
    keyMap[43] = ImGuiKey_Backslash;
    keyMap[44] = ImGuiKey_Z;
    keyMap[45] = ImGuiKey_X;
    keyMap[46] = ImGuiKey_C;
    keyMap[47] = ImGuiKey_V;
    keyMap[48] = ImGuiKey_B;
    keyMap[49] = ImGuiKey_N;
    keyMap[50] = ImGuiKey_M;
    keyMap[51] = ImGuiKey_Comma;
    keyMap[52] = ImGuiKey_Period;
    keyMap[53] = ImGuiKey_Slash;
    keyMap[54] = ImGuiKey_RightShift;
    keyMap[55] = ImGuiKey_KeypadMultiply;
    keyMap[56] = ImGuiKey_ModAlt;
    keyMap[57] = ImGuiKey_Space;
    keyMap[58] = ImGuiKey_CapsLock;
    keyMap[59] = ImGuiKey_F1;
    keyMap[60] = ImGuiKey_F2;
    keyMap[61] = ImGuiKey_F3;
    keyMap[62] = ImGuiKey_F4;
    keyMap[63] = ImGuiKey_F5;
    keyMap[64] = ImGuiKey_F6;
    keyMap[65] = ImGuiKey_F7;
    keyMap[66] = ImGuiKey_F8;
    keyMap[67] = ImGuiKey_F9;
    keyMap[68] = ImGuiKey_F10;
    keyMap[69] = ImGuiKey_NumLock;
    keyMap[70] = ImGuiKey_ScrollLock;
    keyMap[71] = ImGuiKey_Keypad7;
    keyMap[72] = ImGuiKey_Keypad8;
    keyMap[73] = ImGuiKey_Keypad9;
    keyMap[74] = ImGuiKey_KeypadSubtract;
    keyMap[75] = ImGuiKey_Keypad4;
    keyMap[76] = ImGuiKey_Keypad5;
    keyMap[77] = ImGuiKey_Keypad6;
    keyMap[78] = ImGuiKey_KeypadAdd;
    keyMap[79] = ImGuiKey_Keypad1;
    keyMap[80] = ImGuiKey_Keypad2;
    keyMap[81] = ImGuiKey_Keypad3;
    keyMap[82] = ImGuiKey_Keypad0;
    keyMap[83] = ImGuiKey_KeypadDecimal;
    keyMap[87] = ImGuiKey_F11;
    keyMap[88] = ImGuiKey_F12;
    keyMap[156] = ImGuiKey_KeypadEnter;
    keyMap[157] = ImGuiKey_RightCtrl;
    keyMap[181] = ImGuiKey_KeypadDivide;
    keyMap[183] = ImGuiKey_PrintScreen;
    keyMap[184] = ImGuiKey_RightAlt;
    keyMap[197] = ImGuiKey_Pause;
    keyMap[199] = ImGuiKey_Home;
    keyMap[200] = ImGuiKey_UpArrow;
    keyMap[201] = ImGuiKey_PageUp;
    keyMap[203] = ImGuiKey_LeftArrow;
    keyMap[205] = ImGuiKey_RightArrow;
    keyMap[207] = ImGuiKey_End;
    keyMap[208] = ImGuiKey_DownArrow;
    keyMap[209] = ImGuiKey_PageDown;
    keyMap[210] = ImGuiKey_Insert;
    keyMap[211] = ImGuiKey_Delete;

    padMap[1] = ImGuiKey_GamepadDpadUp;
    padMap[2] = ImGuiKey_GamepadDpadDown;
    padMap[4] = ImGuiKey_GamepadDpadLeft;
    padMap[8] = ImGuiKey_GamepadDpadRight;
    padMap[16] = ImGuiKey_GamepadStart;
    padMap[32] = ImGuiKey_GamepadBack;
    padMap[64] = ImGuiKey_GamepadL3;
    padMap[128] = ImGuiKey_GamepadR3;
    padMap[256] = ImGuiKey_GamepadL1;
    padMap[512] = ImGuiKey_GamepadR1;
    padMap[9] = ImGuiKey_GamepadL2;
    padMap[10] = ImGuiKey_GamepadR2;
    padMap[4096] = ImGuiKey_GamepadFaceDown;
    padMap[8192] = ImGuiKey_GamepadFaceRight;
    padMap[16384] = ImGuiKey_GamepadFaceLeft;
    padMap[32768] = ImGuiKey_GamepadFaceUp;
}

void InputHandler::Register() {
    auto device = RE::BSInputDeviceManager::GetSingleton();
    if (!device) return;

    auto input = InputHandler::GetSingleton();
    if (!input) return;

    device->AddEventSink(input);
    logger::info("{} Registered.", typeid(RE::InputEvent).name());
}

uint32_t InputHandler::GetImGuiKey(const uint32_t& _scanCode, RE::INPUT_DEVICE _device) {
    uint32_t key = 0;
    if (_device == RE::INPUT_DEVICE::kKeyboard) {
        key = keyMap[_scanCode];
        if (key == 0) return ImGuiKey_None;
    } else if (_device == RE::INPUT_DEVICE::kGamepad) {
        key = padMap[_scanCode];
    }

    return key;
}

InputHandler::EventResult InputHandler::ProcessEvent(RE::InputEvent* const* _event,
                                                     RE::BSTEventSource<RE::InputEvent*>* _eventSource) {
    if (!_event || !_eventSource) return RE::BSEventNotifyControl::kContinue;

    for (auto event = *_event; event; event = event->next) {
        if (event->eventType == RE::INPUT_EVENT_TYPE::kChar) {
            auto& io = ImGui::GetIO();
            io.AddInputCharacter(static_cast<CharEvent*>(event)->keyCode);

        } else if (event->eventType == RE::INPUT_EVENT_TYPE::kButton) {
            auto& io = ImGui::GetIO();
            const auto button = static_cast<RE::ButtonEvent*>(event);
            if (!button || (button->IsPressed() && !button->IsDown())) continue;

            auto device = button->device.get();
            auto scan_code = button->GetIDCode();
            auto imgui_key = GetImGuiKey(scan_code, device);

            switch (button->device.get()) {
                case RE::INPUT_DEVICE::kMouse:
                    if (scan_code > 7)  // middle scroll
                        io.AddMouseWheelEvent(0, button->Value() * (scan_code == 8 ? 1 : -1));
                    else {
                        if (scan_code > 5) scan_code = 5;
                        io.AddMouseButtonEvent(scan_code, button->IsPressed());
                    }
                    break;

                case RE::INPUT_DEVICE::kKeyboard:
                case RE::INPUT_DEVICE::kGamepad:
                    io.AddKeyEvent(imgui_key, button->IsPressed());
                    break;

                default:
                    break;
            }

            auto config = ConfigHandler::GetSingleton();
            if (!config) return RE::BSEventNotifyControl::kContinue;

            auto manager = EquipsetManager::GetSingleton();
            if (!manager) return RE::BSEventNotifyControl::kContinue;

            if (device == RE::INPUT_DEVICE::kKeyboard || device == RE::INPUT_DEVICE::kGamepad) {
                if (imgui_key == config->Settings.modifier1) {
                    isModifier1 = button->IsPressed();
                } else if (imgui_key == config->Settings.modifier2) {
                    isModifier2 = button->IsPressed();
                } else if (imgui_key == config->Settings.modifier3) {
                    isModifier3 = button->IsPressed();
                }
            }

            if ((device == RE::INPUT_DEVICE::kKeyboard || device == RE::INPUT_DEVICE::kGamepad) && !button->IsUp()) {
                manager->ProcessEquip(imgui_key, isModifier1, isModifier2, isModifier3);
            }

            if ((device == RE::INPUT_DEVICE::kKeyboard || device == RE::INPUT_DEVICE::kGamepad) && !button->IsDown()) {
                manager->CalculateKeydown(imgui_key, isModifier1, isModifier2, isModifier3, button->HeldDuration());
            }
        }
    }

    return RE::BSEventNotifyControl::kContinue;
}