#include "EquipsetManager.h"
#include "ExtraData.h"

using namespace UIHS;

EquipsetManager& EquipsetManager::GetSingleton() noexcept {
    static EquipsetManager instance;
    return instance;
}

void EquipsetManager::NewEquipset(std::string _name, Hotkey* _hotkey, Option* _option, Widget* _widget, Equipment* _equipment)
{
    mEquipset.push_back(new Equipset(_name, _hotkey, _option, _widget, _equipment));
}

void EquipsetManager::NewCycleEquipset(std::string _name, Hotkey* _hotkey, CycleOption* _option, Widget* _widget, std::vector<std::string> _cycleItems, uint32_t _cycleIndex)
{
    mCycleEquipset.push_back(new CycleEquipset(_name, _hotkey, _option, _widget, _cycleItems, _cycleIndex));
}

std::vector<std::string> EquipsetManager::GetEquipsetList()
{
    std::vector<std::string> result;
    for (const auto& elem : mEquipset) {
        result.push_back(elem->mName);
    }

    return result;
}

std::vector<std::string> EquipsetManager::GetAllEquipsetList() {
    std::vector<std::string> result;
    for (const auto& elem : mEquipset) {
        result.push_back(elem->mName);
    }

    for (const auto& elem : mCycleEquipset) {
        result.push_back(elem->mName);
    }

    return result;
}

uint32_t EquipsetManager::GetIndexFromList(std::string _name, MCM::eListType _type)
{
    uint32_t result = 0;

    auto dataHolder = &MCM::DataHolder::GetSingleton();
    if (!dataHolder) {
        return result;
    }

    switch (_type) {
        case MCM::eListType::Widget:
            for (int i = 0; i < dataHolder->list.mWidgetList.size(); i++) {
                if (_name == dataHolder->list.mWidgetList[i].second) {
                    result = i;
                }
            }
            break;

        case MCM::eListType::Shout:
            for (int i = 0; i < dataHolder->list.mShoutList.size(); i++) {
                if (_name == dataHolder->list.mShoutList[i].first) {
                    result = i;
                }
            }
            break;

        case MCM::eListType::CycleItems:
            for (int i = 0; i < dataHolder->list.mCycleItemsList.size(); i++) {
                if (_name == dataHolder->list.mCycleItemsList[i]) {
                    result = i;
                }
            }
            break;
    }

    return result;
}


uint32_t EquipsetManager::GetIndexFromList(RE::TESForm* _form, RE::ExtraDataList* _xList, MCM::eListType _type)
{
    uint32_t result = 0;

    auto dataHolder = &MCM::DataHolder::GetSingleton();
    if (!dataHolder) {
        return result;
    }

    if (_type == MCM::eListType::Weapon) {
        for (int i = 0; i < dataHolder->list.mWeaponList.size(); i++) {
            RE::FormID ID = 0, compareID = 0;
            RE::TESForm* compareForm = std::get<1>(dataHolder->list.mWeaponList[i]);
            if (_form) {
                ID = _form->GetFormID();
            }
            if (compareForm) {
                compareID = compareForm->GetFormID();
            }
            RE::ExtraDataList* comparexList = std::get<2>(dataHolder->list.mWeaponList[i]);
            if (ID == compareID && _xList == comparexList) {
                result = i;
            }
        }
    }
    else if (_type == MCM::eListType::Items) {
        for (int i = 0; i < dataHolder->list.mItemsList.size(); i++) {
            RE::FormID ID = 0, compareID = 0;
            RE::TESForm* compareForm = std::get<1>(dataHolder->list.mItemsList[i]);
            if (_form) {
                ID = _form->GetFormID();
            }
            if (compareForm) {
                compareID = compareForm->GetFormID();
            }
            RE::ExtraDataList* comparexList = std::get<2>(dataHolder->list.mItemsList[i]);
            if (ID == compareID && _xList == comparexList) {
                result = i;
            }
        }
    }

    return result;
}

