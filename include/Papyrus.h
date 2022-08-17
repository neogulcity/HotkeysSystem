#pragma once

namespace Papyrus {
    void Exec(RE::StaticFunctionTag*);
    void Exec2(RE::StaticFunctionTag*);
    void UIHS_Init(RE::StaticFunctionTag*);
    void UIHS_Clear(RE::StaticFunctionTag*);
    bool UIHS_NewEquipset(RE::StaticFunctionTag*,
        RE::BSFixedString _name,
        int32_t _hotkey,
        bool _modifier1,
        bool _modifier2,
        bool _modifier3,
        bool _sound,
        bool _toggle,
        bool _reequip,
        bool _beast,
        int32_t _widget,
        int32_t _hpos,
        int32_t _vpos,
        bool _dwidget,
        bool _dname,
        bool _dhotkey,
        int32_t _left,
        int32_t _right,
        int32_t _shout,
        int32_t _numitems,
        std::vector<int32_t> _items);
    bool UIHS_NewCycleEquipset(RE::StaticFunctionTag*,
        RE::BSFixedString _name,
        int32_t _hotkey,
        bool _modifier1,
        bool _modifier2,
        bool _modifier3,
        bool _persist,
        float _expire,
        float _reset,
        bool _beast,
        int32_t _hpos,
        int32_t _vpos,
        bool _dwidget,
        bool _dname,
        bool _dhotkey,
        int32_t _numitems,
        std::vector<int32_t> _items);
    RE::BSFixedString UIHS_GetNamePrefix(RE::StaticFunctionTag*);
    RE::BSFixedString UIHS_GetStringFromKeycode(RE::StaticFunctionTag*, int32_t _key);
    std::vector<RE::BSFixedString> UIHS_GetList(RE::StaticFunctionTag*, int32_t _type);
    RE::BSFixedString UIHS_GetStringFromList(RE::StaticFunctionTag*, int32_t _index, int32_t _type);
    RE::BSFixedString UIHS_GetKeyConflict(RE::StaticFunctionTag*, int32_t _key, std::vector<bool> _modifier, bool _beast);
    bool UIHS_IsNameConflict(RE::StaticFunctionTag*, RE::BSFixedString _name);
    std::vector<RE::BSFixedString> UIHS_GetEquipsetData(RE::StaticFunctionTag*, RE::BSFixedString _name);

    bool RegisterFuncs(RE::BSScript::IVirtualMachine* vm);
}