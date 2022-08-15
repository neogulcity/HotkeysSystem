#pragma once

namespace Actor {
    uint32_t GetSpellCount(RE::Actor* _actor);
    uint32_t GetSpellCount(RE::TESNPC* _actor);
    RE::SpellItem* GetNthSpell(RE::Actor* _actor, uint32_t _index);
    RE::SpellItem* GetNthSpell(RE::TESNPC* _actor, uint32_t _index);
    uint32_t GetShoutCount(RE::TESNPC* _actor);
    RE::TESShout* GetNthShout(RE::TESNPC* _actor, uint32_t _index);
}