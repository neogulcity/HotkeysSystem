#include "Draw.h"
#include "Data.h"
#include "Config.h"
#include "EquipsetManager.h"
#include "GuiMenu.h"
#include "Translate.h"

#include "Normal.h"
#include "Potion.h"
#include "Cycle.h"

#include <imgui.h>
#include <imgui_internal.h>
#include "extern/imgui_impl_dx11.h"
#include "extern/imgui_stdlib.h"

namespace Draw {
    void HelpMarker(const std::string& _desc) {
        ImGui::SameLine();
        ImGui::TextDisabled("(?)");
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(_desc.c_str());
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }

    void TextCenter(const std::string& _msg) {
        auto windowWidth = ImGui::GetWindowSize().x;
        auto textWidth = ImGui::CalcTextSize(_msg.c_str()).x;

        ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
        ImGui::Text(_msg.c_str());
    }

    void InputButton(uint32_t* _key, const std::string& _id, const std::string& _button,
                         const std::string& _label) {
        auto ts = Translator::GetSingleton();
        if (!ts) return;

        auto buttonMsg = _button + "##" + _id;
        auto popupID = TRANSLATE("_WAITINPUT") + "##" + _id;
        auto popupMsg = "\n  " + TRANSLATE("_WAITINPUTMSG") + "  \n\n";
        auto labelMsg = _label + ImGui::GetKeyName(*_key);

        if (ImGui::Button(buttonMsg.c_str(), ImVec2(80, 0))) {
            ImGui::OpenPopup(popupID.c_str());
        }

        ImGui::SameLine();
        ImGui::AlignTextToFramePadding();
        ImGui::Text(labelMsg.c_str());

        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal(popupID.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text(popupMsg.c_str());
            ImGui::Separator();

            ImGui::SetItemDefaultFocus();
            if (ImGui::Button(C_TRANSLATE("_CANCEL"), ImVec2(-FLT_MIN, 20.0f))) {
                ImGui::CloseCurrentPopup();
            }

            for (uint32_t key = ImGuiKey_NamedKey_BEGIN; key < ImGuiKey_NamedKey_END; ++key) {
                if (ImGui::IsKeyPressed(key)) {
                    if (key == ImGuiKey_Escape) {
                        *_key = ImGuiKey_None;
                    } else {
                        *_key = key;
                    }
                         
                    ImGui::CloseCurrentPopup();
                }
            }

            ImGui::EndPopup();
        }
    }

    void SliderInt(const std::string& _label, int* _value, const int& _min, const int& _max, const std::string& _format, int flags) {
        ImGui::SliderInt(_label.c_str(), _value, _min, _max, _format.c_str(), flags);
        ImGui::SetItemUsingMouseWheel();
        if (ImGui::IsItemHovered()) {
            auto wheel = ImGui::GetIO().MouseWheel;
            if (wheel) {
                if (ImGui::IsItemActive()) {
                    ImGui::ClearActiveID();
                } else {
                    auto ctrl = ImGui::GetIO().KeyCtrl;
                    if (ctrl) {
                        auto value = wheel * 10.0f;
                        if (*_value - value >= _min && *_value - value <= _max) {
                            *_value -= value;
                        } else if (*_value - value < _min) {
                            *_value = _min;
                        } else if (*_value - value > _max) {
                            *_value = _max;
                        }
                    } else {
                        auto value = wheel;
                        if (*_value - value >= _min && *_value - value <= _max) {
                            *_value -= value;
                        } else if (*_value - value < _min) {
                            *_value = _min;
                        } else if (*_value - value > _max) {
                            *_value = _max;
                        }
                    }
                }
            }
        }
    }

    void SliderFloat(const std::string& _label, float* _value, const float& _min, const float& _max, const std::string& _format,
                   int flags) {
        ImGui::SliderFloat(_label.c_str(), _value, _min, _max, _format.c_str(), flags);
        ImGui::SetItemUsingMouseWheel();
        if (ImGui::IsItemHovered()) {
            auto wheel = ImGui::GetIO().MouseWheel;
            if (wheel) {
                if (ImGui::IsItemActive()) {
                    ImGui::ClearActiveID();
                } else {
                    auto ctrl = ImGui::GetIO().KeyCtrl;
                    if (ctrl) {
                        auto value = wheel;
                        if (*_value - value >= _min && *_value - value <= _max) {
                            *_value -= value;
                        } else if (*_value - value < _min) {
                            *_value = _min;
                        } else if (*_value - value > _max) {
                            *_value = _max;
                        }
                    } else {
                        auto value = wheel * 0.1f;
                        if (*_value - value >= _min && *_value - value <= _max) {
                            *_value -= value;
                        } else if (*_value - value < _min) {
                            *_value = _min;
                        } else if (*_value - value > _max) {
                            *_value = _max;
                        }
                    }
                }
            }
        }
    }

