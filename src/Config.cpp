#include "Config.h"

#include <filesystem>
#include <toml++/toml.h>

const std::filesystem::path config_path = "Data/SKSE/Plugins/UIHS/Config.toml";
const std::filesystem::path widgets_path = "Data/SKSE/Plugins/UIHS/Widgets.toml";
const std::filesystem::path potions_path = "Data/SKSE/Plugins/UIHS/Potions.toml";

void ConfigHandler::LoadConfig() {
    ConfigHandler::Clear();

    try {
        auto tbl = toml::parse_file(config_path.c_str());

        this->Widget.General.font = tbl["Widget"]["general_font"].value_or<uint32_t>(0);
        this->Widget.General.displayMode = tbl["Widget"]["general_display_mode"].value_or<uint32_t>((uint32_t)Config::DisplayType::ALWAYS);
        this->Widget.General.animType = tbl["Widget"]["general_animation_type"].value_or<uint32_t>((uint32_t)Config::AnimType::FADE);
        this->Widget.General.animDelay = tbl["Widget"]["general_anim_delay"].value_or<float>(3.0f);

        this->Widget.Equipset.Normal.bgType = tbl["Widget"]["normal_background"].value_or<std::string>("_BACKGROUND");
        this->Widget.Equipset.Normal.bgSize = tbl["Widget"]["normal_background_size"].value_or<uint32_t>(100);
        this->Widget.Equipset.Normal.bgAlpha = tbl["Widget"]["normal_background_alpha"].value_or<uint32_t>(100);
        this->Widget.Equipset.Normal.widgetSize = tbl["Widget"]["normal_widget_size"].value_or<uint32_t>(100);
        this->Widget.Equipset.Normal.fontSize = tbl["Widget"]["normal_font_size"].value_or<uint32_t>(100);
        this->Widget.Equipset.Normal.fontShadow = tbl["Widget"]["normal_font_shadow"].value_or<bool>(false);

        this->Widget.Equipset.Potion.bgType = tbl["Widget"]["potion_background"].value_or<std::string>("_BACKGROUND");
        this->Widget.Equipset.Potion.bgSize = tbl["Widget"]["potion_background_size"].value_or<uint32_t>(100);
        this->Widget.Equipset.Potion.bgAlpha = tbl["Widget"]["potion_background_alpha"].value_or<uint32_t>(100);
        this->Widget.Equipset.Potion.widgetSize = tbl["Widget"]["potion_widget_size"].value_or<uint32_t>(100);
        this->Widget.Equipset.Potion.fontSize = tbl["Widget"]["potion_font_size"].value_or<uint32_t>(100);
        this->Widget.Equipset.Potion.fontShadow = tbl["Widget"]["potion_font_shadow"].value_or<bool>(false);

        this->Widget.Equipset.Cycle.bgType = tbl["Widget"]["cycle_background"].value_or<std::string>("_BACKGROUND");
        this->Widget.Equipset.Cycle.bgSize = tbl["Widget"]["cycle_background_size"].value_or<uint32_t>(100);
        this->Widget.Equipset.Cycle.bgAlpha = tbl["Widget"]["cycle_background_alpha"].value_or<uint32_t>(100);
        this->Widget.Equipset.Cycle.widgetSize = tbl["Widget"]["cycle_widget_size"].value_or<uint32_t>(100);
        this->Widget.Equipset.Cycle.fontSize = tbl["Widget"]["cycle_font_size"].value_or<uint32_t>(100);
        this->Widget.Equipset.Cycle.fontShadow = tbl["Widget"]["cycle_font_shadow"].value_or<bool>(false);

        this->Widget.Equipment.Armor.bgType = tbl["Widget"]["armor_background"].value_or<std::string>("_BACKGROUND");
        this->Widget.Equipment.Armor.bgSize = tbl["Widget"]["armor_background_size"].value_or<uint32_t>(100);
        this->Widget.Equipment.Armor.bgAlpha = tbl["Widget"]["armor_background_alpha"].value_or<uint32_t>(100);
        this->Widget.Equipment.Armor.widgetSize = tbl["Widget"]["armor_widget_size"].value_or<uint32_t>(100);
        this->Widget.Equipment.Armor.fontSize = tbl["Widget"]["armor_font_size"].value_or<uint32_t>(100);
        this->Widget.Equipment.Armor.fontShadow = tbl["Widget"]["armor_font_shadow"].value_or<bool>(false);

        this->Widget.Equipment.Weapon.bgType = tbl["Widget"]["weapon_background"].value_or<std::string>("_BACKGROUND");
        this->Widget.Equipment.Weapon.bgSize = tbl["Widget"]["weapon_background_size"].value_or<uint32_t>(100);
        this->Widget.Equipment.Weapon.bgAlpha = tbl["Widget"]["weapon_background_alpha"].value_or<uint32_t>(100);
        this->Widget.Equipment.Weapon.widgetSize = tbl["Widget"]["weapon_widget_size"].value_or<uint32_t>(100);
        this->Widget.Equipment.Weapon.fontSize = tbl["Widget"]["weapon_font_size"].value_or<uint32_t>(100);
        this->Widget.Equipment.Weapon.fontShadow = tbl["Widget"]["weapon_font_shadow"].value_or<bool>(false);

        this->Widget.Equipment.Shout.bgType = tbl["Widget"]["shout_background"].value_or<std::string>("_BACKGROUND");
        this->Widget.Equipment.Shout.bgSize = tbl["Widget"]["shout_background_size"].value_or<uint32_t>(100);
        this->Widget.Equipment.Shout.bgAlpha = tbl["Widget"]["shout_background_alpha"].value_or<uint32_t>(100);
        this->Widget.Equipment.Shout.widgetSize = tbl["Widget"]["shout_widget_size"].value_or<uint32_t>(100);
        this->Widget.Equipment.Shout.fontSize = tbl["Widget"]["shout_font_size"].value_or<uint32_t>(100);
        this->Widget.Equipment.Shout.fontShadow = tbl["Widget"]["shout_font_shadow"].value_or<bool>(false);

        this->Settings.modifier1 = tbl["Settings"]["modifier1"].value_or<ImGuiKey>(ImGuiKey_ModCtrl);
        this->Settings.modifier2 = tbl["Settings"]["modifier2"].value_or<ImGuiKey>(ImGuiKey_ModShift);
        this->Settings.modifier3 = tbl["Settings"]["modifier3"].value_or<ImGuiKey>(ImGuiKey_ModAlt);
        this->Settings.sort = tbl["Settings"]["sort_order"].value_or<uint32_t>(0);
        this->Settings.favorOnly = tbl["Settings"]["favor_only"].value_or<bool>(false);

        this->Gui.fontPath = tbl["Gui"]["font_path"].value_or<std::string>("NanumGothicBold.ttf");
        this->Gui.fontSize = tbl["Gui"]["font_size"].value_or<float>(13.0f);
        this->Gui.language = tbl["Gui"]["language"].value_or<uint32_t>((uint32_t)Config::LangType::ENGLISH);
        this->Gui.hotkey = tbl["Gui"]["hotkey"].value_or<ImGuiKey>(ImGuiKey_F6);
        this->Gui.style = tbl["Gui"]["style"].value_or<uint32_t>((uint32_t)Config::GuiStyle::DARK);
        this->Gui.rounding = tbl["Gui"]["rounding"].value_or<float>(0.0f);
        this->Gui.windowBorder = tbl["Gui"]["window_border"].value_or<bool>(true);
        this->Gui.frameBorder = tbl["Gui"]["frame_border"].value_or<bool>(false);

        logger::info("Configuration loaded.");
    } catch (const toml::parse_error& err) {
        logger::warn("Failed to parse configuration file. Using default settings.\nError: {}", err.description());
    }

    try {
        auto tbl = toml::parse_file(widgets_path.c_str());
        {
            for (int i = 0; i < 2; i++) {
                auto arr = i == 0 ? tbl["Default_Widget"].as_array() : tbl["Custom_Widget"].as_array();
                if (!arr) continue;

                arr->for_each([this](auto&& elem) {
                    if constexpr (toml::is_table<decltype(elem)>) {
                        auto id = elem["id"].value_or<std::string>("");
                        auto name = elem["name"].value_or<std::string>("");
                        auto path = elem["path"].value_or<std::string>("");

                        WidgetInfo widget;
                        widget.name = name;
                        widget.path = path;

                        if (id.length() != 0 && name.length() != 0 && path.length() != 0) {
                            widgetMap.insert(make_pair(id, widget));
                        }
                    }
                });
            }
        }
        {
            for (int i = 0; i < 2; i++) {
                auto arr = i == 0 ? tbl["Default_Equipment_Widget"].as_array() : tbl["Custom_Equipment_Widget"].as_array();
                if (!arr) continue;

                arr->for_each([this](auto&& elem) {
                    if constexpr (toml::is_table<decltype(elem)>) {
                        auto id = elem["id"].value_or<std::string>("");
                        auto keyword = elem["keyword"].value_or<std::string>("");
                        auto formid = elem["formid"].value_or<std::string>("");
                        auto modname = elem["modname"].value_or<std::string>("");
                        auto path = elem["path"].value_or<std::string>("");

                        EquipmentWidgetInfo widget;
                        widget.id = id;
                        widget.keyword = keyword;
                        widget.formid = formid.length() != 0 ? (uint32_t)std::strtol(formid.c_str(), NULL, 0) : 0U;
                        widget.modname = modname;
                        widget.path = path;

                        if ((id.length() != 0 && path.length() != 0) ||
                            (keyword.length() != 0 && path.length() != 0) ||
                            (formid.length() != 0 && modname.length() != 0 && path.length() != 0)) {
                            eq_widgetVec.push_back(widget);
                        }
                    }
                });
            }
        }

        logger::info("Widget data loaded.");
    } catch (const toml::parse_error& err) {
        logger::error("Failed to parse widget file. \nError: {}", err.description());
    }

    try {
        auto tbl = toml::parse_file(potions_path.c_str());
        for (int i = 0; i < 3; i++) {
            toml::v3::array* arr = nullptr;
            if (i == 0) arr = tbl["Health"].as_array();
            else if (i == 1) arr = tbl["Magicka"].as_array();
            else if (i == 2) arr = tbl["Stamina"].as_array();
            if (!arr) continue;

            arr->for_each([this, i](auto&& elem) {
                if constexpr (toml::is_table<decltype(elem)>) {
                    auto strformid = elem["formid"].value_or<std::string>("0x0");
                    auto modname = elem["modname"].value_or<std::string>("");

                    RE::FormID formid = (uint32_t)strtol(strformid.c_str(), NULL, 0);

                    ConfigHandler::PotionInfo info;
                    info.formid = formid;
                    info.modname = modname;

                    if (i == 0) healthVec.push_back(info);
                    else if (i == 1) magickaVec.push_back(info);
                    else if (i == 2) staminaVec.push_back(info);
                }
            });
        }

        logger::info("Potion data loaded.");
    } catch (const toml::parse_error& err) {
        logger::error("Failed to parse potion file. \nError: {}", err.description());
    }
}