std::vector<RE::BSFixedString> EquipsetManager::GetEquipsetData(RE::BSFixedString _name)
{
    std::vector<std::string> result;
    auto name = static_cast<std::string>(_name);
    for (const auto& elem : mEquipset) {
        if (elem->mName == name) {
            result.push_back(std::to_string(false));
            result.push_back(elem->mName);
            result.push_back(std::to_string(elem->mHotkey->mKeyCode));
            result.push_back(std::to_string(elem->mHotkey->mModifier[0]));
            result.push_back(std::to_string(elem->mHotkey->mModifier[1]));
            result.push_back(std::to_string(elem->mHotkey->mModifier[2]));
            result.push_back(std::to_string(elem->mOption->mSound));
            result.push_back(std::to_string(elem->mOption->mToggleEquip));
            result.push_back(std::to_string(elem->mOption->mReEquip));
            result.push_back(std::to_string(elem->mOption->mBeast));

            uint32_t widgetIndex;
            widgetIndex = GetIndexFromList(elem->mWidget->mWidget, MCM::eListType::Widget);
            result.push_back(std::to_string(widgetIndex));
            result.push_back(std::to_string(elem->mWidget->mHpos));
            result.push_back(std::to_string(elem->mWidget->mVpos));
            result.push_back(std::to_string(elem->mWidget->mDisplayWidget));
            result.push_back(std::to_string(elem->mWidget->mDisplayName));
            result.push_back(std::to_string(elem->mWidget->mDisplayHotkey));
            
            uint32_t leftIndex;
            auto left = elem->mEquipment->mLeft;
            MCM::eAction lAction = static_cast<MCM::eAction>(left.option);
            switch (lAction) {
                case MCM::eAction::Nothing:
                    result.push_back(std::to_string(static_cast<uint32_t>(MCM::eAction::Nothing)));
                    break;

                case MCM::eAction::Unequip:
                    result.push_back(std::to_string(static_cast<uint32_t>(MCM::eAction::Unequip)));
                    break;

                default:
                    leftIndex = GetIndexFromList(left.form, left.xList, MCM::eListType::Weapon);
                    result.push_back(std::to_string(leftIndex));
                    break;
            }

            uint32_t rightIndex;
            auto right = elem->mEquipment->mRight;
            MCM::eAction rAction = static_cast<MCM::eAction>(right.option);
            switch (rAction) {
                case MCM::eAction::Nothing:
                    result.push_back(std::to_string(static_cast<uint32_t>(MCM::eAction::Nothing)));
                    break;

                case MCM::eAction::Unequip:
                    result.push_back(std::to_string(static_cast<uint32_t>(MCM::eAction::Unequip)));
                    break;

                default:
                    rightIndex = GetIndexFromList(right.form, right.xList, MCM::eListType::Weapon);
                    result.push_back(std::to_string(rightIndex));
                    break;
            }

            uint32_t shoutIndex;
            auto shout = elem->mEquipment->mShout;
            MCM::eAction sAction = static_cast<MCM::eAction>(shout.option);
            switch (sAction) {
                case MCM::eAction::Nothing:
                    result.push_back(std::to_string(static_cast<uint32_t>(MCM::eAction::Nothing)));
                    break;

                case MCM::eAction::Unequip:
                    result.push_back(std::to_string(static_cast<uint32_t>(MCM::eAction::Unequip)));
                    break;

                default:
                    auto form = elem->mEquipment->mShout.form;
                    if (!form) {
                        shoutIndex = GetIndexFromList(form->GetName(), MCM::eListType::Shout);
                        result.push_back(std::to_string(shoutIndex));
                    }
                    break;
            }

            Equipment::Items items = elem->mEquipment->mItems;
            result.push_back(std::to_string(items.numItems));
            for (int i = 0; i < items.numItems; i++) {
                uint32_t itemIndex;
                itemIndex = GetIndexFromList(items.form[i], items.xList[i], MCM::eListType::Items);
                result.push_back(std::to_string(itemIndex));
            }
        }
    }

    if (result.size() != 0) {
        std::vector<RE::BSFixedString> strReturn;
        for (const auto& elem : result) {
            strReturn.push_back(static_cast<RE::BSFixedString>(elem));
        }
        return strReturn;
    }

    for (const auto& elem : mCycleEquipset) {
        if (elem->mName == name) {
            result.push_back(std::to_string(true));
            result.push_back(elem->mName);
            result.push_back(std::to_string(elem->mHotkey->mKeyCode));
            result.push_back(std::to_string(elem->mHotkey->mModifier[0]));
            result.push_back(std::to_string(elem->mHotkey->mModifier[1]));
            result.push_back(std::to_string(elem->mHotkey->mModifier[2]));
            result.push_back(std::to_string(elem->mOption->mPersist));
            result.push_back(std::to_string(elem->mOption->mExpire));
            result.push_back(std::to_string(elem->mOption->mReset));
            result.push_back(std::to_string(elem->mOption->mBeast));
            result.push_back(std::to_string(elem->mWidget->mHpos));
            result.push_back(std::to_string(elem->mWidget->mVpos));
            result.push_back(std::to_string(elem->mWidget->mDisplayWidget));
            result.push_back(std::to_string(elem->mWidget->mDisplayName));
            result.push_back(std::to_string(elem->mWidget->mDisplayHotkey));
            result.push_back(std::to_string(elem->mCycleItems.size()));

            for (int i = 0; i < elem->mCycleItems.size(); i++) {
                uint32_t itemIndex;
                itemIndex = GetIndexFromList(elem->mCycleItems[i], MCM::eListType::CycleItems);
                result.push_back(std::to_string(itemIndex));
            }

        }
    }

    std::vector<RE::BSFixedString> strReturn;
    for (const auto& elem : result) {
        strReturn.push_back(static_cast<RE::BSFixedString>(elem));
    }
    return strReturn;
}

