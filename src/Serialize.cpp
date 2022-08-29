#include <fstream>
#include "json/json.h"

#include "EquipsetManager.h"
#include "ExtraData.h"

using namespace UIHS;

namespace UIHS {
    // These four-character record types, which store data in the SKSE cosave, are little-endian. That means they are
    // reversed from the character order written here. Using the byteswap functions reverses them back to the order
    // the characters are written in the source code.
    inline const auto EquipsetRecord = _byteswap_ulong('HSER');
    inline const auto CycleEquipsetRecord = _byteswap_ulong('HSCR');
}

namespace SKSE {
    void SaveOrder(SerializationInterface* _serde, const uint32_t& _order)
    {
        _serde->WriteRecordData(&_order, sizeof(_order));
    }

    void SaveName(SerializationInterface* _serde, const std::string& _name)
    {
        size_t nameSize = _name.length();
        _serde->WriteRecordData(&nameSize, sizeof(nameSize));
        for (auto& elem : _name) {
            _serde->WriteRecordData(&elem, sizeof(elem));
        }
    }

    void SaveHotkey(SerializationInterface* _serde, Hotkey* _hotkey)
    {
        _serde->WriteRecordData(&_hotkey->mKeyCode, sizeof(_hotkey->mKeyCode));
        _serde->WriteRecordData(&_hotkey->mModifier[0], sizeof(_hotkey->mModifier[0]));
        _serde->WriteRecordData(&_hotkey->mModifier[1], sizeof(_hotkey->mModifier[1]));
        _serde->WriteRecordData(&_hotkey->mModifier[2], sizeof(_hotkey->mModifier[2]));
    }

    void SaveOption(SerializationInterface* _serde, Option* _option)
    {
        _serde->WriteRecordData(&_option->mSound, sizeof(_option->mSound));
        _serde->WriteRecordData(&_option->mToggleEquip, sizeof(_option->mToggleEquip));
        _serde->WriteRecordData(&_option->mReEquip, sizeof(_option->mReEquip));
        _serde->WriteRecordData(&_option->mBeast, sizeof(_option->mBeast));
    }

    void SaveWidget(SerializationInterface* _serde, Widget* _widget)
    {
        size_t widgetSize = _widget->mWidget.length();
        _serde->WriteRecordData(&widgetSize, sizeof(widgetSize));
        for (auto& elem : _widget->mWidget) {
            _serde->WriteRecordData(&elem, sizeof(elem));
        }
        _serde->WriteRecordData(&_widget->mHpos, sizeof(_widget->mHpos));
        _serde->WriteRecordData(&_widget->mVpos, sizeof(_widget->mVpos));
        _serde->WriteRecordData(&_widget->mDisplayWidget, sizeof(_widget->mDisplayWidget));
        _serde->WriteRecordData(&_widget->mDisplayName, sizeof(_widget->mDisplayName));
        _serde->WriteRecordData(&_widget->mDisplayHotkey, sizeof(_widget->mDisplayHotkey));
    }

    bool SaveEquipment_Hand(SerializationInterface* _serde, Equipment::Weapon* _hand)
    {
        auto action = static_cast<MCM::eAction>(_hand->option);
        if (action != MCM::eAction::Equip) {
            _serde->WriteRecordData(&_hand->option, sizeof(_hand->option));

            return true;
        }

        if (!_hand->form) {
            uint32_t option = 0;
            _serde->WriteRecordData(&option, sizeof(option));

            return false;
        }

        if (_hand->hasExtra.first && !_hand->extraData.first) {
            uint32_t option = 0;
            _serde->WriteRecordData(&option, sizeof(option));

            return false;
        }

        _serde->WriteRecordData(&_hand->option, sizeof(_hand->option));
        _serde->WriteRecordData(&_hand->form->formID, sizeof(_hand->form->formID));
        _serde->WriteRecordData(&_hand->hasExtra.first, sizeof(_hand->hasExtra.first));
        if (_hand->hasExtra.first) {
            _serde->WriteRecordData(&_hand->numEnch, sizeof(_hand->numEnch));
            RE::FormID ID = _hand->extraData.first->GetFormID();
            _serde->WriteRecordData(&ID, sizeof(ID));
        }
        _serde->WriteRecordData(&_hand->hasExtra.second, sizeof(_hand->hasExtra.second));
        if (_hand->hasExtra.second) {
            _serde->WriteRecordData(&_hand->extraData.second, sizeof(_hand->extraData.second));
        }

        return true;
    }

    bool SaveEquipment_Shout(SerializationInterface* _serde, Equipment::Shout* _shout)
    {
        auto action = static_cast<MCM::eAction>(_shout->option);
        if (action != MCM::eAction::Equip) {
            _serde->WriteRecordData(&_shout->option, sizeof(_shout->option));

            return true;
        }

        if (!_shout->form) {
            uint32_t option = 0;
            _serde->WriteRecordData(&option, sizeof(option));

            return false;
        }

        _serde->WriteRecordData(&_shout->option, sizeof(_shout->option));
        _serde->WriteRecordData(&_shout->form->formID, sizeof(_shout->form->formID));

        return true;
    }

