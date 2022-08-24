#include "Papyrus.h"
#include "EquipsetManager.h"
#include "ExtraData.h"
#include "MCM.h"
#include "Actor.h"
#include "Utility.h"

bool IsMenuOpen()
{
    auto UI = RE::UI::GetSingleton();
    if (!UI) {
        return false;
    }

    return UI->GameIsPaused() || UI->IsMenuOpen("Crafting Menu") || UI->IsMenuOpen("LootMenu") || UI->IsMenuOpen("Dialogue Menu");
}

namespace Papyrus {
    void UIHS_Exec(RE::StaticFunctionTag*, int32_t _code, bool _modifier1, bool _modifier2, bool _modifier3)
    {
        auto manager = &UIHS::EquipsetManager::GetSingleton();
        if (!manager) {
            return;
        }

        if (IsMenuOpen()) {
            return;
        }

        manager->Exec(_code, _modifier1, _modifier2, _modifier3);

        return;
    }

    void UIHS_CalculateKeydown(RE::StaticFunctionTag*, int32_t _code, bool _modifier1, bool _modifier2, bool _modifier3, float _time)
    {
        auto manager = &UIHS::EquipsetManager::GetSingleton();
        if (!manager) {
            return;
        }

        if (IsMenuOpen()) {
            return;
        }

        manager->CalculateKeydown(_code, _modifier1, _modifier2, _modifier3, _time);

        return;
    }

    void UIHS_Init(RE::StaticFunctionTag*)
    {
        MCM::Init_WidgetList();
        MCM::Init_WeaponList();
        MCM::Init_ShoutList();
        MCM::Init_ItemsList();
        MCM::Init_CycleItemsList();
        MCM::Init_FontList();
    }

    void UIHS_Clear(RE::StaticFunctionTag*)
    {
        MCM::ClearList();
    }

    RE::BSFixedString UIHS_GetNamePrefix(RE::StaticFunctionTag*)
    {
        RE::BSFixedString result;

        auto manager = &UIHS::EquipsetManager::GetSingleton();
        if (!manager) {
            return result;
        }

        return manager->GetNamePrefix();
    }

    RE::BSFixedString UIHS_GetStringFromKeycode(RE::StaticFunctionTag*, int32_t _key)
    {
        std::string strKey = GetStringFromKeycode(_key);

	    return static_cast<RE::BSFixedString>(strKey);
    }

    std::vector<RE::BSFixedString> UIHS_GetList(RE::StaticFunctionTag*, int32_t _type)
    {
        std::vector<RE::BSFixedString> result;

        auto manager = &UIHS::EquipsetManager::GetSingleton();
        if (!manager) {
            return result;
        }

        auto holder = &MCM::DataHolder::GetSingleton();
        if (!holder) {
            return result;
        }

        std::vector<std::string> equipsets;
        size_t size;
        auto type = static_cast<MCM::eListType>(_type);
        switch (type) {
            case MCM::eListType::Widget:
                size = holder->list.mWidgetList.size();
                for (int i = 0; i < size; i++) {
                    std::string name = holder->list.mWidgetList[i].first;
                    result.push_back(static_cast<RE::BSFixedString>(name));
                }
                break;

            case MCM::eListType::Weapon:
                size = holder->list.mWeaponList.size();
                for (int i = 0; i < size; i++) {
                    std::string name = std::get<0>(holder->list.mWeaponList[i]);
                    result.push_back(static_cast<RE::BSFixedString>(name));
                }
                break;

            case MCM::eListType::Shout:
                size = holder->list.mShoutList.size();
                for (int i = 0; i < size; i++) {
                    std::string name = holder->list.mShoutList[i].first;
                    result.push_back(static_cast<RE::BSFixedString>(name));
                }
                break;

            case MCM::eListType::Items:
                size = holder->list.mItemsList.size();
                for (int i = 0; i < size; i++) {
                    std::string name = std::get<0>(holder->list.mItemsList[i]);
                    result.push_back(static_cast<RE::BSFixedString>(name));
                }
                break;

            case MCM::eListType::CycleItems:
                size = holder->list.mCycleItemsList.size();
                for (int i = 0; i < size; i++) {
                    std::string name = holder->list.mCycleItemsList[i];
                    result.push_back(static_cast<RE::BSFixedString>(name));
                }
                break;

             case MCM::eListType::SelectList:
                result.push_back(static_cast<RE::BSFixedString>("$UIHS_Cancel"));
                equipsets = manager->GetAllSortedEquipsetList();
                for (const auto& elem : equipsets) {
                    result.push_back(static_cast<RE::BSFixedString>(elem));
                }
                break;

            case MCM::eListType::Equipset:
                equipsets = manager->GetAllSortedEquipsetList();
                for (const auto& elem : equipsets) {
                    result.push_back(static_cast<RE::BSFixedString>(elem));
                }
                break;

             case MCM::eListType::Font:
                size = holder->list.mFontList.size();
                for (int i = 0; i < size; i++) {
                    std::string name = holder->list.mFontList[i];
                    result.push_back(static_cast<RE::BSFixedString>(name));
                }
                break;

            default:
                break;
        }

        return result;
    }

