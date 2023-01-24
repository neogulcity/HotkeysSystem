#pragma once

RE::TESForm* GetDummyDagger() {
    RE::FormID ID = 0x20163;
    RE::TESForm* dummyDagger = RE::TESForm::LookupByID<RE::TESForm>(ID);
    if (!dummyDagger) {
        logger::error("Failed to lookup DummyDagger FormID.");
    }
    return dummyDagger;
}

RE::BGSEquipSlot* GetRightHandSlot() {
    using func_t = decltype(GetRightHandSlot);
    if (REL::Module::IsAE()) {
        REL::Relocation<func_t> func{REL::ID(23608)};
        return func();
    } else {
        REL::Relocation<func_t> func{REL::ID(23151)};
        return func();
    }
}

RE::BGSEquipSlot* GetLeftHandSlot() {
    using func_t = decltype(GetLeftHandSlot);
    if (REL::Module::IsAE()) {
        REL::Relocation<func_t> func{REL::ID(23607)};
        return func();
    } else {
        REL::Relocation<func_t> func{REL::ID(23150)};
        return func();
    }
}