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
}