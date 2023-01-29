#include "Cycle.h"

#include "Config.h"
#include "Translate.h"
#include "EquipsetManager.h"
#include "Data.h"
#include "Draw.h"
#include "GuiMenu.h"

#include <imgui.h>
#include "extern/imgui_impl_dx11.h"
#include "extern/imgui_stdlib.h"
#include "extern/IconsFontAwesome5.h"

namespace Shared::Cycle {
    void HotkeySection(uint32_t* _hotkey, bool* _modifier1, bool* _modifier2, bool* _modifier3) {
        auto ts = Translator::GetSingleton();
        if (!ts) return;

        auto config = ConfigHandler::GetSingleton();
        if (!config) return;

        Draw::InputButton(_hotkey, "EquipsetHotkey", TRANSLATE("_EDIT"), TRANSLATE("_EDIT_HOTKEYLABEL"));

        auto msg1 = fmt::format("+ {}", ImGui::GetKeyName(config->Settings.modifier1));
        auto msg2 = fmt::format("+ {}", ImGui::GetKeyName(config->Settings.modifier2));
        auto msg3 = fmt::format("+ {}", ImGui::GetKeyName(config->Settings.modifier3));
        ImGui::Checkbox(msg1.c_str(), _modifier1);
        ImGui::Checkbox(msg2.c_str(), _modifier2);
        ImGui::Checkbox(msg3.c_str(), _modifier3);
    }

    void OptionSection(bool* _cyclePersist, float* _cycleExpire, float* _cycleReset) {
        auto ts = Translator::GetSingleton();
        if (!ts) return;

        ImGui::Checkbox(C_TRANSLATE("_CYCLEPERSIST"), _cyclePersist);
        Draw::HelpMarker(TRANSLATE("_CYCLEPERSIST_HELP"));
        Draw::SliderFloat(C_TRANSLATE("_CYCLEEXPIRE"), _cycleExpire, 0, 15, "%.1f", ImGuiSliderFlags_AlwaysClamp);
        Draw::HelpMarker(TRANSLATE("_CYCLEEXPIRE_HELP"));
        Draw::SliderFloat(C_TRANSLATE("_CYCLERESET"), _cycleReset, 0, 15, "%.1f", ImGuiSliderFlags_AlwaysClamp);
        Draw::HelpMarker(TRANSLATE("_CYCLERESET_HELP"));
    }

    void WidgetSection(bool* _icon_enable, int* _icon_offsetX, int* _icon_offsetY, bool* _name_enable, uint32_t* _name_align_type,
                       int* _name_offsetX, int* _name_offsetY, bool* _hotkey_enable, uint32_t* _hotkey_align_type,
                       int* _hotkey_offsetX, int* _hotkey_offsetY) {
        auto ts = Translator::GetSingleton();
        if (!ts) return;

        auto config = ConfigHandler::GetSingleton();
        if (!config) return;

        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::TreeNode(C_TRANSLATE("_WIDGET_ICON"))) {
            ImGui::Checkbox(C_TRANSLATE("_WIDGET_ENABLE"), _icon_enable);
            Draw::SliderInt(C_TRANSLATE("_WIDGET_OFFSETX"), _icon_offsetX, Config::icon_smin,
                             Config::icon_smax, "%d", ImGuiSliderFlags_AlwaysClamp);
            Draw::SliderInt(C_TRANSLATE("_WIDGET_OFFSETY"), _icon_offsetY, Config::icon_smin,
                             Config::icon_smax, "%d", ImGuiSliderFlags_AlwaysClamp);

            ImGui::TreePop();
        }

