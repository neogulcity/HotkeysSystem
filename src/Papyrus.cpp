#include "Papyrus.h"
#include "EquipsetManager.h"
#include "ExtraData.h"
#include "MCM.h"
#include "Actor.h"

namespace Papyrus {
    void Exec(RE::StaticFunctionTag*)
    {
        log::debug("Exec Function Start");

        auto manager = &UIHS::EquipsetManager::GetSingleton();
        manager->Display();

        return;
    }

    void Exec2(RE::StaticFunctionTag*)
    {
        log::debug("Exec2 Function Start");

        auto manager = &UIHS::EquipsetManager::GetSingleton();
        manager->DisplayCycle();
    }

    void UIHS_Init(RE::StaticFunctionTag*)
    {
        MCM::Init_WidgetList();
        MCM::Init_WeaponList();
        MCM::Init_ShoutList();
        MCM::Init_ItemsList();
        MCM::Init_CycleItemsList();
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
        std::vector<std::string> table(128, "");
	    table = {
		    "ESC", "1", "2", "3", "4", "5", "6", "7", "8", "9",
		    "0", "-", "=", "Backspace", "Tab", "Q", "W", "E", "R", "T",
		    "Y", "U", "I", "O", "P", "Left Bracket", "Right Bracket", "Enter", "LCtrl", "A",
		    "S", "D", "F", "G", "H", "J", "K", "L", ";", "\"",
		    "~", "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M",
		    ",", ".", "/", "RShift", "NUM *", "LAlt", "Spacebar", "Caps Lock", "F1", "F2",
		    "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "NUM Lock", "Scroll Lock",
		    "NUM 7", "NUM 8", "NUM 9", "NUM -", "NUM 4", "NUM 5", "NUM 6", "NUM +", "NUM1", "NUM 2",
		    "NUM 3", "NUM 0", "NUM .", "F11", "F12", "NUM Enter", "RCtrl", "NUM /", "SysRq / PtrScr", "RAlt",
		    "Pause", "Home", "Up Arrow", "PgUp", "Left Arrow", "Right Arrow", "End", "Down Arrow", "PgDown", "Insert",
		    "Delete", "LMB", "RMB", "M/W MB", "MB3", "MB4", "MB5", "MB6", "MB7", "MW Up",
		    "MW Down", "DPAD_UP", "DPAD_DOWN", "DPAD_LEFT", "DPAD_RIGHT", "Start", "Back", "LThumb", "RThumb", "LShoulder",
		    "RShoulder", "A", "B", "X", "Y", "LT", "RT", ""
	    };

	    uint32_t result;
	    if (_key == -1)
		    result = 127;
	    else if (_key < 84)
		    result = _key - 1;
	    else if (_key < 89)
		    result = _key - 4;
	    else if (_key < 158)
		    result = _key - 71;
	    else if (_key == 181)
		    result = _key - 94;
	    else if (_key < 185)
		    result = _key - 95;
	    else if (_key == 197)
		    result = _key - 107;
	    else if (_key < 202)
		    result = _key - 108;
	    else if (_key == 203)
		    result = _key - 109;
	    else if (_key == 205)
		    result = _key - 110;
	    else if (_key < 212)
		    result = _key - 111;
	    else
		    result = _key - 155;

	    return static_cast<RE::BSFixedString>(table[result]);
    }

