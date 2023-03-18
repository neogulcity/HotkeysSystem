#include "Potion.h"

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

static void DrawComboPotion(DataPotion* _potion, const std::vector<DataPotion>& _potionVec, const std::string& _label) {
    auto ts = Translator::GetSingleton();
    if (!ts) return;

    auto dataHandler = DataHandler::GetSingleton();
    if (!dataHandler) return;

    if (ImGui::BeginCombo(_label.c_str(), _potion->name.c_str())) {
        for (int i = 0; i < _potionVec.size(); i++) {
            const auto& data = _potionVec[i];
            bool is_selected = (_potion->type == data.type) && (_potion->name == data.name);
            ImGui::PushID(i);

            if (ImGui::Selectable(data.name.c_str(), is_selected)) {
                *_potion = data;
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
            ImGui::PopID();
        }
        ImGui::EndCombo();
    }
}

namespace Shared::Potion {
    void HotkeySection(uint32_t* _hotkey, bool* _modifier1, bool* _modifier2, bool* _modifier3) {
        auto ts = Translator::GetSingleton();
        if (!ts) return;

        auto config = ConfigHandler::GetSingleton();
        if (!config) return;

        Draw::InputButton(_hotkey, "EquipsetHotkey", TRANSLATE("_EDIT"), TRANSLATE("_EDIT_HOTKEYLABEL"));

        auto msg1 = fmt::format("+ {}", ImGui::GetKeyName(static_cast<ImGuiKey>(config->Settings.modifier1)));
        auto msg2 = fmt::format("+ {}", ImGui::GetKeyName(static_cast<ImGuiKey>(config->Settings.modifier2)));
        auto msg3 = fmt::format("+ {}", ImGui::GetKeyName(static_cast<ImGuiKey>(config->Settings.modifier3)));
        ImGui::Checkbox(msg1.c_str(), _modifier1);
        ImGui::Checkbox(msg2.c_str(), _modifier2);
        ImGui::Checkbox(msg3.c_str(), _modifier3);
    }

    void OptionSection(bool* _equipSound, bool* _calcDuration) {
        auto ts = Translator::GetSingleton();
        if (!ts) return;

        ImGui::Checkbox(C_TRANSLATE("_EQUIPSOUNDLABEL"), _equipSound);
        Draw::HelpMarker(TRANSLATE("_EQUIPSOUND_HELP"));
        ImGui::Checkbox(C_TRANSLATE("_CALCDURATIONLABEL"), _calcDuration);
        Draw::HelpMarker(TRANSLATE("_CALCDURATION_HELP"));
    }

    void WidgetSection(bool* _icon_enable, std::string* _icon_type, int* _icon_offsetX, int* _icon_offsetY,
                       bool* _name_enable, uint32_t* _name_align_type, int* _name_offsetX, int* _name_offsetY,
                       bool* _amount_enable, uint32_t* _amount_align_type, int* _amount_offsetX, int* _amount_offsetY) {
        auto ts = Translator::GetSingleton();
        if (!ts) return;

        auto config = ConfigHandler::GetSingleton();
        if (!config) return;

        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::TreeNode(C_TRANSLATE("_WIDGET_ICON"))) {
            ImGui::Checkbox(C_TRANSLATE("_WIDGET_ENABLE"), _icon_enable);
            Draw::ComboIcon(_icon_type, TRANSLATE("_WIDGET_TYPE"));
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
        if (ImGui::TreeNode(C_TRANSLATE("_WIDGET_AMOUNT"))) {
            ImGui::Checkbox(C_TRANSLATE("_WIDGET_ENABLE"), _amount_enable);
            Draw::Combo(align_items, _amount_align_type, TRANSLATE("_WIDGET_ALIGN"));
            Draw::SliderInt(C_TRANSLATE("_WIDGET_OFFSETX"), _amount_offsetX, Config::text_smin,
                             Config::text_smax, "%d", ImGuiSliderFlags_AlwaysClamp);
            Draw::SliderInt(C_TRANSLATE("_WIDGET_OFFSETY"), _amount_offsetY, Config::text_smin,
                             Config::text_smax, "%d", ImGuiSliderFlags_AlwaysClamp);

            ImGui::TreePop();
        }
    }

    void PotionSection(DataPotion* _health, DataPotion* _magicka, DataPotion* _stamina) {
        auto ts = Translator::GetSingleton();
        if (!ts) return;

        auto dataHandler = DataHandler::GetSingleton();
        if (!dataHandler) return;

        DrawComboPotion(_health, dataHandler->potion_health, TRANSLATE("_HEALTH_POTION"));
        DrawComboPotion(_magicka, dataHandler->potion_magicka, TRANSLATE("_MAGICKA_POTION"));
        DrawComboPotion(_stamina, dataHandler->potion_stamina, TRANSLATE("_STAMINA_POTION"));
    }

    void PotionItemSection(std::vector<DataPotion>* _potion, uint32_t* _page_potionIndex, uint32_t* _popup_potionIndex) {
        auto ts = Translator::GetSingleton();
        if (!ts) return;

        auto dataHandler = DataHandler::GetSingleton();
        if (!dataHandler) return;

        if (ImGui::BeginTable("Item_Table", 2)) {
            ImGui::TableNextColumn();
            if (ImGui::Button(C_TRANSLATE("_ADD"), {ImGui::GetContentRegionAvail().x - 7.5f, 0.f})) {
                if (dataHandler->potion.size() != 0) {
                    ImGui::OpenPopup(C_TRANSLATE("_ITEM_ADDPOPUP"));
                }
            }

            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
            if (ImGui::BeginPopupModal(C_TRANSLATE("_ITEM_ADDPOPUP"), NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                std::vector<std::string> items;
                for (int i = 0; i < dataHandler->potion.size(); i++) {
                    items.push_back(dataHandler->potion[i].name);
                }

                if (ImGui::BeginListBox("##listbox1")) {
                    for (int i = 0; i < dataHandler->potion.size(); i++) {
                        const bool is_selected = (*_popup_potionIndex == i);
                        if (ImGui::Selectable(items[i].c_str(), is_selected)) {
                            *_popup_potionIndex = i;
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
                        _potion->push_back(dataHandler->potion[*_popup_potionIndex]);
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
                if (*_page_potionIndex < _potion->size()) {
                    _potion->erase(_potion->begin() + *_page_potionIndex);
                }
            }

            ImGui::EndTable();
        }
        ImGui::Separator();

        for (int i = 0; i < _potion->size(); i++) {
            auto potion = *(_potion->begin() + i);
            const bool is_selected = (*_page_potionIndex == i);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5.0f);
            if (ImGui::Selectable(potion.name.c_str(), is_selected, ImGuiSelectableFlags_None,
                                  ImVec2(ImGui::GetContentRegionAvail().x - 15.0f, 0.0f))) {
                *_page_potionIndex = i;
            }

            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
    }
}  // namespace Shared::Potion

namespace Create::Potion {
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
        static bool equipSound = true;
        static bool calcDuration = false;
        static bool icon_enable = false;
        static std::string icon_type = "";
        static int icon_offsetX = 0;
        static int icon_offsetY = 0;
        static bool name_enable = false;
        static uint32_t name_align_type = WidgetText::ALIGN_TYPE::CENTER;
        static int name_offsetX = 0;
        static int name_offsetY = 0;
        static bool amount_enable = false;
        static uint32_t amount_align_type = WidgetText::ALIGN_TYPE::CENTER;
        static int amount_offsetX = 0;
        static int amount_offsetY = 0;
        static DataPotion health;
        static DataPotion magicka;
        static DataPotion stamina;
        static std::vector<DataPotion> potion;
        static uint32_t page_potionIndex = 0U;
        static uint32_t popup_potionIndex = 0U;
        static std::string hotkey_conflictName;

        if (drawHelper->ShouldReload()) {
            name = manager->GetNamePreset();
            hotkey = 0;
            modifier1 = false;
            modifier2 = false;
            modifier3 = false;
            equipSound = true;
            calcDuration = false;
            icon_enable = false;
            icon_type = "_NONE";
            icon_offsetX = 0;
            icon_offsetY = 0;
            name_enable = false;
            name_align_type = WidgetText::ALIGN_TYPE::CENTER;
            name_offsetX = 0;
            name_offsetY = 0;
            amount_enable = false;
            amount_align_type = WidgetText::ALIGN_TYPE::CENTER;
            amount_offsetX = 0;
            amount_offsetY = 0;
            health = DataPotion();
            magicka = DataPotion();
            stamina = DataPotion();
            potion.clear();
            page_potionIndex = 0;
            popup_potionIndex = 0;

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
                    Shared::Potion::HotkeySection(&hotkey, &modifier1, &modifier2, &modifier3);
                }
                Draw::EndGroupPanel();

                auto optionLabel = fmt::format("{}  {}", ICON_FA_SLIDERS_H, TRANSLATE("_ICON_OPTION"));
                Draw::BeginGroupPanel(optionLabel.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0.0f),
                                      ImVec2(15.0f, 10.0f));
                {
                    Shared::Potion::OptionSection(&equipSound, &calcDuration);
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
                    Shared::Potion::WidgetSection(&icon_enable, &icon_type, &icon_offsetX, &icon_offsetY, &name_enable,
                                                  &name_align_type, &name_offsetX, &name_offsetY, &amount_enable,
                                                  &amount_align_type, &amount_offsetX, &amount_offsetY);
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
            ImGui::BeginGroup();
            {
                auto potionLabel = fmt::format("{}  {}", ICON_FA_FLASK, TRANSLATE("_ICON_POTION"));
                Draw::BeginGroupPanel(potionLabel.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0.0f),
                                      ImVec2(15.0f, 10.0f));
                {
                    Shared::Potion::PotionSection(&health, &magicka, &stamina);
                }
                Draw::EndGroupPanel();
            }
            ImGui::EndGroup();
            auto size = ImGui::GetItemRectSize();
            if (size.y > 50.0f) groupLeftSize2 = size;
        }
        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::BeginChild("RightRegion_2", ImVec2(0.0f, groupSize2.y), false);
        {
            ImGui::BeginGroup();
            {
                auto potionItemLabel = fmt::format("{}  {}", ICON_FA_FLASK, TRANSLATE("_ICON_POTION"));
                Draw::BeginGroupPanel(potionItemLabel.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0.0f),
                                      ImVec2(15.0f, 10.0f));
                {
                    Shared::Potion::PotionItemSection(&potion, &page_potionIndex, &popup_potionIndex);
                }
                Draw::EndGroupPanel();
            }
            ImGui::EndGroup();
            auto size = ImGui::GetItemRectSize();
            if (size.y > 50.0f) groupRightSize2 = size;
        }
        ImGui::EndChild();

        groupSize2 = groupLeftSize2.y > groupRightSize2.y ? groupLeftSize2 : groupRightSize2;

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
                    PotionSet equipset;
                    equipset.type = Equipset::TYPE::POTION;
                    equipset.name = name;
                    equipset.hotkey = hotkey;
                    equipset.modifier1 = modifier1;
                    equipset.modifier2 = modifier2;
                    equipset.modifier3 = modifier3;
                    equipset.equipSound = equipSound;
                    equipset.calcDuration = calcDuration;
                    equipset.widgetIcon.enable = icon_enable;
                    equipset.widgetIcon.type = icon_type;
                    equipset.widgetIcon.offsetX = icon_offsetX;
                    equipset.widgetIcon.offsetY = icon_offsetY;
                    equipset.widgetName.enable = name_enable;
                    equipset.widgetName.align = static_cast<WidgetText::ALIGN_TYPE>(name_align_type);
                    equipset.widgetName.offsetX = name_offsetX;
                    equipset.widgetName.offsetY = name_offsetY;
                    equipset.widgetAmount.enable = amount_enable;
                    equipset.widgetAmount.align = static_cast<WidgetText::ALIGN_TYPE>(amount_align_type);
                    equipset.widgetAmount.offsetX = amount_offsetX;
                    equipset.widgetAmount.offsetY = amount_offsetY;
                    equipset.health = health;
                    equipset.magicka = magicka;
                    equipset.stamina = stamina;
                    equipset.items = potion;

                    auto manager = EquipsetManager::GetSingleton();
                    if (!manager) logger::error("Failed to get EquipsetManager!");

                    manager->Create(equipset, true);

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

namespace Show::Potion {
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

    void Main(PotionSet* _potionSet, const uint32_t& _id) {
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

        auto equipset = _potionSet;

        static std::string name = equipset->name;
        static uint32_t hotkey = equipset->hotkey;
        static bool modifier1 = equipset->modifier1;
        static bool modifier2 = equipset->modifier2;
        static bool modifier3 = equipset->modifier3;
        static bool equipSound = equipset->equipSound;
        static bool calcDuration = equipset->calcDuration;
        static bool icon_enable = equipset->widgetIcon.enable;
        static std::string icon_type = equipset->widgetIcon.type;
        static int icon_offsetX = equipset->widgetIcon.offsetX;
        static int icon_offsetY = equipset->widgetIcon.offsetY;
        static bool name_enable = equipset->widgetName.enable;
        static uint32_t name_align_type = equipset->widgetName.align;
        static int name_offsetX = equipset->widgetName.offsetX;
        static int name_offsetY = equipset->widgetName.offsetY;
        static bool amount_enable = equipset->widgetAmount.enable;
        static uint32_t amount_align_type = equipset->widgetAmount.align;
        static int amount_offsetX = equipset->widgetAmount.offsetX;
        static int amount_offsetY = equipset->widgetAmount.offsetY;
        static DataPotion health = equipset->health;
        static DataPotion magicka = equipset->magicka;
        static DataPotion stamina = equipset->stamina;
        static std::vector<DataPotion> potion = equipset->items;
        static uint32_t page_potionIndex = 0U;
        static uint32_t popup_potionIndex = 0U;
        static std::string hotkey_conflictName;

        if (drawHelper->ShouldReload()) {
            name = equipset->name;
            hotkey = equipset->hotkey;
            modifier1 = equipset->modifier1;
            modifier2 = equipset->modifier2;
            modifier3 = equipset->modifier3;
            equipSound = equipset->equipSound;
            calcDuration = equipset->calcDuration;
            icon_enable = equipset->widgetIcon.enable;
            icon_type = equipset->widgetIcon.type;
            icon_offsetX = equipset->widgetIcon.offsetX;
            icon_offsetY = equipset->widgetIcon.offsetY;
            name_enable = equipset->widgetName.enable;
            name_align_type = equipset->widgetName.align;
            name_offsetX = equipset->widgetName.offsetX;
            name_offsetY = equipset->widgetName.offsetY;
            amount_enable = equipset->widgetAmount.enable;
            amount_align_type = equipset->widgetAmount.align;
            amount_offsetX = equipset->widgetAmount.offsetX;
            amount_offsetY = equipset->widgetAmount.offsetY;
            health = equipset->health;
            magicka = equipset->magicka;
            stamina = equipset->stamina;
            potion = equipset->items;
            page_potionIndex = 0;
            popup_potionIndex = 0;

            drawHelper->NotifyReload(false);
        }

        static bool isChanged = false;
        if (name != equipset->name ||
            hotkey != equipset->hotkey ||
            modifier1 != equipset->modifier1 ||
            modifier2 != equipset->modifier2 ||
            modifier3 != equipset->modifier3 ||
            equipSound != equipset->equipSound ||
            calcDuration != equipset->calcDuration ||
            icon_enable != equipset->widgetIcon.enable ||
            icon_type != equipset->widgetIcon.type ||
            icon_offsetX != equipset->widgetIcon.offsetX ||
            icon_offsetY != equipset->widgetIcon.offsetY ||
            name_enable != equipset->widgetName.enable ||
            name_align_type != equipset->widgetName.align ||
            name_offsetX != equipset->widgetName.offsetX ||
            name_offsetY != equipset->widgetName.offsetY ||
            amount_enable != equipset->widgetAmount.enable ||
            amount_align_type != equipset->widgetAmount.align ||
            amount_offsetX != equipset->widgetAmount.offsetX ||
            amount_offsetY != equipset->widgetAmount.offsetY ||
            health != equipset->health ||
            magicka != equipset->magicka ||
            stamina != equipset->stamina ||
            potion != equipset->items) {

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
                    manager->Remove(equipset);
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
                manager->IsEditValid(equipset, name, hotkey, modifier1, modifier2, modifier3);

            if (result_type == EquipsetManager::VALID_TYPE::NAME_BLANK) {
                ImGui::OpenPopup((title + "##NAME_BLANK").c_str());

            } else if (result_type == EquipsetManager::VALID_TYPE::NAME_CONFLICT) {
                ImGui::OpenPopup((title + "##NAME_CONFLICT").c_str());

            } else if (result_type == EquipsetManager::VALID_TYPE::HOTKEY_CONFLICT) {
                hotkey_conflictName = result_string;
                ImGui::OpenPopup((title + "##HOTKEY_CONFLICT").c_str());

            } else if (result_type == EquipsetManager::VALID_TYPE::GOOD) {
                std::string prevName = equipset->name;

                equipset->name = name;
                equipset->hotkey = hotkey;
                equipset->modifier1 = modifier1;
                equipset->modifier2 = modifier2;
                equipset->modifier3 = modifier3;
                equipset->equipSound = equipSound;
                equipset->calcDuration = calcDuration;
                equipset->widgetIcon.enable = icon_enable;
                equipset->widgetIcon.type = icon_type;
                equipset->widgetIcon.offsetX = icon_offsetX;
                equipset->widgetIcon.offsetY = icon_offsetY;
                equipset->widgetName.enable = name_enable;
                equipset->widgetName.align = static_cast<WidgetText::ALIGN_TYPE>(name_align_type);
                equipset->widgetName.offsetX = name_offsetX;
                equipset->widgetName.offsetY = name_offsetY;
                equipset->widgetAmount.enable = amount_enable;
                equipset->widgetAmount.align = static_cast<WidgetText::ALIGN_TYPE>(amount_align_type);
                equipset->widgetAmount.offsetX = amount_offsetX;
                equipset->widgetAmount.offsetY = amount_offsetY;
                equipset->health = health;
                equipset->magicka = magicka;
                equipset->stamina = stamina;
                equipset->items = potion;
                
                equipset->AssignAutoPotion();
                health = equipset->health;
                magicka = equipset->magicka;
                stamina = equipset->stamina;

                equipset->SyncEquipset(prevName, equipset->name);

                equipset->RemoveWidget();
                equipset->CreateWidget();
                equipset->SyncWidget();
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
                    Shared::Potion::HotkeySection(&hotkey, &modifier1, &modifier2, &modifier3);
                }
                Draw::EndGroupPanel();

                auto optionLabel = fmt::format("{}  {}", ICON_FA_SLIDERS_H, TRANSLATE("_ICON_OPTION"));
                Draw::BeginGroupPanel(optionLabel.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0.0f),
                                      ImVec2(15.0f, 10.0f));
                {
                    Shared::Potion::OptionSection(&equipSound, &calcDuration);
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
                    Shared::Potion::WidgetSection(&icon_enable, &icon_type, &icon_offsetX, &icon_offsetY, &name_enable,
                                                  &name_align_type, &name_offsetX, &name_offsetY, &amount_enable,
                                                  &amount_align_type, &amount_offsetX, &amount_offsetY);
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
            ImGui::BeginGroup();
            {
                auto potionLabel = fmt::format("{}  {}", ICON_FA_FLASK, TRANSLATE("_ICON_POTION"));
                Draw::BeginGroupPanel(potionLabel.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0.0f),
                                      ImVec2(15.0f, 10.0f));
                {
                    Shared::Potion::PotionSection(&health, &magicka, &stamina);
                }
                Draw::EndGroupPanel();
            }
            ImGui::EndGroup();
            auto size = ImGui::GetItemRectSize();
            if (size.y > 50.0f) groupLeftSize2 = size;
        }
        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::BeginChild("RightRegion_2", ImVec2(0.0f, groupSize2.y), false);
        {
            ImGui::BeginGroup();
            {
                auto potionItemLabel = fmt::format("{}  {}", ICON_FA_FLASK, TRANSLATE("_ICON_POTION"));
                Draw::BeginGroupPanel(potionItemLabel.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0.0f),
                                      ImVec2(15.0f, 10.0f));
                {
                    Shared::Potion::PotionItemSection(&potion, &page_potionIndex, &popup_potionIndex);
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