    RE::BSFixedString UIHS_GetStringFromList(RE::StaticFunctionTag*, int32_t _index, int32_t _type)
    {
        RE::BSFixedString result;

        auto manager = &UIHS::EquipsetManager::GetSingleton();
        if (!manager) {
            return result;
        }

        auto holder = &MCM::DataHolder::GetSingleton();
        if (!holder) {
            return result;
        }

        std::vector<std::string> equipsets;
        size_t size;
        auto type = static_cast<MCM::eListType>(_type);
        switch (type) {
            case MCM::eListType::Widget:
                size = holder->list.mWidgetList.size();
                if (_index < size) {
                    std::string name = holder->list.mWidgetList[_index].first;
                    result = static_cast<RE::BSFixedString>(name);
                }
                break;

            case MCM::eListType::Weapon:
                size = holder->list.mWeaponList.size();
                if (_index < size) {
                    std::string name = std::get<0>(holder->list.mWeaponList[_index]);
                    result = static_cast<RE::BSFixedString>(name);
                }
                break;

            case MCM::eListType::Shout:
                size = holder->list.mShoutList.size();
                if (_index < size) {
                    std::string name = holder->list.mShoutList[_index].first;
                    result = static_cast<RE::BSFixedString>(name);
                }
                break;

            case MCM::eListType::Items:
                size = holder->list.mItemsList.size();
                if (_index < size) {
                    std::string name = std::get<0>(holder->list.mItemsList[_index]);
                    result = static_cast<RE::BSFixedString>(name);
                }
                break;

            case MCM::eListType::CycleItems:
                size = holder->list.mCycleItemsList.size();
                if (_index < size) {
                    std::string name = holder->list.mCycleItemsList[_index];
                    result = static_cast<RE::BSFixedString>(name);
                }
                break;

             case MCM::eListType::SelectList:
                equipsets = manager->GetAllSortedEquipsetList();
                if (_index != 0 && _index < equipsets.size() + 1) {
                    std::string name = equipsets[_index - 1];
                    result = static_cast<RE::BSFixedString>(name);
                }
                break;

            case MCM::eListType::Equipset:
                equipsets = manager->GetAllSortedEquipsetList();
                if (_index < equipsets.size()) {
                    std::string name = equipsets[_index];
                    result = static_cast<RE::BSFixedString>(name);
                }
                break;

             case MCM::eListType::Font:
                size = holder->list.mFontList.size();
                if (_index < size) {
                    std::string name = holder->list.mFontList[_index];
                    result = static_cast<RE::BSFixedString>(name);
                }
                break;

            default:
                break;
        }

        return result;
    }