    std::vector<RE::BSFixedString> UIHS_GetList(RE::StaticFunctionTag*, int32_t _type)
    {
        std::vector<RE::BSFixedString> result;

        auto manager = &UIHS::EquipsetManager::GetSingleton();
        if (!manager) {
            return result;
        }

        auto dataHolder = &MCM::DataHolder::GetSingleton();
        if (!dataHolder) {
            return result;
        }

        std::vector<std::string> equipsets;
        size_t size;
        auto type = static_cast<MCM::eListType>(_type);
        switch (type) {
            case MCM::eListType::Widget:
                size = dataHolder->list.mWidgetList.size();
                for (int i = 0; i < size; i++) {
                    std::string name = dataHolder->list.mWidgetList[i].first;
                    result.push_back(static_cast<RE::BSFixedString>(name));
                }
                break;

            case MCM::eListType::Weapon:
                size = dataHolder->list.mWeaponList.size();
                for (int i = 0; i < size; i++) {
                    std::string name = std::get<0>(dataHolder->list.mWeaponList[i]);
                    result.push_back(static_cast<RE::BSFixedString>(name));
                }
                break;

            case MCM::eListType::Shout:
                size = dataHolder->list.mShoutList.size();
                for (int i = 0; i < size; i++) {
                    std::string name = dataHolder->list.mShoutList[i].first;
                    result.push_back(static_cast<RE::BSFixedString>(name));
                }
                break;

            case MCM::eListType::Items:
                size = dataHolder->list.mItemsList.size();
                for (int i = 0; i < size; i++) {
                    std::string name = std::get<0>(dataHolder->list.mItemsList[i]);
                    result.push_back(static_cast<RE::BSFixedString>(name));
                }
                break;

            case MCM::eListType::CycleItems:
                size = dataHolder->list.mCycleItemsList.size();
                for (int i = 0; i < size; i++) {
                    std::string name = dataHolder->list.mCycleItemsList[i];
                    result.push_back(static_cast<RE::BSFixedString>(name));
                }
                break;

             case MCM::eListType::Equipsets:
                result.push_back(static_cast<RE::BSFixedString>("$cancel"));
                equipsets = manager->GetAllEquipsetList();
                for (const auto& elem : equipsets) {
                    result.push_back(static_cast<RE::BSFixedString>(elem));
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

        auto dataHolder = &MCM::DataHolder::GetSingleton();
        if (!dataHolder) {
            return result;
        }

        std::vector<std::string> equipsets;
        size_t size;
        auto type = static_cast<MCM::eListType>(_type);
        switch (type) {
            case MCM::eListType::Widget:
                size = dataHolder->list.mWidgetList.size();
                if (_index < size) {
                    std::string name = dataHolder->list.mWidgetList[_index].first;
                    result = static_cast<RE::BSFixedString>(name);
                }
                break;

            case MCM::eListType::Weapon:
                size = dataHolder->list.mWeaponList.size();
                if (_index < size) {
                    std::string name = std::get<0>(dataHolder->list.mWeaponList[_index]);
                    result = static_cast<RE::BSFixedString>(name);
                }
                break;

            case MCM::eListType::Shout:
                size = dataHolder->list.mShoutList.size();
                if (_index < size) {
                    std::string name = dataHolder->list.mShoutList[_index].first;
                    result = static_cast<RE::BSFixedString>(name);
                }
                break;

            case MCM::eListType::Items:
                size = dataHolder->list.mItemsList.size();
                if (_index < size) {
                    std::string name = std::get<0>(dataHolder->list.mItemsList[_index]);
                    result = static_cast<RE::BSFixedString>(name);
                }
                break;

            case MCM::eListType::CycleItems:
                size = dataHolder->list.mCycleItemsList.size();
                if (_index < size) {
                    std::string name = dataHolder->list.mCycleItemsList[_index];
                    result = static_cast<RE::BSFixedString>(name);
                }
                break;

             case MCM::eListType::Equipsets:
                equipsets = manager->GetAllEquipsetList();
                if (_index != 0 && _index < equipsets.size() + 1) {
                    std::string name = equipsets[_index - 1];
                    result = static_cast<RE::BSFixedString>(name);
                }
                break;

            default:
                break;
        }

        return result;
    }

    bool UIHS_NewEquipset(RE::StaticFunctionTag*, RE::BSFixedString _name, int32_t _hotkey, bool _modifier1,
                          bool _modifier2, bool _modifier3, bool _sound, bool _toggle, bool _reequip, bool _beast, int32_t _widget,
                          int32_t _hpos, int32_t _vpos, bool _dwidget, bool _dname, bool _dhotkey, int32_t _left,
                          int32_t _right, int32_t _shout, int32_t _numitems, std::vector<int32_t> _items)
    {
        auto manager = &UIHS::EquipsetManager::GetSingleton();
        if (!manager) {
            return false;
        }

        auto dataHolder = &MCM::DataHolder::GetSingleton();
        if (!dataHolder) {
            return false;
        }

        std::string name = static_cast<std::string>(_name);
        Hotkey* hotkey = new Hotkey;
        hotkey->mKeyCode = _hotkey;
        hotkey->mModifier[0] = _modifier1;
        hotkey->mModifier[1] = _modifier2;
        hotkey->mModifier[2] = _modifier3;

        Option* option = new Option;
        option->mSound = _sound;
        option->mToggleEquip = _toggle;
        option->mReEquip = _reequip;
        option->mBeast = _beast;

        Widget* widget = new Widget;
        widget->mWidget = dataHolder->list.mWidgetList[_widget].second;
        widget->mHpos = _hpos;
        widget->mVpos = _vpos;
        widget->mDisplayWidget = _dwidget;
        widget->mDisplayName = _dname;
        widget->mDisplayHotkey = _dhotkey;

        Equipment* equipment = new Equipment;
        auto left = equipment->mLeft;
        auto lAction = static_cast<MCM::eAction>(_left);
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
        left.form = std::get<1>(dataHolder->list.mWeaponList[_left]);
        auto xList = std::get<2>(dataHolder->list.mWeaponList[_left]);
        bool bEnch = xList && Extra::IsEnchanted(xList);
        bool bTemp = xList && Extra::IsTempered(xList);
        left.hasExtra = std::make_pair(bEnch, bTemp);
        left.extraData = std::make_pair(Extra::GetEnchantment(xList), Extra::GetHealth(xList));
        left.numEnch = Extra::GetNumEnchantment(xList);
        left.xList = xList;

        auto right = equipment->mRight;
        auto rAction = static_cast<MCM::eAction>(_right);
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
        right.form = std::get<1>(dataHolder->list.mWeaponList[_right]);
        xList = nullptr;
        xList = std::get<2>(dataHolder->list.mWeaponList[_right]);
        bEnch = xList && Extra::IsEnchanted(xList);
        bTemp = xList && Extra::IsTempered(xList);
        right.hasExtra = std::make_pair(bEnch, bTemp);
        right.extraData = std::make_pair(Extra::GetEnchantment(xList), Extra::GetHealth(xList));
        right.numEnch = Extra::GetNumEnchantment(xList);
        right.xList = xList;

        auto shout = equipment->mShout;
        shout.option = _shout;
        shout.form = dataHolder->list.mShoutList[_shout].second;

        Equipment::Items items = equipment->mItems;
        items.numItems = _numitems;
        for (const auto& elem : _items) {
            auto form = std::get<1>(dataHolder->list.mItemsList[elem]);
            items.form.push_back(form);

            xList = nullptr;
            xList = std::get<2>(dataHolder->list.mItemsList[elem]);
            bEnch = xList && Extra::IsEnchanted(xList);
            bTemp = xList && Extra::IsTempered(xList);
            items.hasExtra.push_back(std::make_pair(bEnch, bTemp));
            items.extraData.push_back(std::make_pair(Extra::GetEnchantment(xList), Extra::GetHealth(xList)));
            items.numEnch.push_back(Extra::GetNumEnchantment(xList));
            items.xList.push_back(xList);
        }

        manager->NewEquipset(name, hotkey, option, widget, equipment);

        return true;
    }

    bool UIHS_NewCycleEquipset(RE::StaticFunctionTag*, RE::BSFixedString _name, int32_t _hotkey, bool _modifier1,
                          bool _modifier2, bool _modifier3, bool _persist, float _expire, float _reset, bool _beast,
                          int32_t _hpos, int32_t _vpos, bool _dwidget, bool _dname, bool _dhotkey,
                          int32_t _numitems, std::vector<int32_t> _items)
    {
        auto manager = &UIHS::EquipsetManager::GetSingleton();
        if (!manager) {
            return false;
        }

        auto dataHolder = &MCM::DataHolder::GetSingleton();
        if (!dataHolder) {
            return false;
        }

        std::string name = static_cast<std::string>(_name);
        Hotkey* hotkey = new Hotkey;
        hotkey->mKeyCode = _hotkey;
        hotkey->mModifier[0] = _modifier1;
        hotkey->mModifier[1] = _modifier2;
        hotkey->mModifier[2] = _modifier3;

        CycleOption* option = new CycleOption;
        option->mPersist = _persist;
        option->mExpire = _expire;
        option->mReset = _reset;
        option->mBeast = _beast;

        Widget* widget = new Widget;
        widget->mHpos = _hpos;
        widget->mVpos = _vpos;
        widget->mDisplayWidget = _dwidget;
        widget->mDisplayName = _dname;
        widget->mDisplayHotkey = _dhotkey;

        std::vector<std::string> items;
        for (int i = 0; i < _numitems; i++) {
            items.push_back(dataHolder->list.mCycleItemsList[_items[i]]);
        }

        manager->NewCycleEquipset(name, hotkey, option, widget, items, 0);

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

    std::vector<RE::BSFixedString> UIHS_GetEquipsetData(RE::StaticFunctionTag*, RE::BSFixedString _name)
    {
        std::vector<RE::BSFixedString> result;

        auto manager = &UIHS::EquipsetManager::GetSingleton();
        if (!manager) {
            return result;
        }

        return manager->GetEquipsetData(_name);
    }

    bool RegisterFuncs(RE::BSScript::IVirtualMachine* vm)
    {
        vm->RegisterFunction("Exec", "_HotkeysSystem_MCM", Papyrus::Exec);
        vm->RegisterFunction("Exec2", "_HotkeysSystem_MCM", Papyrus::Exec2);
        vm->RegisterFunction("UIHS_Init", "_HotkeysSystem_MCM", Papyrus::UIHS_Init);
        vm->RegisterFunction("UIHS_Clear", "_HotkeysSystem_MCM", Papyrus::UIHS_Clear);
        vm->RegisterFunction("UIHS_GetNamePrefix", "_HotkeysSystem_MCM", Papyrus::UIHS_GetNamePrefix);
        vm->RegisterFunction("UIHS_GetStringFromKeycode", "_HotkeysSystem_MCM", Papyrus::UIHS_GetStringFromKeycode);
        vm->RegisterFunction("UIHS_GetList", "_HotkeysSystem_MCM", Papyrus::UIHS_GetList);
        vm->RegisterFunction("UIHS_GetStringFromList", "_HotkeysSystem_MCM", Papyrus::UIHS_GetStringFromList);
        vm->RegisterFunction("UIHS_NewEquipset", "_HotkeysSystem_MCM", Papyrus::UIHS_NewEquipset);
        vm->RegisterFunction("UIHS_NewCycleEquipset", "_HotkeysSystem_MCM", Papyrus::UIHS_NewCycleEquipset);
        vm->RegisterFunction("UIHS_GetKeyConflict", "_HotkeysSystem_MCM", Papyrus::UIHS_GetKeyConflict);
        vm->RegisterFunction("UIHS_IsNameConflict", "_HotkeysSystem_MCM", Papyrus::UIHS_IsNameConflict);
        vm->RegisterFunction("UIHS_GetEquipsetData", "_HotkeysSystem_MCM", Papyrus::UIHS_GetEquipsetData);

        return true;
    }
}