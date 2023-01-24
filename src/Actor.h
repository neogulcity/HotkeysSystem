#pragma once

namespace Actor {
    uint32_t GetActorSpellCount(RE::Actor* _actor);
    uint32_t GetBaseSpellCount(RE::Actor* _actor);
    uint32_t GetRaceSpellCount(RE::Actor* _actor);

    uint32_t GetBaseShoutCount(RE::Actor* _actor);
    uint32_t GetRaceShoutCount(RE::Actor* _actor);

    RE::SpellItem* GetActorNthSpell(RE::Actor* _actor, const uint32_t& _index);
    RE::SpellItem* GetBaseNthSpell(RE::Actor* _actor, const uint32_t& _index);
    RE::SpellItem* GetRaceNthSpell(RE::Actor* _actor, const uint32_t& _index);

    RE::TESShout* GetBaseNthShout(RE::Actor* _actor, const uint32_t& _index);
    RE::TESShout* GetRaceNthShout(RE::Actor* _actor, const uint32_t& _index);

    RE::TESForm* GetEquippedShout(RE::Actor* _actor);
    std::vector<RE::TESForm*> GetAllEquippedItems();

    bool HasItem(RE::Actor* _actor, RE::TESForm* _item);
    bool HasMagic(RE::Actor* _actor, RE::TESForm* _spell);
    bool HasShout(RE::Actor* _actor, RE::TESForm* _shout);
}