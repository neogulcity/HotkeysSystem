#include "EquipsetManager.h"
#include "ExtraData.h"
#include "Utility.h"
#include "WidgetHandler.h"

using namespace UIHS;

EquipsetManager& EquipsetManager::GetSingleton() noexcept {
    static EquipsetManager instance;
    return instance;
}

void EquipsetManager::NewEquipset(uint32_t _order, std::string _name, Hotkey* _hotkey, Option* _option, Widget* _widget, Equipment* _equipment)
{
    mEquipset.push_back(new Equipset(_order, _name, _hotkey, _option, _widget, _equipment));
}

void EquipsetManager::NewCycleEquipset(uint32_t _order, std::string _name, Hotkey* _hotkey, CycleOption* _option, Widget* _widget, std::vector<std::string> _cycleItems, std::pair<uint32_t, int32_t> _cycleIndex)
{
    mCycleEquipset.push_back(new CycleEquipset(_order, _name, _hotkey, _option, _widget, _cycleItems, _cycleIndex));
}

void EquipsetManager::EditEquipset(std::string _name, std::vector<std::string> _data)
{
    auto holder = MCM::DataHolder::GetSingleton();
    if (!holder) {
        log::error("Unable to get DataHolder.");
        return;
    }

    for (const auto& elem : mEquipset) {
        if (elem->mName != _name) {
            continue;
        }

        std::string name = _data[0];

        Hotkey* hotkey = new Hotkey;
        hotkey->mKeyCode = std::stoi(_data[1]);
        hotkey->mModifier[0] = to_bool(_data[2]);
        hotkey->mModifier[1] = to_bool(_data[3]);
        hotkey->mModifier[2] = to_bool(_data[4]);

        Option* option = new Option;
        option->mSound = to_bool(_data[5]);
        option->mToggleEquip = to_bool(_data[6]);
        option->mReEquip = to_bool(_data[7]);
        option->mBeast = to_bool(_data[8]);

        Widget* widget = new Widget;
        widget->mWidget = holder->list->mWidgetList[std::stoi(_data[9])].second;
        widget->mHpos = std::stoi(_data[10]);
        widget->mVpos = std::stoi(_data[11]);
        widget->mDisplayWidget = to_bool(_data[12]);
        widget->mDisplayName = to_bool(_data[13]);
        widget->mDisplayHotkey = to_bool(_data[14]);

        Equipment* equipment = new Equipment;
        auto left = equipment->mLeft;
        auto lAction = std::stoi(_data[15]);
        switch (lAction) {
            case MCM::eAction::Nothing:
                left->option = MCM::eAction::Nothing;
                break;

            case MCM::eAction::Unequip:
                left->option = MCM::eAction::Unequip;
                break;

            default:
                left->option = MCM::eAction::Equip;
        }
        left->form = std::get<1>(holder->list->mWeaponList[std::stoi(_data[15])]);
        auto xList = std::get<2>(holder->list->mWeaponList[std::stoi(_data[15])]);
        bool bEnch = xList && Extra::IsEnchanted(xList);
        bool bTemp = xList && Extra::IsTempered(xList);
        left->hasExtra = std::make_pair(bEnch, bTemp);
        left->extraData = std::make_pair(Extra::GetEnchantment(xList), Extra::GetHealth(xList));
        left->numEnch = Extra::GetNumEnchantment(xList);
        left->xList = xList;
        if (left->option == MCM::eAction::Equip) {
            if (!left->form) {
                logger::error("[{}] Failed to Edit Lefthand data.", name);
                left->option = MCM::eAction::Nothing;
            }
            else if (left->hasExtra.first && !left->extraData.first) {
                logger::error("[{}] Failed to Edit Lefthand data.", name);
                left->option = MCM::eAction::Nothing;
            }
        }

        auto right = equipment->mRight;
        auto rAction = std::stoi(_data[16]);
        switch (rAction) {
            case MCM::eAction::Nothing:
                right->option = MCM::eAction::Nothing;
                break;

            case MCM::eAction::Unequip:
                right->option = MCM::eAction::Unequip;
                break;

            default:
                right->option = MCM::eAction::Equip;
        }
        right->form = std::get<1>(holder->list->mWeaponList[std::stoi(_data[16])]);
        xList = nullptr;
        xList = std::get<2>(holder->list->mWeaponList[std::stoi(_data[16])]);
        bEnch = xList && Extra::IsEnchanted(xList);
        bTemp = xList && Extra::IsTempered(xList);
        right->hasExtra = std::make_pair(bEnch, bTemp);
        right->extraData = std::make_pair(Extra::GetEnchantment(xList), Extra::GetHealth(xList));
        right->numEnch = Extra::GetNumEnchantment(xList);
        right->xList = xList;
        if (right->option == MCM::eAction::Equip) {
            if (!right->form) {
                logger::error("[{}] Failed to Edit Righthand data.", name);
                right->option = MCM::eAction::Nothing;
            }
            else if (right->hasExtra.first && !right->extraData.first) {
                logger::error("[{}] Failed to Edit Righthand data.", name);
                right->option = MCM::eAction::Nothing;
            }
        }

        auto shout = equipment->mShout;
        auto sAction = std::stoi(_data[17]);
        switch (sAction) {
            case MCM::eAction::Nothing:
                shout->option = MCM::eAction::Nothing;
                break;

            case MCM::eAction::Unequip:
                shout->option = MCM::eAction::Unequip;
                break;

            default:
                shout->option = MCM::eAction::Equip;
        }
        shout->form = holder->list->mShoutList[std::stoi(_data[17])].second;
        if (shout->option == MCM::eAction::Equip) {
            if (!shout->form) {
                logger::error("[{}] Failed to Edit Shout/Power data.", name);
                shout->option = MCM::eAction::Nothing;
            }
        }

        auto& items = equipment->mItems;
        auto itemSize = std::stoi(_data[18]);
        for (int i = 0; i < itemSize; i++) {
            Equipment::Items* item = new Equipment::Items;
            int index = i + 19;
            int elem = std::stoi(_data[index]);

            RE::TESForm* form = nullptr;
            if (elem < holder->list->mItemsList.size()) {
                form = std::get<1>(holder->list->mItemsList[elem]);
            }
            
            item->form = form;

            xList = nullptr;
            xList = std::get<2>(holder->list->mItemsList[elem]);
            bEnch = xList && Extra::IsEnchanted(xList);
            bTemp = xList && Extra::IsTempered(xList);
            item->hasExtra = std::make_pair(bEnch, bTemp);
            item->extraData = std::make_pair(Extra::GetEnchantment(xList), Extra::GetHealth(xList));
            item->numEnch = Extra::GetNumEnchantment(xList);
            item->xList = xList;

            if (!item->form) {
                logger::error("[{}] Failed to Edit items.", name);
                delete item;
            }
            else if (item->hasExtra.first && !item->extraData.first) {
                logger::error("[{}] Failed to Edit items.", name);
                delete item;
            }
            else {
                items.push_back(item);
            }
        }

        delete elem->mHotkey;
        delete elem->mOption;
        delete elem->mWidget;
        delete elem->mEquipment;

        elem->mName = name;
        elem->mHotkey = hotkey;
        elem->mOption = option;
        elem->mWidget = widget;
        elem->mEquipment = equipment;
    }

    for (const auto& elem : mCycleEquipset) {
        if (elem->mName != _name) {
            continue;
        }

        if (elem->mOption->mReset != 0.0f && elem->IsResetWorking()) {
            try {
                elem->SetResetClose(true);
                elem->mResetHandle.get();
            }
            catch (const std::future_error& error) {
                log::error("Caught a future_error: {}", error.what());
            }
        }
        if (elem->mOption->mExpire != 0.0f && elem->IsExpireWorking()) {
            try {
                elem->SetExpireClose(true);
                elem->mExpireHandle.get();
            }
            catch (const std::future_error& error) {
                log::error("Caught a future_error: {}", error.what());
            }
        }

        std::string name = _data[0];
        Hotkey* hotkey = new Hotkey;
        hotkey->mKeyCode = std::stoi(_data[1]);
        hotkey->mModifier[0] = to_bool(_data[2]);
        hotkey->mModifier[1] = to_bool(_data[3]);
        hotkey->mModifier[2] = to_bool(_data[4]);

        CycleOption* option = new CycleOption;
        option->mPersist = to_bool(_data[5]);
        option->mExpire = std::stof(_data[6]);
        option->mReset = std::stof(_data[7]);
        option->mBeast = to_bool(_data[8]);

        Widget* widget = new Widget;
        widget->mHpos = std::stoi(_data[9]);
        widget->mVpos = std::stoi(_data[10]);
        widget->mDisplayWidget = to_bool(_data[11]);
        widget->mDisplayName = to_bool(_data[12]);
        widget->mDisplayHotkey = to_bool(_data[13]);

        std::vector<std::string> items;
        int numItems = std::stoi(_data[14]);
        for (int i = 0; i < numItems; i++) {
            int index = i + 15;
            int elem = std::stoi(_data[index]);

            if (elem < holder->list->mCycleItemsList.size()) {
                items.push_back(holder->list->mCycleItemsList[elem]);
            }
        }

        delete elem->mHotkey;
        delete elem->mOption;
        delete elem->mWidget;

        elem->mName = name;
        elem->mHotkey = hotkey;
        elem->mOption = option;
        elem->mWidget = widget;
        elem->mCycleItems = items;
        elem->mCycleIndex = std::make_pair(0, -1);
        elem->SetExpireClose(true);
        elem->SetResetClose(true);
        elem->SetRemain(0.0f);
    }
}