    bool SaveEquipment_Items(SerializationInterface* _serde, std::vector<Equipment::Items*> _items)
    {
        for (const auto item : _items) {
            if (!item->form) {
                uint32_t size = 0;
                _serde->WriteRecordData(&size, sizeof(size));

                return false;
            }

            if (item->hasExtra.first && !item->extraData.first) {
                uint32_t size = 0;
                _serde->WriteRecordData(&size, sizeof(size));

                return false;
            }
        }

        uint32_t itemsSize = _items.size();
        _serde->WriteRecordData(&itemsSize, sizeof(itemsSize));
        for (const auto item : _items) {
            _serde->WriteRecordData(&item->form->formID, sizeof(item->form->formID));
            _serde->WriteRecordData(&item->hasExtra.first, sizeof(item->hasExtra.first));
            if (item->hasExtra.first) {
                _serde->WriteRecordData(&item->numEnch, sizeof(item->numEnch));
                RE::FormID ID = item->extraData.first->GetFormID();
                _serde->WriteRecordData(&ID, sizeof(ID));
            }
            _serde->WriteRecordData(&item->hasExtra.second, sizeof(item->hasExtra.second));
            if (item->hasExtra.second) {
                _serde->WriteRecordData(&item->extraData.second, sizeof(item->extraData.second));
            }
        }

        return true;
    }

    void SaveCycleOption(SerializationInterface* _serde, CycleOption* _option)
    {
        _serde->WriteRecordData(&_option->mPersist, sizeof(_option->mPersist));
        _serde->WriteRecordData(&_option->mExpire, sizeof(_option->mExpire));
        _serde->WriteRecordData(&_option->mReset, sizeof(_option->mReset));
        _serde->WriteRecordData(&_option->mBeast, sizeof(_option->mBeast));
    }

    void SaveCycleWidget(SerializationInterface* _serde, Widget* _widget)
    {
        _serde->WriteRecordData(&_widget->mHpos, sizeof(_widget->mHpos));
        _serde->WriteRecordData(&_widget->mVpos, sizeof(_widget->mVpos));
        _serde->WriteRecordData(&_widget->mDisplayWidget, sizeof(_widget->mDisplayWidget));
        _serde->WriteRecordData(&_widget->mDisplayName, sizeof(_widget->mDisplayName));
        _serde->WriteRecordData(&_widget->mDisplayHotkey, sizeof(_widget->mDisplayHotkey));
    }

    void SaveCycleItems(SerializationInterface* _serde, std::vector<std::string> _items)
    {
        size_t itemsSize = _items.size();
        _serde->WriteRecordData(&itemsSize, sizeof(itemsSize));
        for (const auto& item : _items) {
            size_t size = item.length();
            _serde->WriteRecordData(&size, sizeof(size));
            for (const auto& elem : item) {
                _serde->WriteRecordData(&elem, sizeof(elem));
            }
        }
    }

    void SaveCycleIndex(SerializationInterface* _serde, std::pair<uint32_t, int32_t> _cycleIndex)
    {
        _serde->WriteRecordData(&_cycleIndex.first, sizeof(_cycleIndex.first));
        _serde->WriteRecordData(&_cycleIndex.second, sizeof(_cycleIndex.second));
    }

    uint32_t LoadOrder(SerializationInterface* _serde)
    {
        uint32_t order;
        _serde->ReadRecordData(&order, sizeof(order));
        return order;
    }

    std::string LoadName(SerializationInterface* _serde)
    {
        std::string mName;
        size_t nameSize;
        _serde->ReadRecordData(&nameSize, sizeof(nameSize));
        for (; nameSize > 0; --nameSize) {
            char name;
            _serde->ReadRecordData(&name, sizeof(name));
            mName += name;
        }

        return mName;
    }

    Hotkey* LoadHotkey(SerializationInterface* _serde)
    {
        Hotkey* hotkey = new Hotkey;
        _serde->ReadRecordData(&hotkey->mKeyCode, sizeof(hotkey->mKeyCode));
        _serde->ReadRecordData(&hotkey->mModifier[0], sizeof(hotkey->mModifier[0]));
        _serde->ReadRecordData(&hotkey->mModifier[1], sizeof(hotkey->mModifier[1]));
        _serde->ReadRecordData(&hotkey->mModifier[2], sizeof(hotkey->mModifier[2]));

        return hotkey;
    }

    Option* LoadOption(SerializationInterface* _serde)
    {
        Option* option = new Option;
        _serde->ReadRecordData(&option->mSound, sizeof(option->mSound));
        _serde->ReadRecordData(&option->mToggleEquip, sizeof(option->mToggleEquip));
        _serde->ReadRecordData(&option->mReEquip, sizeof(option->mReEquip));
        _serde->ReadRecordData(&option->mBeast, sizeof(option->mBeast));

        return option;
    }

    Widget* LoadWidget(SerializationInterface* _serde)
    {
        Widget* widget = new Widget;

        size_t WidgetSize;
        _serde->ReadRecordData(&WidgetSize, sizeof(WidgetSize));
        for (; WidgetSize > 0; --WidgetSize) {
            char name;
            _serde->ReadRecordData(&name, sizeof(name));
            widget->mWidget += name;
        }
        _serde->ReadRecordData(&widget->mHpos, sizeof(widget->mHpos));
        _serde->ReadRecordData(&widget->mVpos, sizeof(widget->mVpos));
        _serde->ReadRecordData(&widget->mDisplayWidget, sizeof(widget->mDisplayWidget));
        _serde->ReadRecordData(&widget->mDisplayName, sizeof(widget->mDisplayName));
        _serde->ReadRecordData(&widget->mDisplayHotkey, sizeof(widget->mDisplayHotkey));

        return widget;
    }