void ConfigHandler::SaveConfig() {
    std::ofstream f(config_path);
    if (!f.is_open()) {
        logger::error("Failed to save configuration!");
        return;
    }
    auto tbl = toml::table{
        {"Widget", toml::table{
                {"general_font", this->Widget.General.font},
                {"general_display_mode", this->Widget.General.displayMode},
                {"general_animation_type", this->Widget.General.animType},
                {"general_anim_delay", this->Widget.General.animDelay},
                {"normal_background", this->Widget.Equipset.Normal.bgType},
                {"normal_background_size", this->Widget.Equipset.Normal.bgSize},
                {"normal_background_alpha", this->Widget.Equipset.Normal.bgAlpha},
                {"normal_widget_size", this->Widget.Equipset.Normal.widgetSize},
                {"normal_font_size", this->Widget.Equipset.Normal.fontSize},
                {"normal_font_shadow", this->Widget.Equipset.Normal.fontShadow},
                {"potion_background", this->Widget.Equipset.Potion.bgType},
                {"potion_background_size", this->Widget.Equipset.Potion.bgSize},
                {"potion_background_alpha", this->Widget.Equipset.Potion.bgAlpha},
                {"potion_widget_size", this->Widget.Equipset.Potion.widgetSize},
                {"potion_font_size", this->Widget.Equipset.Potion.fontSize},
                {"potion_font_shadow", this->Widget.Equipset.Potion.fontShadow},
                {"cycle_background", this->Widget.Equipset.Cycle.bgType},
                {"cycle_background_size", this->Widget.Equipset.Cycle.bgSize},
                {"cycle_background_alpha", this->Widget.Equipset.Cycle.bgAlpha},
                {"cycle_widget_size", this->Widget.Equipset.Cycle.widgetSize},
                {"cycle_font_size", this->Widget.Equipset.Cycle.fontSize},
                {"cycle_font_shadow", this->Widget.Equipset.Cycle.fontShadow},
                {"armor_background", this->Widget.Equipment.Armor.bgType},
                {"armor_background_size", this->Widget.Equipment.Armor.bgSize},
                {"armor_background_alpha", this->Widget.Equipment.Armor.bgAlpha},
                {"armor_widget_size", this->Widget.Equipment.Armor.widgetSize},
                {"armor_font_size", this->Widget.Equipment.Armor.fontSize},
                {"armor_font_shadow", this->Widget.Equipment.Armor.fontShadow},
                {"weapon_background", this->Widget.Equipment.Weapon.bgType},
                {"weapon_background_size", this->Widget.Equipment.Weapon.bgSize},
                {"weapon_background_alpha", this->Widget.Equipment.Weapon.bgAlpha},
                {"weapon_widget_size", this->Widget.Equipment.Weapon.widgetSize},
                {"weapon_font_size", this->Widget.Equipment.Weapon.fontSize},
                {"weapon_font_shadow", this->Widget.Equipment.Weapon.fontShadow},
                {"shout_background", this->Widget.Equipment.Shout.bgType},
                {"shout_background_size", this->Widget.Equipment.Shout.bgSize},
                {"shout_background_alpha", this->Widget.Equipment.Shout.bgAlpha},
                {"shout_widget_size", this->Widget.Equipment.Shout.widgetSize},
                {"shout_font_size", this->Widget.Equipment.Shout.fontSize},
                {"shout_font_shadow", this->Widget.Equipment.Shout.fontShadow},
            }
        },
        {"Settings", toml::table{
                {"modifier1", this->Settings.modifier1},
                {"modifier2", this->Settings.modifier2},
                {"modifier3", this->Settings.modifier3},
                {"sort_order", this->Settings.sort},
                {"favor_only", this->Settings.favorOnly},
            }
        },
        {"Gui", toml::table{
                {"font_path", this->Gui.fontPath},
                {"font_size", this->Gui.fontSize},
                {"language", this->Gui.language},
                {"hotkey", this->Gui.hotkey},
                {"style", this->Gui.style},
                {"rounding", this->Gui.rounding},
                {"window_border", this->Gui.windowBorder},
                {"frame_border", this->Gui.frameBorder},
            }
        },
    };
    f << tbl;
    logger::info("Configuration saved.");
}

std::string ConfigHandler::GetWidgetPath(const std::string& _type) {
    return widgetMap[_type].path;
}

void ConfigHandler::Clear() {
    widgetMap.clear();
    eq_widgetVec.clear();
    healthVec.clear();
    magickaVec.clear();
    staminaVec.clear();
}