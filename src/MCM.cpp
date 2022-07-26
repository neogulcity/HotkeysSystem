#include <fstream>
#include "json/json.h"

#include "Actor.h"
#include "EquipsetManager.h"
#include "ExtraData.h"
#include "MCM.h"
#include "Utility.h"

using namespace SKSE;

namespace MCM {
    void Init_WidgetList()
    {
        auto dataHolder = DataHolder::GetSingleton();
        if (!dataHolder) {
            return;
        }

        std::ifstream file("Data/SKSE/Plugins/UIHS/Widget.json", std::ifstream::binary);
        if (!file.is_open()) {
            log::error("Unable to open Widget.json file.");
            return ;
        }
        Json::Value root;
        file >> root;
        file.close();

        const Json::Value jValue = root["WidgetList"];
        for (int i = 0; i < jValue.size(); ++i) {
            std::string name = jValue[i].asString();
            std::string path = root["Path"].get(name, "").asString();
            if (path != "" && name != "") {
                dataHolder->list->mWidgetList.push_back(std::make_pair(name, path));
            }
        }
    }

    void Init_WeaponList()
    {
        std::vector<std::tuple<std::string, RE::TESForm*, RE::ExtraDataList*>> result;
        
        auto playerref = RE::PlayerCharacter::GetSingleton();
        if (!playerref) {
            return;
        }

        auto playerbase = playerref->GetActorBase();
        if (!playerbase) {
            return;
        }

        auto dataHolder = MCM::DataHolder::GetSingleton();
        if (!dataHolder) {
            return;
        }

        auto inv = playerref->GetInventory();
        for (const auto& [item, data] : inv) {
            const auto& [numItem, entry] = data;
            if (numItem > 0 && item->Is(RE::FormType::Weapon)) {
                int numExtra = 0;
                int numFavor = 0;
                uint32_t favorSize = 0;
                auto extraLists = entry->extraLists;
                if (extraLists) {
                    favorSize = Extra::GetNumFavorited(extraLists);
                    for (auto& xList : *extraLists) {
                        if (Extra::IsEnchanted(xList) && Extra::IsTempered(xList)) {
                            if (!dataHolder->setting->mFavor || Extra::IsFavorited(xList)) {
                                RE::BSFixedString name;
                                name = Extra::HasDisplayName(xList) ? Extra::GetDisplayName(xList) : item->GetName();
                                std::string sName = static_cast<std::string>(name);
                                result.push_back(std::make_tuple(sName, item, xList));
                                ++numFavor;
                            }
                            ++numExtra;
                        } else if (Extra::IsEnchanted(xList) && !Extra::IsTempered(xList)) {
                            if (!dataHolder->setting->mFavor || Extra::IsFavorited(xList)) {
                                RE::BSFixedString name;
                                name = Extra::HasDisplayName(xList) ? Extra::GetDisplayName(xList) : item->GetName();
                                std::string sName = static_cast<std::string>(name);
                                result.push_back(std::make_tuple(sName, item, xList));
                                ++numFavor;
                            }
                            ++numExtra;
                        } else if (!Extra::IsEnchanted(xList) && Extra::IsTempered(xList)) {
                            if (!dataHolder->setting->mFavor || Extra::IsFavorited(xList)) {
                                RE::BSFixedString name;
                                name = Extra::HasDisplayName(xList) ? Extra::GetDisplayName(xList) : item->GetName();
                                std::string sName = static_cast<std::string>(name);
                                result.push_back(std::make_tuple(sName, item, xList));
                                ++numFavor;
                            }
                            ++numExtra;
                        }
                    }
                }
                if (numItem - numExtra != 0) {
                    if (!dataHolder->setting->mFavor || favorSize - numFavor != 0) {
                        RE::BSFixedString name;
                        name = item->GetName();
                        std::string sName = static_cast<std::string>(name);
                        result.push_back(std::make_tuple(sName, item, nullptr));
                    }
                }
            }
        }

        auto baseSpellSize = Actor::GetSpellCount(playerbase);
        for (int i = 0; i < baseSpellSize; i++) {
            auto spell = Actor::GetNthSpell(playerbase, i);
            if (spell) {
                auto type = spell->GetSpellType();
                if (type == RE::MagicSystem::SpellType::kSpell) {
                    if (!dataHolder->setting->mFavor || Extra::IsMagicFavorited(spell)) {
                        std::string name = spell->GetName();
                        result.push_back(std::make_tuple(name, spell, nullptr));
                    }
                }
            }
        }

        auto spellSize = Actor::GetSpellCount(playerref);
        for (int i = 0; i < spellSize; i++) {
            auto spell = Actor::GetNthSpell(playerref, i);
            if (spell) {
                auto type = spell->GetSpellType();
                if (type == RE::MagicSystem::SpellType::kSpell) {
                    if (!dataHolder->setting->mFavor || Extra::IsMagicFavorited(spell)) {
                        std::string name = spell->GetName();
                        result.push_back(std::make_tuple(name, spell, nullptr));
                    }
                }
            }
        }

        sort(result.begin(), result.end());
        dataHolder->list->mWeaponList.push_back(std::make_tuple("$UIHS_Nothing", nullptr, nullptr));
        dataHolder->list->mWeaponList.push_back(std::make_tuple("$UIHS_Unequip", nullptr, nullptr));

        for (auto& elem : result) {
            auto& name = std::get<0>(elem);
            name = name == "" ? std::get<1>(elem)->GetName() : name;
            name = name == "" ? "_HIDDEN_" : name;
            dataHolder->list->mWeaponList.push_back(elem);
        }
    }

