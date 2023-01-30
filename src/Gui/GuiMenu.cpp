#include "GuiMenu.h"
#include "Data.h"
#include "Draw.h"
#include "Config.h"
#include "EquipsetManager.h"
#include "Equipment.h"
#include "Translate.h"
#include "WidgetHandler.h"

#include <filesystem>

#include <imgui.h>
#include <imgui_internal.h>
#include "extern/imgui_impl_dx11.h"
#include "extern/imgui_stdlib.h"
#include "extern/IconsFontAwesome5.h"

GuiMenu::GuiMenu() {
    ConfigHandler::GetSingleton()->LoadConfig();
    EquipmentManager::GetSingleton()->Load();
    logger::info("GuiMenu initialized!");
}

void GuiMenu::DisableInput(bool _status) {
    auto control = RE::ControlMap::GetSingleton();
    if (!control) return;

    control->ToggleControls(RE::ControlMap::UEFlag::kAll, !_status);
}

void GuiMenu::Toggle(std::optional<bool> enabled = std::nullopt) {
    auto dataHandler = DataHandler::GetSingleton();
    if (!dataHandler) return;

    auto drawHelper = DrawHelper::GetSingleton();
    if (!drawHelper) return;

    auto widgetHandler = WidgetHandler::GetSingleton();
    if (!widgetHandler) return;

    auto& io = ImGui::GetIO();
    io.ClearInputCharacters();
    io.ClearInputKeys();
    show = enabled.value_or(!show);
    DisableInput(show);
    if (show) {
        dataHandler->Init();
        widgetHandler->CloseExpireTimer();
        widgetHandler->SetMenuAlpha(100);
    } else {
        drawHelper->NotifyReload(true);
        dataHandler->Clear();
        widgetHandler->ProcessFadeOut();
    }
}