    bool LoadEquipment_Hand(SerializationInterface* _serde, Equipment::Weapon* _hand)
    {
        bool result = true;

        _serde->ReadRecordData(&_hand->option, sizeof(_hand->option));

        auto action = static_cast<MCM::eAction>(_hand->option);
        if (action != MCM::eAction::Equip) {
            return true;
        }

        RE::FormID ID, newID;
        _serde->ReadRecordData(&ID, sizeof(ID));
        if (!_serde->ResolveFormID(ID, newID)) {
            log::warn("Form ID {:X} could not be found after loading the save.", ID);
            _hand->option = static_cast<int32_t>(MCM::eAction::Nothing);
            newID = 0;
            result = false;
        }
        _hand->form = newID != 0 ? RE::TESForm::LookupByID<RE::TESForm>(newID) : nullptr;

        bool hasEnch, hasTemp;
        RE::EnchantmentItem* enchItem;
        float tempItem;
        _serde->ReadRecordData(&hasEnch, sizeof(hasEnch));
        if (hasEnch) {
            _serde->ReadRecordData(&_hand->numEnch, sizeof(_hand->numEnch));
            RE::FormID ExtraID, newExtraID;
            _serde->ReadRecordData(&ExtraID, sizeof(ExtraID));
            if (!_serde->ResolveFormID(ExtraID, newExtraID)) {
                log::warn("Form ID {:X} could not be found after loading the save.", ExtraID);
                hasEnch = false;
                _hand->numEnch = 0;
                newExtraID = 0;
                result = false;
            }
            enchItem = newExtraID != 0 ? RE::TESForm::LookupByID<RE::EnchantmentItem>(newExtraID) : nullptr;
        }

        _serde->ReadRecordData(&hasTemp, sizeof(hasTemp));
        if (hasTemp) {
            _serde->ReadRecordData(&tempItem, sizeof(tempItem));
        }

        _hand->hasExtra = std::make_pair(hasEnch, hasTemp);
        _hand->extraData = std::make_pair(enchItem, tempItem);

        RE::ExtraDataList* xList;
        xList = Extra::SearchExtraList(_hand->form, _hand->numEnch, _hand->extraData.first, _hand->extraData.second);
        _hand->xList = xList;

        return result;
    }

    bool LoadEquipment_Shout(SerializationInterface* _serde, Equipment::Shout* _shout)
    {
        bool result = true;

        _serde->ReadRecordData(&_shout->option, sizeof(_shout->option));

        auto action = static_cast<MCM::eAction>(_shout->option);
        if (action != MCM::eAction::Equip) {
            return true;
        }

        RE::FormID ID, newID;
        _serde->ReadRecordData(&ID, sizeof(ID));
        if (!_serde->ResolveFormID(ID, newID)) {
            log::warn("Form ID {:X} could not be found after loading the save.", ID);
            _shout->option = static_cast<int32_t>(MCM::eAction::Nothing);
            newID = 0;
            result = false;
        }
        _shout->form = newID != 0 ? RE::TESForm::LookupByID<RE::TESForm>(newID) : nullptr;

        return result;
    }

    bool LoadEquipment_Items(SerializationInterface* _serde, std::vector<Equipment::Items*>& _items)
    {
        uint32_t itemsSize;
        _serde->ReadRecordData(&itemsSize, sizeof(itemsSize));

        for (; itemsSize > 0; --itemsSize) {
            bool result = true;
            Equipment::Items* items = new Equipment::Items;

            RE::FormID ID, newID;
            _serde->ReadRecordData(&ID, sizeof(ID));
            if (!_serde->ResolveFormID(ID, newID)) {
                log::warn("Form ID {:X} could not be found after loading the save.", ID);
                newID = 0;
                result = false;
            }
            items->form = newID != 0 ? RE::TESForm::LookupByID<RE::TESForm>(newID) : nullptr;

            bool hasItemsExtraEnch, hasItemsExtraTemp;
            RE::EnchantmentItem* itemsExtraEnch;
            float itemsExtraTemp;

            _serde->ReadRecordData(&hasItemsExtraEnch, sizeof(hasItemsExtraEnch));
            if (hasItemsExtraEnch) {
                uint32_t numItemsEnch;
                _serde->ReadRecordData(&numItemsEnch, sizeof(numItemsEnch));
                items->numEnch = numItemsEnch;

                RE::FormID ExtraID, newExtraID;
                _serde->ReadRecordData(&ExtraID, sizeof(ExtraID));
                if (!_serde->ResolveFormID(ExtraID, newExtraID)) {
                    log::warn("Form ID {:X} could not be found after loading the save.", ExtraID);
                    newExtraID = 0;
                    result = false;
                }
                itemsExtraEnch = newID != 0 ? RE::TESForm::LookupByID<RE::EnchantmentItem>(newExtraID) : nullptr;
            }

            _serde->ReadRecordData(&hasItemsExtraTemp, sizeof(hasItemsExtraTemp));
            if (hasItemsExtraTemp) {
                _serde->ReadRecordData(&itemsExtraTemp, sizeof(itemsExtraTemp));
            }

            items->hasExtra = std::make_pair(hasItemsExtraEnch, hasItemsExtraTemp);
            items->extraData = std::make_pair(itemsExtraEnch, itemsExtraTemp);

            RE::ExtraDataList* xList;
            xList = Extra::SearchExtraList(items->form, items->numEnch, items->extraData.first, items->extraData.second);
            items->xList = xList;

            if (result) {
                _items.push_back(items);
            }
            else {
                delete items;
            }
        }

        return true;
    }

    CycleOption* LoadCycleOption(SerializationInterface* _serde)
    {
        CycleOption* option = new CycleOption;
        _serde->ReadRecordData(&option->mPersist, sizeof(option->mPersist));
        _serde->ReadRecordData(&option->mExpire, sizeof(option->mExpire));
        _serde->ReadRecordData(&option->mReset, sizeof(option->mReset));
        _serde->ReadRecordData(&option->mBeast, sizeof(option->mBeast));

        return option;
    }