void EquipsetManager::Display()
{
    auto equipsetSize = mEquipset.size();

    log::debug("equipsetSize: {}", mEquipset.size());
    for (auto& equipset : mEquipset) {
        log::debug("Equipset Name: {}", equipset->mName);

        auto hotkey = equipset->mHotkey;
        log::debug("KeyCode: {}", hotkey->mKeyCode);
        log::debug("Modifier Key 1: {}", hotkey->mModifier[0]);
        log::debug("Modifier Key 2: {}", hotkey->mModifier[1]);
        log::debug("Modifier Key 3: {}", hotkey->mModifier[2]);

        auto option = equipset->mOption;
        log::debug("Equip Sound: {}", option->mSound);
        log::debug("Toggle Equip/Unequip: {}", option->mToggleEquip);
        log::debug("Re Equip: {}", option->mReEquip);
        log::debug("Beast Hotkey: {}", option->mBeast);

        auto widget = equipset->mWidget;
        log::debug("Widget Type: {}", widget->mWidget);
        log::debug("H Pos: {}", widget->mHpos);
        log::debug("V Pos: {}", widget->mVpos);
        log::debug("Display Widget: {}", widget->mDisplayWidget);
        log::debug("Display Name: {}", widget->mDisplayName);
        log::debug("Display Hotkey: {}", widget->mDisplayHotkey);

        auto equipment = equipset->mEquipment;
        auto left = equipment->mLeft;
        log::debug("Lefthand Option: {}", left.option);
        if (left.option == static_cast<int32_t>(MCM::eAction::Equip)) {
            auto leftForm = left.form;
            bool IsEnchanted = left.hasExtra.first;
            bool IsTempered = left.hasExtra.second;
            auto name = Extra::HasDisplayName(left.xList) ? Extra::GetDisplayName(left.xList) : leftForm->GetName();
            if (IsEnchanted && IsTempered) {
                log::debug("[Lefthand] {}({:X}) - {}: {} - {}", name, leftForm->formID, left.numEnch, left.extraData.first->GetName(), left.extraData.second);
            } else if (!IsEnchanted && IsTempered) {
                log::debug("[Lefthand] {}({:X}) - None - {}", name, leftForm->formID, left.extraData.second);
            } else if (IsEnchanted && !IsTempered) {
                log::debug("[Lefthand] {}({:X}) - {}: {} - None", name, leftForm->formID, left.numEnch, left.extraData.first->GetName());
            } else {
                log::debug("[Lefthand] {}({:X}) - None - None", name, leftForm->formID);
            }
        }

        auto right = equipment->mRight;
        log::debug("Righthand Option: {}", right.option);
        if (right.option == static_cast<int32_t>(MCM::eAction::Equip)) {
            auto rightForm = right.form;
            bool IsEnchanted = right.hasExtra.first;
            bool IsTempered = right.hasExtra.second;
            auto name = Extra::HasDisplayName(right.xList) ? Extra::GetDisplayName(right.xList) : rightForm->GetName();
            if (IsEnchanted && IsTempered) {
                log::debug("[Righthand] {}({:X}) - {}: {} - {}", name, rightForm->formID, right.numEnch, right.extraData.first->GetName(), right.extraData.second);
            } else if (!IsEnchanted && IsTempered) {
                log::debug("[Righthand] {}({:X}) - None - {}", name, rightForm->formID, right.extraData.second);
            } else if (IsEnchanted && !IsTempered) {
                log::debug("[Righthand] {}({:X}) - {}: {} - None", name, rightForm->formID, right.numEnch, right.extraData.first->GetName());
            } else {
                log::debug("[Righthand] {}({:X}) - None - None", name, rightForm->formID);
            }
        }

        auto shout = equipment->mShout;
        log::debug("Shout Option: {}", shout.option);
        if (shout.option == static_cast<int32_t>(MCM::eAction::Equip)) {
            log::debug("Shout: {}({:X})", shout.form->GetName(), shout.form->formID);
        }

        log::debug("Contained Items");

        auto items = equipment->mItems;
        uint32_t size = items.numItems;
        log::debug("Items size: {}", size);
        for (int i = 0; i < size; i++) {
            auto itemsForm = items.form[i];
            bool IsEnchanted = items.hasExtra[i].first;
            bool IsTempered = items.hasExtra[i].second;
            auto name = Extra::HasDisplayName(items.xList[i]) ? Extra::GetDisplayName(items.xList[i]) : itemsForm->GetName();
            if (IsEnchanted && IsTempered) {
                log::debug("[Items] {}({:X}) - {}: {} - {}", name, itemsForm->formID, items.numEnch[i], items.extraData[i].first->GetName(), items.extraData[i].second);
            } else if (!IsEnchanted && IsTempered) {
                log::debug("[Items] {}({:X}) - None - {}", name, itemsForm->formID, items.extraData[i].second);
            } else if (IsEnchanted && !IsTempered) {
                log::debug("[Items] {}({:X}) - {}: {} - None", name, itemsForm->formID, items.numEnch[i], items.extraData[i].first->GetName());
            } else {
                log::debug("[Items] {}({:X}) - None - None", name, itemsForm->formID);
            }
        }
    }
}