    void Init_ShoutList() {
        std::vector<std::pair<std::string, RE::TESForm*>> result;

        auto playerref = RE::PlayerCharacter::GetSingleton();
        if (!playerref) {
            return;
        }

        auto playerbase = playerref->GetActorBase();
        if (!playerbase) {
            return;
        }

        auto dataHolder = MCM::DataHolder::GetSingleton();
        if (!dataHolder) {
            return;
        }

        auto basePowerSize = Actor::GetSpellCount(playerbase);
        for (int i = 0; i < basePowerSize; i++) {
            auto power = Actor::GetNthSpell(playerbase, i);
            if (power) {
                auto type = power->GetSpellType();
                if (type == RE::MagicSystem::SpellType::kLesserPower) {
                    if (!dataHolder->setting->mFavor || Extra::IsMagicFavorited(power)) {
                        std::string name = power->GetName();
                        result.push_back(std::make_pair(name, power));
                    }
                }
            }
        }

        auto powerSize = Actor::GetSpellCount(playerref);
        for (int i = 0; i < powerSize; i++) {
            auto power = Actor::GetNthSpell(playerref, i);
            if (power) {
                auto type = power->GetSpellType();
                if (type == RE::MagicSystem::SpellType::kLesserPower) {
                    if (!dataHolder->setting->mFavor || Extra::IsMagicFavorited(power)) {
                        std::string name = power->GetName();
                        result.push_back(std::make_pair(name, power));
                    }
                }
            }
        }

        auto baseShoutSize = Actor::GetShoutCount(playerbase);
        for (int i = 0; i < baseShoutSize; i++) {
            auto shout = Actor::GetNthShout(playerbase, i);
            if (shout) {
                if (!dataHolder->setting->mFavor || Extra::IsMagicFavorited(shout)) {
                    std::string name = shout->GetName();
                    result.push_back(std::make_pair(name, shout));
                }
            }
        }

        sort(result.begin(), result.end());
        dataHolder->list->mShoutList.push_back(std::make_pair("$UIHS_Nothing", nullptr));
        dataHolder->list->mShoutList.push_back(std::make_pair("$UIHS_Unequip", nullptr));

        for (int i = 0; i < result.size(); i++) {
            auto& name = result[i].first;
            name = name == "" ? "_HIDDEN_" : name;
            dataHolder->list->mShoutList.push_back(result[i]);
        }
    }