        std::vector<std::string> align_items = {TRANSLATE("_ALIGN_LEFT"),
                                                TRANSLATE("_ALIGN_RIGHT"),
                                                TRANSLATE("_ALIGN_CENTER")};

        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::TreeNode(C_TRANSLATE("_WIDGET_NAME"))) {
            ImGui::Checkbox(C_TRANSLATE("_WIDGET_ENABLE"), _name_enable);
            Draw::Combo(align_items, _name_align_type, TRANSLATE("_WIDGET_ALIGN"));
            Draw::SliderInt(C_TRANSLATE("_WIDGET_OFFSETX"), _name_offsetX, Config::text_smin,
                             Config::text_smax, "%d", ImGuiSliderFlags_AlwaysClamp);
            Draw::SliderInt(C_TRANSLATE("_WIDGET_OFFSETY"), _name_offsetY, Config::text_smin,
                             Config::text_smax, "%d", ImGuiSliderFlags_AlwaysClamp);

            ImGui::TreePop();
        }

        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::TreeNode(C_TRANSLATE("_WIDGET_HOTKEY"))) {
            ImGui::Checkbox(C_TRANSLATE("_WIDGET_ENABLE"), _hotkey_enable);
            Draw::Combo(align_items, _hotkey_align_type, TRANSLATE("_WIDGET_ALIGN"));
            Draw::SliderInt(C_TRANSLATE("_WIDGET_OFFSETX"), _hotkey_offsetX, Config::text_smin,
                             Config::text_smax, "%d", ImGuiSliderFlags_AlwaysClamp);
            Draw::SliderInt(C_TRANSLATE("_WIDGET_OFFSETY"), _hotkey_offsetY, Config::text_smin,
                             Config::text_smax, "%d", ImGuiSliderFlags_AlwaysClamp);

            ImGui::TreePop();
        }
    }

    void EquipsetSection(std::vector<std::string>* _equipset, uint32_t* _page_equipsetIndex, uint32_t* _popup_equipsetIndex) {
        auto ts = Translator::GetSingleton();
        if (!ts) return;

        auto manager = EquipsetManager::GetSingleton();
        if (!manager) return;

        if (ImGui::BeginTable("Item_Table", 2)) {
            ImGui::TableNextColumn();
            if (ImGui::Button(C_TRANSLATE("_ADD"), {ImGui::GetContentRegionAvail().x - 7.5f, 0.f})) {
                uint32_t count = 0U;
                for (auto elem : manager->equipsetVec) {
                    if (elem->type == Equipset::TYPE::CYCLE ||
                        elem->type == Equipset::TYPE::POTION) continue;

                    count++;
                }

                if (count != 0U) {
                    ImGui::OpenPopup(C_TRANSLATE("_ITEM_ADDPOPUP"));
                }
            }

            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
            if (ImGui::BeginPopupModal(C_TRANSLATE("_ITEM_ADDPOPUP"), NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                std::vector<std::string> items;
                for (auto elem : manager->equipsetVec) {
                    if (elem->type == Equipset::TYPE::CYCLE ||
                        elem->type == Equipset::TYPE::POTION) continue;
                    items.push_back(elem->name);
                }

                if (ImGui::BeginListBox("##listbox1")) {
                    for (int i = 0; i < items.size(); i++) {
                        const bool is_selected = (*_popup_equipsetIndex == i);
                        if (ImGui::Selectable(items[i].c_str(), is_selected)) {
                            *_popup_equipsetIndex = i;
                        }

                        if (is_selected) {
                            ImGui::SetItemDefaultFocus();
                        }
                    }
                    ImGui::EndListBox();
                }

                if (ImGui::BeginTable("Button_Table", 2)) {
                    ImGui::TableNextColumn();
                    auto buttonSize = ImGui::CalcTextSize((TRANSLATE("_OK") + TRANSLATE("_CANCEL")).c_str());
                    if (ImGui::Button(C_TRANSLATE("_OK"),
                                      ImVec2(ImGui::GetWindowContentRegionMax().x * 0.45f, buttonSize.y + 15.0f))) {
                        _equipset->push_back(items[*_popup_equipsetIndex]);
                        ImGui::CloseCurrentPopup();
                    }

                    ImGui::TableNextColumn();
                    if (ImGui::Button(C_TRANSLATE("_CANCEL"),
                                      ImVec2(ImGui::GetWindowContentRegionMax().x * 0.45f, buttonSize.y + 15.0f))) {
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndTable();
                }
                ImGui::EndPopup();
            }

            ImGui::TableNextColumn();
            if (ImGui::Button(C_TRANSLATE("_REMOVE"), {ImGui::GetContentRegionAvail().x - 7.5f, 0.f})) {
                if (*_page_equipsetIndex < _equipset->size()) {
                    _equipset->erase(_equipset->begin() + *_page_equipsetIndex);
                }
            }

            ImGui::EndTable();
        }
        ImGui::Separator();

        for (int i = 0; i < _equipset->size(); i++) {
            auto equipset = *(_equipset->begin() + i);
            const bool is_selected = (*_page_equipsetIndex == i);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5.0f);
            if (ImGui::Selectable(equipset.c_str(), is_selected, ImGuiSelectableFlags_None,
                                  ImVec2(ImGui::GetContentRegionAvail().x - 15.0f, 0.0f))) {
                *_page_equipsetIndex = i;
            }

            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
    }
}  // namespace Shared::Cycle

namespace Create::Cycle {
    void NameSection(std::string* _name) {
        auto ts = Translator::GetSingleton();
        if (!ts) return;

        auto manager = EquipsetManager::GetSingleton();
        if (!manager) return;

        auto drawHelper = DrawHelper::GetSingleton();
        if (!drawHelper) return;

        auto buttonSize = ImGui::CalcTextSize(C_TRANSLATE("_EDIT"));
        if (ImGui::Button(C_TRANSLATE("_EDIT"), ImVec2(buttonSize.x + 30.0f, 0.0f))) {
            ImGui::OpenPopup(C_TRANSLATE("_EDIT_POPUP_TITLE"));
        }
        ImGui::SameLine();
        ImGui::AlignTextToFramePadding();
        ImGui::Text((TRANSLATE("_EDIT_NAMELABEL") + *_name).c_str());

        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal(C_TRANSLATE("_EDIT_POPUP_TITLE"), NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            static std::string tempName = *_name;
            if (drawHelper->ShouldReloadName()) {
                tempName = *_name;
                drawHelper->NotifyReloadName(false);
            }

            ImGui::Text(C_TRANSLATE("_EDIT_NAMEMSG"));
            ImGui::Text(" ");
            ImGui::InputText("##NameInput", &tempName);
            ImGui::Separator();

            if (ImGui::BeginTable("Button_Table", 2)) {
                ImGui::TableNextColumn();
                auto buttonSize = ImGui::CalcTextSize((TRANSLATE("_OK") + TRANSLATE("_CANCEL")).c_str());
                if (ImGui::Button(C_TRANSLATE("_OK"),
                                  ImVec2(ImGui::GetWindowContentRegionMax().x * 0.45f, buttonSize.y + 15.0f))) {
                    *_name = tempName;
                    drawHelper->NotifyReloadName(true);
                    ImGui::CloseCurrentPopup();
                }

                ImGui::TableNextColumn();
                if (ImGui::Button(C_TRANSLATE("_CANCEL"),
                                  ImVec2(ImGui::GetWindowContentRegionMax().x * 0.45f, buttonSize.y + 15.0f))) {
                    drawHelper->NotifyReloadName(true);
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndTable();
            }
            ImGui::EndPopup();
        }
    }

	bool Main() {
        bool close_popup = false;
        
        auto ts = Translator::GetSingleton();
        if (!ts) return close_popup;

        auto drawHelper = DrawHelper::GetSingleton();
        if (!drawHelper) return close_popup;

        auto manager = EquipsetManager::GetSingleton();
        if (!manager) return close_popup;

        auto config = ConfigHandler::GetSingleton();
        if (!config) return close_popup;

        static std::string name = manager->GetNamePreset();
        static uint32_t hotkey = 0U;
        static bool modifier1 = false;
        static bool modifier2 = false;
        static bool modifier3 = false;
        static bool cyclePersist = false;
        static float cycleExpire = 0.0f;
        static float cycleReset = 0.0f;
        static bool icon_enable = false;
        static int icon_offsetX = 0;
        static int icon_offsetY = 0;
        static bool name_enable = false;
        static uint32_t name_align_type = WidgetText::ALIGN_TYPE::CENTER;
        static int name_offsetX = 0;
        static int name_offsetY = 0;
        static bool hotkey_enable = false;
        static uint32_t hotkey_align_type = WidgetText::ALIGN_TYPE::CENTER;
        static int hotkey_offsetX = 0;
        static int hotkey_offsetY = 0;
        static std::vector<std::string> equipset;
        static uint32_t page_equipsetIndex = 0U;
        static uint32_t popup_equipsetIndex = 0U;
        static std::string hotkey_conflictName;

        if (drawHelper->ShouldReload()) {
            name = manager->GetNamePreset();
            hotkey = 0;
            modifier1 = false;
            modifier2 = false;
            modifier3 = false;
            cyclePersist = false;
            cycleExpire = 0.0f;
            cycleReset = 0.0f;
            icon_enable = false;
            icon_offsetX = 0;
            icon_offsetY = 0;
            name_enable = false;
            name_align_type = WidgetText::ALIGN_TYPE::CENTER;
            name_offsetX = 0;
            name_offsetY = 0;
            hotkey_enable = false;
            hotkey_align_type = WidgetText::ALIGN_TYPE::CENTER;
            hotkey_offsetX = 0;
            hotkey_offsetY = 0;
            equipset.clear();
            page_equipsetIndex = 0;
            popup_equipsetIndex = 0;

            drawHelper->NotifyReload(false);
        }

        NameSection(&name);
        ImGui::Separator();

        static auto groupSize = ImVec2(0.0f, 0.0f);
        static auto groupLeftSize = ImVec2(0.0f, 0.0f);
        static auto groupRightSize = ImVec2(0.0f, 0.0f);
        ImGui::BeginChild("LeftRegion", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, groupSize.y), false);
        {
            ImGui::BeginGroup();
            {
                auto hotkeyLabel = fmt::format("{}  {}", ICON_FA_KEYBOARD, TRANSLATE("_ICON_HOTKEY"));
                Draw::BeginGroupPanel(hotkeyLabel.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0.0f),
                                      ImVec2(15.0f, 10.0f));
                {
                    Shared::Cycle::HotkeySection(&hotkey, &modifier1, &modifier2, &modifier3);
                }
                Draw::EndGroupPanel();

                auto optionLabel = fmt::format("{}  {}", ICON_FA_SLIDERS_H, TRANSLATE("_ICON_OPTION"));
                Draw::BeginGroupPanel(optionLabel.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0.0f),
                                      ImVec2(15.0f, 10.0f));
                {
                    Shared::Cycle::OptionSection(&cyclePersist, &cycleExpire, &cycleReset);
                }
                Draw::EndGroupPanel();
            }
            ImGui::EndGroup();
            auto size = ImGui::GetItemRectSize();
            if (size.y > 50.0f) groupLeftSize = size;
        }
        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::BeginChild("RightRegion", ImVec2(0.0f, groupSize.y), false);
        {
            ImGui::BeginGroup();
            {
                auto widgetLabel = fmt::format("{}  {}", ICON_FA_IMAGE, TRANSLATE("_ICON_WIDGET"));
                Draw::BeginGroupPanel(widgetLabel.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0.0f),
                                      ImVec2(15.0f, 10.0f));
                {
                    Shared::Cycle::WidgetSection(&icon_enable, &icon_offsetX, &icon_offsetY, &name_enable,
                                                 &name_align_type, &name_offsetX, &name_offsetY, &hotkey_enable,
                                                 &hotkey_align_type, &hotkey_offsetX, &hotkey_offsetY);
                }
                Draw::EndGroupPanel();
            }
            ImGui::EndGroup();
            auto size = ImGui::GetItemRectSize();
            if (size.y > 50.0f) groupRightSize = size;
        }
        ImGui::EndChild();

        groupSize = groupLeftSize.y > groupRightSize.y ? groupLeftSize : groupRightSize;

        ImGui::Separator();

        static auto groupSize2 = ImVec2(0.0f, 0.0f);
        static auto groupLeftSize2 = ImVec2(0.0f, 0.0f);
        static auto groupRightSize2 = ImVec2(0.0f, 0.0f);

        auto margin = ImGui::CalcTextSize((TRANSLATE("_OK") + TRANSLATE("_CANCEL")).c_str()).y + 30.0f;
        if (groupSize2.y < ImGui::GetContentRegionAvail().y - margin) {
            groupSize2.y = ImGui::GetContentRegionAvail().y - margin;
        }

        ImGui::BeginChild("LeftRegion_2", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, groupSize2.y), false);
        {
            //ImGui::BeginGroup();
            //{
            //    auto weaponLabel = fmt::format("{}  {}", ICON_FA_HAMMER, TRANSLATE("_ICON_WEAPON"));
            //    Draw::BeginGroupPanel(weaponLabel.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0.0f),
            //                          ImVec2(15.0f, 10.0f));
            //    {
            //        Shared::Normal::WeaponSection(&lefthand, &righthand, &shout);
            //    }
            //    Draw::EndGroupPanel();
            //}
            //ImGui::EndGroup();
            //groupLeftSize2 = ImGui::GetItemRectSize();
        }
        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::BeginChild("RightRegion_2", ImVec2(0.0f, groupSize2.y), false);
        {
            ImGui::BeginGroup();
            {
                auto equipsetLabel = fmt::format("{}  {}", ICON_FA_DATABASE, TRANSLATE("_ICON_EQUIPSET"));
                Draw::BeginGroupPanel(equipsetLabel.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0.0f),
                                      ImVec2(15.0f, 10.0f));
                {
                    Shared::Cycle::EquipsetSection(&equipset, &page_equipsetIndex, &popup_equipsetIndex);
                }
                Draw::EndGroupPanel();
            }
            ImGui::EndGroup();
            auto size = ImGui::GetItemRectSize();
            if (size.y > 50.0f) groupRightSize2 = size;
        }
        ImGui::EndChild();

        groupSize2 = groupLeftSize2.y > groupRightSize2.y ? groupLeftSize2 : groupRightSize2;

        ImGui::Separator();

        if (ImGui::BeginTable("End_Table", 2)) {
            ImGui::TableNextColumn();

            auto title = TRANSLATE("_WARNING");
            auto buttonSize = ImGui::CalcTextSize((TRANSLATE("_OK") + TRANSLATE("_CANCEL")).c_str());
            if (ImGui::Button(C_TRANSLATE("_OK"), {-FLT_MIN, buttonSize.y + 15.0f})) {
                auto [result_type, result_string] =
                    manager->IsCreateValid(name, hotkey, modifier1, modifier2, modifier3);

                if (result_type == EquipsetManager::VALID_TYPE::NAME_BLANK) {
                    ImGui::OpenPopup((title + "##NAME_BLANK").c_str());

                } else if (result_type == EquipsetManager::VALID_TYPE::NAME_CONFLICT) {
                    ImGui::OpenPopup((title + "##NAME_CONFLICT").c_str());

                } else if (result_type == EquipsetManager::VALID_TYPE::HOTKEY_CONFLICT) {
                    hotkey_conflictName = result_string;
                    ImGui::OpenPopup((title + "##HOTKEY_CONFLICT").c_str());

                } else if (result_type == EquipsetManager::VALID_TYPE::GOOD) {
                    CycleSet cycleset;
                    cycleset.type = Equipset::TYPE::CYCLE;
                    cycleset.name = name;
                    cycleset.hotkey = hotkey;
                    cycleset.modifier1 = modifier1;
                    cycleset.modifier2 = modifier2;
                    cycleset.modifier3 = modifier3;
                    cycleset.cyclePersist = cyclePersist;
                    cycleset.cycleExpire = cycleExpire;
                    cycleset.cycleReset = cycleReset;
                    cycleset.widgetIcon.enable = icon_enable;
                    cycleset.widgetIcon.offsetX = icon_offsetX;
                    cycleset.widgetIcon.offsetY = icon_offsetY;
                    cycleset.widgetName.enable = name_enable;
                    cycleset.widgetName.align = static_cast<WidgetText::ALIGN_TYPE>(name_align_type);
                    cycleset.widgetName.offsetX = name_offsetX;
                    cycleset.widgetName.offsetY = name_offsetY;
                    cycleset.widgetHotkey.enable = hotkey_enable;
                    cycleset.widgetHotkey.align = static_cast<WidgetText::ALIGN_TYPE>(hotkey_align_type);
                    cycleset.widgetHotkey.offsetX = hotkey_offsetX;
                    cycleset.widgetHotkey.offsetY = hotkey_offsetY;
                    cycleset.items = equipset;

                    auto manager = EquipsetManager::GetSingleton();
                    if (!manager) logger::error("Failed to get EquipsetManager!");

                    manager->Create(cycleset, true);

                    close_popup = true;
                }
            }

            Draw::PopupConflict(title, name, &hotkey_conflictName);

            ImGui::TableNextColumn();
            if (ImGui::Button(C_TRANSLATE("_CANCEL"), {-FLT_MIN, buttonSize.y + 15.0f})) {
                close_popup = true;
            }
            ImGui::EndTable();
        }

        return close_popup;
	}
}