    void Combo(const std::vector<std::string>& _items, uint32_t* _current, const std::string& _label) {
        if (_items.size() == 0) return;

        if (ImGui::BeginCombo(_label.c_str(), _items[*_current].c_str())) {
            for (int i = 0; i < _items.size(); i++) {
                bool is_selected = (*_current == i);
                ImGui::PushID(i);
                if (ImGui::Selectable(_items[i].c_str(), is_selected)) {
                    *_current = i;
                }
                if (is_selected) {
                    ImGui::SetItemDefaultFocus();
                }
                ImGui::PopID();
            }
            ImGui::EndCombo();
        }
    }

    void ComboIcon(std::string* _icon_type, const std::string& _label) {
        auto config = ConfigHandler::GetSingleton();
        if (!config) return;

        std::vector<std::string> icon_name, icon_key;
        for (auto& [key, widget] : config->widgetMap) {
            icon_key.push_back(key);
            icon_name.push_back(widget.name);
        }
        if (icon_name.size() != 0) {
            auto name = config->widgetMap[*_icon_type].name;
            if (name == "") name = *_icon_type;
            if (ImGui::BeginCombo(_label.c_str(), name.c_str())) {
                for (int i = 0; i < icon_key.size(); i++) {
                    bool is_selected = (*_icon_type == icon_key[i]);
                    ImGui::PushID(i);
                    if (ImGui::Selectable(icon_name[i].c_str(), is_selected)) {
                        *_icon_type = icon_key[i];
                    }
                    if (is_selected) {
                        ImGui::SetItemDefaultFocus();
                    }
                    ImGui::PopID();
                }
                ImGui::EndCombo();
            }
        } else {
            Draw::Combo({"None"}, 0, _label);
        }
    }

    void PopupConflict(const std::string& _title, const std::string& _name, std::string* _conflictName) {
        auto ts = Translator::GetSingleton();
        if (!ts) return;

        auto name_blank_msg = _title + "##NAME_BLANK";
        auto name_conflict_msg = _title + "##NAME_CONFLICT";
        auto hotkey_conflict_msg = _title + "##HOTKEY_CONFLICT";

        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        if (ImGui::BeginPopupModal(name_blank_msg.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            auto msg = fmt::format("\n  {}  \n\n", "Equipset name must not be blanked.");
            ImGui::Text(msg.c_str());
            ImGui::Separator();

            if (ImGui::Button(C_TRANSLATE("_OK"), ImVec2(-FLT_MIN, 20.0f))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        if (ImGui::BeginPopupModal(name_conflict_msg.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            auto msg = fmt::format("\n  \"{}\" {}  \n\n", _name, "Already Used in other Equipset.");
            ImGui::Text(msg.c_str());
            ImGui::Separator();

            if (ImGui::Button(C_TRANSLATE("_OK"), ImVec2(-FLT_MIN, 20.0f))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        if (ImGui::BeginPopupModal(hotkey_conflict_msg.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            auto msg =
                fmt::format("\n  {}  \n  \"{}\"  \n\n", "Hotkey conflict with following Equipset.", *_conflictName);
            ImGui::Text(msg.c_str());
            ImGui::Separator();

            if (ImGui::Button(C_TRANSLATE("_OK"), ImVec2(-FLT_MIN, 20.0f))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }

    bool CreateNormal() {
        return Create::Normal::Main();
    }

    void ShowNormal(NormalSet* _normalSet, const uint32_t& _id) {
        Show::Normal::Main(_normalSet, _id);
    }

    bool CreatePotion() {
        return Create::Potion::Main();
    }

    void ShowPotion(PotionSet* _potionSet, const uint32_t& _id) {
        Show::Potion::Main(_potionSet, _id);
    }

    bool CreateCycle() {
        return Create::Cycle::Main();
    }

    void ShowCycle(CycleSet* _cycleSet, const uint32_t& _id) {
        Show::Cycle::Main(_cycleSet, _id);
    }
} // namespace Draw