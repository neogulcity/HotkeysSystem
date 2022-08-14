#include "Papyrus.h"
#include "EquipsetManager.h"
#include "ExtraData.h"
#include "MCM.h"

namespace Papyrus {
    void Exec(RE::StaticFunctionTag*)
    {
        log::debug("Exec Function Start");

        //auto playerref = RE::PlayerCharacter::GetSingleton();
        //if (!playerref) {
        //    return;
        //}

        //std::vector<std::string> Name, EnchName, Health;

        //auto inv = playerref->GetInventory();
        //for (const auto& [item, data] : inv) {
        //    const auto& [count, entry] = data;
        //    if (count > 0) {
        //        int extracount = 0;
        //        auto extraLists = entry->extraLists;
        //        if (Extra::IsEnchanted(item, extraLists)) {
        //            auto enchantment = Extra::GetEnchantment(item, extraLists);
        //            if (enchantment) {
        //                Name.push_back(item->GetName());
        //                EnchName.push_back(enchantment->GetName());
        //                Health.push_back("None");
        //                ++extracount;
        //            }
        //        }
        //        if (Extra::IsTempered(extraLists)) {
        //            auto health = Extra::GetHealth(extraLists);
        //            Name.push_back(item->GetName());
        //            EnchName.push_back("None");
        //            Health.push_back(std::to_string(health));
        //            ++extracount;
        //        }
        //        if (count - extracount != 0) {
        //            Name.push_back(item->GetName());
        //            EnchName.push_back("None");
        //            Health.push_back("None");
        //        }
        //    }
        //}

        //for (int i = 0; i < Name.size(); i++) {
        //    log::debug("Item #{} : {} - {} - {}", i, Name[i], EnchName[i], Health[i]);
        //}

        RE::BSTArray<RE::BSFixedString> result;
        result.push_back(static_cast<RE::BSFixedString>((std::string) "$Nothing"));
        result.push_back(static_cast<RE::BSFixedString>((std::string) "$Unequip"));

        auto playerref = RE::PlayerCharacter::GetSingleton();
        if (!playerref) {
            return ;
        }

        auto setting = &MCM::Setting::GetSingleton();
        if (!setting) {
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
                            if (!setting->mFavor || Extra::IsFavorited(xList)) {
                                RE::BSFixedString name;
                                name = Extra::HasDisplayName(xList) ? Extra::GetDisplayName(xList) : item->GetName();
                                result.push_back(name);
                                ++numFavor;
                            }
                            ++numExtra;
                        } else if (Extra::IsEnchanted(xList) && !Extra::IsTempered(xList)) {
                            if (!setting->mFavor || Extra::IsFavorited(xList)) {
                                RE::BSFixedString name;
                                name = Extra::HasDisplayName(xList) ? Extra::GetDisplayName(xList) : item->GetName();
                                result.push_back(name);

                                auto xSize = Extra::GetEnchantment(xList)->effects.size();
                                log::debug("xSize: {}", xSize);

                                ++numFavor;
                            }
                            ++numExtra;
                        } else if (!Extra::IsEnchanted(xList) && Extra::IsTempered(xList)) {
                            if (!setting->mFavor || Extra::IsFavorited(xList)) {
                                RE::BSFixedString name;
                                name = Extra::HasDisplayName(xList) ? Extra::GetDisplayName(xList) : item->GetName();
                                result.push_back(name);
                                ++numFavor;
                            }
                            ++numExtra;
                        }
                    }
                }
                if (numItem - numExtra != 0) {
                    if (!setting->mFavor || favorSize - numFavor != 0) {
                        RE::BSFixedString name;
                        name = item->GetName();
                        result.push_back(name);
                    }
                }
            }
        }

        for (int i = 0; i < result.size(); i++) {
            log::debug("Items #{}: {}", i, static_cast<std::string>(result[i]));
        }

        return;
    }

    void Exec2(RE::StaticFunctionTag*)
    {
        log::debug("Exec2 Function Start");

        auto manager = &UIHS::EquipsetManager::GetSingleton();
        manager->Display();
    }

    RE::BSTArray<RE::BSFixedString> GetWeaponList(RE::StaticFunctionTag*)
    {
        log::debug("Papyrus::GetWeaponList function called.");

        RE::BSTArray<RE::BSFixedString> result;
        result.push_back(static_cast<RE::BSFixedString>((std::string) "$Nothing"));
        result.push_back(static_cast<RE::BSFixedString>((std::string) "$Unequip"));

        auto playerref = RE::PlayerCharacter::GetSingleton();
        if (!playerref) {
            return result;
        }
        
        auto setting = &MCM::Setting::GetSingleton();
        if (!setting) {
            return result;
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
                            if (!setting->mFavor || Extra::IsFavorited(xList)) {
                                RE::BSFixedString name;
                                name = Extra::HasDisplayName(xList) ? Extra::GetDisplayName(xList) : item->GetName();
                                result.push_back(name);
                                ++numFavor;
                            }
                            ++numExtra;
                        } else if (Extra::IsEnchanted(xList) && !Extra::IsTempered(xList)) {
                            if (!setting->mFavor || Extra::IsFavorited(xList)) {
                                RE::BSFixedString name;
                                name = Extra::HasDisplayName(xList) ? Extra::GetDisplayName(xList) : item->GetName();
                                result.push_back(name);
                                ++numFavor;
                            }
                            ++numExtra;
                        } else if (!Extra::IsEnchanted(xList) && Extra::IsTempered(xList)) {
                            if (!setting->mFavor || Extra::IsFavorited(xList)) {
                                RE::BSFixedString name;
                                name = Extra::HasDisplayName(xList) ? Extra::GetDisplayName(xList) : item->GetName();
                                result.push_back(name);
                                ++numFavor;
                            }
                            ++numExtra;
                        }
                    }
                }
                if (numItem - numExtra != 0) {
                    if (!setting->mFavor || favorSize - numFavor != 0) {
                        RE::BSFixedString name;
                        name = item->GetName();
                        result.push_back(name);
                    }
                }
            }
        }

        return result;
    }

    bool RegisterFuncs(RE::BSScript::IVirtualMachine* vm)
    {
        vm->RegisterFunction("Exec", "_HotkeysSystem_MCM", Papyrus::Exec);
        vm->RegisterFunction("Exec2", "_HotkeysSystem_MCM", Papyrus::Exec2);
        vm->RegisterFunction("GetWeaponList", "_HotkeysSystem_MCM", Papyrus::GetWeaponList);

        return true;
    }
}