    Widget* LoadCycleWidget(SerializationInterface* _serde)
    {
        Widget* widget = new Widget;

        _serde->ReadRecordData(&widget->mHpos, sizeof(widget->mHpos));
        _serde->ReadRecordData(&widget->mVpos, sizeof(widget->mVpos));
        _serde->ReadRecordData(&widget->mDisplayWidget, sizeof(widget->mDisplayWidget));
        _serde->ReadRecordData(&widget->mDisplayName, sizeof(widget->mDisplayName));
        _serde->ReadRecordData(&widget->mDisplayHotkey, sizeof(widget->mDisplayHotkey));

        return widget;
    }

    std::vector<std::string> LoadCycleItems(SerializationInterface* _serde)
    {
        std::vector<std::string> items;
        size_t itemsSize;
        _serde->ReadRecordData(&itemsSize, sizeof(itemsSize));
        for (; itemsSize > 0; --itemsSize) {
            std::string itemsName;
            size_t inameSize;
            _serde->ReadRecordData(&inameSize, sizeof(inameSize));
            for (; inameSize > 0; --inameSize) {
                char name;
                _serde->ReadRecordData(&name, sizeof(name));
                itemsName += name;
            }
            items.push_back(itemsName);
        }

        return items;
    }

    std::pair<uint32_t, int32_t> LoadCycleIndex(SerializationInterface* _serde)
    {
        uint32_t index;
        int32_t prevIndex;
        _serde->ReadRecordData(&index, sizeof(index));
        _serde->ReadRecordData(&prevIndex, sizeof(prevIndex));

        return std::make_pair(index, prevIndex);
    }
}
namespace Json {
    void SaveOrder(Json::Value& _root, const std::string& _name, const uint32_t _order)
    {
        _root[_name]["mOrder"] = _order;
    }

    void SaveName(Json::Value& _root, const std::string& _name, const std::string _mName)
    {
        _root[_name]["mName"] = _mName;
    }

    void SaveHotkey(Json::Value& _root, const std::string& _name, const Hotkey* _hotkey)
    {
        _root[_name]["mKeyCode"] = _hotkey->mKeyCode;
        _root[_name]["mModifier1"] = _hotkey->mModifier[0];
        _root[_name]["mModifier2"] = _hotkey->mModifier[1];
        _root[_name]["mModifier3"] = _hotkey->mModifier[2];
    }

    void SaveOption(Json::Value& _root, const std::string& _name, const Option* _option)
    {
        _root[_name]["mSound"] = _option->mSound;
        _root[_name]["mToggleEquip"] = _option->mToggleEquip;
        _root[_name]["mReEquip"] = _option->mReEquip;
        _root[_name]["mBeast"] = _option->mBeast;
    }

    void SaveWidget(Json::Value& _root, const std::string& _name, const Widget* _widget)
    {
        _root[_name]["mWidget"] = _widget->mWidget;
        _root[_name]["mHpos"] = _widget->mHpos;
        _root[_name]["mVpos"] = _widget->mVpos;
        _root[_name]["mDisplayWidget"] = _widget->mDisplayWidget;
        _root[_name]["mDisplayName"] = _widget->mDisplayName;
        _root[_name]["mDisplayHotkey"] = _widget->mDisplayHotkey;
    }

    bool SaveEquipment_Hand(Json::Value& _root, const std::string& _name, const Equipment::Weapon* _hand, bool isLeft)
    {
        std::string strOption = isLeft ? "lOption" : "rOption";
        std::string strForm = isLeft ? "lForm" : "rForm";
        std::string strHasExtra_first = isLeft ? "lHasExtra_first" : "rHasExtra_first";
        std::string strNumEnch = isLeft ? "lNumEnch" : "rNumEnch";
        std::string strExtraData_first = isLeft ? "lExtraData_first" : "rExtraData_first";
        std::string strHasExtra_second = isLeft ? "lHasExtra_second" : "rHasExtra_second";
        std::string strExtraData_second = isLeft ? "lExtraData_second" : "rExtraData_second";

        _root[_name][strOption] = _hand->option;
        _root[_name][strForm] = _hand->option == MCM::eAction::Equip && _hand->form ? _hand->form->formID : 0;
        _root[_name][strHasExtra_first] = _hand->hasExtra.first;
        _root[_name][strNumEnch] = _hand->numEnch;
        _root[_name][strExtraData_first] = _hand->hasExtra.first && _hand->extraData.first ? _hand->extraData.first->GetFormID() : 0;
        _root[_name][strHasExtra_second] = _hand->hasExtra.second;
        _root[_name][strExtraData_second] = _hand->extraData.second;

        if (_hand->option != MCM::eAction::Equip) {
            return true;
        }

        if (!_hand->form) {
            _root[_name][strOption] = MCM::eAction::Nothing;
            return false;
        }

        if (_hand->hasExtra.first && !_hand->extraData.first) {
            _root[_name][strOption] = MCM::eAction::Nothing;
            return false;
        }

        return true;
    }

    bool SaveEquipment_Shout(Json::Value& _root, const std::string& _name, const Equipment::Shout* _shout)
    {
        _root[_name]["sOption"] = _shout->option;
        _root[_name]["sForm"] = _shout->option == MCM::eAction::Equip && _shout->form ? _shout->form->formID : 0;

        if (_shout->option != MCM::eAction::Equip) {
            return true;
        }

        if (!_shout->form) {
            _root[_name]["sOption"] = MCM::eAction::Nothing;
            return false;
        }

        return true;
    }