namespace Show::Cycle {
    void NameSection(std::string* _name) {
        auto ts = Translator::GetSingleton();
        if (!ts) return;

        auto drawHelper = DrawHelper::GetSingleton();
        if (!drawHelper) return;

        auto buttonSize = ImGui::CalcTextSize(C_TRANSLATE("_EDIT_NAME"));
        if (ImGui::Button(C_TRANSLATE("_EDIT_NAME"), ImVec2(buttonSize.x + 30.0f, 0.0f))) {
            ImGui::OpenPopup(C_TRANSLATE("_EDIT_POPUP_TITLE"));
        }

        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal(C_TRANSLATE("_EDIT_POPUP_TITLE"), NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            static std::string tempName = *_name;
            if (drawHelper->ShouldReloadName()) {
                tempName = *_name;
                drawHelper->NotifyReloadName(false);
            }

            ImGui::Text(C_TRANSLATE("_EDIT_NAMEMSG"));
            ImGui::Text(" ");
            ImGui::InputText("##NameInput", &tempName);
            ImGui::Separator();

            if (ImGui::BeginTable("Button_Table", 2)) {
                ImGui::TableNextColumn();
                auto buttonSize = ImGui::CalcTextSize((TRANSLATE("_OK") + TRANSLATE("_CANCEL")).c_str());
                if (ImGui::Button(C_TRANSLATE("_OK"),
                                  ImVec2(ImGui::GetWindowContentRegionMax().x * 0.45f, buttonSize.y + 15.0f))) {
                    *_name = tempName;
                    drawHelper->NotifyReloadName(true);
                    ImGui::CloseCurrentPopup();
                }

                ImGui::TableNextColumn();
                if (ImGui::Button(C_TRANSLATE("_CANCEL"),
                                  ImVec2(ImGui::GetWindowContentRegionMax().x * 0.45f, buttonSize.y + 15.0f))) {
                    drawHelper->NotifyReloadName(true);
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndTable();
            }
            ImGui::EndPopup();
        }
    }

    void Main(CycleSet* _cycleSet, const uint32_t& _id) {
        auto ts = Translator::GetSingleton();
        if (!ts) return;

        auto drawHelper = DrawHelper::GetSingleton();
        if (!drawHelper) return;

        auto manager = EquipsetManager::GetSingleton();
        if (!manager) return;

        auto gui = GuiMenu::GetSingleton();
        if (!gui) return;

        if (gui->GetCurrentOpened() != _id) {
            drawHelper->NotifyReload(true);
        }
        gui->SetCurrentOpened(_id);

        auto cycleset = _cycleSet;

        static std::string name = cycleset->name;
        static uint32_t hotkey = cycleset->hotkey;
        static bool modifier1 = cycleset->modifier1;
        static bool modifier2 = cycleset->modifier2;
        static bool modifier3 = cycleset->modifier3;
        static bool cyclePersist = cycleset->cyclePersist;
        static float cycleExpire = cycleset->cycleExpire;
        static float cycleReset = cycleset->cycleReset;
        static bool icon_enable = cycleset->widgetIcon.enable;
        static int icon_offsetX = cycleset->widgetIcon.offsetX;
        static int icon_offsetY = cycleset->widgetIcon.offsetY;
        static bool name_enable = cycleset->widgetName.enable;
        static uint32_t name_align_type = cycleset->widgetName.align;
        static int name_offsetX = cycleset->widgetName.offsetX;
        static int name_offsetY = cycleset->widgetName.offsetY;
        static bool hotkey_enable = cycleset->widgetHotkey.enable;
        static uint32_t hotkey_align_type = cycleset->widgetHotkey.align;
        static int hotkey_offsetX = cycleset->widgetHotkey.offsetX;
        static int hotkey_offsetY = cycleset->widgetHotkey.offsetY;
        static std::vector<std::string> equipset = cycleset->items;
        static uint32_t page_equipsetIndex = 0U;
        static uint32_t popup_equipsetIndex = 0U;
        static std::string hotkey_conflictName;

        if (drawHelper->ShouldReload()) {
            name = cycleset->name;
            hotkey = cycleset->hotkey;
            modifier1 = cycleset->modifier1;
            modifier2 = cycleset->modifier2;
            modifier3 = cycleset->modifier3;
            cyclePersist = cycleset->cyclePersist;
            cycleExpire = cycleset->cycleExpire;
            cycleReset = cycleset->cycleReset;
            icon_enable = cycleset->widgetIcon.enable;
            icon_offsetX = cycleset->widgetIcon.offsetX;
            icon_offsetY = cycleset->widgetIcon.offsetY;
            name_enable = cycleset->widgetName.enable;
            name_align_type = cycleset->widgetName.align;
            name_offsetX = cycleset->widgetName.offsetX;
            name_offsetY = cycleset->widgetName.offsetY;
            hotkey_enable = cycleset->widgetHotkey.enable;
            hotkey_align_type = cycleset->widgetHotkey.align;
            hotkey_offsetX = cycleset->widgetHotkey.offsetX;
            hotkey_offsetY = cycleset->widgetHotkey.offsetY;
            equipset = cycleset->items;
            page_equipsetIndex = 0U;
            popup_equipsetIndex = 0U;

            drawHelper->NotifyReload(false);
        }

        static bool isChanged = false;
        if (name != cycleset->name ||
            hotkey != cycleset->hotkey ||
            modifier1 != cycleset->modifier1 ||
            modifier2 != cycleset->modifier2 ||
            modifier3 != cycleset->modifier3 ||
            cyclePersist != cycleset->cyclePersist ||
            cycleExpire != cycleset->cycleExpire ||
            cycleReset != cycleset->cycleReset ||
            icon_enable != cycleset->widgetIcon.enable ||
            icon_offsetX != cycleset->widgetIcon.offsetX ||
            icon_offsetY != cycleset->widgetIcon.offsetY ||
            name_enable != cycleset->widgetName.enable ||
            name_align_type != cycleset->widgetName.align ||
            name_offsetX != cycleset->widgetName.offsetX ||
            name_offsetY != cycleset->widgetName.offsetY ||
            hotkey_enable != cycleset->widgetHotkey.enable ||
            hotkey_align_type != cycleset->widgetHotkey.align ||
            hotkey_offsetX != cycleset->widgetHotkey.offsetX ||
            hotkey_offsetY != cycleset->widgetHotkey.offsetY ||
            equipset != cycleset->items) {

            isChanged = true;
        } else {
            isChanged = false;
        }

        NameSection(&name);
        ImGui::SameLine();

        auto buttonSize = ImGui::CalcTextSize((TRANSLATE("_REMOVE") + TRANSLATE("_SAVECHANGES")).c_str());
        ImGui::InvisibleButton("##Invisible", ImVec2(-buttonSize.x - 80.0f, buttonSize.y));
        ImGui::SameLine();

        if (ImGui::Button(C_TRANSLATE("_REMOVE"), ImVec2(ImGui::CalcTextSize(C_TRANSLATE("_REMOVE")).x + 30.0f, 0))) {
            ImGui::OpenPopup("##Remove");
        }
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowSize({200, 110}, ImGuiCond_Once);
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        if (ImGui::BeginPopupModal("##Remove", NULL)) {
            auto msg = fmt::format("  {}  ", C_TRANSLATE("_REMOVE_MSG"));
            ImGui::Text(" ");
            ImGui::Text(msg.c_str());
            ImGui::Text(" ");
            ImGui::Separator();

            if (ImGui::BeginTable("Button_Table", 2)) {
                ImGui::TableNextColumn();
                auto buttonSize = ImGui::CalcTextSize((TRANSLATE("_OK") + TRANSLATE("_CANCEL")).c_str());
                if (ImGui::Button(C_TRANSLATE("_OK"), ImVec2(-FLT_MIN, buttonSize.y + 15.0f))) {
                    manager->RemoveAllWidget();
                    manager->Remove(cycleset);
                    manager->CreateAllWidget();
                    ImGui::CloseCurrentPopup();
                }
                ImGui::TableNextColumn();
                if (ImGui::Button(C_TRANSLATE("_CANCEL"), ImVec2(-FLT_MIN, buttonSize.y + 15.0f))) {
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndTable();
            }
            ImGui::EndPopup();
        }

        ImGui::SameLine();

        if (!isChanged) {
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.0f, 0.35f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 0.0f, 0.35f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.0f, 0.35f));
        }

        auto title = TRANSLATE("_WARNING");
        if (ImGui::Button(C_TRANSLATE("_SAVECHANGES"),
                          ImVec2(ImGui::CalcTextSize(C_TRANSLATE("_SAVECHANGES")).x + 30.0f, 0))) {
            auto [result_type, result_string] =
                manager->IsEditValid(cycleset, name, hotkey, modifier1, modifier2, modifier3);

            if (result_type == EquipsetManager::VALID_TYPE::NAME_BLANK) {
                ImGui::OpenPopup((title + "##NAME_BLANK").c_str());

            } else if (result_type == EquipsetManager::VALID_TYPE::NAME_CONFLICT) {
                ImGui::OpenPopup((title + "##NAME_CONFLICT").c_str());

            } else if (result_type == EquipsetManager::VALID_TYPE::HOTKEY_CONFLICT) {
                hotkey_conflictName = result_string;
                ImGui::OpenPopup((title + "##HOTKEY_CONFLICT").c_str());

            } else if (result_type == EquipsetManager::VALID_TYPE::GOOD) {
                cycleset->name = name;
                cycleset->hotkey = hotkey;
                cycleset->modifier1 = modifier1;
                cycleset->modifier2 = modifier2;
                cycleset->modifier3 = modifier3;
                cycleset->cyclePersist = cyclePersist;
                cycleset->cycleExpire = cycleExpire;
                cycleset->cycleReset = cycleReset;
                cycleset->widgetIcon.enable = icon_enable;
                cycleset->widgetIcon.offsetX = icon_offsetX;
                cycleset->widgetIcon.offsetY = icon_offsetY;
                cycleset->widgetName.enable = name_enable;
                cycleset->widgetName.align = static_cast<WidgetText::ALIGN_TYPE>(name_align_type);
                cycleset->widgetName.offsetX = name_offsetX;
                cycleset->widgetName.offsetY = name_offsetY;
                cycleset->widgetHotkey.enable = hotkey_enable;
                cycleset->widgetHotkey.align = static_cast<WidgetText::ALIGN_TYPE>(hotkey_align_type);
                cycleset->widgetHotkey.offsetX = hotkey_offsetX;
                cycleset->widgetHotkey.offsetY = hotkey_offsetY;
                cycleset->items = equipset;
                cycleset->cycleIndex = 0U;
                cycleset->CloseExpireTimer();
                cycleset->CloseResetTimer();

                cycleset->RemoveWidget();
                cycleset->CreateWidget();
            }
        }
        if (!isChanged) ImGui::PopStyleColor(3);

        Draw::PopupConflict(title, name, &hotkey_conflictName);

        ImGui::Separator();

        static auto groupSize = ImVec2(0.0f, 0.0f);
        static auto groupLeftSize = ImVec2(0.0f, 0.0f);
        static auto groupRightSize = ImVec2(0.0f, 0.0f);
        ImGui::BeginChild("LeftRegion", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, groupSize.y), false);
        {
            ImGui::BeginGroup();
            {
                auto hotkeyLabel = fmt::format("{}  {}", ICON_FA_KEYBOARD, TRANSLATE("_ICON_HOTKEY"));
                Draw::BeginGroupPanel(hotkeyLabel.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0.0f),
                                      ImVec2(15.0f, 10.0f));
                {
                    Shared::Cycle::HotkeySection(&hotkey, &modifier1, &modifier2, &modifier3);
                }
                Draw::EndGroupPanel();

                auto optionLabel = fmt::format("{}  {}", ICON_FA_SLIDERS_H, TRANSLATE("_ICON_OPTION"));
                Draw::BeginGroupPanel(optionLabel.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0.0f),
                                      ImVec2(15.0f, 10.0f));
                {
                    Shared::Cycle::OptionSection(&cyclePersist, &cycleExpire, &cycleReset);
                }
                Draw::EndGroupPanel();
            }
            ImGui::EndGroup();
            auto size = ImGui::GetItemRectSize();
            if (size.y > 50.0f) groupLeftSize = size;
        }
        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::BeginChild("RightRegion", ImVec2(0.0f, groupSize.y), false);
        {
            ImGui::BeginGroup();
            {
                auto widgetLabel = fmt::format("{}  {}", ICON_FA_IMAGE, TRANSLATE("_ICON_WIDGET"));
                Draw::BeginGroupPanel(widgetLabel.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0.0f),
                                      ImVec2(15.0f, 10.0f));
                {
                    Shared::Cycle::WidgetSection(&icon_enable, &icon_offsetX, &icon_offsetY, &name_enable,
                                                 &name_align_type, &name_offsetX, &name_offsetY, &hotkey_enable,
                                                 &hotkey_align_type, &hotkey_offsetX, &hotkey_offsetY);
                }
                Draw::EndGroupPanel();
            }
            ImGui::EndGroup();
            auto size = ImGui::GetItemRectSize();
            if (size.y > 50.0f) groupRightSize = size;
        }
        ImGui::EndChild();

        groupSize = groupLeftSize.y > groupRightSize.y ? groupLeftSize : groupRightSize;

        ImGui::Separator();

        static auto groupSize2 = ImVec2(0.0f, 0.0f);
        static auto groupLeftSize2 = ImVec2(0.0f, 0.0f);
        static auto groupRightSize2 = ImVec2(0.0f, 0.0f);
        ImGui::BeginChild("LeftRegion_2", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, groupSize2.y), false);
        {
            //ImGui::BeginGroup();
            //{
            //    auto weaponLabel = fmt::format("{}  {}", ICON_FA_HAMMER, TRANSLATE("_ICON_WEAPON"));
            //    Draw::BeginGroupPanel(weaponLabel.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0.0f),
            //                          ImVec2(15.0f, 10.0f));
            //    {
            //        Shared::Normal::WeaponSection(&lefthand, &righthand, &shout);
            //    }
            //    Draw::EndGroupPanel();
            //}
            //ImGui::EndGroup();
            //groupLeftSize2 = ImGui::GetItemRectSize();
        }
        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::BeginChild("RightRegion_2", ImVec2(0.0f, groupSize2.y), false);
        {
            ImGui::BeginGroup();
            {
                auto equipsetLabel = fmt::format("{}  {}", ICON_FA_DATABASE, TRANSLATE("_ICON_EQUIPSET"));
                Draw::BeginGroupPanel(equipsetLabel.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0.0f),
                                      ImVec2(15.0f, 10.0f));
                {
                    Shared::Cycle::EquipsetSection(&equipset, &page_equipsetIndex, &popup_equipsetIndex);
                }
                Draw::EndGroupPanel();
            }
            ImGui::EndGroup();
            auto size = ImGui::GetItemRectSize();
            if (size.y > 50.0f) groupRightSize2 = size;
        }
        ImGui::EndChild();

        groupSize2 = groupLeftSize2.y > groupRightSize2.y ? groupLeftSize2 : groupRightSize2;
    }
}  // namespace Show::Potion