    void Init_ItemsList()
    {
        std::vector<std::tuple<std::string, RE::TESForm*, RE::ExtraDataList*>> result;
        
        auto playerref = RE::PlayerCharacter::GetSingleton();
        if (!playerref) {
            return;
        }

        auto dataHolder = MCM::DataHolder::GetSingleton();
        if (!dataHolder) {
            return;
        }

        auto inv = playerref->GetInventory();
        for (const auto& [item, data] : inv) {
            if (item->Is(RE::FormType::LeveledItem, RE::FormType::Weapon)) {
                continue;
            }

            const auto& [numItem, entry] = data;
            if (numItem > 0) {
                if (item->Is(RE::FormType::Armor)) {
                    int numExtra = 0;
                    int numFavor = 0;
                    uint32_t favorSize = 0;
                    auto extraLists = entry->extraLists;
                    if (extraLists) {
                        favorSize = Extra::GetNumFavorited(extraLists);
                        for (auto& xList : *extraLists) {
                            if (Extra::IsEnchanted(xList) && Extra::IsTempered(xList)) {
                                if (!dataHolder->setting->mFavor || Extra::IsFavorited(xList)) {
                                    RE::BSFixedString name;
                                    name = Extra::HasDisplayName(xList) ? Extra::GetDisplayName(xList) : item->GetName();
                                    std::string sName = static_cast<std::string>(name);
                                    result.push_back(std::make_tuple(sName, item, xList));
                                    ++numFavor;
                                }
                                ++numExtra;
                            } else if (Extra::IsEnchanted(xList) && !Extra::IsTempered(xList)) {
                                if (!dataHolder->setting->mFavor || Extra::IsFavorited(xList)) {
                                    RE::BSFixedString name;
                                    name = Extra::HasDisplayName(xList) ? Extra::GetDisplayName(xList) : item->GetName();
                                    std::string sName = static_cast<std::string>(name);
                                    result.push_back(std::make_tuple(sName, item, xList));
                                    ++numFavor;
                                }
                                ++numExtra;
                            } else if (!Extra::IsEnchanted(xList) && Extra::IsTempered(xList)) {
                                if (!dataHolder->setting->mFavor || Extra::IsFavorited(xList)) {
                                    RE::BSFixedString name;
                                    name = Extra::HasDisplayName(xList) ? Extra::GetDisplayName(xList) : item->GetName();
                                    std::string sName = static_cast<std::string>(name);
                                    result.push_back(std::make_tuple(sName, item, xList));
                                    ++numFavor;
                                }
                                ++numExtra;
                            }
                        }
                    }
                    if (numItem - numExtra != 0) {
                        if (!dataHolder->setting->mFavor || favorSize - numFavor != 0) {
                            RE::BSFixedString name;
                            name = item->GetName();
                            std::string sName = static_cast<std::string>(name);
                            result.push_back(std::make_tuple(sName, item, nullptr));
                        }
                    }
                }
                else {
                    if (!dataHolder->setting->mFavor || Extra::IsFavorited(entry->extraLists)) {
                        RE::BSFixedString name;
                        name = item->GetName();
                        std::string sName = static_cast<std::string>(name);
                        result.push_back(std::make_tuple(sName, item, nullptr));
                    }
                }
            }
        }

        sort(result.begin(), result.end());
        dataHolder->list->mItemsList.push_back(std::make_tuple("$UIHS_Nothing", nullptr, nullptr));

        for (auto& elem : result) {
            auto& name = std::get<0>(elem);
            name = name == "" ? std::get<1>(elem)->GetName() : name;
            name = name == "" ? "_HIDDEN_" : name;
            dataHolder->list->mItemsList.push_back(elem);
        }
    }

    void Init_CycleItemsList()
    {
        std::vector<std::string> result;

        auto dataHolder = MCM::DataHolder::GetSingleton();
        if (!dataHolder) {
            return;
        }

        auto manager = &UIHS::EquipsetManager::GetSingleton();
        if (!manager) {
            return;
        }

        result.push_back("$UIHS_Nothing");

        auto list = manager->GetSortedEquipsetList();
        for (const auto& elem : list) {
            if (elem != "") {
                result.push_back(elem);
            }
        }

        dataHolder->list->mCycleItemsList = result;
    }

    void Init_FontList()
    {
        auto dataHolder = MCM::DataHolder::GetSingleton();
        if (!dataHolder) {
            return;
        }

        std::ifstream file("Data/SKSE/Plugins/UIHS/Widget.json", std::ifstream::binary);
        if (!file.is_open()) {
            log::error("Unable to open Widget.json file.");
            return;
        }
        Json::Value root;
        file >> root;

        const Json::Value jValue = root["Font"];
        for (int i = 0; i < jValue.size(); ++i) {
            if (jValue[i].asString() != "") {
                dataHolder->list->mFontList.push_back(jValue[i].asString());
            }
        }
    }

    void SetSetting(std::vector<std::string> _data)
    {
        auto dataHolder = MCM::DataHolder::GetSingleton();
        if (!dataHolder) {
            return;
        }

        assert(_data.size() == 6);

        dataHolder->setting->mModifier[0] = std::stoi(_data[0]);
        dataHolder->setting->mModifier[1] = std::stoi(_data[1]);
        dataHolder->setting->mModifier[2] = std::stoi(_data[2]);
        dataHolder->setting->mSort = static_cast<eSortType>(std::stoi(_data[3]));
        dataHolder->setting->mFavor = to_bool(_data[4]);
        dataHolder->setting->mWidgetActive = to_bool(_data[5]);
    }

