#include "Papyrus.h"

RE::BGSEquipSlot* GetRightHandSlot() {
    using func_t = decltype(GetRightHandSlot);
    REL::Relocation<func_t> func{REL::ID(23151)};
    return func();
}

void Exec(StaticFunctionTag*) {
    log::debug("Exec Function Start");

    auto playerref = RE::PlayerCharacter::GetSingleton();
    if (!playerref) return;

    const auto xContainer = playerref ? playerref->extraList.GetByType<RE::ExtraContainerChanges>() : nullptr;
    const auto invChanges = xContainer ? xContainer->changes : nullptr;

    std::vector<std::string> Name, EnchName, Health;

    if (invChanges) {
        auto inv = playerref->GetInventory();
        for (const auto& [item, data] : inv) {
            const auto& [count, entry] = data;
            if (count > 0) {
                int extracount = 0;
                auto extraLists = entry->extraLists;
                if (extraLists) {
                    for (auto& xList : *extraLists) {
                        if (xList && xList->HasType(RE::ExtraDataType::kEnchantment)) {
                            auto xEnch = xList->GetByType<RE::ExtraEnchantment>();
                            if (xEnch && xEnch->enchantment) {
                                Name.push_back(item->GetName());
                                EnchName.push_back(xEnch->enchantment->GetName());
                                Health.push_back("None");
                                ++extracount;
                            }
                        } else if (xList && xList->HasType(RE::ExtraDataType::kHealth)) {
                            auto xEnch = xList->GetByType<RE::ExtraHealth>();
                            if (xEnch) {
                                Name.push_back(item->GetName());
                                EnchName.push_back("None");
                                Health.push_back(std::to_string(xEnch->health));
                                ++extracount;
                            }
                        }
                    }
                }
                if (count - extracount != 0) {
                    Name.push_back(item->GetName());
                    EnchName.push_back("None");
                    Health.push_back("None");
                }
            }
        }
    }

    for (int i = 0; i < Name.size(); i++) {
        log::debug("Item #{} : {} - {} - {}", i, Name[i], EnchName[i], Health[i]);
    }
}

void EquipItem(RE::TESForm* a_form, RE::BGSEquipSlot* a_slot, bool a_sound, RE::ExtraDataList* a_extralist, bool a_queue = true, bool a_force = false) {
    if (!a_form) return;

    RE::ActorEquipManager* equipManager = RE::ActorEquipManager::GetSingleton();
    if (!equipManager) return;

    auto playerref = RE::PlayerCharacter::GetSingleton();
    if (!playerref) return;

    // Check FormType Spell
    if (a_form->Is(RE::FormType::Spell)) {
        RE::SpellItem* a_spell = a_form->As<RE::SpellItem>();

        equipManager->EquipSpell(playerref, a_spell, a_slot);

     // Check FormType Shout
    } else if (a_form->Is(RE::FormType::Shout)) {
        RE::TESShout* a_shout = a_form->As<RE::TESShout>();

        equipManager->EquipShout(playerref, a_shout);

     // Items
    } else {
        if (a_form->GetFormType() == RE::FormType::Light) {
            RE::TESBoundObject* a_object = a_form->As<RE::TESBoundObject>();

            equipManager->EquipObject(playerref, a_object, nullptr, 1U, a_slot, a_queue, a_force, a_sound, false);
        } else {
            RE::TESBoundObject* a_object = a_form->As<RE::TESBoundObject>();

            equipManager->EquipObject(playerref, a_object, a_extralist, 1U, a_slot, a_queue, a_force, a_sound, false);
        }
    }
}

namespace UIHS {
    bool RegisterFuncs(IVirtualMachine* vm) {
        vm->RegisterFunction("Exec", "_SimpleHotkeys_MCM", Exec);

        return true;
    }
}