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

#define IMGUI_DEFINE_MATH_OPERATORS

static ImVector<ImRect> s_GroupPanelLabelStack;
static ImVector<ImVec2> s_GroupPanelPaddingStack;

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
        auto popupMsg = "  " + TRANSLATE("_WAITINPUTMSG") + "  ";
        auto labelMsg = _label + ImGui::GetKeyName(static_cast<ImGuiKey>(*_key));

        auto buttonSize = ImGui::CalcTextSize(_button.c_str());
        if (ImGui::Button(buttonMsg.c_str(), ImVec2(buttonSize.x + 30.0f, 0.0f))) {
            ImGui::OpenPopup(popupID.c_str());
        }

        ImGui::SameLine();
        ImGui::AlignTextToFramePadding();
        ImGui::Text(labelMsg.c_str());

        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal(popupID.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text(" ");
            ImGui::Text(popupMsg.c_str());
            ImGui::Text(" ");
            ImGui::Separator();

            auto cancelSize = ImGui::CalcTextSize(C_TRANSLATE("_CANCEL"));
            ImGui::SetItemDefaultFocus();
            if (ImGui::Button(C_TRANSLATE("_CANCEL"), ImVec2(-FLT_MIN, cancelSize.y + 15.0f))) {
                ImGui::CloseCurrentPopup();
            }

            for (uint32_t key = ImGuiKey_NamedKey_BEGIN; key < ImGuiKey_NamedKey_END; ++key) {
                if (ImGui::IsKeyPressed(static_cast<ImGuiKey>(key))) {
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

    bool SliderInt(const std::string& _label, int* _value, const int& _min, const int& _max, const std::string& _format, int flags) {
        auto beforeValue = *_value;
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

        return beforeValue != *_value;
    }

    bool SliderFloat(const std::string& _label, float* _value, const float& _min, const float& _max,
                     const std::string& _format, int flags) {
        auto beforeValue = *_value;
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

        return beforeValue != *_value;
    }

    bool Combo(const std::vector<std::string>& _items, uint32_t* _current, const std::string& _label) {
        if (_items.size() == 0) return false;

        auto beforeValue = *_current;

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

        return beforeValue != *_current;
    }

    bool ComboIcon(std::string* _icon_type, const std::string& _label) {
        auto config = ConfigHandler::GetSingleton();
        if (!config) return false;

        std::vector<std::string> icon_name, icon_key;
        for (auto& [key, widget] : config->widgetMap) {
            icon_key.push_back(key);
            icon_name.push_back(widget.name);
        }

        if (icon_name.size() == 0) {
            Draw::Combo({"None"}, 0, _label);
            return false;
        }

        auto beforeValue = *_icon_type;

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

        return beforeValue != *_icon_type;
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
            auto msg = fmt::format("  {}  ", TRANSLATE("_CONFLICT_MSGA"));
            ImGui::Text(" ");
            ImGui::Text(msg.c_str());
            ImGui::Text(" ");
            ImGui::Separator();

            auto buttonSize = ImGui::CalcTextSize(C_TRANSLATE("_OK"));
            if (ImGui::Button(C_TRANSLATE("_OK"), ImVec2(-FLT_MIN, buttonSize.y + 15.0f))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        if (ImGui::BeginPopupModal(name_conflict_msg.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            auto msgName = fmt::format("\"{}\"", _name);
            auto msg = fmt::format("  {}  ", TRANSLATE("_CONFLICT_MSGB"));
            ImGui::Text(" ");
            Draw::TextCenter(msgName.c_str());
            ImGui::Text(msg.c_str());
            ImGui::Text(" ");
            ImGui::Separator();

            auto buttonSize = ImGui::CalcTextSize(C_TRANSLATE("_OK"));
            if (ImGui::Button(C_TRANSLATE("_OK"), ImVec2(-FLT_MIN, buttonSize.y + 15.0f))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        if (ImGui::BeginPopupModal(hotkey_conflict_msg.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            auto msgName = fmt::format("\"{}\"", *_conflictName);
            auto msg = fmt::format("  {}  ", TRANSLATE("_CONFLICT_MSGC"));
            ImGui::Text(" ");
            ImGui::Text(msg.c_str());
            Draw::TextCenter(msgName.c_str());
            ImGui::Text(" ");
            ImGui::Separator();

            auto buttonSize = ImGui::CalcTextSize(C_TRANSLATE("_OK"));
            if (ImGui::Button(C_TRANSLATE("_OK"), ImVec2(-FLT_MIN, buttonSize.y + 15.0f))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }

    void BeginGroupPanel(const char* name, const ImVec2& size, const ImVec2& _padding) {
        ImGui::BeginGroup();

        auto cursorPos = ImGui::GetCursorScreenPos();
        auto itemSpacing = ImGui::GetStyle().ItemSpacing;
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

        auto frameHeight = ImGui::GetFrameHeight();
        ImGui::BeginGroup();

        ImVec2 effectiveSize = size;
        if (size.x < 0.0f)
            effectiveSize.x = ImGui::GetContentRegionAvail().x;
        else
            effectiveSize.x = size.x;
        ImGui::Dummy(ImVec2(effectiveSize.x, 0.0f));

        ImGui::Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
        ImGui::SameLine(0.0f, 0.0f);
        ImGui::BeginGroup();
        ImGui::Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
        ImGui::SameLine(0.0f, 0.0f);
        ImGui::TextUnformatted(name);
        auto labelMin = ImGui::GetItemRectMin();
        auto labelMax = ImGui::GetItemRectMax();
        ImGui::SameLine(0.0f, 0.0f);
        ImGui::Dummy(ImVec2(0.0, frameHeight + itemSpacing.y));
        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + _padding.x, ImGui::GetCursorPos().y + _padding.y)); // Padding Workaround.
        ImGui::BeginGroup();

        // ImGui::GetWindowDrawList()->AddRect(labelMin, labelMax, IM_COL32(255, 0, 255, 255));

        ImGui::PopStyleVar(2);

#if IMGUI_VERSION_NUM >= 17301
        ImGui::GetCurrentWindow()->ContentRegionRect.Max.x -= frameHeight * 0.5f;
        ImGui::GetCurrentWindow()->WorkRect.Max.x -= frameHeight * 0.5f;
        ImGui::GetCurrentWindow()->InnerRect.Max.x -= frameHeight * 0.5f;
#else
        ImGui::GetCurrentWindow()->ContentsRegionRect.Max.x -= frameHeight * 0.5f;
#endif
        ImGui::GetCurrentWindow()->Size.x -= frameHeight;

        auto itemWidth = ImGui::CalcItemWidth();
        ImGui::PushItemWidth(ImMax(0.0f, itemWidth - frameHeight));

        s_GroupPanelLabelStack.push_back(ImRect(labelMin, labelMax));
        s_GroupPanelPaddingStack.push_back(ImVec2(_padding.x, _padding.y));
    }

    void EndGroupPanel() {
        ImGui::PopItemWidth();

        auto itemSpacing = ImGui::GetStyle().ItemSpacing;

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

        auto frameHeight = ImGui::GetFrameHeight();

        ImGui::EndGroup();

        // ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(0, 255,
        // 0, 64), 4.0f);

        ImGui::EndGroup();

        ImGui::SameLine(0.0f, 0.0f);
        ImGui::Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
        ImGui::Dummy(ImVec2(0.0, frameHeight - frameHeight * 0.5f - itemSpacing.y));

        auto padding = s_GroupPanelPaddingStack.back();
        s_GroupPanelPaddingStack.pop_back();
        ImGui::Dummy(ImVec2(0.0f, padding.y)); // Padding workaround.

        ImGui::EndGroup();

        auto itemMin = ImGui::GetItemRectMin();
        auto itemMax = ImGui::GetItemRectMax();
        // ImGui::GetWindowDrawList()->AddRectFilled(itemMin, itemMax, IM_COL32(255, 0, 0, 64), 4.0f);

        auto labelRect = s_GroupPanelLabelStack.back();
        s_GroupPanelLabelStack.pop_back();

        ImVec2 halfFrame = ImVec2(frameHeight * 0.25f * 0.5f, frameHeight * 0.5f);
        ImRect frameRect =
            ImRect({itemMin.x + halfFrame.x, itemMin.y + halfFrame.y}, {itemMax.x - halfFrame.x, itemMax.y});
        labelRect.Min.x -= itemSpacing.x;
        labelRect.Max.x += itemSpacing.x;
        for (int i = 0; i < 4; ++i) {
            switch (i) {
                // left half-plane
                case 0:
                    ImGui::PushClipRect(ImVec2(-FLT_MAX, -FLT_MAX), ImVec2(labelRect.Min.x, FLT_MAX), true);
                    break;
                // right half-plane
                case 1:
                    ImGui::PushClipRect(ImVec2(labelRect.Max.x, -FLT_MAX), ImVec2(FLT_MAX, FLT_MAX), true);
                    break;
                // top
                case 2:
                    ImGui::PushClipRect(ImVec2(labelRect.Min.x, -FLT_MAX), ImVec2(labelRect.Max.x, labelRect.Min.y),
                                        true);
                    break;
                // bottom
                case 3:
                    ImGui::PushClipRect(ImVec2(labelRect.Min.x, labelRect.Max.y), ImVec2(labelRect.Max.x, FLT_MAX),
                                        true);
                    break;
            }

            ImGui::GetWindowDrawList()->AddRect(frameRect.Min, frameRect.Max,
                                                ImColor(ImGui::GetStyleColorVec4(ImGuiCol_Border)), halfFrame.x);

            ImGui::PopClipRect();
        }

        ImGui::PopStyleVar(2);

#if IMGUI_VERSION_NUM >= 17301
        ImGui::GetCurrentWindow()->ContentRegionRect.Max.x += frameHeight * 0.5f;
        ImGui::GetCurrentWindow()->WorkRect.Max.x += frameHeight * 0.5f;
        ImGui::GetCurrentWindow()->InnerRect.Max.x += frameHeight * 0.5f;
#else
        ImGui::GetCurrentWindow()->ContentsRegionRect.Max.x += frameHeight * 0.5f;
#endif
        ImGui::GetCurrentWindow()->Size.x += frameHeight;

        ImGui::Dummy(ImVec2(0.0f, 0.0f));

        ImGui::EndGroup();
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