void EquipsetManager::RemoveEquipset(std::string _name)
{
    std::vector<std::string> list = GetEquipsetList();

    for (int i = 0; i < list.size(); i++) {
        if (list[i] == _name) {
            delete mEquipset[i];
            mEquipset.erase(mEquipset.begin() + i);
        }
    }

    std::vector<std::string> cycleList = GetCycleEquipsetList();

    for (int i = 0; i < cycleList.size(); i++) {
        if (cycleList[i] == _name) {
            auto equipset = mCycleEquipset[i];
            if (equipset->mOption->mExpire != 0.0f && equipset->IsExpireWorking()) {
                try {
                    equipset->SetExpireClose(true);
                    equipset->mExpireHandle.get();
                }
                catch (const std::future_error& error) {
                    log::error("Caught a future_error: {}", error.what());
                }
            }
            if (equipset->mOption->mReset != 0.0f && equipset->IsResetWorking()) {
                try {
                    equipset->SetResetClose(true);
                    equipset->mResetHandle.get();
                }
                catch (const std::future_error& error) {
                    log::error("Caught a future_error: {}", error.what());
                }
            }
            delete equipset;
            mCycleEquipset.erase(mCycleEquipset.begin() + i);
        }
    }
}

std::vector<std::string> EquipsetManager::GetEquipsetList()
{
    std::vector<std::string> result;
    for (const auto& elem : mEquipset) {
        result.push_back(elem->mName);
    }

    return result;
}