void EquipsetManager::DisplayCycle()
{
    auto equipsetSize = mCycleEquipset.size();

    log::debug("equipsetSize: {}", mCycleEquipset.size());
    for (auto& equipset : mCycleEquipset) {
        log::debug("Equipset Name: {}", equipset->mName);

        auto hotkey = equipset->mHotkey;
        log::debug("KeyCode: {}", hotkey->mKeyCode);
        log::debug("Modifier Key 1: {}", hotkey->mModifier[0]);
        log::debug("Modifier Key 2: {}", hotkey->mModifier[1]);
        log::debug("Modifier Key 3: {}", hotkey->mModifier[2]);

        auto option = equipset->mOption;
        log::debug("Cycle Persist: {}", option->mPersist);
        log::debug("Cycle Expire: {}sec", option->mExpire);
        log::debug("Cycle Reset: {}sec", option->mReset);
        log::debug("Beast Hotkey: {}", option->mBeast);

        auto widget = equipset->mWidget;
        log::debug("H Pos: {}", widget->mHpos);
        log::debug("V Pos: {}", widget->mVpos);
        log::debug("Display Widget: {}", widget->mDisplayWidget);
        log::debug("Display Name: {}", widget->mDisplayName);
        log::debug("Display Hotkey: {}", widget->mDisplayHotkey);

        log::debug("Contained Equipsets");

        uint32_t size = equipset->mCycleItems.size();
        log::debug("Equipsets size: {}", size);
        for (int i = 0; i < size; i++) {
            log::debug("Equipset #{}: {}", i, equipset->mCycleItems[i]);
        }
    }
}

Equipset* EquipsetManager::SearchEquipsetByName(std::string _name)
{
    for (auto& elem : mEquipset) {
        if (elem->mName == _name) {
            return elem;
        }
    }

    return nullptr;
}

const RE::BSFixedString EquipsetManager::GetNamePrefix()
{
    RE::BSFixedString result;

    int size = mEquipset.size() + mCycleEquipset.size();
    if (size == 0) {
        result = "Equipset - 1";
        return result;
    }

    int prefix = size + 1;
    std::string name = "Equipset - ";
    while (true) {
        bool isConflict = false;
        std::string strPrefix = std::to_string(prefix);
        for (const auto& elem : mEquipset) {
            if (elem->mName == name + strPrefix) {
                isConflict = true;
            }
        }

        for (const auto& elem : mCycleEquipset) {
            if (elem->mName == name + strPrefix) {
                isConflict = true;
            }
        }
        
        if (isConflict) {
            ++prefix;
        } else {
            result = static_cast<RE::BSFixedString>(name + strPrefix);
            return result;
        }
    }   

    return result;
}

RE::BSFixedString EquipsetManager::GetKeyConflict(int32_t _key, std::vector<bool> _modifier, bool _beast)
{
    std::string result = "_NOTFOUND_";

    for (const auto& elem : mEquipset) {
        if (elem->mHotkey->mKeyCode != _key) {
            continue;
        }
        if (elem->mHotkey->mModifier[0] != _modifier[0] ||
            elem->mHotkey->mModifier[1] != _modifier[1] ||
            elem->mHotkey->mModifier[2] != _modifier[2]) {
            
            continue;
        }
        if (elem->mOption->mBeast != _beast) {
            continue;
        }
        result = elem->mName;
        break;
    }
    for (const auto& elem : mCycleEquipset) {
        if (elem->mHotkey->mKeyCode != _key) {
            continue;
        }
        if (elem->mHotkey->mModifier[0] != _modifier[0] || elem->mHotkey->mModifier[1] != _modifier[1] ||
            elem->mHotkey->mModifier[2] != _modifier[2]) {
            continue;
        }
        if (elem->mOption->mBeast != _beast) {
            continue;
        }
        result = elem->mName;
        break;
    }


    return static_cast<RE::BSFixedString>(result);
}

bool EquipsetManager::IsNameConflict(RE::BSFixedString _name)
{
    std::string name = static_cast<std::string>(_name);

    for (const auto& elem : mEquipset) {
        if (elem->mName == name) {
            return true;
        }
    }
    for (const auto& elem : mCycleEquipset) {
        if (elem->mName == name) {
            return true;
        }
    }

    return false;
}