    bool UIHS_NewEquipset(RE::StaticFunctionTag*, std::vector<RE::BSFixedString> _data)
    {
        auto manager = &UIHS::EquipsetManager::GetSingleton();
        if (!manager) {
            return false;
        }

        auto holder = &MCM::DataHolder::GetSingleton();
        if (!holder) {
            return false;
        }

        std::vector<std::string> stringData;
        for (const auto& elem : _data) {
            stringData.push_back(static_cast<std::string>(elem));
        }

        uint32_t order = manager->GetAllEquipsetSize();

        std::string name = stringData[0];
        Hotkey* hotkey = new Hotkey;
        hotkey->mKeyCode = std::stoi(stringData[1]);
        hotkey->mModifier[0] = to_bool(stringData[2]);
        hotkey->mModifier[1] = to_bool(stringData[3]);
        hotkey->mModifier[2] = to_bool(stringData[4]);

        Option* option = new Option;
        option->mSound = to_bool(stringData[5]);
        option->mToggleEquip = to_bool(stringData[6]);
        option->mReEquip = to_bool(stringData[7]);
        option->mBeast = to_bool(stringData[8]);

        Widget* widget = new Widget;
        widget->mWidget = holder->list.mWidgetList[std::stoi(stringData[9])].second;
        widget->mHpos = std::stoi(stringData[10]);
        widget->mVpos = std::stoi(stringData[11]);
        widget->mDisplayWidget = to_bool(stringData[12]);
        widget->mDisplayName = to_bool(stringData[13]);
        widget->mDisplayHotkey = to_bool(stringData[14]);

        Equipment* equipment = new Equipment;
        auto& left = equipment->mLeft;
        auto lAction = static_cast<MCM::eAction>(std::stoi(stringData[15]));
        switch (lAction) {
            case MCM::eAction::Nothing:
                left.option = static_cast<int32_t>(MCM::eAction::Nothing);
                break;

            case MCM::eAction::Unequip:
                left.option = static_cast<int32_t>(MCM::eAction::Unequip);
                break;

            default:
                left.option = static_cast<int32_t>(MCM::eAction::Equip);
        }
        left.form = std::get<1>(holder->list.mWeaponList[std::stoi(stringData[15])]);
        auto xList = std::get<2>(holder->list.mWeaponList[std::stoi(stringData[15])]);
        bool bEnch = xList && Extra::IsEnchanted(xList);
        bool bTemp = xList && Extra::IsTempered(xList);
        left.hasExtra = std::make_pair(bEnch, bTemp);
        left.extraData = std::make_pair(Extra::GetEnchantment(xList), Extra::GetHealth(xList));
        left.numEnch = Extra::GetNumEnchantment(xList);
        left.xList = xList;

        auto& right = equipment->mRight;
        auto rAction = static_cast<MCM::eAction>(std::stoi(stringData[16]));
        switch (rAction) {
            case MCM::eAction::Nothing:
                right.option = static_cast<int32_t>(MCM::eAction::Nothing);
                break;

            case MCM::eAction::Unequip:
                right.option = static_cast<int32_t>(MCM::eAction::Unequip);
                break;

            default:
                right.option = static_cast<int32_t>(MCM::eAction::Equip);
        }
        right.form = std::get<1>(holder->list.mWeaponList[std::stoi(stringData[16])]);
        xList = nullptr;
        xList = std::get<2>(holder->list.mWeaponList[std::stoi(stringData[16])]);
        bEnch = xList && Extra::IsEnchanted(xList);
        bTemp = xList && Extra::IsTempered(xList);
        right.hasExtra = std::make_pair(bEnch, bTemp);
        right.extraData = std::make_pair(Extra::GetEnchantment(xList), Extra::GetHealth(xList));
        right.numEnch = Extra::GetNumEnchantment(xList);
        right.xList = xList;

        auto& shout = equipment->mShout;
        shout.option = std::stoi(stringData[17]);
        shout.form = holder->list.mShoutList[std::stoi(stringData[17])].second;

        auto& items = equipment->mItems;
        items.numItems = std::stoi(stringData[18]);

        for (int i = 0; i < items.numItems; i++) {
            int index = i + 19;
            int elem = std::stoi(stringData[index]);

            auto form = std::get<1>(holder->list.mItemsList[elem]);
            items.form.push_back(form);

            xList = nullptr;
            xList = std::get<2>(holder->list.mItemsList[elem]);
            bEnch = xList && Extra::IsEnchanted(xList);
            bTemp = xList && Extra::IsTempered(xList);
            items.hasExtra.push_back(std::make_pair(bEnch, bTemp));
            items.extraData.push_back(std::make_pair(Extra::GetEnchantment(xList), Extra::GetHealth(xList)));
            items.numEnch.push_back(Extra::GetNumEnchantment(xList));
            items.xList.push_back(xList);
        }

        manager->NewEquipset(order, name, hotkey, option, widget, equipment);

        return true;
    }