    bool SaveEquipment_Items(Json::Value& _root, const std::string& _name, std::vector<Equipment::Items*> _items)
    {
        for (const auto& item : _items) {
            if (!item->form) {
                _root[_name]["mNumItems"] = 0;
                return false;
            }

            if (item->hasExtra.first && !item->extraData.first) {
                _root[_name]["mNumItems"] = 0;
                return false;
            }
        }

        _root[_name]["mNumItems"] = _items.size();
        for (int i = 0; i < _items.size(); i++) {
            auto itemName = "Item_" + std::to_string(i);
            _root[_name]["Items"][itemName]["form"] = _items[i]->form ? _items[i]->form->formID : 0;
            _root[_name]["Items"][itemName]["hasExtra_first"] = _items[i]->hasExtra.first;
            _root[_name]["Items"][itemName]["numEnch"] = _items[i]->numEnch;
            _root[_name]["Items"][itemName]["extraData_first"] = _items[i]->hasExtra.first && _items[i]->extraData.first ? _items[i]->extraData.first->GetFormID() : 0;
            _root[_name]["Items"][itemName]["hasExtra_second"] = _items[i]->hasExtra.second;
            _root[_name]["Items"][itemName]["extraData_second"] = _items[i]->extraData.second;
        }

        return true;
    }

    void SaveCycleOption(Json::Value& _root, const std::string& _name, const CycleOption* _option)
    {
        _root[_name]["mPersist"] = _option->mPersist;
        _root[_name]["mExpire"] = _option->mExpire;
        _root[_name]["mReset"] = _option->mReset;
        _root[_name]["mBeast"] = _option->mBeast;
    }

    void SaveCycleWidget(Json::Value& _root, const std::string& _name, const Widget* _widget)
    {
        _root[_name]["mHpos"] = _widget->mHpos;
        _root[_name]["mVpos"] = _widget->mVpos;
        _root[_name]["mDisplayWidget"] = _widget->mDisplayWidget;
        _root[_name]["mDisplayName"] = _widget->mDisplayName;
        _root[_name]["mDisplayHotkey"] = _widget->mDisplayHotkey;
    }

    void SaveCycleItems(Json::Value& _root, const std::string& _name, std::vector<std::string> _items)
    {
        _root[_name]["mNumItems"] = _items.size();
        for (int i = 0; i < _items.size(); i++) {
            auto itemName = "Item_" + std::to_string(i);
            _root[_name]["Items"][itemName]["name"] = _items[i];
        }
    }

    int32_t LoadOrder(Json::Value& _root, const std::string& _name)
    {
        return _root[_name].get("mOrder", 0).asInt();
    }

    std::string LoadName(Json::Value& _root, const std::string& _name)
    {
        return _root[_name].get("mName", "").asString();
    }

    Hotkey* LoadHotkey(Json::Value& _root, const std::string& _name)
    {
        Hotkey* hotkey = new Hotkey;
        hotkey->mKeyCode = _root[_name].get("mKeyCode", -1).asInt();
        hotkey->mModifier[0] = _root[_name].get("mModifier1", false).asBool();
        hotkey->mModifier[1] = _root[_name].get("mModifier2", false).asBool();
        hotkey->mModifier[2] = _root[_name].get("mModifier3", false).asBool();

        return hotkey;
    }

    Option* LoadOption(Json::Value& _root, const std::string& _name)
    {
        Option* option = new Option;
        option->mSound = _root[_name].get("mSound", false).asBool();
        option->mToggleEquip = _root[_name].get("mToggleEquip", false).asBool();
        option->mReEquip = _root[_name].get("mReEquip", false).asBool();
        option->mBeast = _root[_name].get("mBeast", false).asBool();

        return option;
    }

    Widget* LoadWidget(Json::Value& _root, const std::string& _name)
    {
        Widget* widget = new Widget;
        widget->mWidget = _root[_name].get("mWidget", "").asString();
        widget->mHpos = _root[_name].get("mHpos", 0).asInt();
        widget->mVpos = _root[_name].get("mVpos", 0).asInt();
        widget->mDisplayWidget = _root[_name].get("mDisplayWidget", false).asBool();
        widget->mDisplayName = _root[_name].get("mDisplayName", false).asBool();
        widget->mDisplayHotkey = _root[_name].get("mDisplayHotkey", false).asBool();

        return widget;
    }

    bool LoadEquipment_Hand(Json::Value& _root, const std::string& _name, Equipment::Weapon* _hand, bool isLeft)
    {
        std::string strOption = isLeft ? "lOption" : "rOption";
        std::string strForm = isLeft ? "lForm" : "rForm";
        std::string strHasExtra_first = isLeft ? "lHasExtra_first" : "rHasExtra_first";
        std::string strNumEnch = isLeft ? "lNumEnch" : "rNumEnch";
        std::string strExtraData_first = isLeft ? "lExtraData_first" : "rExtraData_first";
        std::string strHasExtra_second = isLeft ? "lHasExtra_second" : "rHasExtra_second";
        std::string strExtraData_second = isLeft ? "lExtraData_second" : "rExtraData_second";

        _hand->option = _root[_name].get(strOption, 0).asInt();
        RE::FormID formID = _root[_name].get(strForm, 0).asInt64();
        _hand->form = _hand->option == 2 && formID != 0 ? RE::TESForm::LookupByID<RE::TESForm>(formID) : nullptr;
        _hand->hasExtra.first = _root[_name].get(strHasExtra_first, false).asBool();
        _hand->numEnch = _root[_name].get(strNumEnch, 0).asInt();
        RE::FormID enchantID = _root[_name].get(strExtraData_first, 0).asInt64();
        RE::EnchantmentItem* enchantForm = _hand->hasExtra.first && enchantID != 0 ? RE::TESForm::LookupByID<RE::EnchantmentItem>(enchantID) : nullptr;
        _hand->extraData.first = _hand->hasExtra.first && enchantForm ? enchantForm : nullptr;
        _hand->hasExtra.second = _root[_name].get(strHasExtra_second, false).asBool();
        _hand->extraData.second = _root[_name].get(strExtraData_second, 0.0f).asFloat();
        RE::ExtraDataList* xList;
        xList = Extra::SearchExtraList(_hand->form, _hand->numEnch, _hand->extraData.first, _hand->extraData.second);
        _hand->xList = xList;

        if (_hand->option != MCM::eAction::Equip) {
            return true;
        }

        if (!_hand->form) {
            _hand->option = MCM::eAction::Nothing;
            return false;
        }

        if (_hand->hasExtra.first && !_hand->extraData.first) {
            _hand->option = MCM::eAction::Nothing;
            return false;
        }

        return true;
    }

