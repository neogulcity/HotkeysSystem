#include "Actor.h"

namespace Actor {
    uint32_t GetActorSpellCount(RE::Actor* _actor) {
        if (!_actor) return 0;

        return _actor->GetActorRuntimeData().addedSpells.size();
    }

    uint32_t GetBaseSpellCount(RE::Actor* _actor) {
        if (!_actor) return 0;

        auto base = _actor->GetActorBase();
        if (!base) return 0;

        auto data = base->GetSpellList();
        if (!data) return 0;
        
        return data->numSpells;
    }

    uint32_t GetRaceSpellCount(RE::Actor* _actor) {
        if (!_actor) return 0;

        auto race = _actor->GetRace();
        if (!race) return 0;

        auto data = race->actorEffects;
        if (!data) return 0;

        return data->numSpells;
    }

    uint32_t GetBaseShoutCount(RE::Actor* _actor) {
        if (!_actor) return 0;

        auto base = _actor->GetActorBase();
        if (!base) return 0;

        auto data = base->GetSpellList();
        if (!data) return 0;
        
        return data->numShouts;
    }

    uint32_t GetRaceShoutCount(RE::Actor* _actor) {
        if (!_actor) return 0;

        auto race = _actor->GetRace();
        if (!race) return 0;

        auto data = race->actorEffects;
        if (!data) return 0;

        return data->numShouts;
    }

    RE::SpellItem* GetActorNthSpell(RE::Actor* _actor, const uint32_t& _index) {
        if (!_actor) return nullptr;

        if (_index < GetActorSpellCount(_actor)) {
            return _actor->GetActorRuntimeData().addedSpells[_index];
        }

        return nullptr;
    }

    RE::SpellItem* GetBaseNthSpell(RE::Actor* _actor, const uint32_t& _index) {
        if (!_actor) return nullptr;

        auto base = _actor->GetActorBase();
        if (!base) return nullptr;

        auto data = base->GetSpellList();
        if (data && _index < data->numSpells) {
            return data->spells[_index];
        }

        return nullptr;
    }

    RE::SpellItem* GetRaceNthSpell(RE::Actor* _actor, const uint32_t& _index) {
        if (!_actor) return nullptr;

        auto race = _actor->GetRace();
        if (!race) return nullptr;

        auto data = race->actorEffects;
        if (data && _index < data->numSpells) {
            return data->spells[_index];
        }

        return nullptr;
    }

    RE::TESShout* GetBaseNthShout(RE::Actor* _actor, const uint32_t& _index) {
        if (!_actor) return nullptr;

        auto base = _actor->GetActorBase();
        if (!base) return nullptr;

        auto data = base->GetSpellList();
        if (data && _index < data->numShouts) {
            return data->shouts[_index];
        }

        return nullptr;
    }

    RE::TESShout* GetRaceNthShout(RE::Actor* _actor, const uint32_t& _index) {
        if (!_actor) return nullptr;

        auto race = _actor->GetRace();
        if (!race) return nullptr;

        auto data = race->actorEffects;
        if (data && _index < data->numShouts) {
            return data->shouts[_index];
        }

        return nullptr;
    }

    RE::TESForm* GetEquippedShout(RE::Actor* _actor) {
        if (!_actor) {
            return nullptr;
        }

        return _actor->GetActorRuntimeData().selectedPower;
    }

    std::vector<RE::TESForm*> GetAllEquippedItems() {
        std::vector<RE::TESForm*> result;

        auto player = RE::PlayerCharacter::GetSingleton();
        if (!player) {
            return result;
        }

        auto inv = player->GetInventory();
        for (const auto& [item, data] : inv) {
            if (item->Is(RE::FormType::LeveledItem)) {
                continue;
            }

            const auto& [numItems, entry] = data;
            if (numItems > 0 && entry->IsWorn()) {
                result.push_back(item);
            }
        }

        return result;
    }

    bool HasItem(RE::Actor* _actor, RE::TESForm* _form) {
        if (!_actor) {
            return false;
        }

        if (!_form) {
            return false;
        }

        auto inv = _actor->GetInventory();
        for (const auto& [item, data] : inv) {
            const auto& [numItem, entry] = data;

            if (numItem > 0 && item->GetFormID() == _form->GetFormID()) {
                return true;
            }
        }

        return false;
    }

    bool HasMagic(RE::Actor* _actor, RE::TESForm* _spell) {
        bool result = false;

        if (!_actor) return result;
        if (!_spell) return result;

        auto actorSpellSize = GetActorSpellCount(_actor);
        for (int i = 0; i < actorSpellSize; i++) {
            auto spell = GetActorNthSpell(_actor, i);
            if (spell && spell->GetFormID() == _spell->GetFormID()) {
                result = true;
                break;
            }
        }

        auto baseSpellSize = GetBaseSpellCount(_actor);
        for (int i = 0; i < baseSpellSize; i++) {
            auto spell = GetBaseNthSpell(_actor, i);
            if (spell && spell->GetFormID() == _spell->GetFormID()) {
                result = true;
                break;
            }
        }

        auto raceSpellSize = GetRaceSpellCount(_actor);
        for (int i = 0; i < raceSpellSize; i++) {
            auto spell = GetRaceNthSpell(_actor, i);
            if (spell && spell->GetFormID() == _spell->GetFormID()) {
                result = true;
                break;
            }
        }

        return result;
    }

    bool HasShout(RE::Actor* _actor, RE::TESForm* _shout) {
        bool result = false;

        if (!_actor) return result;
        if (!_shout) return result;

        if (HasMagic(_actor, _shout)) {
            result = true;
        }

        auto baseShoutSize = GetBaseShoutCount(_actor);
        for (int i = 0; i < baseShoutSize; i++) {
            auto shout = GetBaseNthShout(_actor, i);
            if (shout && shout->GetFormID() == _shout->GetFormID()) {
                result = true;
                break;
            }
        }

        auto raceShoutSize = GetRaceShoutCount(_actor);
        for (int i = 0; i < raceShoutSize; i++) {
            auto shout = GetRaceNthShout(_actor, i);
            if (shout && shout->GetFormID() == _shout->GetFormID()) {
                result = true;
                break;
            }
        }

        return result;
    }
}