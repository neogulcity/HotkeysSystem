#pragma once

namespace Papyrus {
    void Exec(RE::StaticFunctionTag*);
    void Exec2(RE::StaticFunctionTag*);
    void UIHS_Init(RE::StaticFunctionTag*);
    void UIHS_Clear(RE::StaticFunctionTag*);
    RE::BSFixedString UIHS_GetNamePrefix(RE::StaticFunctionTag*);
    RE::BSFixedString UIHS_GetStringFromKeycode(RE::StaticFunctionTag*, int32_t _key);
    std::vector<RE::BSFixedString> UIHS_GetList(RE::StaticFunctionTag*, int32_t _type);
    RE::BSFixedString UIHS_GetStringFromList(RE::StaticFunctionTag*, int32_t _index, int32_t _type);

    bool RegisterFuncs(RE::BSScript::IVirtualMachine* vm);
}