    bool LoadEquipment_Shout(Json::Value& _root, const std::string& _name, Equipment::Shout* _shout)
    {
        _shout->option = _root[_name].get("sOption", 0).asInt();
        RE::FormID formID = _root[_name].get("sForm", 0).asInt64();
        _shout->form = _shout->option == 2 && formID != 0 ? RE::TESForm::LookupByID<RE::TESForm>(formID) : nullptr;

        if (_shout->option != MCM::eAction::Equip) {
            return true;
        }

        if (!_shout->form) {
            _shout->option = MCM::eAction::Nothing;
            return false;
        }

        return true;
    }

    bool LoadEquipment_Items(Json::Value& _root, const std::string& _name, std::vector<Equipment::Items*>& _Items)
    {
        int numItems = _root[_name].get("mNumItems", 0).asInt();
        for (int j = 0; j < numItems; j++) {
            bool result = true;

            Equipment::Items* item = new Equipment::Items;
            auto itemName = "Item_" + std::to_string(j);

            RE::FormID formID = _root[_name]["Items"][itemName].get("form", 0).asInt64();
            item->form = formID != 0 ? RE::TESForm::LookupByID<RE::TESForm>(formID) : nullptr;
            item->hasExtra.first = _root[_name]["Items"][itemName].get("hasExtra_first", false).asBool();
            item->numEnch = _root[_name]["Items"][itemName].get("numEnch", 0).asInt();
            RE::FormID enchantID = _root[_name]["Items"][itemName].get("extraData_first", 0).asInt64();
            RE::EnchantmentItem* enchantForm = item->hasExtra.first && enchantID != 0 ? RE::TESForm::LookupByID<RE::EnchantmentItem>(enchantID) : nullptr;
            item->extraData.first = item->hasExtra.first && enchantForm ? enchantForm : nullptr;
            item->hasExtra.second = _root[_name]["Items"][itemName].get("hasExtra_second", false).asBool();
            item->extraData.second = _root[_name]["Items"][itemName].get("extraData_second", 0.0f).asFloat();
            RE::ExtraDataList* xList;
            xList = Extra::SearchExtraList(item->form, item->numEnch, item->extraData.first, item->extraData.second);
            item->xList = xList;

            if (!item->form) {
                delete item;
                continue;
            }

            if (item->hasExtra.first && !item->extraData.first) {
                delete item;
                continue;
            }

            _Items.push_back(item);
        }

        return true;
    }

    CycleOption* LoadCycleOption(Json::Value& _root, const std::string& _name)
    {
        CycleOption* option = new CycleOption;
        option->mPersist = _root[_name].get("mPersist", false).asBool();
        option->mExpire = _root[_name].get("mExpire", false).asBool();
        option->mReset = _root[_name].get("mReset", false).asBool();
        option->mBeast = _root[_name].get("mBeast", false).asBool();

        return option;
    }

    Widget* LoadCycleWidget(Json::Value& _root, const std::string& _name)
    {
        Widget* widget = new Widget;
        widget->mHpos = _root[_name].get("mHpos", 0).asInt();
        widget->mVpos = _root[_name].get("mVpos", 0).asInt();
        widget->mDisplayWidget = _root[_name].get("mDisplayWidget", false).asBool();
        widget->mDisplayName = _root[_name].get("mDisplayName", false).asBool();
        widget->mDisplayHotkey = _root[_name].get("mDisplayHotkey", false).asBool();

        return widget;
    }

    std::vector<std::string> LoadCycleItems(Json::Value& _root, const std::string& _name)
    {
        std::vector<std::string> items;

        size_t itemsSize = _root[_name].get("mNumItems", 0).asInt();
        for (int j = 0; j < itemsSize; j++) {
            auto itemName = "Item_" + std::to_string(j);
            std::string cycleItemsName = _root[_name]["Items"][itemName].get("name", 0).asString();
            items.push_back(cycleItemsName);
        }

        return items;
    }
}

void EquipsetManager::OnRevert(SerializationInterface*)
{
    std::unique_lock lock(GetSingleton()._lock);

    auto manager = &GetSingleton();
    if (!manager) {
        log::error("Unable to get EquipsetManager");
        return;
    }

    manager->RemoveAllEquipset();
}