std::vector<std::string> EquipsetManager::GetCycleEquipsetList()
{
    std::vector<std::string> result;
    for (const auto& elem : mCycleEquipset) {
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

    auto dataHolder = MCM::DataHolder::GetSingleton();
    if (!dataHolder) {
        return result;
    }

    switch (_type) {
        case MCM::eListType::Widget:
            for (int i = 0; i < dataHolder->list->mWidgetList.size(); i++) {
                if (_name == dataHolder->list->mWidgetList[i].second) {
                    result = i;
                }
            }
            break;

        case MCM::eListType::Shout:
            for (int i = 0; i < dataHolder->list->mShoutList.size(); i++) {
                if (_name == dataHolder->list->mShoutList[i].first) {
                    result = i;
                }
            }
            break;

        case MCM::eListType::CycleItems:
            for (int i = 0; i < dataHolder->list->mCycleItemsList.size(); i++) {
                if (_name == dataHolder->list->mCycleItemsList[i]) {
                    result = i;
                }
            }
            break;

        case MCM::eListType::Font:
            for (int i = 0; i < dataHolder->list->mFontList.size(); i++) {
                if (_name == dataHolder->list->mFontList[i]) {
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

    auto dataHolder = MCM::DataHolder::GetSingleton();
    if (!dataHolder) {
        return result;
    }

    if (_type == MCM::eListType::Weapon) {
        for (int i = 0; i < dataHolder->list->mWeaponList.size(); i++) {
            RE::FormID ID = 0, compareID = 0;
            RE::TESForm* compareForm = std::get<1>(dataHolder->list->mWeaponList[i]);
            if (_form) {
                ID = _form->GetFormID();
            }
            if (compareForm) {
                compareID = compareForm->GetFormID();
            }
            RE::ExtraDataList* comparexList = std::get<2>(dataHolder->list->mWeaponList[i]);
            if (ID == compareID && _xList == comparexList) {
                result = i;
            }
        }
    }
    else if (_type == MCM::eListType::Items) {
        for (int i = 0; i < dataHolder->list->mItemsList.size(); i++) {
            RE::FormID ID = 0, compareID = 0;
            RE::TESForm* compareForm = std::get<1>(dataHolder->list->mItemsList[i]);
            if (_form) {
                ID = _form->GetFormID();
            }
            if (compareForm) {
                compareID = compareForm->GetFormID();
            }
            RE::ExtraDataList* comparexList = std::get<2>(dataHolder->list->mItemsList[i]);
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
            MCM::eAction lAction = static_cast<MCM::eAction>(left->option);
            switch (lAction) {
                case MCM::eAction::Nothing:
                    result.push_back(std::to_string(static_cast<uint32_t>(MCM::eAction::Nothing)));
                    break;

                case MCM::eAction::Unequip:
                    result.push_back(std::to_string(static_cast<uint32_t>(MCM::eAction::Unequip)));
                    break;

                default:
                    leftIndex = GetIndexFromList(left->form, left->xList, MCM::eListType::Weapon);
                    result.push_back(std::to_string(leftIndex));
                    break;
            }

            uint32_t rightIndex;
            auto right = elem->mEquipment->mRight;
            MCM::eAction rAction = static_cast<MCM::eAction>(right->option);
            switch (rAction) {
                case MCM::eAction::Nothing:
                    result.push_back(std::to_string(static_cast<uint32_t>(MCM::eAction::Nothing)));
                    break;

                case MCM::eAction::Unequip:
                    result.push_back(std::to_string(static_cast<uint32_t>(MCM::eAction::Unequip)));
                    break;

                default:
                    rightIndex = GetIndexFromList(right->form, right->xList, MCM::eListType::Weapon);
                    result.push_back(std::to_string(rightIndex));
                    break;
            }

            uint32_t shoutIndex;
            auto shout = elem->mEquipment->mShout;
            MCM::eAction sAction = static_cast<MCM::eAction>(shout->option);
            switch (sAction) {
                case MCM::eAction::Nothing:
                    result.push_back(std::to_string(static_cast<uint32_t>(MCM::eAction::Nothing)));
                    break;

                case MCM::eAction::Unequip:
                    result.push_back(std::to_string(static_cast<uint32_t>(MCM::eAction::Unequip)));
                    break;

                default:
                    auto form = shout->form;
                    if (form) {
                        shoutIndex = GetIndexFromList(form->GetName(), MCM::eListType::Shout);
                        result.push_back(std::to_string(shoutIndex));
                    }
                    break;
            }

            auto& items = elem->mEquipment->mItems;
            result.push_back(std::to_string(items.size()));
            for (const auto item : items) {
                uint32_t itemIndex;
                itemIndex = GetIndexFromList(item->form, item->xList, MCM::eListType::Items);
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

Equipset* EquipsetManager::SearchEquipsetByName(std::string _name)
{
    for (auto& elem : mEquipset) {
        if (elem->mName == _name) {
            return elem;
        }
    }

    return nullptr;
}

CycleEquipset* EquipsetManager::SearchCycleEquipsetByName(std::string _name) {
    for (auto& elem : mCycleEquipset) {
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

    if (_key == -1) {
        return result;
    }

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

bool EquipsetManager::IsCycleEquipset(RE::BSFixedString _name)
{
    std::string name = static_cast<std::string>(_name);

    for (const auto& elem : mCycleEquipset) {
        if (elem->mName == name) {
            return true;
        }
    }

    return false;
}

std::vector<int32_t> EquipsetManager::GetKeycodeList()
{
    std::vector<int32_t> result;

    for (const auto& elem : mEquipset) {
        result.push_back(elem->mHotkey->mKeyCode);
    }
    for (const auto& elem : mCycleEquipset) {
        result.push_back(elem->mHotkey->mKeyCode);
    }

    return result;
}

uint32_t EquipsetManager::GetAllEquipsetSize() 
{
    return mEquipset.size() + mCycleEquipset.size();
}

bool CreateDesc(std::pair<uint32_t, std::string> _data1, std::pair<uint32_t, std::string> _data2)
{
    return _data2.first < _data1.first;
}

bool NameAsc(std::pair<uint32_t, std::string> _data1, std::pair<uint32_t, std::string> _data2)
{
    return _data1.second < _data2.second;
}

bool NameDesc(std::pair<uint32_t, std::string> _data1, std::pair<uint32_t, std::string> _data2)
{
    return _data2.second < _data1.second;
}

std::vector<std::string> EquipsetManager::GetSortedEquipsetList()
{
    std::vector<std::string> result;

    auto holder = MCM::DataHolder::GetSingleton();
    if (!holder) {
        return result;
    }

    std::vector<std::pair<uint32_t, std::string>> pairList;

    for (const auto& elem : mEquipset) {
        pairList.push_back(std::make_pair(elem->mOrder, elem->mName));
    }

    switch (holder->setting->mSort) {
        case MCM::eSortType::CreateAsc:
            std::sort(pairList.begin(), pairList.end());
            break;

        case MCM::eSortType::CreateDesc:
            std::sort(pairList.begin(), pairList.end(), CreateDesc);
            break;

        case MCM::eSortType::NameAsc:
            std::sort(pairList.begin(), pairList.end(), NameAsc);
            break;

        case MCM::eSortType::NameDesc:
            std::sort(pairList.begin(), pairList.end(), NameDesc);
            break;
    }

    for (const auto& elem : pairList) {
        result.push_back(elem.second);
    }

    return result;
}

std::vector<std::string> EquipsetManager::GetAllSortedEquipsetList()
{
    std::vector<std::string> result;

    auto holder = MCM::DataHolder::GetSingleton();
    if (!holder) {
        return result;
    }

    std::vector<std::pair<uint32_t, std::string>> pairList;

    for (const auto& elem : mEquipset) {
        pairList.push_back(std::make_pair(elem->mOrder, elem->mName));
    }

    for (const auto& elem : mCycleEquipset) {
        pairList.push_back(std::make_pair(elem->mOrder, elem->mName));
    }

    switch (holder->setting->mSort) {
        case MCM::eSortType::CreateAsc:
            std::sort(pairList.begin(), pairList.end());
            break;

        case MCM::eSortType::CreateDesc:
            std::sort(pairList.begin(), pairList.end(), CreateDesc);
            break;

        case MCM::eSortType::NameAsc:
            std::sort(pairList.begin(), pairList.end(), NameAsc);
            break;

        case MCM::eSortType::NameDesc:
            std::sort(pairList.begin(), pairList.end(), NameDesc);
            break;
    }

    for (const auto& elem : pairList) {
        result.push_back(elem.second);
    }

    return result;
}

bool IsPlayerBeast()
{
	auto menu = RE::MenuControls::GetSingleton();
    if (!menu) {
        return false;
    }
	return menu->InBeastForm();
}

void EquipsetManager::Exec(int32_t _code, bool _modifier1, bool _modifier2, bool _modifier3)
{
    auto playerref = RE::PlayerCharacter::GetSingleton();
    const auto xContainer = playerref ? playerref->extraList.GetByType<RE::ExtraContainerChanges>() : nullptr;
	const auto invChanges = xContainer ? xContainer->changes : nullptr;

    for (const auto& elem : mEquipset) {
        auto hotkey = elem->mHotkey;
        if (hotkey->mKeyCode == _code &&
            hotkey->mModifier[0] == _modifier1 &&
            hotkey->mModifier[1] == _modifier2 &&
            hotkey->mModifier[2] == _modifier3 &&
            elem->mOption->mBeast == IsPlayerBeast()) {
            elem->Equip();
        }
    }

    for (const auto& elem : mCycleEquipset) {
        auto hotkey = elem->mHotkey;
        if (hotkey->mKeyCode == _code &&
            hotkey->mModifier[0] == _modifier1 &&
            hotkey->mModifier[1] == _modifier2 &&
            hotkey->mModifier[2] == _modifier3 &&
            elem->mOption->mBeast == IsPlayerBeast()) {
            if (elem->mOption->mReset == 0.0f) {
                elem->Equip();
            }
            else {
                if (!elem->IsResetWorking()) {
                    elem->SetResetTimer();
                }
            }
        }
    }
}

void EquipsetManager::CalculateKeydown(int32_t _code, bool _modifier1, bool _modifier2, bool _modifier3, float _time)
{
    for (const auto& elem : mCycleEquipset) {
        auto hotkey = elem->mHotkey;
        if (hotkey->mKeyCode == _code &&
            hotkey->mModifier[0] == _modifier1 &&
            hotkey->mModifier[1] == _modifier2 &&
            hotkey->mModifier[2] == _modifier3 &&
            elem->mOption->mBeast == IsPlayerBeast()) {
            if (elem->mOption->mReset != 0.0f && _time < elem->mOption->mReset) {
                try {
                    elem->Equip();
                    elem->SetResetClose(true);
                    elem->mResetHandle.get();
                }
                catch (const std::future_error& error) {
                    log::error("Caught a future_error: {}", error.what());
                }
            }
        }
    }
}

void EquipsetManager::ClearWidget()
{
    auto widgetHandler = WidgetHandler::GetSingleton();
    if (!widgetHandler) {
        return;
    }

    {
        auto list = GetEquipsetList();
        for (const auto& elem : list) {
            auto equipset = SearchEquipsetByName(elem);
            if (!equipset) {
                continue;
            }

            std::vector<int32_t> clear;
            equipset->mWidget->mWidgetID = clear;
            equipset->mWidget->mNameID = clear;
            equipset->mWidget->mHotkeyID = -1;
            equipset->mWidget->mBackgroundID = -1;
        }
    }
    {
        auto list = GetCycleEquipsetList();
        for (const auto& elem : list) {
            auto equipset = SearchCycleEquipsetByName(elem);
            if (!equipset) {
                continue;
            }

            std::vector<int32_t> clear;
            equipset->mWidget->mWidgetID = clear;
            equipset->mWidget->mNameID = clear;
            equipset->mWidget->mHotkeyID = -1;
            equipset->mWidget->mBackgroundID = -1;
        }
    }

    widgetHandler->CloseWidgetMenu();
}

void EquipsetManager::InitWidget()
{
    auto widgetHandler = WidgetHandler::GetSingleton();
    if (!widgetHandler) {
        return;
    }

    auto dataHolder = MCM::DataHolder::GetSingleton();
    if (!dataHolder) {
        return;
    }

    int32_t id = 0;

    widgetHandler->OpenWidgetMenu();

    {
        auto list = GetEquipsetList();
        for (const auto& elem : list) {
            auto equipset = SearchEquipsetByName(elem);
            if (!equipset) {
                continue;
            }

            auto widget = equipset->mWidget;
            if (widget->mDisplayWidget) {
                widgetHandler->LoadWidget("UIHS/Background.dds", widget->mHpos, widget->mVpos, 70 * dataHolder->widget->mSize / 100, 70 * dataHolder->widget->mSize / 100, dataHolder->widget->mAlpha);
                widget->mBackgroundID = id;
                ++id;
            }
            if (widget->mDisplayWidget) {
                widgetHandler->LoadWidget(widget->mWidget, widget->mHpos, widget->mVpos, 50 * dataHolder->widget->mSize / 100, 50 * dataHolder->widget->mSize / 100, 100);
                widget->mWidgetID.push_back(id);
                ++id;
            }
            if (widget->mDisplayName) {
                widgetHandler->LoadText(equipset->mName, dataHolder->widget->mFont, 15 * dataHolder->widget->mFontSize / 100, widget->mHpos, widget->mVpos + (50 * dataHolder->widget->mSize / 100));
                widgetHandler->SetAlpha(id, 100);
                widget->mNameID.push_back(id);
                ++id;
            }
            if (widget->mDisplayHotkey) {
                std::string name = "";
                name = equipset->mHotkey->mModifier[0] ? name + GetStringFromKeycode(dataHolder->setting->mModifier[0]) + " + " : name;
                name = equipset->mHotkey->mModifier[1] ? name + GetStringFromKeycode(dataHolder->setting->mModifier[1]) + " + " : name;
                name = equipset->mHotkey->mModifier[2] ? name + GetStringFromKeycode(dataHolder->setting->mModifier[2]) + " + " : name;
                name += GetStringFromKeycode(equipset->mHotkey->mKeyCode);
                widgetHandler->LoadText(name, dataHolder->widget->mFont, 15 * dataHolder->widget->mFontSize / 100, widget->mHpos, widget->mVpos - (50 * dataHolder->widget->mSize / 100));
                widgetHandler->SetAlpha(id, 100);
                widget->mHotkeyID = id;
                ++id;
            }
        }
    }

    {
        auto list = GetCycleEquipsetList();
        for (const auto& elem : list) {
            auto cycleEquipset = SearchCycleEquipsetByName(elem);
            if (!cycleEquipset) {
                continue;
            }

            auto cycleWidget = cycleEquipset->mWidget;
            if (cycleWidget->mDisplayWidget) {
                widgetHandler->LoadWidget("UIHS/Background.dds", cycleWidget->mHpos, cycleWidget->mVpos, 70 * dataHolder->widget->mSize / 100, 70 * dataHolder->widget->mSize / 100, dataHolder->widget->mAlpha);
                cycleWidget->mBackgroundID = id;
                ++id;
            }
            if (cycleWidget->mDisplayWidget) {
                for (const auto& strItem : cycleEquipset->mCycleItems) {
                    auto equipset = SearchEquipsetByName(strItem);
                    if (!equipset) {
                        continue;
                    }

                    int alpha = 0;
                    if (cycleEquipset->mCycleIndex.first < cycleEquipset->mCycleItems.size()) {
                        alpha = strItem == cycleEquipset->mCycleItems[cycleEquipset->mCycleIndex.first] ? 100 : 0;
                    }
                    auto widget = equipset->mWidget;
                    widgetHandler->LoadWidget(widget->mWidget, cycleWidget->mHpos, cycleWidget->mVpos, 50 * dataHolder->widget->mSize / 100, 50 * dataHolder->widget->mSize / 100, alpha);
                    cycleWidget->mWidgetID.push_back(id);
                    ++id;
                }
            }
            if (cycleWidget->mDisplayName) {
                for (const auto& strItem : cycleEquipset->mCycleItems) {
                    auto equipset = SearchEquipsetByName(strItem);
                    if (!equipset) {
                        continue;
                    }

                    int alpha = 0;
                    if (cycleEquipset->mCycleIndex.first < cycleEquipset->mCycleItems.size()) {
                        alpha = strItem == cycleEquipset->mCycleItems[cycleEquipset->mCycleIndex.first] ? 100 : 0;
                    }
                    auto widget = equipset->mWidget;
                    widgetHandler->LoadText(equipset->mName, dataHolder->widget->mFont, 15 * dataHolder->widget->mFontSize / 100, cycleWidget->mHpos, cycleWidget->mVpos + (50 * dataHolder->widget->mSize / 100));
                    widgetHandler->SetAlpha(id, alpha);
                    cycleWidget->mNameID.push_back(id);
                    ++id;
                }
            }
            if (cycleWidget->mDisplayHotkey) {
                std::string name = "";
                name = cycleEquipset->mHotkey->mModifier[0] ? name + GetStringFromKeycode(dataHolder->setting->mModifier[0]) + " + " : name;
                name = cycleEquipset->mHotkey->mModifier[1] ? name + GetStringFromKeycode(dataHolder->setting->mModifier[1]) + " + " : name;
                name = cycleEquipset->mHotkey->mModifier[2] ? name + GetStringFromKeycode(dataHolder->setting->mModifier[2]) + " + " : name;
                name += GetStringFromKeycode(cycleEquipset->mHotkey->mKeyCode);
                widgetHandler->LoadText(name, dataHolder->widget->mFont, 15 * dataHolder->widget->mFontSize / 100, cycleWidget->mHpos, cycleWidget->mVpos - (50 * dataHolder->widget->mSize / 100));
                widgetHandler->SetAlpha(id, 100);
                cycleWidget->mHotkeyID = id;
                ++id;
            }
        }
    }

    MCM::eWidgetDisplay type = static_cast<MCM::eWidgetDisplay>(dataHolder->widget->mDisplay);
    if (type != MCM::eWidgetDisplay::InCombat) {
        return;
    }

    if (dataHolder->widget->mDelay == 0.0f) {
        return;
    }

    if (IsThreadWorking()) {
        SetRemain(dataHolder->widget->mDelay + 1.0f);
    }
    else {
        SetDissolveTimer();
    }
}

bool EquipsetManager::DissolveIn_Function()
{
    SetThreadWorking(true);

    auto UI = RE::UI::GetSingleton();
    if (!UI) {
        return false;
    }

    auto dataHolder = MCM::DataHolder::GetSingleton();
    if (!dataHolder) {
        return false;
    }

    auto widgetHandler = WidgetHandler::GetSingleton();
    if (!widgetHandler) {
        return false;
    }

    auto playerref = RE::PlayerCharacter::GetSingleton();
    if (!playerref) {
        return false;
    }

    SetRemain(dataHolder->widget->mDelay + 1.0f);
    while (!IsThreadClosing() && GetRemain() > 0.0f) {
        float remain = GetRemain();
        remain = !UI->GameIsPaused() && !playerref->IsInCombat() ? remain - 0.1f : remain;
        SetRemain(remain);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if (!IsThreadClosing()) {
        widgetHandler->DissolveIn();
    }

    SetRemain(0.0f);
    SetThreadClose(false);
    SetThreadWorking(false);
    return true;
}

void EquipsetManager::SetDissolveTimer()
{
    mThreadHandle = std::async(std::launch::async, &EquipsetManager::DissolveIn_Function, this);
}

void EquipsetManager::DissolveOut_Function()
{
    auto UI = RE::UI::GetSingleton();
    if (!UI) {
        return;
    }

    auto dataHolder = MCM::DataHolder::GetSingleton();
    if (!dataHolder) {
        return;
    }

    auto widgetHandler = WidgetHandler::GetSingleton();
    if (!widgetHandler) {
        return;
    }

    widgetHandler->DissolveOut();
}

float EquipsetManager::GetRemain()
{
    std::unique_lock lock(GetSingleton()._lock);
    return mRemain;
}

void EquipsetManager::SetRemain(float _param)
{
    std::unique_lock lock(GetSingleton()._lock);
    mRemain = _param;
}

bool EquipsetManager::IsThreadClosing()
{
    std::unique_lock lock(GetSingleton()._lock);
    return mThreadClose;
}

void EquipsetManager::SetThreadClose(bool _param)
{
    std::unique_lock lock(GetSingleton()._lock);
    mThreadClose = _param;
}

bool EquipsetManager::IsThreadWorking()
{
    std::unique_lock lock(GetSingleton()._lock);
    return mThreadWorking;
}

void EquipsetManager::SetThreadWorking(bool _param)
{
    std::unique_lock lock(GetSingleton()._lock);
    mThreadWorking = _param;
}

void EquipsetManager::RemoveAllEquipset()
{
    while (mEquipset.size() > 0) {
        RemoveEquipset(mEquipset[0]->mName);
    }

    while (mCycleEquipset.size() > 0) {
        RemoveEquipset(mCycleEquipset[0]->mName);
    }
}