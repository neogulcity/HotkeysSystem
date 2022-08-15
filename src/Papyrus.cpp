#include "Papyrus.h"
#include "EquipsetManager.h"
#include "ExtraData.h"
#include "MCM.h"
#include "Actor.h"

namespace Papyrus {
    void Exec(RE::StaticFunctionTag*)
    {
        log::debug("Exec Function Start");

        std::vector<std::tuple<std::string, RE::TESForm*, RE::ExtraDataList*>> result;
        result.push_back(std::make_tuple("$Nothing", nullptr, nullptr));
        result.push_back(std::make_tuple("$Unequip", nullptr, nullptr));

        auto playerref = RE::PlayerCharacter::GetSingleton();
        if (!playerref) {
            return ;
        }

        auto playerbase = playerref->GetActorBase();
        if (!playerbase) {
            return;
        }

        auto dataHolder = &MCM::DataHolder::GetSingleton();
        if (!dataHolder) {
            return ;
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

        if (result.size() > 2) {
            sort(result.begin() + 2, result.end());
        }

        for (int i = 0; i < result.size(); i++) {
            //dataHolder->list.mWeaponList.push_back(result[i]);
        }

        return;
    }

    void Exec2(RE::StaticFunctionTag*)
    {
        log::debug("Exec2 Function Start");

        auto manager = &UIHS::EquipsetManager::GetSingleton();
        manager->Display();
    }

    void UIHS_Init(RE::StaticFunctionTag*)
    {
        MCM::Init_WidgetList();
        MCM::Init_WeaponList();
        MCM::Init_ShoutList();
        // MCM::Init_ItemsList();
    }

    void UIHS_Clear(RE::StaticFunctionTag*)
    {
        MCM::ClearList();
    }

    RE::BSFixedString UIHS_GetNamePrefix(RE::StaticFunctionTag*)
    {
        RE::BSFixedString result;

        auto manager = &UIHS::EquipsetManager::GetSingleton();
        if (!manager) {
            return result;
        }

        return manager->GetNamePrefix();
    }

    RE::BSFixedString UIHS_GetStringFromKeycode(RE::StaticFunctionTag*, int32_t _key)
    {
        std::vector<std::string> table(128, "");
	    table = {
		    "ESC", "1", "2", "3", "4", "5", "6", "7", "8", "9",
		    "0", "-", "=", "Backspace", "Tab", "Q", "W", "E", "R", "T",
		    "Y", "U", "I", "O", "P", "Left Bracket", "Right Bracket", "Enter", "LCtrl", "A",
		    "S", "D", "F", "G", "H", "J", "K", "L", ";", "\"",
		    "~", "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M",
		    ",", ".", "/", "RShift", "NUM *", "LAlt", "Spacebar", "Caps Lock", "F1", "F2",
		    "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "NUM Lock", "Scroll Lock",
		    "NUM 7", "NUM 8", "NUM 9", "NUM -", "NUM 4", "NUM 5", "NUM 6", "NUM +", "NUM1", "NUM 2",
		    "NUM 3", "NUM 0", "NUM .", "F11", "F12", "NUM Enter", "RCtrl", "NUM /", "SysRq / PtrScr", "RAlt",
		    "Pause", "Home", "Up Arrow", "PgUp", "Left Arrow", "Right Arrow", "End", "Down Arrow", "PgDown", "Insert",
		    "Delete", "LMB", "RMB", "M/W MB", "MB3", "MB4", "MB5", "MB6", "MB7", "MW Up",
		    "MW Down", "DPAD_UP", "DPAD_DOWN", "DPAD_LEFT", "DPAD_RIGHT", "Start", "Back", "LThumb", "RThumb", "LShoulder",
		    "RShoulder", "A", "B", "X", "Y", "LT", "RT", ""
	    };

	    uint32_t result;
	    if (_key == -1)
		    result = 127;
	    else if (_key < 84)
		    result = _key - 1;
	    else if (_key < 89)
		    result = _key - 4;
	    else if (_key < 158)
		    result = _key - 71;
	    else if (_key == 181)
		    result = _key - 94;
	    else if (_key < 185)
		    result = _key - 95;
	    else if (_key == 197)
		    result = _key - 107;
	    else if (_key < 202)
		    result = _key - 108;
	    else if (_key == 203)
		    result = _key - 109;
	    else if (_key == 205)
		    result = _key - 110;
	    else if (_key < 212)
		    result = _key - 111;
	    else
		    result = _key - 155;

	    return static_cast<RE::BSFixedString>(table[result]);
    }

    std::vector<RE::BSFixedString> UIHS_GetList(RE::StaticFunctionTag*, int32_t _type)
    {
        std::vector<RE::BSFixedString> result;

        auto dataHolder = &MCM::DataHolder::GetSingleton();
        if (!dataHolder) {
            return result;
        }

        size_t size;
        auto type = static_cast<MCM::eListType>(_type);
        switch (type) {
            case MCM::eListType::Widget:
                size = dataHolder->list.mWidgetList.size();
                for (int i = 0; i < size; i++) {
                    result.push_back(static_cast<RE::BSFixedString>(dataHolder->list.mWidgetList[i].first));
                }
                break;

            case MCM::eListType::Weapon:
                size = dataHolder->list.mWeaponList.size();
                for (int i = 0; i < size; i++) {
                    std::string name = std::get<0>(dataHolder->list.mWeaponList[i]);
                    result.push_back(static_cast<RE::BSFixedString>(name));
                }
                break;

            case MCM::eListType::Shout:
                size = dataHolder->list.mShoutList.size();
                for (int i = 0; i < size; i++) {
                    std::string name = dataHolder->list.mShoutList[i].first;
                    result.push_back(static_cast<RE::BSFixedString>(name));
                }
                break;

            default:
                break;
        }

        return result;
    }

    RE::BSFixedString UIHS_GetStringFromList(RE::StaticFunctionTag*, int32_t _index, int32_t _type)
    {
        RE::BSFixedString result;

        auto dataHolder = &MCM::DataHolder::GetSingleton();
        if (!dataHolder) {
            return result;
        }

        size_t size;
        auto type = static_cast<MCM::eListType>(_type);
        switch (type) {
            case MCM::eListType::Widget:
                size = dataHolder->list.mWidgetList.size();
                if (_index < size) {
                    std::string name = dataHolder->list.mWidgetList[_index].first;
                    result = static_cast<RE::BSFixedString>(name);
                }
                break;

            case MCM::eListType::Weapon:
                size = dataHolder->list.mWeaponList.size();
                if (_index < size) {
                    std::string name = std::get<0>(dataHolder->list.mWeaponList[_index]);
                    result = static_cast<RE::BSFixedString>(name);
                }
                break;

            case MCM::eListType::Shout:
                size = dataHolder->list.mShoutList.size();
                if (_index < size) {
                    std::string name = dataHolder->list.mShoutList[_index].first;
                    result = static_cast<RE::BSFixedString>(name);
                }
                break;

            default:
                break;
        }

        return result;
    }

    bool RegisterFuncs(RE::BSScript::IVirtualMachine* vm)
    {
        vm->RegisterFunction("Exec", "_HotkeysSystem_MCM", Papyrus::Exec);
        vm->RegisterFunction("Exec2", "_HotkeysSystem_MCM", Papyrus::Exec2);
        vm->RegisterFunction("UIHS_Init", "_HotkeysSystem_MCM", Papyrus::UIHS_Init);
        vm->RegisterFunction("UIHS_Clear", "_HotkeysSystem_MCM", Papyrus::UIHS_Clear);
        vm->RegisterFunction("UIHS_GetNamePrefix", "_HotkeysSystem_MCM", Papyrus::UIHS_GetNamePrefix);
        vm->RegisterFunction("UIHS_GetStringFromKeycode", "_HotkeysSystem_MCM", Papyrus::UIHS_GetStringFromKeycode);
        vm->RegisterFunction("UIHS_GetList", "_HotkeysSystem_MCM", Papyrus::UIHS_GetList);
        vm->RegisterFunction("UIHS_GetStringFromList", "_HotkeysSystem_MCM", Papyrus::UIHS_GetStringFromList);

        return true;
    }
}