    bool UIHS_NewCycleEquipset(RE::StaticFunctionTag*, std::vector<RE::BSFixedString> _data)
    {
        auto manager = &UIHS::EquipsetManager::GetSingleton();
        if (!manager) {
            return false;
        }

        auto holder = &MCM::DataHolder::GetSingleton();
        if (!holder) {
            return false;
        }

        std::vector<std::string> stringData;
        for (const auto& elem : _data) {
            stringData.push_back(static_cast<std::string>(elem));
        }
        
        uint32_t order = manager->GetAllEquipsetSize();

        std::string name = stringData[0];
        Hotkey* hotkey = new Hotkey;
        hotkey->mKeyCode = std::stoi(stringData[1]);
        hotkey->mModifier[0] = to_bool(stringData[2]);
        hotkey->mModifier[1] = to_bool(stringData[3]);
        hotkey->mModifier[2] = to_bool(stringData[4]);

        CycleOption* option = new CycleOption;
        option->mPersist = to_bool(stringData[5]);
        option->mExpire = std::stof(stringData[6]);
        option->mReset = std::stof(stringData[7]);
        option->mBeast = to_bool(stringData[8]);

        Widget* widget = new Widget;
        widget->mHpos = std::stoi(stringData[9]);
        widget->mVpos = std::stoi(stringData[10]);
        widget->mDisplayWidget = to_bool(stringData[11]);
        widget->mDisplayName = to_bool(stringData[12]);
        widget->mDisplayHotkey = to_bool(stringData[13]);

        std::vector<std::string> items;
        int numItems = std::stoi(stringData[14]);
        for (int i = 0; i < numItems; i++) {
            int index = i + 15;
            int elem = std::stoi(stringData[index]);

            items.push_back(holder->list.mCycleItemsList[elem]);
        }

        manager->NewCycleEquipset(order, name, hotkey, option, widget, items, std::make_pair(0, -1));

        return true;
    }

    bool UIHS_EditEquipset(RE::StaticFunctionTag*, RE::BSFixedString _name, std::vector<RE::BSFixedString> _data)
    {
        auto manager = &UIHS::EquipsetManager::GetSingleton();
        if (!manager) {
            return false;
        }

        std::string name = static_cast<std::string>(_name);

        std::vector<std::string> stringData;
        for (const auto& elem : _data) {
            stringData.push_back(static_cast<std::string>(elem));
        }

        manager->EditEquipset(name, stringData);

        return true;
    }

    bool UIHS_RemoveEquipset(RE::StaticFunctionTag*, RE::BSFixedString _data)
    {
        auto manager = &UIHS::EquipsetManager::GetSingleton();
        if (!manager) {
            return false;
        }

        std::string stringData = static_cast<std::string>(_data);

        manager->RemoveEquipset(stringData);

        return true;
    }

    RE::BSFixedString UIHS_GetKeyConflict(RE::StaticFunctionTag*, int32_t _key, std::vector<bool> _modifier, bool _beast)
    {
        RE::BSFixedString result = "_NOTFOUND_";

        auto manager = &UIHS::EquipsetManager::GetSingleton();
        if (!manager) {
            return result;
        }

        return manager->GetKeyConflict(_key, _modifier, _beast);
    }

    bool UIHS_IsNameConflict(RE::StaticFunctionTag*, RE::BSFixedString _name)
    {
        bool result = false;
        auto manager = &UIHS::EquipsetManager::GetSingleton();
        if (!manager) {
            return result;
        }

        return manager->IsNameConflict(_name);
    }