    void SetWidgetData(std::vector<std::string> _data)
    {
        auto dataHolder = MCM::DataHolder::GetSingleton();
        if (!dataHolder) {
            return;
        }

        assert(_data.size() == 6);

        dataHolder->widget->mFont = dataHolder->list->mFontList[std::stoi(_data[0])];
        dataHolder->widget->mFontSize = std::stoi(_data[1]);
        dataHolder->widget->mSize = std::stoi(_data[2]);
        dataHolder->widget->mAlpha = std::stoi(_data[3]);
        dataHolder->widget->mDisplay = std::stoi(_data[4]);
        dataHolder->widget->mDelay = std::stof(_data[5]);
    }

    void SaveSetting(std::vector<std::string> _data)
    {
        assert(_data.size() == 14);

        Json::Value root;
        root["sWidget_Font"]        = _data[0];
        root["sWidget_FontSize"]    = _data[1];
        root["sWidget_Size"]        = _data[2];
        root["sWidget_Alpha"]       = _data[3];
        root["sWidget_Display"]     = _data[4];
        root["sWidget_Delay"]       = _data[5];
        root["sSetting_Modifier1"]  = _data[6];
        root["sSetting_Modifier2"]  = _data[7];
        root["sSetting_Modifier3"]  = _data[8];
        root["sSetting_Sort"]       = _data[9];
        root["sSetting_Gamepad"]    = _data[10];
        root["sSetting_Favor"]      = _data[11];
        root["sMaintenance_Mod"]    = _data[12];
        root["sMaintenance_Widget"] = _data[13];

        Json::StyledWriter writer;
        std::string outString = writer.write(root);
        std::ofstream out("Data/SKSE/Plugins/UIHS/Settings.json");
        out << outString;
        out.close();
    }

    std::vector<std::string> LoadSetting()
    {
        std::vector<std::string> result;

        std::ifstream file("Data/SKSE/Plugins/UIHS/Settings.json", std::ifstream::binary);
        if (!file.is_open()) {
            log::warn("Unable to open Settings.json file.");
            return result;
        }
        Json::Value root;
        file >> root;
        file.close();

        result.push_back(root.get("sWidget_Font", "0").asString());
        result.push_back(root.get("sWidget_FontSize", "100").asString());
        result.push_back(root.get("sWidget_Size", "100").asString());
        result.push_back(root.get("sWidget_Alpha", "100").asString());
        result.push_back(root.get("sWidget_Display", "0").asString());
        result.push_back(root.get("sWidget_Delay", "3.000000").asString());
        result.push_back(root.get("sSetting_Modifier1", "42").asString());
        result.push_back(root.get("sSetting_Modifier2", "29").asString());
        result.push_back(root.get("sSetting_Modifier3", "56").asString());
        result.push_back(root.get("sSetting_Sort", "0").asString());
        result.push_back(root.get("sSetting_Gamepad", "False").asString());
        result.push_back(root.get("sSetting_Favor", "False").asString());
        result.push_back(root.get("sMaintenance_Mod", "True").asString());
        result.push_back(root.get("sMaintenance_Widget", "True").asString());
        
        return result;
    }

    void ClearList()
    {
        auto dataHolder = MCM::DataHolder::GetSingleton();
        if (!dataHolder) {
            return;
        }

        dataHolder->list->mWidgetList.clear();
        dataHolder->list->mWeaponList.clear();
        dataHolder->list->mShoutList.clear();
        dataHolder->list->mItemsList.clear();
        dataHolder->list->mCycleItemsList.clear();
        dataHolder->list->mFontList.clear();
    }

    bool IsInventoryInit()
    {
        auto playerref = RE::PlayerCharacter::GetSingleton();
        if (!playerref) {
            log::error("Unable to get playerref.");
            return false;
        }

        auto playerbase = playerref->GetActorBase();
        if (!playerbase) {
            log::error("Unable to get PlayerActorBase.");
            return false;
        }

        auto dataHolder = MCM::DataHolder::GetSingleton();
        if (!dataHolder) {
            log::error("Unable to get DataHolder.");
            return false;
        }

        auto inv = playerref->GetInventory();
        for (const auto& [item, data] : inv) {
            const auto& [numItem, entry] = data;
            if (!item->Is(RE::FormType::Weapon, RE::FormType::Armor)) {
                continue;
            }

            if (numItem > 0) {
                auto extraLists = entry->extraLists;
                if (extraLists) {
                    for (auto& xList : *extraLists) {
                        if (Extra::IsEnchanted(xList) || Extra::IsTempered(xList)) {
                            if (Extra::HasDisplayName(xList)) {
                                RE::BSFixedString name;
                                name = Extra::GetDisplayName(xList);
                                std::string sName = static_cast<std::string>(name);
                                if (sName == "") {
                                    return false;
                                }
                            }
                        }
                    }
                }
            }
        }
        
        return true;
    }
}