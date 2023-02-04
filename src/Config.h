#pragma once

#include <imgui.h>
#include <unordered_map>

namespace Config {
    enum class LangType {
        CHINESE = 0,
        CZECH = 1,
        ENGLISH = 2,
        FRENCH = 3,
        GERMAN = 4,
        ITALIAN = 5,
        JAPANESE = 6,
        KOREAN = 7,
        POLISH = 8,
        RUSSIAN = 9,
        SPANISH = 10,
        THAI = 11,
        VIETNAMESE = 12
    };

    enum class SortType{
        CREATEASC = 0,
        CREATEDESC = 1,
        NAMEASC = 2,
        NAMEDESC = 3
    };

    enum class DisplayType {
        ALWAYS = 0,
        INCOMBAT = 1
    };

    enum class AnimType {
        FADE = 0,
        INSTANT = 1
    };

    enum class GuiStyle {
        DARK,
        LIGHT,
        CLASSIC
    };

    const int icon_smin = -320;
    const int icon_smax = 1600;
    const int text_smin = -200;
    const int text_smax = 200;
} // namespace Config

class ConfigHandler {
public:
    struct WidgetGeneral {
        uint32_t font{0};
        uint32_t displayMode{(uint32_t)Config::DisplayType::ALWAYS};
        uint32_t animType{(uint32_t)Config::AnimType::FADE};
        float animDelay{3.0f};
    };

    struct WidgetBase {
        std::string bgType{"_BACKGROUND"};
        int bgAlpha{100};
        int bgSize{100};
        int widgetSize{100};
        int fontSize{100};
        bool fontShadow{false};
    };

    struct WidgetEquipset {
        WidgetBase Normal;
        WidgetBase Potion;
        WidgetBase Cycle;
    };

    struct WidgetEquipment {
        WidgetBase Armor;
        WidgetBase Weapon;
        WidgetBase Shout;
    };

    struct config_widget {
        WidgetGeneral General;
        WidgetEquipset Equipset;
        WidgetEquipment Equipment;
    } Widget;

    struct config_settings {
        uint32_t modifier1{641};
        uint32_t modifier2{642};
        uint32_t modifier3{643};
        uint32_t sort{(uint32_t)Config::SortType::CREATEASC};
        bool favorOnly{false};
    } Settings;

    struct config_gui {
        std::string fontPath{"NotoSans-Medium.otf"};
        float fontSize{13.0f};
        float fontScaling{1.0f};
        ImGuiKey hotkey{ImGuiKey_F6};
        uint32_t language{(uint32_t)Config::LangType::ENGLISH};
        uint32_t style{(uint32_t)Config::GuiStyle::DARK};
        float rounding{0.0f};
        bool windowBorder{true};
        bool frameBorder{false};
    } Gui;

    struct WidgetInfo {
        std::string name;
        std::string path;
    };

    struct PotionInfo {
        RE::FormID formid;
        std::string modname;
    };

    struct EquipmentWidgetInfo {
        std::string id{""};
        std::string keyword{""};
        RE::FormID formid{0};
        std::string modname{""};
        std::string path{""};
    };

    std::map<std::string, WidgetInfo> widgetMap;
    std::vector<EquipmentWidgetInfo> eq_widgetVec;
    std::vector<PotionInfo> healthVec;
    std::vector<PotionInfo> magickaVec;
    std::vector<PotionInfo> staminaVec;

    void LoadConfig();
    void SaveConfig();
    std::string GetWidgetPath(const std::string& _type);
    void Clear();

    std::vector<std::string> fontVec = {"$ConsoleFont",    "$StartMenuFont",      "$DialogueFont",
                                        "$EverywhereFont", "$EverywhereBoldFont", "$EverywhereMediumFont",
                                        "$DragonFont",     "$SkyrimBooks",        "$HandwrittenFont",
                                        "$HandwrittenBold","$FalmerFont",         "$DwemerFont",
                                        "$DaedricFont",    "$MageScriptFont",     "$SkyrimSymbolsFont",
                                        "$SkyrimBooks_UnreadableFont"};

public:
    static ConfigHandler* GetSingleton() {
        static ConfigHandler singleton;
        return std::addressof(singleton);
    }
private:
    ConfigHandler() {}
    ConfigHandler(const ConfigHandler&) = delete;
    ConfigHandler(ConfigHandler&&) = delete;

    ~ConfigHandler() = default;

    ConfigHandler& operator=(const ConfigHandler&) = delete;
    ConfigHandler& operator=(ConfigHandler&&) = delete;
};