    bool UIHS_IsCycleEquipset(RE::StaticFunctionTag*, RE::BSFixedString _name)
    {
        bool result = false;
        auto manager = &UIHS::EquipsetManager::GetSingleton();
        if (!manager) {
            return result;
        }

        return manager->IsCycleEquipset(_name);
    }

    std::vector<RE::BSFixedString> UIHS_GetEquipsetData(RE::StaticFunctionTag*, RE::BSFixedString _name)
    {
        std::vector<RE::BSFixedString> result;

        auto manager = &UIHS::EquipsetManager::GetSingleton();
        if (!manager) {
            return result;
        }

        return manager->GetEquipsetData(_name);
    }

    std::vector<int32_t> UIHS_GetKeycodeList(RE::StaticFunctionTag*)
    {
        std::vector<int32_t> result;

        auto manager = &UIHS::EquipsetManager::GetSingleton();
        if (!manager) {
            return result;
        }

        return manager->GetKeycodeList();
    }

    void UIHS_SendSettingData(RE::StaticFunctionTag*, std::vector<RE::BSFixedString> _data)
    {
        std::vector<std::string> stringData;

        for (const auto& elem : _data) {
            stringData.push_back(static_cast<std::string>(elem));
        }

        MCM::SetSetting(stringData);
    }

    void UIHS_SendWidgetData(RE::StaticFunctionTag*, std::vector<RE::BSFixedString> _data)
    {
        std::vector<std::string> stringData;

        for (const auto& elem : _data) {
            stringData.push_back(static_cast<std::string>(elem));
        }

        MCM::SetWidgetData(stringData);
    }

    void UIHS_SaveSetting(RE::StaticFunctionTag*, std::vector<RE::BSFixedString> _data)
    {
        std::vector<std::string> stringData;

        for (const auto& elem : _data) {
            stringData.push_back(static_cast<std::string>(elem));
        }

        MCM::SaveSetting(stringData);
    }

    std::vector<RE::BSFixedString> UIHS_LoadSetting(RE::StaticFunctionTag*)
    {
        std::vector<std::string> data;
        std::vector<RE::BSFixedString> result;

        data = MCM::LoadSetting();

        for (const auto& elem : data) {
            result.push_back(static_cast<RE::BSFixedString>(elem));
        }
        
        return result;
    }

    void UIHS_InitWidget(RE::StaticFunctionTag*)
    {
        auto manager = &UIHS::EquipsetManager::GetSingleton();
        if (!manager) {
            return;
        }

        manager->InitWidget();
    }

    void UIHS_InitWidgetNext(RE::StaticFunctionTag*)
    {
        auto manager = &UIHS::EquipsetManager::GetSingleton();
        if (!manager) {
            return;
        }

        manager->InitWidgetNext();
    }

    void UIHS_ClearWidget(RE::StaticFunctionTag*)
    {
        auto manager = &UIHS::EquipsetManager::GetSingleton();
        if (!manager) {
            return;
        }

        manager->ClearWidget();
    }

    void UIHS_SaveEquipsetData(RE::StaticFunctionTag*)
    {
        auto manager = &UIHS::EquipsetManager::GetSingleton();
        if (!manager) {
            return;
        }

        manager->SaveEquipsetData();
    }

    void UIHS_LoadEquipsetData(RE::StaticFunctionTag*)
    {
        auto manager = &UIHS::EquipsetManager::GetSingleton();
        if (!manager) {
            return;
        }

        manager->RemoveAllEquipset();
        manager->LoadEquipsetData();
    }

    void UIHS_RemoveAllEquipset(RE::StaticFunctionTag*)
    {
        auto manager = &UIHS::EquipsetManager::GetSingleton();
        if (!manager) {
            return;
        }

        manager->RemoveAllEquipset();
    }