void GuiMenu::DrawMain() {
    if (!imgui_inited) return;

    auto ui = RE::UI::GetSingleton();
    if (!ui) return;

    auto config = ConfigHandler::GetSingleton();
    if (!config) return;

    auto ts = Translator::GetSingleton();
    if (!ts) return;

    auto manager = EquipsetManager::GetSingleton();
    if (!manager) return;

    auto dataHandler = DataHandler::GetSingleton();
    if (!dataHandler) return;

    auto drawHelper = DrawHelper::GetSingleton();
    if (!drawHelper) return;

    auto equipment = EquipmentManager::GetSingleton();
    if (!equipment) return;

    auto& io = ImGui::GetIO();
    if (ImGui::IsKeyPressed(config->Gui.hotkey) && !ui->IsMenuOpen(RE::MainMenu::MENU_NAME) &&
        !ui->IsMenuOpen(RE::LoadingMenu::MENU_NAME)) {
        Toggle();
    }
    //if (ImGui::IsKeyPressed(config->Gui.hotkey)) {
    //    Toggle();
    //}
    io.MouseDrawCursor = show;
    io.BackendFlags |= ImGuiBackendFlags_HasGamepad;

    if (!show) return;

    if (font) ImGui::PushFont(font);
    auto& style = ImGui::GetStyle();
    switch (static_cast<Config::GuiStyle>(config->Gui.style)) {
        case Config::GuiStyle::DARK: ImGui::StyleColorsDark(); break;
        case Config::GuiStyle::LIGHT: ImGui::StyleColorsLight(); break;
        case Config::GuiStyle::CLASSIC: ImGui::StyleColorsClassic(); break;
    }
    style.WindowRounding = config->Gui.rounding;
    style.ChildRounding = config->Gui.rounding;
    style.FrameRounding = config->Gui.rounding;
    style.PopupRounding = config->Gui.rounding;
    style.ScrollbarRounding = config->Gui.rounding;
    style.GrabRounding = config->Gui.rounding;
    style.WindowBorderSize = config->Gui.windowBorder ? 1.0f : 0.0f;
    style.FrameBorderSize = config->Gui.frameBorder ? 1.0f : 0.0f;
    style.ScaleAllSizes(config->Gui.fontScaling);

    // Reload data whenever user changes 'Favorited only' option.
    static bool ShouldReloadData = false;
    if (ShouldReloadData != config->Settings.favorOnly) {
        ShouldReloadData = config->Settings.favorOnly;
        dataHandler->Init();
    }

    auto viewport = ImGui::GetMainViewport();
    if (!viewport) return;

    ImGui::SetNextWindowPos({0, 0}, ImGuiCond_Once);
    ImGui::SetNextWindowSize({viewport->Size.x / 3, viewport->Size.y}, ImGuiCond_Once);
    if (ImGui::Begin(fmt::format("UI-Integrated Hotkeys System {}", SKSE::PluginDeclaration::GetSingleton()->GetVersion().string()).c_str(),
                     nullptr,
                     ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar)) {

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu(C_TRANSLATE("_MENUBAR_FILE"))) {
                ImGui::MenuItem(C_TRANSLATE("_TAB_EQUIPSETS"), NULL, false, false);
                ImGui::Separator();
                if (ImGui::MenuItem(C_TRANSLATE("_MENUBAR_SAVE"))) {
                    manager->ExportEquipsets();
                }
                if (ImGui::MenuItem(C_TRANSLATE("_MENUBAR_LOAD"))) {
                    manager->RemoveAllWidget();
                    manager->RemoveAll();
                    manager->ImportEquipsets();
                    manager->SyncSortOrder();
                    manager->CreateAllWidget();
                }
                ImGui::MenuItem("##BLANK", NULL, false, false);
                ImGui::MenuItem(C_TRANSLATE("_TAB_EQUIPMENT"), NULL, false, false);
                ImGui::Separator();
                if (ImGui::MenuItem(C_TRANSLATE("_MENUBAR_SAVE"))) {
                    equipment->Save();
                }
                if (ImGui::MenuItem(C_TRANSLATE("_MENUBAR_LOAD"))) {
                    equipment->RemoveAllArmorWidget();
                    equipment->RemoveAllWeaponWidget();
                    equipment->RemoveAllShoutWidget();
                    equipment->Load();
                    equipment->CreateAllArmorWidget();
                    equipment->CreateAllWeaponWidget();
                    equipment->CreateAllShoutWidget();
                }
                ImGui::MenuItem("##BLANK", NULL, false, false);
                ImGui::MenuItem(C_TRANSLATE("_TAB_CONFIG"), NULL, false, false);
                ImGui::Separator();
                if (ImGui::MenuItem(C_TRANSLATE("_MENUBAR_SAVE"))) {
                    config->SaveConfig();
                }
                if (ImGui::MenuItem(C_TRANSLATE("_MENUBAR_LOAD"))) {
                    config->LoadConfig();
                    ts->Load();
                    dataHandler->Init();
                    GuiMenu::NotifyFontReload();
                }
                
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::BeginChild("main", {0.f, -ImGui::GetFontSize() - 2.f});

        if (ImGui::BeginTabBar("##")) {
            if (ImGui::BeginTabItem(C_TRANSLATE("_TAB_EQUIPSETS"))) {
                auto newSize = ImGui::CalcTextSize(C_TRANSLATE("_NEW"));
                if (ImGui::Button(C_TRANSLATE("_NEW"), ImVec2(newSize.x + 30.0f, 0.0f))) {
                    ImGui::SetNextWindowSize({200, 230}, ImGuiCond_Once);
                    ImGui::OpenPopup(C_TRANSLATE("_SELECT_NEW_POPUP"));
                }
                ImGui::Separator();

                ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

                if (ImGui::BeginPopupModal(C_TRANSLATE("_SELECT_NEW_POPUP"), NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                    bool shouldClose = false;

                    auto normalSize = ImGui::CalcTextSize(C_TRANSLATE("_SELECT_NEW_NORMAL"));
                    auto potionSize = ImGui::CalcTextSize(C_TRANSLATE("_SELECT_NEW_POTION"));
                    auto cycleSize = ImGui::CalcTextSize(C_TRANSLATE("_SELECT_NEW_CYCLE"));
                    auto cancelSize = ImGui::CalcTextSize(C_TRANSLATE("_CANCEL"));

                    std::vector<float> vecX = {
                        normalSize.x,
                        potionSize.x,
                        cycleSize.x,
                        cancelSize.x
                    };
                    std::vector<float> vecY = {
                        normalSize.y,
                        potionSize.y,
                        cycleSize.y,
                        cancelSize.y
                    };

                    auto compare = [](float a, float b) {
                        return a > b;
                    };

                    std::sort(vecX.begin(), vecX.end(), compare);
                    std::sort(vecY.begin(), vecY.end(), compare);
                    float sizeX = *vecX.begin() * 2.5f;
                    float sizeY = *vecY.begin() * 2.0f;

                    if (ImGui::Button(C_TRANSLATE("_SELECT_NEW_NORMAL"), ImVec2(sizeX, sizeY))) {
                        drawHelper->NotifyReload(true);
                        ImGui::OpenPopup(C_TRANSLATE("_SELECT_NEW_OPEN_NORMAL"));
                    }
                    ImGui::SetNextWindowSize({viewport->Size.x / 3, viewport->Size.y / 5 * 3}, ImGuiCond_Once);
                    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
                    if (ImGui::BeginPopupModal(C_TRANSLATE("_SELECT_NEW_OPEN_NORMAL"), NULL)) {
                        shouldClose = Draw::CreateNormal();
                        ImGui::EndPopup();
                    }

                    if (ImGui::Button(C_TRANSLATE("_SELECT_NEW_POTION"), ImVec2(sizeX, sizeY))) {
                        drawHelper->NotifyReload(true);
                        ImGui::OpenPopup(C_TRANSLATE("_SELECT_NEW_OPEN_POTION"));
                    }
                    ImGui::SetNextWindowSize({viewport->Size.x / 3, viewport->Size.y / 5 * 3}, ImGuiCond_Once);
                    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
                    if (ImGui::BeginPopupModal(C_TRANSLATE("_SELECT_NEW_OPEN_POTION"), NULL)) {
                        shouldClose = Draw::CreatePotion();
                        ImGui::EndPopup();
                    }

                    if (ImGui::Button(C_TRANSLATE("_SELECT_NEW_CYCLE"), ImVec2(sizeX, sizeY))) {
                        drawHelper->NotifyReload(true);
                        ImGui::OpenPopup(C_TRANSLATE("_SELECT_NEW_OPEN_CYCLE"));
                    }
                    ImGui::SetNextWindowSize({viewport->Size.x / 3, viewport->Size.y / 5 * 3}, ImGuiCond_Once);
                    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
                    if (ImGui::BeginPopupModal(C_TRANSLATE("_SELECT_NEW_OPEN_CYCLE"), NULL)) {
                        shouldClose = Draw::CreateCycle();
                        ImGui::EndPopup();
                    }

                    ImGui::Dummy(ImVec2(sizeX, sizeY));
                    ImGui::Separator();
                    if (ImGui::Button(C_TRANSLATE("_CANCEL"), ImVec2(sizeX, sizeY))) {
                        ImGui::CloseCurrentPopup();
                    }
                    if (shouldClose) {
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }

                auto sort = static_cast<Config::SortType>(config->Settings.sort);
                std::vector<Equipset*> equipsetVec;
                if (sort == Config::SortType::CREATEASC) {
                    equipsetVec = manager->equipsetVec;
                } else if (sort == Config::SortType::CREATEDESC) {
                    auto compare = [](Equipset* _first, Equipset* _second) {
                        if (!_first || !_second) return false;

                        return _first->order > _second->order;
                    };
                    equipsetVec = manager->equipsetVec;
                    std::sort(equipsetVec.begin(), equipsetVec.end(), compare);
                } else if (sort == Config::SortType::NAMEASC) {
                    auto compare = [](Equipset* _first, Equipset* _second) {
                        if (!_first || !_second) return false;

                        return _first->name < _second->name;
                    };
                    equipsetVec = manager->equipsetVec;
                    std::sort(equipsetVec.begin(), equipsetVec.end(), compare);
                } else if (sort == Config::SortType::NAMEDESC) {
                    auto compare = [](Equipset* _first, Equipset* _second) {
                        if (!_first || !_second) return false;

                        return _first->name > _second->name;
                    };
                    equipsetVec = manager->equipsetVec;
                    std::sort(equipsetVec.begin(), equipsetVec.end(), compare);
                }
                for (int i = 0; i < equipsetVec.size(); i++) {
                    auto equipset = equipsetVec[i];

                    if (current_opened.load() != i) {
                        ImGui::SetNextItemOpen(false);
                    }

                    if (ImGui::CollapsingHeader(equipset->name.c_str())) {
                        ImGui::Indent();
                        ImGui::PushID(i);

                        if (equipset->type == Equipset::TYPE::NORMAL) {
                            Draw::ShowNormal(static_cast<NormalSet*>(equipset), i);
                        } else if (equipset->type == Equipset::TYPE::POTION) {
                            Draw::ShowPotion(static_cast<PotionSet*>(equipset), i);
                        } else if (equipset->type == Equipset::TYPE::CYCLE) {
                            Draw::ShowCycle(static_cast<CycleSet*>(equipset), i);
                        }

                        ImGui::PopID();
                        ImGui::Unindent();
                    }
                }

                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem(C_TRANSLATE("_TAB_EQUIPMENT"))) {
                DrawEquipment();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem(C_TRANSLATE("_TAB_CONFIG"))) {
                DrawConfig();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        
        ImGui::EndChild();
    } else {
        Toggle(false);
    }
    ImGui::End();

    if (font) ImGui::PopFont();
}

void GuiMenu::LoadFont() {
    if (!reload_font.load()) return;
    reload_font.store(false);

    auto& io = ImGui::GetIO();
    auto config = ConfigHandler::GetSingleton();
    std::filesystem::path path = "Data/SKSE/Plugins/UIHS/Fonts/" + config->Gui.fontPath;
    if (std::filesystem::is_regular_file(path) && ((path.extension() == ".ttf") || (path.extension() == ".otf"))) {
        ImVector<ImWchar> ranges;
        ImFontGlyphRangesBuilder builder;
        builder.AddRanges(io.Fonts->GetGlyphRangesDefault());
        if (config->Gui.language == (int)Config::LangType::CHINESE) {
            builder.AddRanges(io.Fonts->GetGlyphRangesChineseFull());
        } else if (config->Gui.language == (int)Config::LangType::JAPANESE) {
            builder.AddRanges(io.Fonts->GetGlyphRangesJapanese());
        } else if (config->Gui.language == (int)Config::LangType::KOREAN) {
            builder.AddRanges(io.Fonts->GetGlyphRangesKorean());
        } else if (config->Gui.language == (int)Config::LangType::RUSSIAN) {
            builder.AddRanges(io.Fonts->GetGlyphRangesCyrillic());
        } else if (config->Gui.language == (int)Config::LangType::THAI) {
            builder.AddRanges(io.Fonts->GetGlyphRangesThai());
        } else if (config->Gui.language == (int)Config::LangType::VIETNAMESE) {
            builder.AddRanges(io.Fonts->GetGlyphRangesVietnamese());
        }
        builder.BuildRanges(&ranges);

        io.Fonts->Clear();
        io.Fonts->AddFontDefault();
        font = io.Fonts->AddFontFromFileTTF(path.string().c_str(), config->Gui.fontSize, NULL, ranges.Data);

        std::filesystem::path iconPath = "Data/SKSE/Plugins/UIHS/Fonts/fa-solid-900.ttf";
        if (std::filesystem::is_regular_file(iconPath)) {
            static const ImWchar icons_ranges[] = {ICON_MIN_FA, ICON_MAX_16_FA, 0};
            ImFontConfig icons_config;
            icons_config.MergeMode = true;
            icons_config.PixelSnapH = true;
            io.Fonts->AddFontFromFileTTF(iconPath.string().c_str(), config->Gui.fontSize, &icons_config, icons_ranges);
        }

        if (io.Fonts->Build()) {
            ImGui_ImplDX11_ReCreateFontsTexture();
            logger::info("Font loaded.");
            return;
        } else
            logger::error("Failed to build font {}", config->Gui.fontPath);
    } else {
        font = nullptr;
        logger::info("{} is not a font file", config->Gui.fontPath);
    }

    io.Fonts->Clear();
    io.Fonts->AddFontDefault();
    assert(io.Fonts->Build());

    ImGui_ImplDX11_ReCreateFontsTexture();

    logger::info("Font loaded.");
}

void GuiMenu::DrawEquipment() {
    auto config = ConfigHandler::GetSingleton();
    if (!config) return;

    auto ts = Translator::GetSingleton();
    if (!ts) return;

    auto equipment = EquipmentManager::GetSingleton();
    if (!equipment) return;

    bool shouldReload = false;
    auto Reload = [&shouldReload]() { shouldReload = true; };

    auto DrawWidgetIconSection = [ts, Reload](WidgetIcon* _widget) {
        if (ImGui::Checkbox(C_TRANSLATE("_WIDGET_ENABLE"), &_widget->enable)) { Reload(); }
        if (Draw::SliderInt(C_TRANSLATE("_WIDGET_OFFSETX"), &_widget->offsetX, Config::icon_smin,
                        Config::icon_smax, "%d", ImGuiSliderFlags_AlwaysClamp)) { Reload(); }
        if (Draw::SliderInt(C_TRANSLATE("_WIDGET_OFFSETY"), &_widget->offsetY, Config::icon_smin,
                        Config::icon_smax, "%d", ImGuiSliderFlags_AlwaysClamp)) { Reload(); }
    }; 
    auto DrawWidgetTextSection = [ts, Reload](WidgetText* _widget) {
        std::vector<std::string> align_items = {TRANSLATE("_ALIGN_LEFT"),
                                                TRANSLATE("_ALIGN_RIGHT"),
                                                TRANSLATE("_ALIGN_CENTER")};

        if (ImGui::Checkbox(C_TRANSLATE("_WIDGET_ENABLE"), &_widget->enable)) { Reload(); }
        uint32_t* align = reinterpret_cast<uint32_t*>(&_widget->align);
        if (Draw::Combo(align_items, align, C_TRANSLATE("_WIDGET_ALIGN"))) { Reload(); }
        if (Draw::SliderInt(C_TRANSLATE("_WIDGET_OFFSETX"), &_widget->offsetX, Config::text_smin,
                        Config::text_smax, "%d", ImGuiSliderFlags_AlwaysClamp)) { Reload(); }
        if (Draw::SliderInt(C_TRANSLATE("_WIDGET_OFFSETY"), &_widget->offsetY, Config::text_smin,
                        Config::text_smax, "%d", ImGuiSliderFlags_AlwaysClamp)) { Reload(); }
    };

    if (ImGui::CollapsingHeader(C_TRANSLATE("_TAB_EQUIPMENT_ARMOR"))) {
        ImGui::Indent();

        static std::vector<ImVec2> groupSize(32, ImVec2(0.0f, 0.0f));
        static std::vector<ImVec2> groupLeftSize(32, ImVec2(0.0f, 0.0f));
        static std::vector<ImVec2> groupRightSize(32, ImVec2(0.0f, 0.0f));
        for (int i = 0; i < 32; i++) {
            auto treeName = fmt::format("Slot{}", i + 30);
            if (ImGui::TreeNode(treeName.c_str())) {
                ImGui::PushID(i);

                auto groupLabel = fmt::format("{}  {}", ICON_FA_IMAGE, TRANSLATE("_TAB_CONFIG_WIDGET"));
                Draw::BeginGroupPanel(groupLabel.c_str(), ImVec2(-FLT_MIN, 0.0f), ImVec2(15.0f, 10.0f));
                {
                    ImGui::BeginChild("LeftRegion", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, groupSize[i].y), false);
                    {
                        ImGui::BeginGroup();
                        {
                            ImGui::SetNextItemOpen(true, ImGuiCond_Once);
                            if (ImGui::TreeNode(C_TRANSLATE("_WIDGET_ICON"))) {
                                DrawWidgetIconSection(&equipment->armor[i].widgetIcon);
                                ImGui::TreePop();
                            }
                        }
                        ImGui::EndGroup();
                        auto size = ImGui::GetItemRectSize();
                        if (size.y != 0.0f) groupLeftSize[i] = size;
                    }
                    ImGui::EndChild();
                    ImGui::SameLine();
                    ImGui::BeginChild("RightRegion", ImVec2(0.0f, groupSize[i].y), false);
                    {
                        ImGui::BeginGroup();
                        {
                            ImGui::SetNextItemOpen(true, ImGuiCond_Once);
                            if (ImGui::TreeNode(C_TRANSLATE("_WIDGET_NAME"))) {
                                DrawWidgetTextSection(&equipment->armor[i].widgetName);
                                ImGui::TreePop();
                            }
                        }
                        ImGui::EndGroup();
                        auto size = ImGui::GetItemRectSize();
                        if (size.y != 0.0f) groupRightSize[i] = size;
                    }
                    ImGui::EndChild();

                    groupSize[i] = groupLeftSize[i].y > groupRightSize[i].y ? groupLeftSize[i] : groupRightSize[i];
                }
                Draw::EndGroupPanel();

                ImGui::PopID();
                ImGui::TreePop();
            }
        }

        ImGui::Unindent();
    }

    if (ImGui::CollapsingHeader(C_TRANSLATE("_TAB_EQUIPMENT_WEAPON"))) {
        ImGui::Indent();

        if (ImGui::TreeNode(C_TRANSLATE("_WEAPON_LEFTHAND"))) {
            static auto groupSize = ImVec2(0.0f, 0.0f);
            static auto groupLeftSize = ImVec2(0.0f, 0.0f);
            static auto groupRightSize = ImVec2(0.0f, 0.0f);
            auto groupLabel = fmt::format("{}  {}", ICON_FA_IMAGE, TRANSLATE("_TAB_CONFIG_WIDGET"));
            Draw::BeginGroupPanel(groupLabel.c_str(), ImVec2(-FLT_MIN, 0.0f), ImVec2(15.0f, 10.0f));
            {
                ImGui::BeginChild("LeftRegion", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, groupSize.y), false);
                {
                    ImGui::BeginGroup();
                    {
                        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
                        if (ImGui::TreeNode(C_TRANSLATE("_WIDGET_ICON"))) {
                            DrawWidgetIconSection(&equipment->lefthand.widgetIcon);
                            ImGui::TreePop();
                        }
                    }
                    ImGui::EndGroup();
                    auto size = ImGui::GetItemRectSize();
                    if (size.y != 0.0f) groupLeftSize = size;
                }
                ImGui::EndChild();
                ImGui::SameLine();
                ImGui::BeginChild("RightRegion", ImVec2(0.0f, groupSize.y), false);
                {
                    ImGui::BeginGroup();
                    {
                        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
                        if (ImGui::TreeNode(C_TRANSLATE("_WIDGET_NAME"))) {
                            DrawWidgetTextSection(&equipment->lefthand.widgetName);
                            ImGui::TreePop();
                        }
                    }
                    ImGui::EndGroup();
                    auto size = ImGui::GetItemRectSize();
                    if (size.y != 0.0f) groupRightSize = size;
                }
                ImGui::EndChild();

                groupSize = groupLeftSize.y > groupRightSize.y ? groupLeftSize : groupRightSize;
            }
            Draw::EndGroupPanel();
            ImGui::TreePop();
        }
        if (ImGui::TreeNode(C_TRANSLATE("_WEAPON_RIGHTHAND"))) {
            static auto groupSize = ImVec2(0.0f, 0.0f);
            static auto groupLeftSize = ImVec2(0.0f, 0.0f);
            static auto groupRightSize = ImVec2(0.0f, 0.0f);
            auto groupLabel = fmt::format("{}  {}", ICON_FA_IMAGE, TRANSLATE("_TAB_CONFIG_WIDGET"));
            Draw::BeginGroupPanel(groupLabel.c_str(), ImVec2(-FLT_MIN, 0.0f), ImVec2(15.0f, 10.0f));
            {
                ImGui::BeginChild("LeftRegion", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, groupSize.y), false);
                {
                    ImGui::BeginGroup();
                    {
                        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
                        if (ImGui::TreeNode(C_TRANSLATE("_WIDGET_ICON"))) {
                            DrawWidgetIconSection(&equipment->righthand.widgetIcon);
                            ImGui::TreePop();
                        }
                    }
                    ImGui::EndGroup();
                    auto size = ImGui::GetItemRectSize();
                    if (size.y != 0.0f) groupLeftSize = size;
                }
                ImGui::EndChild();
                ImGui::SameLine();
                ImGui::BeginChild("RightRegion", ImVec2(0.0f, groupSize.y), false);
                {
                    ImGui::BeginGroup();
                    {
                        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
                        if (ImGui::TreeNode(C_TRANSLATE("_WIDGET_NAME"))) {
                            DrawWidgetTextSection(&equipment->righthand.widgetName);
                            ImGui::TreePop();
                        }
                    }
                    ImGui::EndGroup();
                    auto size = ImGui::GetItemRectSize();
                    if (size.y != 0.0f) groupRightSize = size;
                }
                ImGui::EndChild();

                groupSize = groupLeftSize.y > groupRightSize.y ? groupLeftSize : groupRightSize;
            }
            Draw::EndGroupPanel();
            ImGui::TreePop();
        }

        ImGui::Unindent();
    }

    if (ImGui::CollapsingHeader(C_TRANSLATE("_TAB_EQUIPMENT_SHOUT"))) {
        ImGui::Indent();

        static auto groupSize = ImVec2(0.0f, 0.0f);
        static auto groupLeftSize = ImVec2(0.0f, 0.0f);
        static auto groupRightSize = ImVec2(0.0f, 0.0f);
        auto groupLabel = fmt::format("{}  {}", ICON_FA_IMAGE, TRANSLATE("_TAB_CONFIG_WIDGET"));
        Draw::BeginGroupPanel(groupLabel.c_str(), ImVec2(-FLT_MIN, 0.0f), ImVec2(15.0f, 10.0f));
        {       
            ImGui::BeginChild("LeftRegion", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, groupSize.y), false);
            {
                ImGui::BeginGroup();
                {
                    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
                    if (ImGui::TreeNode(C_TRANSLATE("_WIDGET_ICON"))) {
                        DrawWidgetIconSection(&equipment->shout.widgetIcon);
                        ImGui::TreePop();
                    }
                }
                ImGui::EndGroup();
                auto size = ImGui::GetItemRectSize();
                if (size.y != 0.0f) groupLeftSize = size;
            }
            ImGui::EndChild();
            ImGui::SameLine();
            ImGui::BeginChild("RightRegion", ImVec2(0.0f, groupSize.y), false);
            {
                ImGui::BeginGroup();
                {
                    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
                    if (ImGui::TreeNode(C_TRANSLATE("_WIDGET_NAME"))) {
                        DrawWidgetTextSection(&equipment->shout.widgetName);
                        ImGui::TreePop();
                    }
                }
                ImGui::EndGroup();
                auto size = ImGui::GetItemRectSize();
                if (size.y != 0.0f) groupRightSize = size;
            }
            ImGui::EndChild();

            groupSize = groupLeftSize.y > groupRightSize.y ? groupLeftSize : groupRightSize;
        }
        Draw::EndGroupPanel();
        ImGui::Unindent();
    }

    if (shouldReload) {
        auto equipment = EquipmentManager::GetSingleton();
        if (!equipment) return;

        auto equipset = EquipsetManager::GetSingleton();
        if (!equipset) return;

        auto widgetHandler = WidgetHandler::GetSingleton();
        if (!widgetHandler) return;

        widgetHandler->CloseWidgetMenu();
        widgetHandler->OpenWidgetMenu();
        equipset->RemoveAllWidget();
        equipment->RemoveAllArmorWidget();
        equipment->RemoveAllWeaponWidget();
        equipment->RemoveAllShoutWidget();
        equipment->CreateAllArmorWidget();
        equipment->CreateAllWeaponWidget();
        equipment->CreateAllShoutWidget();
        equipset->CreateAllWidget();
    }
}

