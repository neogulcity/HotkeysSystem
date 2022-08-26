#include "Actor.h"

namespace Actor {
    uint32_t GetSpellCount(RE::Actor* _actor)
    {
        if (!_actor) {
            return 0;
        }
        
        return _actor->addedSpells.size();
    }

    uint32_t GetSpellCount(RE::TESNPC* _actor)
    {
        if (!_actor) {
            return 0;
        }

        auto data = _actor->GetSpellList();
        if (data) {
            return data->numSpells;
        }
        
        return 0;
    }

    RE::SpellItem* GetNthSpell(RE::Actor* _actor, uint32_t _index) {
        if (!_actor) {
            return nullptr;
        }

        if (_index < GetSpellCount(_actor)) {
            return _actor->addedSpells[_index];
        }

        return nullptr;
    }

    RE::SpellItem* GetNthSpell(RE::TESNPC* _actor, uint32_t _index)
    {
        if (!_actor) {
            return nullptr;
        }

        auto data = _actor->GetSpellList();
        if (data && _index < data->numSpells) {
            return data->spells[_index];
        }

        return nullptr;
    }

    uint32_t GetShoutCount(RE::TESNPC* _actor)
    {
        if (!_actor) {
            return 0;
        }

        auto data = _actor->GetSpellList();
        if (data) {
            return data->numShouts;
        }

        return 0;
    }

    RE::TESShout* GetNthShout(RE::TESNPC* _actor, uint32_t _index)
    {
        if (!_actor) {
            return nullptr;
        }

        auto data = _actor->GetSpellList();
        if (data && _index < data->numSpells) {
            return data->shouts[_index];
        }

        return nullptr;
    }

    RE::TESForm* GetEquippedShout(RE::Actor* _actor)
    {
        if (!_actor) {
            return nullptr;
        }

        return _actor->selectedPower;
    }

    bool HasItem(RE::Actor* _actor, RE::TESForm* _form)
    {
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

    bool HasMagic(RE::Actor* _actor, RE::SpellItem* _spell)
    {
        if (!_actor) {
            return false;
        }

        if (!_spell) {
            return false;
        }
        
        auto spellSize = GetSpellCount(_actor);
        for (int i = 0; i < spellSize; i++) {
            auto spell = Actor::GetNthSpell(_actor, i);
            if (spell && spell->GetFormID() == _spell->GetFormID()) {
                return true;
            }
        }

        return false;
    }

    bool HasMagic(RE::TESNPC* _actor, RE::SpellItem* _spell)
    {
        if (!_actor) {
            return false;
        }

        if (!_spell) {
            return false;
        }

        
        auto baseSpellSize = GetSpellCount(_actor);
        for (int i = 0; i < baseSpellSize; i++) {
            auto spell = GetNthSpell(_actor, i);
            if (spell && spell->GetFormID() == _spell->GetFormID()) {
                return true;
            }
        }

        return false;
    }

    bool HasShout(RE::TESNPC* _actor, RE::TESShout* _shout)
    {
        if (!_actor) {
            return false;
        }

        if (!_shout) {
            return false;
        }

        auto baseShoutSize = Actor::GetShoutCount(_actor);
        for (int i = 0; i < baseShoutSize; i++) {
            auto shout = Actor::GetNthShout(_actor, i);
            if (shout && shout->GetFormID() == _shout->GetFormID()) {
                return true;
            }
        }

        return false;
    }
}