void EquipsetManager::OnGameSaved(SerializationInterface* serde)
{
    std::unique_lock lock(GetSingleton()._lock);

    auto manager = &GetSingleton();
    if (!manager) {
        log::error("Unable to get EquipsetManager");
        return;
    }
    
    if (!serde->OpenRecord(EquipsetRecord, 0)) {
        log::error("Unable to open record to write cosave data.");
        return;
    }

    logger::debug("Saving {} Equipsets...", manager->mEquipset.size());

    size_t equipsetSize = manager->mEquipset.size();
    serde->WriteRecordData(&equipsetSize, sizeof(equipsetSize));

    for (auto equipset : manager->mEquipset) {

        logger::trace("Saving '{}'...", equipset->mName);
 
        SaveOrder(serde, equipset->mOrder);
        SaveName(serde, equipset->mName);
        SaveHotkey(serde, equipset->mHotkey);
        SaveOption(serde, equipset->mOption);
        SaveWidget(serde, equipset->mWidget);

        auto equipment = equipset->mEquipment;

        if (!SaveEquipment_Hand(serde, equipment->mLeft)) {
            logger::error("[{}] Failed to save Lefthand data.", equipset->mName);
        }

        if (!SaveEquipment_Hand(serde, equipment->mRight)) {
            logger::error("[{}] Failed to save Righthand data.", equipset->mName);
        }

        if (!SaveEquipment_Shout(serde, equipment->mShout)) {
            logger::error("[{}] Failed to save Shout/Power data.", equipset->mName);
        }

        if (!SaveEquipment_Items(serde, equipment->mItems)) {
            logger::error("[{}] Failed to save Items data.", equipset->mName);
        }

        logger::debug("'{}' saved.", equipset->mName);
    }

    logger::info("Total {} Equipsets saved.", manager->mEquipset.size());
    
    if (!serde->OpenRecord(CycleEquipsetRecord, 0)) {
        log::error("Unable to open record to write cosave data.");
        return;
    }
    
    logger::debug("Saving {} Cycle Equipsets...", manager->mCycleEquipset.size());

    size_t cycleEquipsetSize = manager->mCycleEquipset.size();
    serde->WriteRecordData(&cycleEquipsetSize, sizeof(cycleEquipsetSize));

    for (auto equipset : manager->mCycleEquipset) {

        logger::trace("Saving '{}'...", equipset->mName);

        SaveOrder(serde, equipset->mOrder);
        SaveName(serde, equipset->mName);
        SaveHotkey(serde, equipset->mHotkey);
        SaveCycleOption(serde, equipset->mOption);
        SaveCycleWidget(serde, equipset->mWidget);
        SaveCycleItems(serde, equipset->mCycleItems);
        SaveCycleIndex(serde, equipset->mCycleIndex);

        logger::debug("'{}' saved.", equipset->mName);
    }

    logger::info("Total {} Cycle Equipsets saved.", manager->mCycleEquipset.size());
}


void EquipsetManager::OnGameLoaded(SerializationInterface* serde) {
    std::uint32_t type;
    std::uint32_t size;
    std::uint32_t version;

    while (serde->GetNextRecordInfo(type, version, size)) {
        if (type == EquipsetRecord) {

            size_t equipsetSize;
            serde->ReadRecordData(&equipsetSize, sizeof(equipsetSize));

            logger::debug("Loading {} Equipsets...", equipsetSize);

            for (int i = 0; i < equipsetSize; i++) {
                uint32_t order = LoadOrder(serde);;
                std::string mName = LoadName(serde);

                logger::trace("Loading '{}'...", mName);

                Hotkey* hotkey = LoadHotkey(serde);
                Option* option = LoadOption(serde);
                Widget* widget = LoadWidget(serde);

                Equipment* equipment = new Equipment;

                if (!LoadEquipment_Hand(serde, equipment->mLeft)) {
                    logger::error("[{}] Failed to load Lefthand data.", mName);
                }

                if (!LoadEquipment_Hand(serde, equipment->mRight)) {
                    logger::error("[{}] Failed to load Righthand data.", mName);
                }

                if (!LoadEquipment_Shout(serde, equipment->mShout)) {
                    logger::error("[{}] Failed to load Shout/Power data.", mName);
                }

                if (!LoadEquipment_Items(serde, equipment->mItems)) {
                    logger::error("[{}] Failed to load Items data.", mName);
                }

                auto manager = &GetSingleton();
                if (!manager) {
                    log::error("Unable to get EquipsetManager");
                    return;
                }

                manager->NewEquipset(order, mName, hotkey, option, widget, equipment);

                logger::debug("'{}' loaded.", mName);
            }

            logger::info("Total {} Equipsets loaded.", equipsetSize);
        }
        else if (type == CycleEquipsetRecord) {
            size_t equipsetSize;
            serde->ReadRecordData(&equipsetSize, sizeof(equipsetSize));

            logger::debug("Loading {} Cycle Equipsets...", equipsetSize);

            for (int i = 0; i < equipsetSize; i++) {
                uint32_t order = LoadOrder(serde);;
                std::string mName = LoadName(serde);

                logger::trace("Loading '{}'...", mName);

                Hotkey* hotkey = LoadHotkey(serde);
                CycleOption* option = LoadCycleOption(serde);
                Widget* widget = LoadCycleWidget(serde);
                std::vector<std::string> items = LoadCycleItems(serde);
                std::pair<uint32_t, int32_t> index = LoadCycleIndex(serde);

                auto manager = &GetSingleton();
                if (!manager) {
                    log::error("Unable to get EquipsetManager");
                    return;
                }

                manager->NewCycleEquipset(order, mName, hotkey, option, widget, items, index);

                logger::debug("'{}' loaded.", mName);
            }

            logger::info("Total {} Cycle Equipsets loaded.", equipsetSize);
        }
        else {
            log::warn("Unknown record type in cosave.");
            __assume(false);
        }
    }
}

