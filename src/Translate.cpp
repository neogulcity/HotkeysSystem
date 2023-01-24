#include "Translate.h"
#include "Config.h"

#include <filesystem>

#include <toml++/toml.h>

void Translator::Load() {
    try {
        map.clear();
        std::filesystem::path file_path;

        auto Config = ConfigHandler::GetSingleton();
        switch (Config->Gui.language) {
            case (int)Config::LangType::CHINESE:
                file_path = "Data/SKSE/Plugins/UIHS/Translate/Translate_chinese.toml";
                break;
            case (int)Config::LangType::CZECH:
                file_path = "Data/SKSE/Plugins/UIHS/Translate/Translate_czech.toml";
                break;
            case (int)Config::LangType::ENGLISH:
                file_path = "Data/SKSE/Plugins/UIHS/Translate/Translate_english.toml";
                break;
            case (int)Config::LangType::FRENCH:
                file_path = "Data/SKSE/Plugins/UIHS/Translate/Translate_french.toml";
                break;
            case (int)Config::LangType::GERMAN:
                file_path = "Data/SKSE/Plugins/UIHS/Translate/Translate_german.toml";
                break;
            case (int)Config::LangType::ITALIAN:
                file_path = "Data/SKSE/Plugins/UIHS/Translate/Translate_italian.toml";
                break;
            case (int)Config::LangType::JAPANESE:
                file_path = "Data/SKSE/Plugins/UIHS/Translate/Translate_japanese.toml";
                break;
            case (int)Config::LangType::KOREAN:
                file_path = "Data/SKSE/Plugins/UIHS/Translate/Translate_korean.toml";
                break;
            case (int)Config::LangType::POLISH:
                file_path = "Data/SKSE/Plugins/UIHS/Translate/Translate_polish.toml";
                break;
            case (int)Config::LangType::RUSSIAN:
                file_path = "Data/SKSE/Plugins/UIHS/Translate/Translate_russian.toml";
                break;
            case (int)Config::LangType::SPANISH:
                file_path = "Data/SKSE/Plugins/UIHS/Translate/Translate_spanish.toml";
                break;
            case (int)Config::LangType::THAI:
                file_path = "Data/SKSE/Plugins/UIHS/Translate/Translate_thai.toml";
                break;
            case (int)Config::LangType::VIETNAMESE:
                file_path = "Data/SKSE/Plugins/UIHS/Translate/Translate_vietnamese.toml";
                break;
            default:
                file_path = "Data/SKSE/Plugins/UIHS/Translate/Translate_english.toml";
        }
        
        auto tbl = toml::parse_file(file_path.c_str());
    
        auto nd = tbl["Translate"];
        for (auto& [key, value] : *nd.as_table()) {
            auto msg = value.value<std::string>();
            if (msg.has_value()) {
                map.insert(make_pair((std::string)key, msg.value()));
                continue;
            }

            map.insert(make_pair((std::string)key, (std::string)key));
        }
    
        logger::info("Translation loaded.");
    } catch (const toml::parse_error& err) {
        logger::warn("Failed to parse translation file.\nError: {}", err.description());
    }
}