    bool RegisterFuncs(RE::BSScript::IVirtualMachine* vm)
    {
        vm->RegisterFunction("UIHS_Exec", "_HotkeysSystem_MCM", Papyrus::UIHS_Exec);
        vm->RegisterFunction("UIHS_CalculateKeydown", "_HotkeysSystem_MCM", Papyrus::UIHS_CalculateKeydown);
        vm->RegisterFunction("UIHS_Init", "_HotkeysSystem_MCM", Papyrus::UIHS_Init);
        vm->RegisterFunction("UIHS_Clear", "_HotkeysSystem_MCM", Papyrus::UIHS_Clear);
        vm->RegisterFunction("UIHS_GetNamePrefix", "_HotkeysSystem_MCM", Papyrus::UIHS_GetNamePrefix);
        vm->RegisterFunction("UIHS_GetStringFromKeycode", "_HotkeysSystem_MCM", Papyrus::UIHS_GetStringFromKeycode);
        vm->RegisterFunction("UIHS_GetList", "_HotkeysSystem_MCM", Papyrus::UIHS_GetList);
        vm->RegisterFunction("UIHS_GetStringFromList", "_HotkeysSystem_MCM", Papyrus::UIHS_GetStringFromList);
        vm->RegisterFunction("UIHS_NewEquipset", "_HotkeysSystem_MCM", Papyrus::UIHS_NewEquipset);
        vm->RegisterFunction("UIHS_NewCycleEquipset", "_HotkeysSystem_MCM", Papyrus::UIHS_NewCycleEquipset);
        vm->RegisterFunction("UIHS_EditEquipset", "_HotkeysSystem_MCM", Papyrus::UIHS_EditEquipset);
        vm->RegisterFunction("UIHS_RemoveEquipset", "_HotkeysSystem_MCM", Papyrus::UIHS_RemoveEquipset);
        vm->RegisterFunction("UIHS_GetKeyConflict", "_HotkeysSystem_MCM", Papyrus::UIHS_GetKeyConflict);
        vm->RegisterFunction("UIHS_IsNameConflict", "_HotkeysSystem_MCM", Papyrus::UIHS_IsNameConflict);
        vm->RegisterFunction("UIHS_IsCycleEquipset", "_HotkeysSystem_MCM", Papyrus::UIHS_IsCycleEquipset);
        vm->RegisterFunction("UIHS_GetEquipsetData", "_HotkeysSystem_MCM", Papyrus::UIHS_GetEquipsetData);
        vm->RegisterFunction("UIHS_GetKeycodeList", "_HotkeysSystem_MCM", Papyrus::UIHS_GetKeycodeList);
        vm->RegisterFunction("UIHS_SendSettingData", "_HotkeysSystem_MCM", Papyrus::UIHS_SendSettingData);
        vm->RegisterFunction("UIHS_SendWidgetData", "_HotkeysSystem_MCM", Papyrus::UIHS_SendWidgetData);
        vm->RegisterFunction("UIHS_SaveSetting", "_HotkeysSystem_MCM", Papyrus::UIHS_SaveSetting);
        vm->RegisterFunction("UIHS_LoadSetting", "_HotkeysSystem_MCM", Papyrus::UIHS_LoadSetting);
        vm->RegisterFunction("UIHS_InitWidget", "_HotkeysSystem_MCM", Papyrus::UIHS_InitWidget);
        vm->RegisterFunction("UIHS_InitWidgetNext", "_HotkeysSystem_MCM", Papyrus::UIHS_InitWidgetNext);
        vm->RegisterFunction("UIHS_ClearWidget", "_HotkeysSystem_MCM", Papyrus::UIHS_ClearWidget);
        vm->RegisterFunction("UIHS_SaveEquipsetData", "_HotkeysSystem_MCM", Papyrus::UIHS_SaveEquipsetData);
        vm->RegisterFunction("UIHS_LoadEquipsetData", "_HotkeysSystem_MCM", Papyrus::UIHS_LoadEquipsetData);
        vm->RegisterFunction("UIHS_RemoveAllEquipset", "_HotkeysSystem_MCM", Papyrus::UIHS_RemoveAllEquipset);

        return true;
    }
}