void EquipsetManager::SaveEquipsetData()
{
    Json::Value root;
    
    {
        auto size = GetEquipsetList().size();
        root["Equipset Size"] = size;

        logger::debug("Exporting {} Equipsets...", size);

        for (int i = 0; i < size; i++) {
            auto name = "Equipset_" + std::to_string(i);
            auto equipset = mEquipset[i];

            logger::trace("Exporting '{}'...", equipset->mName);
            
            Json::SaveOrder(root, name, equipset->mOrder);
            Json::SaveName(root, name, equipset->mName);
            Json::SaveHotkey(root, name, equipset->mHotkey);
            Json::SaveOption(root, name, equipset->mOption);
            Json::SaveWidget(root, name, equipset->mWidget);

            auto equipment = equipset->mEquipment;
            if (!Json::SaveEquipment_Hand(root, name, equipment->mLeft, true)) {
                logger::error("[{}] Failed to export Lefthand data.", equipset->mName);
            }
            if (!Json::SaveEquipment_Hand(root, name, equipment->mRight, false)) {
                logger::error("[{}] Failed to export Righthand data.", equipset->mName);
            }
            if (!Json::SaveEquipment_Shout(root, name, equipment->mShout)) {
                logger::error("[{}] Failed to export Shout/Power data.", equipset->mName);
            }
            if (!Json::SaveEquipment_Items(root, name, equipment->mItems)) {
                logger::error("[{}] Failed to export Items data.", equipset->mName);
            }

            logger::debug("'{}' exported.", equipset->mName);
        }

        logger::info("Total {} Equipsets exported.", size);
    }

    {
        auto size = GetCycleEquipsetList().size();
        root["CycleEquipset Size"] = size;

        logger::debug("Exporting {} Cycle Equipsets...", size);

        for (int i = 0; i < size; i++) {
            auto name = "CycleEquipset_" + std::to_string(i);
            auto equipset = mCycleEquipset[i];

            logger::trace("Exporting '{}'...", equipset->mName);
            
            Json::SaveOrder(root, name, equipset->mOrder);
            Json::SaveName(root, name, equipset->mName);
            Json::SaveHotkey(root, name, equipset->mHotkey);
            Json::SaveCycleOption(root, name, equipset->mOption);
            Json::SaveCycleWidget(root, name, equipset->mWidget);
            Json::SaveCycleItems(root, name, equipset->mCycleItems);

            logger::debug("'{}' exported.", equipset->mName);
        }

        logger::info("Total {} Cycle Equipsets exported.", size);
    }

    Json::StyledWriter writer;
    std::string outString = writer.write(root);
    std::ofstream out("Data/SKSE/Plugins/UIHS/Equipset.json");
    out << outString;
    out.close();
}

void EquipsetManager::LoadEquipsetData()
{
    auto manager = &GetSingleton();
    if (!manager) {
        log::error("Unable to get EquipsetManager");
        return;
    }

    std::ifstream file("Data/SKSE/Plugins/UIHS/Equipset.json", std::ifstream::binary);
    if (!file.is_open()) {
        log::error("Unable to open Equipset.json file.");
        return;
    }

    Json::Value root;
    file >> root;
    file.close();

    {
        size_t size;
        size = root.get("Equipset Size", 0).asInt();

        logger::debug("Loading {} Equipsets...", size);

        for (int i = 0; i < size; i++) {
            std::string name = "Equipset_" + std::to_string(i);

            int32_t mOrder = Json::LoadOrder(root, name);
            std::string mName = Json::LoadName(root, name);

            logger::trace("Loading '{}'...", mName);

            Hotkey* hotkey = Json::LoadHotkey(root, name);
            Option* option = Json::LoadOption(root, name);
            Widget* widget = Json::LoadWidget(root, name);

            Equipment* equipment = new Equipment;
            if (!Json::LoadEquipment_Hand(root, name, equipment->mLeft, true)) {
                logger::error("[{}] Failed to load Lefthand data.", mName);
            }
            if (!Json::LoadEquipment_Hand(root, name, equipment->mRight, false)) {
                logger::error("[{}] Failed to load Righthand data.", mName);
            }
            if (!Json::LoadEquipment_Shout(root, name, equipment->mShout)) {
                logger::error("[{}] Failed to load Shout/Power data.", mName);
            }
            if (!Json::LoadEquipment_Items(root, name, equipment->mItems)) {
                logger::error("[{}] Failed to load Items data.", mName);
            }

            manager->NewEquipset(mOrder, mName, hotkey, option, widget, equipment);

            logger::debug("'{}' loaded.", mName);
        }

        logger::info("Total {} Equipsets loaded.", size);
    }
    {
        size_t size;
        size = root.get("CycleEquipset Size", 0).asInt();

        logger::debug("Loading {} Cycle Equipsets...", size);

        for (int i = 0; i < size; i++) {
            std::string name = "CycleEquipset_" + std::to_string(i);

            int32_t mOrder = Json::LoadOrder(root, name);
            std::string mName = Json::LoadName(root, name);

            logger::trace("Loading '{}'...", mName);

            Hotkey* hotkey = Json::LoadHotkey(root, name);
            CycleOption* option = Json::LoadCycleOption(root, name);
            Widget* widget = Json::LoadCycleWidget(root, name);
            std::vector<std::string> items = Json::LoadCycleItems(root, name);

            manager->NewCycleEquipset(mOrder, mName, hotkey, option, widget, items, std::make_pair(0, -1));

            logger::debug("'{}' loaded.", mName);
        }

        logger::info("Total {} Cycle Equipsets loaded.", size);
    }
}