void GuiMenu::DrawConfig() {
    auto config = ConfigHandler::GetSingleton();
    if (!config) return;

    auto ts = Translator::GetSingleton();
    if (!ts) return;

    bool shouldReload = false;
    auto Reload = [&shouldReload]() { shouldReload = true; };

    auto DrawWidgetSection = [ts, Reload](ConfigHandler::WidgetBase* _widget) {
        if (Draw::ComboIcon(&_widget->bgType, C_TRANSLATE("_BACKGROUND_TYPE"))) { Reload(); }
        if (Draw::SliderInt(C_TRANSLATE("_BACKGROUND_SIZE"), &_widget->bgSize, 0, 200, "%d%%",
                        ImGuiSliderFlags_AlwaysClamp)) { Reload(); }
        if (Draw::SliderInt(C_TRANSLATE("_BACKGROUND_ALPHA"), &_widget->bgAlpha, 0, 100, "%d%%",
                        ImGuiSliderFlags_AlwaysClamp)) { Reload(); }
        if (Draw::SliderInt(C_TRANSLATE("_WIDGET_SIZE"), &_widget->widgetSize, 0, 200, "%d%%",
                        ImGuiSliderFlags_AlwaysClamp)) { Reload(); }
        if (Draw::SliderInt(C_TRANSLATE("_FONT_SIZE"), &_widget->fontSize, 0, 200, "%d%%",
                        ImGuiSliderFlags_AlwaysClamp)) { Reload(); }
        if (ImGui::Checkbox(C_TRANSLATE("_FONT_SHADOW"), &_widget->fontShadow)) { Reload(); }
    };

    static auto groupWidgetSize = ImVec2(0.0f, 0.0f);
    static auto groupWidgetLeftSize = ImVec2(0.0f, 0.0f);
    static auto groupWidgetRightSize = ImVec2(0.0f, 0.0f);

    ImGui::Text(" ");

    auto groupWidgetLabel = fmt::format("{}  {}", ICON_FA_IMAGE, TRANSLATE("_TAB_CONFIG_WIDGET"));
    Draw::BeginGroupPanel(groupWidgetLabel.c_str(), ImVec2(-FLT_MIN, 0.0f), ImVec2(15.0f, 10.0f));
    {
        ImGui::BeginChild("WidgetRegionLeft", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, groupWidgetSize.y), false);
        {
            ImGui::BeginGroup();
            {
                ImGui::SetNextItemOpen(true, ImGuiCond_Once);
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
                if (ImGui::TreeNode(C_TRANSLATE("_TAB_CONFIG_WIDGET_GENERAL"))) {
                    if (Draw::Combo(config->fontVec, &config->Widget.General.font, C_TRANSLATE("_TAB_CONFIG_WIDGET_GENERAL_FONT"))) { Reload(); }

                    std::vector<std::string> displayVec = {TRANSLATE("_DISPLAYMODE_ALWAYS"),
                                                           TRANSLATE("_DISPLAYMODE_INCOMBAT")};
                    Draw::Combo(displayVec, &config->Widget.General.displayMode,
                                C_TRANSLATE("_TAB_CONFIG_WIDGET_GENERAL_DISPLAY"));

                    std::vector<std::string> animVec = {TRANSLATE("_ANIMATIONTYPE_FADE"),
                                                        TRANSLATE("_ANIMATIONTYPE_INSTANT")};
                    Draw::Combo(animVec, &config->Widget.General.animType, C_TRANSLATE("_TAB_CONFIG_WIDGET_GENERAL_ANIM"));

                    auto msg = "%.1f" + TRANSLATE("_TIMESECOND");
                    Draw::SliderFloat(C_TRANSLATE("_TAB_CONFIG_WIDGET_GENERAL_ANIMDELAY"), &config->Widget.General.animDelay,
                                      1.0f, 5.0f, msg.c_str(), ImGuiSliderFlags_AlwaysClamp);

                    ImGui::TreePop();
                }
                ImGui::PopItemWidth();
            }
            ImGui::EndGroup();
            auto size = ImGui::GetItemRectSize();
            if (size.y != 0.0f) groupWidgetLeftSize = size;
        }
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("WidgetRegionRight", ImVec2(0.0f, groupWidgetSize.y), false);
        {
            ImGui::BeginGroup();
            {
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
                ImGui::SetNextItemOpen(true, ImGuiCond_Once);
                if (ImGui::TreeNode(C_TRANSLATE("_TAB_EQUIPSETS"))) {
                    if (ImGui::TreeNode(C_TRANSLATE("_TAB_EQUIPSETS_NORMAL"))) {
                        DrawWidgetSection(&config->Widget.Equipset.Normal);
                        ImGui::TreePop();
                    }
                    if (ImGui::TreeNode(C_TRANSLATE("_TAB_EQUIPSETS_POTION"))) {
                        DrawWidgetSection(&config->Widget.Equipset.Potion);
                        ImGui::TreePop();
                    }
                    if (ImGui::TreeNode(C_TRANSLATE("_TAB_EQUIPSETS_CYCLE"))) {
                        DrawWidgetSection(&config->Widget.Equipset.Cycle);
                        ImGui::TreePop();
                    }
                    ImGui::TreePop();
                }
                ImGui::SetNextItemOpen(true, ImGuiCond_Once);
                if (ImGui::TreeNode(C_TRANSLATE("_TAB_EQUIPMENT"))) {
                    if (ImGui::TreeNode(C_TRANSLATE("_TAB_EQUIPMENT_ARMOR"))) {
                        DrawWidgetSection(&config->Widget.Equipment.Armor);
                        ImGui::TreePop();
                    }
                    if (ImGui::TreeNode(C_TRANSLATE("_TAB_EQUIPMENT_WEAPON"))) {
                        DrawWidgetSection(&config->Widget.Equipment.Weapon);
                        ImGui::TreePop();
                    }
                    if (ImGui::TreeNode(C_TRANSLATE("_TAB_EQUIPMENT_SHOUT"))) {
                        DrawWidgetSection(&config->Widget.Equipment.Shout);
                        ImGui::TreePop();
                    }
                    ImGui::TreePop();
                }
                ImGui::PopItemWidth();
            }
            ImGui::EndGroup();
            auto size = ImGui::GetItemRectSize();
            if (size.y != 0.0f) groupWidgetRightSize = size;
        }
        ImGui::EndChild();
    }
    Draw::EndGroupPanel();
    groupWidgetSize = groupWidgetLeftSize.y > groupWidgetRightSize.y
            ? groupWidgetLeftSize
            : groupWidgetRightSize;

    ImGui::Text(" ");
    
    static auto settingsSize = ImVec2(0.0f, 0.0f);
    auto groupSettingLabel = fmt::format("{}  {}", ICON_FA_COG, TRANSLATE("_TAB_CONFIG_SETTINGS"));
    Draw::BeginGroupPanel(groupSettingLabel.c_str(), ImVec2(-FLT_MIN, 0.0f), ImVec2(15.0f, 10.0f));
    {
        ImGui::BeginChild("SettingsRegionLeft", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, settingsSize.y), false);
        {
            ImGui::BeginGroup();
            {
                Draw::InputButton(&config->Settings.modifier1, "Modifier1", TRANSLATE("_EDIT"), TRANSLATE("_MODIFIER1"));
                Draw::InputButton(&config->Settings.modifier2, "Modifier2", TRANSLATE("_EDIT"), TRANSLATE("_MODIFIER2"));
                Draw::InputButton(&config->Settings.modifier3, "Modifier3", TRANSLATE("_EDIT"), TRANSLATE("_MODIFIER3"));
            }
            ImGui::EndGroup();
            auto size = ImGui::GetItemRectSize();
            if (size.y != 0.0f) settingsSize = size;
        }
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("SettingsRegionRight", ImVec2(0.0f, settingsSize.y), false);
        {
            ImGui::BeginGroup();
            {
                std::vector<std::string> items = {TRANSLATE("_TAB_CONFIG_SETTINGS_SORT_CREATEASC"),
                                                  TRANSLATE("_TAB_CONFIG_SETTINGS_SORT_CREATEDESC"),
                                                  TRANSLATE("_TAB_CONFIG_SETTINGS_SORT_NAMEASC"),
                                                  TRANSLATE("_TAB_CONFIG_SETTINGS_SORT_NAMEDESC")};
                Draw::Combo(items, &config->Settings.sort, C_TRANSLATE("_TAB_CONFIG_SETTINGS_SORTORDER"));
                ImGui::Checkbox(C_TRANSLATE("_TAB_CONFIG_SETTINGS_FAVOR"), &config->Settings.favorOnly);
            }
            ImGui::EndGroup();
        }
        ImGui::EndChild();
    }
    Draw::EndGroupPanel();

    ImGui::Text(" ");

    static auto guiSize = ImVec2(0.0f, 0.0f);
    auto groupGuiLabel = fmt::format("{}  {}", ICON_FA_DESKTOP, TRANSLATE("_TAB_CONFIG_GUI"));
    Draw::BeginGroupPanel(groupGuiLabel.c_str(), ImVec2(-FLT_MIN, 0.0f), ImVec2(15.0f, 10.0f));
    {
        ImGui::BeginChild("GuiRegionLeft", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, guiSize.y), false);
        {
            ImGui::BeginGroup();
            {
                auto hotkey = reinterpret_cast<uint32_t*>(&config->Gui.hotkey);
                if (hotkey) Draw::InputButton(hotkey, "GuiHotkey", TRANSLATE("_EDIT"), TRANSLATE("_TAB_CONFIG_GUI_HOTKEY"));
                ImGui::Checkbox(C_TRANSLATE("_TAB_CONFIG_GUI_WINDOWBORDER"), &config->Gui.windowBorder);
                ImGui::Checkbox(C_TRANSLATE("_TAB_CONFIG_GUI_FRAMEBORDER"), &config->Gui.frameBorder);
            }
            ImGui::EndGroup();
        }
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("GuiRegionRight", ImVec2(0.0f, guiSize.y), false);
        {
            ImGui::BeginGroup();
            {
                {
                    std::vector<std::string> items = {"Chinese", "Czech",    "English",   "French", "German",
                                                      "Italian", "Japanese", "Korean",    "Polish", "Russian",
                                                      "Spanish", "Thai",     "Vietnamese"};
                    if (Draw::Combo(items, &config->Gui.language, TRANSLATE("_TAB_CONFIG_GUI_LANGUAGE"))) {
                        reload_font.store(true);
                        Translator::GetSingleton()->Load();
                        DataHandler::GetSingleton()->Init();
                    }
                }
                {
                    std::vector<std::string> items = {TRANSLATE("_STYLE_DARK"),
                                                      TRANSLATE("_STYLE_LIGHT"),
                                                      TRANSLATE("_STYLE_CLASSIC")};
                    Draw::Combo(items, &config->Gui.style, TRANSLATE("_TAB_CONFIG_GUI_STYLE"));
                }
                Draw::SliderFloat(TRANSLATE("_TAB_CONFIG_GUI_ROUNDING"), &config->Gui.rounding, 0.0f, 12.0f, "%.1f",
                                  ImGuiSliderFlags_AlwaysClamp);
                ImGui::InputFloat(C_TRANSLATE("_FONT_SIZE"), &config->Gui.fontSize, 1.0f, 0.0f, "%.0f");
                if (ImGui::IsItemDeactivatedAfterEdit()) {
                    if (config->Gui.fontSize < 13.0f) {
                        config->Gui.fontSize = 13.0f;
                    } else if (config->Gui.fontSize > 26.0f) {
                        config->Gui.fontSize = 26.0f;
                    }
                    reload_font.store(true);
                }
            }
            ImGui::EndGroup();
            auto size = ImGui::GetItemRectSize();
            if (size.y != 0.0f) guiSize = size;
        }
        ImGui::EndChild();
    }
    Draw::EndGroupPanel();

    if (shouldReload) {
        auto equipment = EquipmentManager::GetSingleton();
        if (!equipment) return;

        auto equipset = EquipsetManager::GetSingleton();
        if (!equipset) return;

        auto widgetHandler = WidgetHandler::GetSingleton();
        if (!widgetHandler) return;

        widgetHandler->CloseWidgetMenu();
        widgetHandler->OpenWidgetMenu();
        equipset->RemoveAllWidget();
        equipment->RemoveAllArmorWidget();
        equipment->RemoveAllWeaponWidget();
        equipment->RemoveAllShoutWidget();
        equipment->CreateAllArmorWidget();
        equipment->CreateAllWeaponWidget();
        equipment->CreateAllShoutWidget();
        equipset->CreateAllWidget();
    }
}