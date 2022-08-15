#include <fstream>

#include "Actor.h"
#include "MCM.h"
#include "ExtraData.h"
#include "json/json.h"

using namespace SKSE;

namespace MCM {
    DataHolder& DataHolder::GetSingleton() noexcept {
        static DataHolder instance;
        return instance;
    }

    void Init_WidgetList()
    {
        auto dataHolder = &DataHolder::GetSingleton();
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

        const Json::Value jValue = root["WidgetList"];
        std::vector<std::string> name;
        std::vector<std::string> path;
        for (int i = 0; i < jValue.size(); ++i) {
            name.push_back(jValue[i].asString());
            path.push_back(root["Path"].get(name[i], "").asString());
            if (path[i] != "") {
                dataHolder->list.mWidgetList.push_back(std::make_pair(name[i], path[i]));
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

        auto dataHolder = &MCM::DataHolder::GetSingleton();
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
                            if (!dataHolder->setting.mFavor || Extra::IsFavorited(xList)) {
                                RE::BSFixedString name;
                                name = Extra::HasDisplayName(xList) ? Extra::GetDisplayName(xList) : item->GetName();
                                std::string sName = static_cast<std::string>(name);
                                result.push_back(std::make_tuple(sName, item, xList));
                                ++numFavor;
                            }
                            ++numExtra;
                        } else if (Extra::IsEnchanted(xList) && !Extra::IsTempered(xList)) {
                            if (!dataHolder->setting.mFavor || Extra::IsFavorited(xList)) {
                                RE::BSFixedString name;
                                name = Extra::HasDisplayName(xList) ? Extra::GetDisplayName(xList) : item->GetName();
                                std::string sName = static_cast<std::string>(name);
                                result.push_back(std::make_tuple(sName, item, xList));
                                ++numFavor;
                            }
                            ++numExtra;
                        } else if (!Extra::IsEnchanted(xList) && Extra::IsTempered(xList)) {
                            if (!dataHolder->setting.mFavor || Extra::IsFavorited(xList)) {
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
                    if (!dataHolder->setting.mFavor || favorSize - numFavor != 0) {
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
                    if (!dataHolder->setting.mFavor || Extra::IsMagicFavorited(spell)) {
                        RE::FormID table = 0x1031D3;  // Combat Heal Rate
                        if (spell->GetFormID() != table) {
                            std::string name = spell->GetName();
                            result.push_back(std::make_tuple(name, spell, nullptr));
                        }
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
                    if (!dataHolder->setting.mFavor || Extra::IsMagicFavorited(spell)) {
                        RE::FormID table[5] = {0x4027332,   // Ahzidal's Genius
                                               0x403B563,   // Deathbrand Instinct
                                               0x1711D,     // Shrouded Armor Full Set
                                               0x1711F,     // Nightingale Armor Full Set
                                               0x2012CCC};  // Crossbow bonus

                        bool checkID = false;
                        for (int j = 0; j < 5; j++) {
                            if (spell->GetFormID() == table[j]) {
                                checkID = true;
                            }
                        }

                        if (!checkID) {
                            std::string name = spell->GetName();
                            result.push_back(std::make_tuple(name, spell, nullptr));
                        }
                    }
                }
            }
        }

        sort(result.begin(), result.end());
        dataHolder->list.mWeaponList.push_back(std::make_tuple("$Nothing", nullptr, nullptr));
        dataHolder->list.mWeaponList.push_back(std::make_tuple("$Unequip", nullptr, nullptr));

        for (int i = 0; i < result.size(); i++) {
            dataHolder->list.mWeaponList.push_back(result[i]);
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

        auto dataHolder = &MCM::DataHolder::GetSingleton();
        if (!dataHolder) {
            return;
        }

        auto basePowerSize = Actor::GetSpellCount(playerbase);
        for (int i = 0; i < basePowerSize; i++) {
            auto power = Actor::GetNthSpell(playerbase, i);
            if (power) {
                auto type = power->GetSpellType();
                if (type == RE::MagicSystem::SpellType::kPower) {
                    if (!dataHolder->setting.mFavor || Extra::IsMagicFavorited(power)) {
                        RE::FormID table = 0x1031D3;  // Combat Heal Rate
                        if (power->GetFormID() != table) {
                            std::string name = power->GetName();
                            result.push_back(std::make_pair(name, power));
                        }
                    }
                }
            }
        }

        auto powerSize = Actor::GetSpellCount(playerref);
        for (int i = 0; i < powerSize; i++) {
            auto power = Actor::GetNthSpell(playerref, i);
            if (power) {
                auto type = power->GetSpellType();
                if (type == RE::MagicSystem::SpellType::kPower) {
                    if (!dataHolder->setting.mFavor || Extra::IsMagicFavorited(power)) {
                        RE::FormID table[5] = {0x4027332,   // Ahzidal's Genius
                                               0x403B563,   // Deathbrand Instinct
                                               0x1711D,     // Shrouded Armor Full Set
                                               0x1711F,     // Nightingale Armor Full Set
                                               0x2012CCC};  // Crossbow bonus

                        bool checkID = false;
                        for (int j = 0; j < 5; j++) {
                            if (power->GetFormID() == table[j]) {
                                checkID = true;
                            }
                        }

                        if (!checkID) {
                            std::string name = power->GetName();
                            result.push_back(std::make_pair(name, power));
                        }
                    }
                }
            }
        }

        auto baseShoutSize = Actor::GetShoutCount(playerbase);
        for (int i = 0; i < baseShoutSize; i++) {
            auto shout = Actor::GetNthShout(playerbase, i);
            if (shout) {
                if (!dataHolder->setting.mFavor || Extra::IsMagicFavorited(shout)) {
                    std::string name = shout->GetName();
                    result.push_back(std::make_pair(name, shout));
                }
            }
        }

        sort(result.begin(), result.end());
        dataHolder->list.mShoutList.push_back(std::make_pair("$Nothing", nullptr));
        dataHolder->list.mShoutList.push_back(std::make_pair("$Unequip", nullptr));

        for (int i = 0; i < result.size(); i++) {
            dataHolder->list.mShoutList.push_back(result[i]);
        }
    }

    void ClearList()
    {
        auto dataHolder = &MCM::DataHolder::GetSingleton();
        if (!dataHolder) {
            return;
        }

        std::vector<std::pair<std::string, std::string>> widget;
        std::vector<std::tuple<std::string, RE::TESForm*, RE::ExtraDataList*>> weapon;
        std::vector<std::pair<std::string, RE::TESForm*>> shout;
        std::vector<std::tuple<std::string, RE::TESForm*, RE::ExtraDataList*>> items;

        dataHolder->list.mWidgetList = widget;
        dataHolder->list.mWeaponList = weapon;
        dataHolder->list.mShoutList = shout;
        dataHolder->list.mItemsList = items;
    }
}