#pragma once

namespace Papyrus {
    /** 
     * _data[0]: Equipset Name
     * _data[1]: Keycode
     * _data[2]: Modifier1 
     * _data[3]: Modifier2
     * _data[4]: Modifier3
     * _data[5]: Equip Sound
     * _data[6]: Toggle Equip/Unequip
     * _data[7]: Re Equip
     * _data[8]: Beast Hotkey
     * _data[9]: Widget Type
     * _data[10]: Horizontal Position
     * _data[11]: Vertical Position
     * _data[12]: Display Widget
     * _data[13]: Display Name
     * _data[14]: Display Hotkey
     * _data[15]: Lefthand
     * _data[16]: Righthand
     * _data[17]: Shout/Power
     * _data[18]: Number of Items Added
     * _data[19+]: Items
    */ 
    bool UIHS_NewEquipset(RE::StaticFunctionTag*, std::vector<RE::BSFixedString> _data);

    /**
     * _data[0]: Equipset Name
     * _data[1]: Keycode
     * _data[2]: Modifier1
     * _data[3]: Modifier2
     * _data[4]: Modifier3
     * _data[5]: Cycle Persist
     * _data[6]: Cycle Expire
     * _data[7]: Cycle Reset
     * _data[8]: Beast Hotkey
     * _data[9]: Horizontal Position
     * _data[10]: Vertical Position
     * _data[11]: Display Widget
     * _data[12]: Display Name
     * _data[13]: Display Hotkey
     * _data[14]: Number of CycleItems Added
     * _data[15+]: CycleItems
     */ 
    bool UIHS_NewCycleEquipset(RE::StaticFunctionTag*, std::vector<RE::BSFixedString> _data);
    bool UIHS_EditEquipset(RE::StaticFunctionTag*, RE::BSFixedString _name, std::vector<RE::BSFixedString> _data);
    bool UIHS_RemoveEquipset(RE::StaticFunctionTag*, RE::BSFixedString _data);
    void UIHS_Exec(RE::StaticFunctionTag*, int32_t _code, bool _modifier1, bool _modifier2, bool _modifier3);
    void UIHS_CalculateKeydown(RE::StaticFunctionTag*, int32_t _code, bool _modifier1, bool _modifier2, bool _modifier3, float _time);
    void UIHS_Init(RE::StaticFunctionTag*);
    void UIHS_Clear(RE::StaticFunctionTag*);
    RE::BSFixedString UIHS_GetNamePrefix(RE::StaticFunctionTag*);
    RE::BSFixedString UIHS_GetStringFromKeycode(RE::StaticFunctionTag*, int32_t _key);
    std::vector<RE::BSFixedString> UIHS_GetList(RE::StaticFunctionTag*, int32_t _type);
    RE::BSFixedString UIHS_GetStringFromList(RE::StaticFunctionTag*, int32_t _index, int32_t _type);
    RE::BSFixedString UIHS_GetKeyConflict(RE::StaticFunctionTag*, int32_t _key, std::vector<bool> _modifier, bool _beast);
    bool UIHS_IsNameConflict(RE::StaticFunctionTag*, RE::BSFixedString _name);
    bool UIHS_IsCycleEquipset(RE::StaticFunctionTag*, RE::BSFixedString _name);
    std::vector<RE::BSFixedString> UIHS_GetEquipsetData(RE::StaticFunctionTag*, RE::BSFixedString _name);
    std::vector<int32_t> UIHS_GetKeycodeList(RE::StaticFunctionTag*);
    void UIHS_SendSettingData(RE::StaticFunctionTag*, std::vector<RE::BSFixedString> _data);
    void UIHS_SendWidgetData(RE::StaticFunctionTag*, std::vector<RE::BSFixedString> _data);
    void UIHS_SaveSetting(RE::StaticFunctionTag*, std::vector<RE::BSFixedString> _data);
    std::vector<RE::BSFixedString> UIHS_LoadSetting(RE::StaticFunctionTag*);
    void UIHS_InitWidget(RE::StaticFunctionTag*);
    void UIHS_ClearWidget(RE::StaticFunctionTag*);
    void UIHS_SaveEquipsetData(RE::StaticFunctionTag*);
    void UIHS_LoadEquipsetData(RE::StaticFunctionTag*);
    void UIHS_RemoveAllEquipset(RE::StaticFunctionTag*);
    bool UIHS_IsInventoryInit(RE::StaticFunctionTag*);

    bool RegisterFuncs(RE::BSScript::IVirtualMachine* vm);
}