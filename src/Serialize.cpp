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
            
            root[name]["mOrder"] = equipset->mOrder;
            root[name]["mName"] = equipset->mName;

            auto hotkey = equipset->mHotkey;
            root[name]["mKeyCode"] = hotkey->mKeyCode;
            root[name]["mModifier1"] = hotkey->mModifier[0];
            root[name]["mModifier2"] = hotkey->mModifier[1];
            root[name]["mModifier3"] = hotkey->mModifier[2];

            auto option = equipset->mOption;
            root[name]["mSound"] = option->mSound;
            root[name]["mToggleEquip"] = option->mToggleEquip;
            root[name]["mReEquip"] = option->mReEquip;
            root[name]["mBeast"] = option->mBeast;

            auto widget = equipset->mWidget;
            root[name]["mWidget"] = widget->mWidget;
            root[name]["mHpos"] = widget->mHpos;
            root[name]["mVpos"] = widget->mVpos;
            root[name]["mDisplayWidget"] = widget->mDisplayWidget;
            root[name]["mDisplayName"] = widget->mDisplayName;
            root[name]["mDisplayHotkey"] = widget->mDisplayHotkey;

            auto equipment = equipset->mEquipment;

            auto left = equipment->mLeft;
            root[name]["lOption"] = left->option;
            root[name]["lForm"] = left->option == 2 && left->form ? left->form->formID : 0;
            root[name]["lHasExtra_first"] = left->hasExtra.first;
            root[name]["lNumEnch"] = left->numEnch;
            root[name]["lExtraData_first"] = left->hasExtra.first ? left->extraData.first->GetFormID() : 0;

            root[name]["lHasExtra_second"] = left->hasExtra.second;
            root[name]["lExtraData_second"] = left->extraData.second;

            auto right = equipment->mRight;
            root[name]["rOption"] = right->option;
            root[name]["rForm"] = right->option == 2 && right->form ? right->form->formID : 0;
            root[name]["rHasExtra_first"] = right->hasExtra.first;
            root[name]["rNumEnch"] = right->numEnch;
            root[name]["rExtraData_first"] = right->hasExtra.first ? right->extraData.first->GetFormID() : 0;
            root[name]["rHasExtra_second"] = right->hasExtra.second;
            root[name]["rExtraData_second"] = right->extraData.second;

            auto shout = equipment->mShout;
            root[name]["sOption"] = shout->option;
            root[name]["sForm"] = shout->option == 2 && shout->form ? shout->form->formID : 0;

            auto& items = equipment->mItems;
            root[name]["mNumItems"] = items.size();
            for (int j = 0; j < items.size(); j++) {
                auto itemName = "Item_" + std::to_string(j);
                root[name]["Items"][itemName]["form"] = items[j]->form ? items[j]->form->formID : 0;
                root[name]["Items"][itemName]["hasExtra_first"] = items[j]->hasExtra.first;
                root[name]["Items"][itemName]["numEnch"] = items[j]->numEnch;
                root[name]["Items"][itemName]["extraData_first"] = items[j]->hasExtra.first ? items[j]->extraData.first->GetFormID() : 0;
                root[name]["Items"][itemName]["hasExtra_second"] = items[j]->hasExtra.second;
                root[name]["Items"][itemName]["extraData_second"] = items[j]->extraData.second;
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
            
            root[name]["mOrder"] = equipset->mOrder;
            root[name]["mName"] = equipset->mName;

            auto hotkey = equipset->mHotkey;
            root[name]["mKeyCode"] = hotkey->mKeyCode;
            root[name]["mModifier1"] = hotkey->mModifier[0];
            root[name]["mModifier2"] = hotkey->mModifier[1];
            root[name]["mModifier3"] = hotkey->mModifier[2];

            auto option = equipset->mOption;
            root[name]["mPersist"] = option->mPersist;
            root[name]["mExpire"] = option->mExpire;
            root[name]["mReset"] = option->mReset;
            root[name]["mBeast"] = option->mBeast;

            auto widget = equipset->mWidget;
            root[name]["mHpos"] = widget->mHpos;
            root[name]["mVpos"] = widget->mVpos;
            root[name]["mDisplayWidget"] = widget->mDisplayWidget;
            root[name]["mDisplayName"] = widget->mDisplayName;
            root[name]["mDisplayHotkey"] = widget->mDisplayHotkey;

            auto& items = equipset->mCycleItems;
            root[name]["mNumItems"] = items.size();
            for (int j = 0; j < items.size(); j++) {
                auto itemName = "Item_" + std::to_string(j);
                root[name]["Items"][itemName]["name"] = items[j];
            }

            logger::debug("'{}' exported.", equipset->mName);
        }

        logger::info("Total {} Equipsets exported.", size);
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
        return ;
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

            int32_t mOrder = root[name].get("mOrder", 0).asInt();
            std::string mName = root[name].get("mName", "").asString();

            logger::trace("Loading '{}'...", mName);

            Hotkey* hotkey = new Hotkey;
            hotkey->mKeyCode = root[name].get("mKeyCode", -1).asInt();
            hotkey->mModifier[0] = root[name].get("mModifier1", false).asBool();
            hotkey->mModifier[1] = root[name].get("mModifier2", false).asBool();
            hotkey->mModifier[2] = root[name].get("mModifier3", false).asBool();

            Option* option = new Option;
            option->mSound = root[name].get("mSound", false).asBool();
            option->mToggleEquip = root[name].get("mToggleEquip", false).asBool();
            option->mReEquip = root[name].get("mReEquip", false).asBool();
            option->mBeast = root[name].get("mBeast", false).asBool();

            Widget* widget = new Widget;
            widget->mWidget = root[name].get("mWidget", "").asString();
            widget->mHpos = root[name].get("mHpos", 0).asInt();
            widget->mVpos = root[name].get("mVpos", 0).asInt();
            widget->mDisplayWidget = root[name].get("mDisplayWidget", false).asBool();
            widget->mDisplayName = root[name].get("mDisplayName", false).asBool();
            widget->mDisplayHotkey = root[name].get("mDisplayHotkey", false).asBool();

            Equipment* equipment = new Equipment;
            {
                auto left = equipment->mLeft;
                left->option = root[name].get("lOption", 0).asInt();
                RE::FormID formID = root[name].get("lForm", 0).asInt64();
                left->form = left->option == 2 && formID != 0 ? RE::TESForm::LookupByID<RE::TESForm>(formID) : nullptr;
                left->hasExtra.first = root[name].get("lHasExtra_first", false).asBool();
                left->numEnch = root[name].get("lNumEnch", 0).asInt();
                RE::FormID enchantID = root[name].get("lExtraData_first", 0).asInt64();
                RE::EnchantmentItem* enchantForm = left->hasExtra.first && enchantID != 0 ? RE::TESForm::LookupByID<RE::EnchantmentItem>(enchantID) : nullptr;
                left->extraData.first = left->hasExtra.first && enchantForm ? enchantForm : nullptr;
                left->hasExtra.second = root[name].get("lHasExtra_second", false).asBool();
                left->extraData.second = root[name].get("lExtraData_second", 0.0f).asFloat();
                RE::ExtraDataList* xList;
                xList = Extra::SearchExtraList(left->form, left->numEnch, left->extraData.first, left->extraData.second);
                left->xList = xList;

                if (!left->form) {
                    left->option = static_cast<int32_t>(MCM::eAction::Nothing);
                    logger::error("[{}] Failed to load Lefthand data.", mName);
                }
            }
            {
                auto right = equipment->mRight;
                right->option = root[name].get("rOption", 0).asInt();
                RE::FormID formID = root[name].get("rForm", 0).asInt64();
                right->form = right->option == 2 && formID != 0 ? RE::TESForm::LookupByID<RE::TESForm>(formID) : nullptr;
                right->hasExtra.first = root[name].get("rHasExtra_first", false).asBool();
                right->numEnch = root[name].get("rNumEnch", 0).asInt();
                RE::FormID enchantID = root[name].get("rExtraData_first", 0).asInt64();
                RE::EnchantmentItem* enchantForm = right->hasExtra.first && enchantID != 0 ? RE::TESForm::LookupByID<RE::EnchantmentItem>(enchantID) : nullptr;
                right->extraData.first = right->hasExtra.first && enchantForm  ? enchantForm : nullptr;
                right->hasExtra.second = root[name].get("rHasExtra_second", false).asBool();
                right->extraData.second = root[name].get("rExtraData_second", 0.0f).asFloat();
                RE::ExtraDataList* xList;
                xList = Extra::SearchExtraList(right->form, right->numEnch, right->extraData.first, right->extraData.second);
                right->xList = xList;

                if (!right->form) {
                    right->option = static_cast<int32_t>(MCM::eAction::Nothing);
                    logger::error("[{}] Failed to load Righthand data.", mName);
                }
            }
            {
                auto shout = equipment->mShout;
                shout->option = root[name].get("sOption", 0).asInt();
                RE::FormID formID = root[name].get("sForm", 0).asInt();
                shout->form = shout->option == 2 && formID != 0 ? RE::TESForm::LookupByID<RE::TESForm>(formID) : nullptr;

                if (!shout->form) {
                    shout->option = static_cast<int32_t>(MCM::eAction::Nothing);
                    logger::error("[{}] Failed to load Shout data.", mName);
                }
            }

            auto& items = equipment->mItems;
            int numItems = root[name].get("mNumItems", 0).asInt();
            for (int j = 0; j < numItems; j++) {
                Equipment::Items* item = new Equipment::Items;

                auto itemName = "Item_" + std::to_string(j);
                RE::FormID formID = root[name]["Items"][itemName].get("form", 0).asInt64();
                RE::TESForm* form = formID != 0 ? RE::TESForm::LookupByID<RE::TESForm>(formID) : nullptr;
                if (form) {
                    item->form = form;
                }
                bool hasItemsExtraEnch = root[name]["Items"][itemName].get("hasExtra_first", false).asBool();
                uint32_t numItemsEnch = root[name]["Items"][itemName].get("numEnch", 0).asInt();
                item->numEnch = numItemsEnch;
                RE::FormID enchantID = root[name]["Items"][itemName].get("extraData_first", 0).asInt64();
                RE::EnchantmentItem* enchantForm = hasItemsExtraEnch && enchantID != 0 ? RE::TESForm::LookupByID<RE::EnchantmentItem>(enchantID) : nullptr;
                bool hasItemsExtraTemp = root[name]["Items"][itemName].get("hasExtra_second", false).asBool();
                item->hasExtra = std::make_pair(hasItemsExtraEnch, hasItemsExtraTemp);
                float itemsExtraTemp = hasItemsExtraTemp ? root[name]["Items"][itemName].get("extraData_second", 0).asFloat() : 0.0f;
                item->extraData = std::make_pair(enchantForm, itemsExtraTemp);
                RE::ExtraDataList* xList;
                xList = Extra::SearchExtraList(item->form, item->numEnch, item->extraData.first, item->extraData.second);
                item->xList = xList;

                if (form) {
                    items.push_back(item);
                }
                else {
                    delete item;
                    logger::error("[{}] Failed to load Items data.", mName);
                }
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

            int32_t mOrder = root[name].get("mOrder", 0).asInt();
            std::string mName = root[name].get("mName", "").asString();

            logger::trace("Loading '{}'...", mName);

            Hotkey* hotkey = new Hotkey;
            hotkey->mKeyCode = root[name].get("mKeyCode", -1).asInt();
            hotkey->mModifier[0] = root[name].get("mModifier1", false).asBool();
            hotkey->mModifier[1] = root[name].get("mModifier2", false).asBool();
            hotkey->mModifier[2] = root[name].get("mModifier3", false).asBool();

            CycleOption* option = new CycleOption;
            option->mPersist = root[name].get("mPersist", false).asBool();
            option->mExpire = root[name].get("mExpire", false).asBool();
            option->mReset = root[name].get("mReset", false).asBool();
            option->mBeast = root[name].get("mBeast", false).asBool();

            Widget* widget = new Widget;
            widget->mHpos = root[name].get("mHpos", 0).asInt();
            widget->mVpos = root[name].get("mVpos", 0).asInt();
            widget->mDisplayWidget = root[name].get("mDisplayWidget", false).asBool();
            widget->mDisplayName = root[name].get("mDisplayName", false).asBool();
            widget->mDisplayHotkey = root[name].get("mDisplayHotkey", false).asBool();

            std::vector<std::string> items;
            size_t itemsSize = root[name].get("mNumItems", 0).asInt();
            for (int j = 0; j < itemsSize; j++) {
                auto itemName = "Item_" + std::to_string(j);
                std::string cycleItemsName = root[name]["Items"][itemName].get("name", 0).asString();
                items.push_back(cycleItemsName);
            }

            manager->NewCycleEquipset(mOrder, mName, hotkey, option, widget, items, std::make_pair(0, -1));

            logger::debug("'{}' loaded.", mName);
        }

        logger::info("Total {} Cycle Equipsets loaded.", size);
    }
}