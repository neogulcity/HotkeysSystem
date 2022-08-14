#pragma once

namespace Papyrus {
    void Exec(RE::StaticFunctionTag*);
    void Exec2(RE::StaticFunctionTag*);
    RE::BSTArray<RE::BSFixedString> GetweaponList(RE::StaticFunctionTag*);

    bool RegisterFuncs(RE::BSScript::IVirtualMachine* vm);
}