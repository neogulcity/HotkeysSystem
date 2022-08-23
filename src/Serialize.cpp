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
}  // namespace UIHS

void EquipsetManager::OnRevert(SerializationInterface*)
{
    std::unique_lock lock(GetSingleton()._lock);
}

void EquipsetManager::OnGameSaved(SerializationInterface* serde)
{
    std::unique_lock lock(GetSingleton()._lock);
    
    // To write data open a record with a given name. The name must be unique within your mod, and has a version number
    // assigned (in this case 0). You can increase the version number if making breaking format change from a previous
    // version of your mod, so that the load handler can know which version of the format is being used.
    if (!serde->OpenRecord(EquipsetRecord, 0)) {
        log::error("Unable to open record to write cosave data.");
        return;
    }
    
    auto manager = &GetSingleton();
    if (!manager) {
        log::error("Unable to get EquipsetManager");
        return;
    }
    // First, write the number of items that will be written in this record. That way when we load the data, we know how
    // many times to iterate reading in items.
    size_t equipsetSize = manager->mEquipset.size();
    serde->WriteRecordData(&equipsetSize, sizeof(equipsetSize));
    for (int i = 0; i < equipsetSize; i++) {
        auto equipset = manager->mEquipset[i];
 
        // Equipset::mOrder
        serde->WriteRecordData(&equipset->mOrder, sizeof(equipset->mOrder));                                            // Equipset::mOrder

        // Equipset::mName
        size_t nameSize = equipset->mName.length();                                                                     // Equipset::mName.length()
        serde->WriteRecordData(&nameSize, sizeof(nameSize));
        for (auto& elem : equipset->mName) {
            serde->WriteRecordData(&elem, sizeof(elem));                                                                // Equipset::mName
        }

        // Equipset::Hotkey
        auto hotkey = equipset->mHotkey;
        serde->WriteRecordData(&hotkey->mKeyCode, sizeof(hotkey->mKeyCode));                                            // Equipset::Hotkey::mKeyCode
        serde->WriteRecordData(&hotkey->mModifier[0], sizeof(hotkey->mModifier[0]));                                    // Equipset::Hotkey::mModifier[0]
        serde->WriteRecordData(&hotkey->mModifier[1], sizeof(hotkey->mModifier[1]));                                    // Equipset::Hotkey::mModifier[1]
        serde->WriteRecordData(&hotkey->mModifier[2], sizeof(hotkey->mModifier[2]));                                    // Equipset::Hotkey::mModifier[2]

        // Equipset::Option
        auto option = equipset->mOption;
        serde->WriteRecordData(&option->mSound, sizeof(option->mSound));                                                // Equipset::Option::mSound
        serde->WriteRecordData(&option->mToggleEquip, sizeof(option->mToggleEquip));                                    // Equipset::Option::mToggleEquip
        serde->WriteRecordData(&option->mReEquip, sizeof(option->mReEquip));                                            // Equipset::Option::mReEquip
        serde->WriteRecordData(&option->mBeast, sizeof(option->mBeast));                                                // Equipset::Option::mBeast

        // Equipset::Widget
        auto widget = equipset->mWidget;

        // Equipset::Widget::mWidget
        size_t widgetSize = widget->mWidget.length();                                                                   // Equipset::Widget::mWidget.length()
        serde->WriteRecordData(&widgetSize, sizeof(widgetSize));
        for (auto& elem : widget->mWidget) {
            serde->WriteRecordData(&elem, sizeof(elem));                                                                // Equipset::Widget::mWidget
        }
        serde->WriteRecordData(&widget->mHpos, sizeof(widget->mHpos));                                                  // Equipset::Widget::mHpos
        serde->WriteRecordData(&widget->mVpos, sizeof(widget->mVpos));                                                  // Equipset::Widget::mVpos
        serde->WriteRecordData(&widget->mDisplayWidget, sizeof(widget->mDisplayWidget));                                // Equipset::Widget::mDisplayWidget
        serde->WriteRecordData(&widget->mDisplayName, sizeof(widget->mDisplayName));                                    // Equipset::Widget::mDisplayName
        serde->WriteRecordData(&widget->mDisplayHotkey, sizeof(widget->mDisplayHotkey));                                // Equipset::Widget::mDisplayHotkey

        // Equipset::Equipment
        auto equipment = equipset->mEquipment;
        auto& left = equipment->mLeft;
        serde->WriteRecordData(&left.option, sizeof(left.option));                                                      // Equipset::Equipment::Left::option
        if (left.option == static_cast<int32_t>(MCM::eAction::Equip)) {
            serde->WriteRecordData(&left.form->formID, sizeof(left.form->formID));                                      // Equipset::Equipment::Left::form
            serde->WriteRecordData(&left.hasExtra.first, sizeof(left.hasExtra.first));                                  // Equipset::Equipment::Left::hasExtra.first
            if (left.hasExtra.first) {
                serde->WriteRecordData(&left.numEnch, sizeof(left.numEnch));                                            // Equipset::Equipment::Left::numEnch
                RE::FormID ID = left.extraData.first->GetFormID();
                serde->WriteRecordData(&ID, sizeof(ID));                                                                // Equipset::Equipment::Left::extraData.first
            }
            serde->WriteRecordData(&left.hasExtra.second, sizeof(left.hasExtra.second));                                // Equipset::Equipment::Left::hasExtra.second
            if (left.hasExtra.second) {
                serde->WriteRecordData(&left.extraData.second, sizeof(left.extraData.second));                          // Equipset::Equipment::Left::extraData.second
            }
        }

        auto& right = equipment->mRight;
        serde->WriteRecordData(&right.option, sizeof(right.option));                                                    // Equipset::Equipment::Right::option
        if (right.option == static_cast<int32_t>(MCM::eAction::Equip)) {
            serde->WriteRecordData(&right.form->formID, sizeof(right.form->formID));                                    // Equipset::Equipment::Right::form
            serde->WriteRecordData(&right.hasExtra.first, sizeof(right.hasExtra.first));                                // Equipset::Equipment::Right::hasExtra.first
            if (right.hasExtra.first) {
                serde->WriteRecordData(&right.numEnch, sizeof(right.numEnch));                                          // Equipset::Equipment::Right::numEnch
                RE::FormID ID = right.extraData.first->GetFormID();
                serde->WriteRecordData(&ID, sizeof(ID));                                                                // Equipset::Equipment::Right::extraData.first
            }
            serde->WriteRecordData(&right.hasExtra.second, sizeof(right.hasExtra.second));                              // Equipset::Equipment::Right::hasExtra.second
            if (right.hasExtra.second) {
                serde->WriteRecordData(&right.extraData.second, sizeof(right.extraData.second));                        // Equipset::Equipment::Right::extraData.second
            }
        }

        auto& shout = equipment->mShout;
        serde->WriteRecordData(&shout.option, sizeof(shout.option));                                                    // Equipset::Equipment::Shout::option
        if (shout.option == static_cast<int32_t>(MCM::eAction::Equip)) {
            serde->WriteRecordData(&shout.form->formID, sizeof(shout.form->formID));                                    // Equipset::Equipment::Shout::form
        }

        auto& items = equipment->mItems;
        uint32_t itemsSize = items.numItems;
        serde->WriteRecordData(&itemsSize, sizeof(itemsSize));                                                          // Equipset::Equipment::Items::numItems
        for (int j = 0; j < itemsSize; j++) {
            serde->WriteRecordData(&items.form[j]->formID, sizeof(items.form[j]->formID));                              // Equipset::Equipment::Items::form
            serde->WriteRecordData(&items.hasExtra[j].first, sizeof(items.hasExtra[j].first));                          // Equipset::Equipment::Items::hasExtra.first
            if (items.hasExtra[j].first) {
                serde->WriteRecordData(&items.numEnch[j], sizeof(items.numEnch[j]));                                    // Equipset::Equipment::Items::numEnch
                RE::FormID ID = items.extraData[j].first->GetFormID();
                serde->WriteRecordData(&ID, sizeof(ID));                                                                // Equipset::Equipment::Items::extraData.first
            }
            serde->WriteRecordData(&items.hasExtra[j].second, sizeof(items.hasExtra[j].second));                        // Equipset::Equipment::Items::hasExtra.second
            if (items.hasExtra[j].second) {
                serde->WriteRecordData(&items.extraData[j].second, sizeof(items.extraData[j].second));                  // Equipset::Equipment::Items::extraData.second
            }
        }
    }
    
    if (!serde->OpenRecord(CycleEquipsetRecord, 0)) {
        log::error("Unable to open record to write cosave data.");
        return;
    }
    
    // First, write the number of items that will be written in this record. That way when we load the data, we know how
    // many times to iterate reading in items.
    size_t cycleEquipsetSize = manager->mCycleEquipset.size();
    serde->WriteRecordData(&cycleEquipsetSize, sizeof(cycleEquipsetSize));
    for (int i = 0; i < cycleEquipsetSize; i++) {
        auto equipset = manager->mCycleEquipset[i];

        // CycleEquipset::mOrder
        serde->WriteRecordData(&equipset->mOrder, sizeof(equipset->mOrder));                                            // CycleEquipset::mOrder
 
        // CycleEquipset::mName
        size_t nameSize = equipset->mName.length();                                                                     // CycleEquipset::mName.length()
        serde->WriteRecordData(&nameSize, sizeof(nameSize));
        for (auto& elem : equipset->mName) {
            serde->WriteRecordData(&elem, sizeof(elem));                                                                // CycleEquipset::mName
        }

        // CycleEquipset::Hotkey
        auto hotkey = equipset->mHotkey;
        serde->WriteRecordData(&hotkey->mKeyCode, sizeof(hotkey->mKeyCode));                                            // CycleEquipset::Hotkey::mKeyCode
        serde->WriteRecordData(&hotkey->mModifier[0], sizeof(hotkey->mModifier[0]));                                    // CycleEquipset::Hotkey::mModifier[0]
        serde->WriteRecordData(&hotkey->mModifier[1], sizeof(hotkey->mModifier[1]));                                    // CycleEquipset::Hotkey::mModifier[1]
        serde->WriteRecordData(&hotkey->mModifier[2], sizeof(hotkey->mModifier[2]));                                    // CycleEquipset::Hotkey::mModifier[2]

        // CycleEquipset::CycleOption
        auto option = equipset->mOption;
        serde->WriteRecordData(&option->mPersist, sizeof(option->mPersist));                                            // CycleEquipset::CycleOption::mPersist
        serde->WriteRecordData(&option->mExpire, sizeof(option->mExpire));                                              // CycleEquipset::CycleOption::mExpire
        serde->WriteRecordData(&option->mReset, sizeof(option->mReset));                                                // CycleEquipset::CycleOption::mReset
        serde->WriteRecordData(&option->mBeast, sizeof(option->mBeast));                                                // CycleEquipset::CycleOption::mBeast

        // CycleEquipset::Widget
        auto widget = equipset->mWidget;
        serde->WriteRecordData(&widget->mHpos, sizeof(widget->mHpos));                                                  // CycleEquipset::Widget::mHpos
        serde->WriteRecordData(&widget->mVpos, sizeof(widget->mVpos));                                                  // CycleEquipset::Widget::mVpos
        serde->WriteRecordData(&widget->mDisplayWidget, sizeof(widget->mDisplayWidget));                                // CycleEquipset::Widget::mDisplayWidget
        serde->WriteRecordData(&widget->mDisplayName, sizeof(widget->mDisplayName));                                    // CycleEquipset::Widget::mDisplayName
        serde->WriteRecordData(&widget->mDisplayHotkey, sizeof(widget->mDisplayHotkey));                                // CycleEquipset::Widget::mDisplayHotkey

        // CycleEquipset::MCM::CycleItems
        std::vector<std::string> items = equipset->mCycleItems;

        size_t itemsSize = items.size();
        serde->WriteRecordData(&itemsSize, sizeof(itemsSize));                                                          // CycleEquipset::CycleItems.size()
        for (int j = 0; j < itemsSize; j++) {
            size_t size = items[j].length();                                                                            // CycleEquipset::CycleItems[].length()
            serde->WriteRecordData(&size, sizeof(size));                                                                
            for (auto& elem : items[j]) {
                serde->WriteRecordData(&elem, sizeof(elem));                                                            // CycleEquipset::CycleItems[] name
            }
        }
        serde->WriteRecordData(&equipset->mCycleIndex.first, sizeof(equipset->mCycleIndex.first));                      // CycleEquipset::CycleIndex.first
        serde->WriteRecordData(&equipset->mCycleIndex.second, sizeof(equipset->mCycleIndex.second));                    // CycleEquipset::CycleIndex.second
    }
}


void EquipsetManager::OnGameLoaded(SerializationInterface* serde) {
    std::uint32_t type;
    std::uint32_t size;
    std::uint32_t version;
    // To load records from a cosave, use <code>GetNextRecordInfo</code> to iterate from one record to the next.
    // You will be given records in the order they were written, but otherwise you do not look up a record by its name.
    // Instead check the result of each iteration to find out which record it is and handle it appropriately.
    //
    // If you make breaking changes to your data format, you can increase the version number used when writing the data
    // out and check that number here to handle previous versions.
    while (serde->GetNextRecordInfo(type, version, size)) {
        if (type == EquipsetRecord) {
            // First read how many items follow in this record, so we know how many times to iterate.
            size_t equipsetSize;
            serde->ReadRecordData(&equipsetSize, sizeof(equipsetSize));
            // Iterate over the remaining data in the record.
            for (int i = 0; i < equipsetSize; i++) {
                // Equipset::mOrder
                uint32_t order;
                serde->ReadRecordData(&order, sizeof(order));                                                                  // Equipset::mOrder

                // Equipset::mName
                std::string mName;
                size_t nameSize;
                serde->ReadRecordData(&nameSize, sizeof(nameSize));                                                            // Equipset::mName.length()
                for (; nameSize > 0; --nameSize) {
                    char name;
                    serde->ReadRecordData(&name, sizeof(name));                                                                // Equipset::mName
                    mName += name;
                }

                // Equipset::Hotkey
                Hotkey* hotkey = new Hotkey;
                serde->ReadRecordData(&hotkey->mKeyCode, sizeof(hotkey->mKeyCode));                                            // Equipset::Hotkey::mKeyCode
                serde->ReadRecordData(&hotkey->mModifier[0], sizeof(hotkey->mModifier[0]));                                    // Equipset::Hotkey::mModifier[0]
                serde->ReadRecordData(&hotkey->mModifier[1], sizeof(hotkey->mModifier[1]));                                    // Equipset::Hotkey::mModifier[1]
                serde->ReadRecordData(&hotkey->mModifier[2], sizeof(hotkey->mModifier[2]));                                    // Equipset::Hotkey::mModifier[2]

                // Equipset::Option
                Option* option = new Option;
                serde->ReadRecordData(&option->mSound, sizeof(option->mSound));                                                // Equipset::Option::mSound
                serde->ReadRecordData(&option->mToggleEquip, sizeof(option->mToggleEquip));                                    // Equipset::Option::mToggleEquip
                serde->ReadRecordData(&option->mReEquip, sizeof(option->mReEquip));                                            // Equipset::Option::mReEquip
                serde->ReadRecordData(&option->mBeast, sizeof(option->mBeast));                                                // Equipset::Option::mBeast

                // Equipset::Widget
                Widget* widget = new Widget;

                size_t WidgetSize;
                serde->ReadRecordData(&WidgetSize, sizeof(WidgetSize));                                                        // Equipset::Widget::mWidget->length()
                for (; WidgetSize > 0; --WidgetSize) {
                    char name;
                    serde->ReadRecordData(&name, sizeof(name));                                                                // Equipset::Widget::mWidget
                    widget->mWidget += name;
                }
                serde->ReadRecordData(&widget->mHpos, sizeof(widget->mHpos));                                                  // Equipset::Widget::mHpos
                serde->ReadRecordData(&widget->mVpos, sizeof(widget->mVpos));                                                  // Equipset::Widget::mVpos
                serde->ReadRecordData(&widget->mDisplayWidget, sizeof(widget->mDisplayWidget));                                // Equipset::Widget::mDisplayWidget
                serde->ReadRecordData(&widget->mDisplayName, sizeof(widget->mDisplayName));                                    // Equipset::Widget::mDisplayName
                serde->ReadRecordData(&widget->mDisplayHotkey, sizeof(widget->mDisplayHotkey));                                // Equipset::Widget::mDisplayHotkey

                // Equipset::Equipment
                Equipment* equipment = new Equipment;
                auto& left = equipment->mLeft;
                serde->ReadRecordData(&left.option, sizeof(left.option));                                                      // Equipset::Equipment::Left::option
                if (left.option == static_cast<int32_t>(MCM::eAction::Equip)) {
                    RE::FormID ID, newID;
                    serde->ReadRecordData(&ID, sizeof(ID));                                                                    // Equipset::Equipment::Left::form
                    if (!serde->ResolveFormID(ID, newID)) {
                        log::warn("Form ID {:X} could not be found after loading the save.", ID);
                        continue;
                    }
                    left.form = RE::TESForm::LookupByID<RE::TESForm>(newID);
                    serde->ReadRecordData(&left.hasExtra.first, sizeof(left.hasExtra.first));                                  // Equipset::Equipment::Left::hasExtra.first
                    if (left.hasExtra.first) {
                        serde->ReadRecordData(&left.numEnch, sizeof(left.numEnch));                                            // Equipset::Equipment::Left::numEnch
                        RE::FormID ExtraID, newExtraID;
                        serde->ReadRecordData(&ExtraID, sizeof(ExtraID));                                                      // Equipset::Equipment::Left::extraData.first
                        if (!serde->ResolveFormID(ExtraID, newExtraID)) {
                            log::warn("Form ID {:X} could not be found after loading the save.", ExtraID);
                            continue;
                        }
                        left.extraData.first = RE::TESForm::LookupByID<RE::EnchantmentItem>(newExtraID);
                    }
                    serde->ReadRecordData(&left.hasExtra.second, sizeof(left.hasExtra.second));                                // Equipset::Equipment::Left::hasExtra.second
                    if (left.hasExtra.second) {
                        serde->ReadRecordData(&left.extraData.second, sizeof(left.extraData.second));                          // Equipset::Equipment::Left::extraData.second
                    }
                    RE::ExtraDataList* xList;
                    xList = Extra::SearchExtraList(left.form, left.numEnch, left.extraData.first, left.extraData.second);
                    left.xList = xList;                                                                                        // Equipset::Equipment::Left::xList
                }

                auto& right = equipment->mRight;
                serde->ReadRecordData(&right.option, sizeof(right.option));                                                    // Equipset::Equipment::Right::option
                if (right.option == static_cast<int32_t>(MCM::eAction::Equip)) {
                    RE::FormID ID, newID;
                    serde->ReadRecordData(&ID, sizeof(ID));                                                                    // Equipset::Equipment::Right::form
                    if (!serde->ResolveFormID(ID, newID)) {
                        log::warn("Form ID {:X} could not be found after loading the save.", ID);
                        continue;
                    }
                    right.form = RE::TESForm::LookupByID<RE::TESForm>(newID);
                    serde->ReadRecordData(&right.hasExtra.first, sizeof(right.hasExtra.first));                                // Equipset::Equipment::Right::hasExtra.first
                    if (right.hasExtra.first) {
                        serde->ReadRecordData(&right.numEnch, sizeof(right.numEnch));                                          // Equipset::Equipment::Right::numEnch
                        RE::FormID ExtraID, newExtraID;
                        serde->ReadRecordData(&ExtraID, sizeof(ExtraID));                                                      // Equipset::Equipment::Right::extraData.first
                        if (!serde->ResolveFormID(ExtraID, newExtraID)) {
                            log::warn("Form ID {:X} could not be found after loading the save.", ExtraID);
                            continue;
                        }
                        right.extraData.first = RE::TESForm::LookupByID<RE::EnchantmentItem>(newExtraID);
                    }
                    serde->ReadRecordData(&right.hasExtra.second, sizeof(right.hasExtra.second));                              // Equipset::Equipment::Right::hasExtra.second
                    if (right.hasExtra.second) {
                        serde->ReadRecordData(&right.extraData.second, sizeof(right.extraData.second));                        // Equipset::Equipment::Right::extraData.second
                    }
                    RE::ExtraDataList* xList;
                    xList = Extra::SearchExtraList(right.form, right.numEnch, right.extraData.first, right.extraData.second);
                    right.xList = xList;                                                                                       // Equipset::Equipment::Right::xList
                }

                auto& shout = equipment->mShout;
                serde->ReadRecordData(&shout.option, sizeof(shout.option));                                                    // Equipset::Equipment::Shout::option
                if (shout.option == static_cast<int32_t>(MCM::eAction::Equip)) {
                    RE::FormID ID, newID;
                    serde->ReadRecordData(&ID, sizeof(ID));                                                                    // Equipset::Equipment::Shout::form
                    if (!serde->ResolveFormID(ID, newID)) {
                        log::warn("Form ID {:X} could not be found after loading the save.", ID);
                        continue;
                    }
                    shout.form = RE::TESForm::LookupByID<RE::TESForm>(newID);
                }

                auto& items = equipment->mItems;
                uint32_t itemsSize;
                serde->ReadRecordData(&itemsSize, sizeof(itemsSize));                                                          // Equipset::Equipment::Items::numItems
                items.numItems = itemsSize;                
                for (int j = 0; j < itemsSize; j++) {
                    RE::FormID ID, newID;
                    serde->ReadRecordData(&ID, sizeof(ID));                                                                    // Equipset::Equipment::Items::form
                    if (!serde->ResolveFormID(ID, newID)) {
                        log::warn("Form ID {:X} could not be found after loading the save.", ID);
                        continue;
                    }
                    items.form.push_back(RE::TESForm::LookupByID<RE::TESForm>(newID));

                    bool hasItemsExtraEnch;
                    RE::EnchantmentItem* itemsExtraEnch;
                    serde->ReadRecordData(&hasItemsExtraEnch, sizeof(hasItemsExtraEnch));                                      // Equipset::Equipment::Items::hasExtra.first
                    if (hasItemsExtraEnch) {
                        uint32_t numItemsEnch;
                        serde->ReadRecordData(&numItemsEnch, sizeof(numItemsEnch));                                            // Equipset::Equipment::Items::numEnch
                        items.numEnch.push_back(numItemsEnch);

                        RE::FormID ExtraID, newExtraID;
                        serde->ReadRecordData(&ExtraID, sizeof(ExtraID));                                                      // Equipset::Equipment::Items::extraData.first
                        if (!serde->ResolveFormID(ExtraID, newExtraID)) {
                            log::warn("Form ID {:X} could not be found after loading the save.", ExtraID);
                            continue;
                        }
                        itemsExtraEnch = RE::TESForm::LookupByID<RE::EnchantmentItem>(newExtraID);
                    }
                    bool hasItemsExtraTemp;
                    float itemsExtraTemp;
                    serde->ReadRecordData(&hasItemsExtraTemp, sizeof(hasItemsExtraTemp));                                      // Equipset::Equipment::Items::hasExtra.second
                    items.hasExtra.push_back(std::make_pair(hasItemsExtraEnch, hasItemsExtraTemp));
                    if (hasItemsExtraTemp) {
                        serde->ReadRecordData(&itemsExtraTemp, sizeof(itemsExtraTemp));                                        // Equipset::Equipment::Items::extraData.second
                    }
                    items.extraData.push_back(std::make_pair(itemsExtraEnch, itemsExtraTemp));

                    RE::ExtraDataList* xList;
                    xList = Extra::SearchExtraList(items.form[j], items.numEnch[j], items.extraData[j].first, items.extraData[j].second);
                    items.xList.push_back(xList);                                                                              // Equipset::Equipment::Items::xList
                }
                

                auto manager = &GetSingleton();
                if (!manager) {
                    log::error("Unable to get EquipsetManager");
                    return;
                }

                manager->NewEquipset(order, mName, hotkey, option, widget, equipment);
            }
        }
        if (type == CycleEquipsetRecord) {
            // First read how many items follow in this record, so we know how many times to iterate.
            size_t equipsetSize;
            serde->ReadRecordData(&equipsetSize, sizeof(equipsetSize));
            // Iterate over the remaining data in the record.
            for (int i = 0; i < equipsetSize; i++) {
                // CycleEquipset::mOrder
                uint32_t order;
                serde->ReadRecordData(&order, sizeof(order));                                                                  // CycleEquipset::mOrder

                // CycleEquipset::mName
                std::string mName;
                size_t nameSize;
                serde->ReadRecordData(&nameSize, sizeof(nameSize));                                                            // CycleEquipset::mName.length()
                for (; nameSize > 0; --nameSize) {
                    char name;
                    serde->ReadRecordData(&name, sizeof(name));                                                                // CycleEquipset::mName
                    mName += name;
                }

                // CycleEquipset::Hotkey
                Hotkey* hotkey = new Hotkey;
                serde->ReadRecordData(&hotkey->mKeyCode, sizeof(hotkey->mKeyCode));                                            // CycleEquipset::Hotkey::mKeyCode
                serde->ReadRecordData(&hotkey->mModifier[0], sizeof(hotkey->mModifier[0]));                                    // CycleEquipset::Hotkey::mModifier[0]
                serde->ReadRecordData(&hotkey->mModifier[1], sizeof(hotkey->mModifier[1]));                                    // CycleEquipset::Hotkey::mModifier[1]
                serde->ReadRecordData(&hotkey->mModifier[2], sizeof(hotkey->mModifier[2]));                                    // CycleEquipset::Hotkey::mModifier[2]

                // CycleEquipset::CycleOption
                CycleOption* option = new CycleOption;
                serde->ReadRecordData(&option->mPersist, sizeof(option->mPersist));                                            // CycleEquipset::CycleOption::mPersist
                serde->ReadRecordData(&option->mExpire, sizeof(option->mExpire));                                              // CycleEquipset::CycleOption::mExpire
                serde->ReadRecordData(&option->mReset, sizeof(option->mReset));                                                // CycleEquipset::CycleOption::mReset
                serde->ReadRecordData(&option->mBeast, sizeof(option->mBeast));                                                // CycleEquipset::CycleOption::mBeast

                // CycleEquipset::Widget
                Widget* widget = new Widget;
                serde->ReadRecordData(&widget->mHpos, sizeof(widget->mHpos));                                                  // CycleEquipset::Widget::mHpos
                serde->ReadRecordData(&widget->mVpos, sizeof(widget->mVpos));                                                  // CycleEquipset::Widget::mVpos
                serde->ReadRecordData(&widget->mDisplayWidget, sizeof(widget->mDisplayWidget));                                // CycleEquipset::Widget::mDisplayWidget
                serde->ReadRecordData(&widget->mDisplayName, sizeof(widget->mDisplayName));                                    // CycleEquipset::Widget::mDisplayName
                serde->ReadRecordData(&widget->mDisplayHotkey, sizeof(widget->mDisplayHotkey));                                // CycleEquipset::Widget::mDisplayHotkey

                // CycleEquipset::CycleItems
                std::vector<std::string> items;
                size_t itemsSize;
                serde->ReadRecordData(&itemsSize, sizeof(itemsSize));                                                          // CycleEquipset::CycleItems.size()
                for (int j = 0; j < itemsSize; j++) {
                    std::string itemsName;
                    size_t inameSize;
                    serde->ReadRecordData(&inameSize, sizeof(inameSize));                                                      // CycleEquipset::CycleItems[].length()
                    for (; inameSize > 0; --inameSize) {
                        char name;
                        serde->ReadRecordData(&name, sizeof(name));                                                            // CycleEquipset::CycleItems[] name
                        itemsName += name;
                    }
                    items.push_back(itemsName);
                }
                uint32_t index;
                int32_t prevIndex;
                serde->ReadRecordData(&index, sizeof(index));                                                                  // CycleEquipset::CycleIndex.first
                serde->ReadRecordData(&prevIndex, sizeof(prevIndex));                                                          // CycleEquipset::CycleIndex.second

                auto manager = &GetSingleton();
                if (!manager) {
                    log::error("Unable to get EquipsetManager");
                    return;
                }

                manager->NewCycleEquipset(order, mName, hotkey, option, widget, items, std::make_pair(index, prevIndex));
            }
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
        for (int i = 0; i < size; i++) {
            auto name = "Equipset_" + std::to_string(i);
            auto equipset = mEquipset[i];
            
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

            auto& left = equipment->mLeft;
            root[name]["lOption"] = left.option;
            root[name]["lForm"] = left.option == 2 && left.form ? left.form->formID : 0;
            root[name]["lHasExtra_first"] = left.hasExtra.first;
            root[name]["lNumEnch"] = left.numEnch;
            root[name]["lExtraData_first"] = left.hasExtra.first ? left.extraData.first->GetFormID() : 0;
            root[name]["lHasExtra_second"] = left.hasExtra.second;
            root[name]["lExtraData_second"] = left.extraData.second;

            auto& right = equipment->mRight;
            root[name]["rOption"] = right.option;
            root[name]["rForm"] = right.option == 2 && right.form ? right.form->formID : 0;
            root[name]["rHasExtra_first"] = right.hasExtra.first;
            root[name]["rNumEnch"] = right.numEnch;
            root[name]["rExtraData_first"] = right.hasExtra.first ? right.extraData.first->GetFormID() : 0;
            root[name]["rHasExtra_second"] = right.hasExtra.second;
            root[name]["rExtraData_second"] = right.extraData.second;

            auto& shout = equipment->mShout;
            root[name]["sOption"] = shout.option;
            root[name]["sForm"] = shout.option == 2 && shout.form ? shout.form->formID : 0;

            auto& items = equipment->mItems;
            root[name]["mNumItems"] = items.numItems;
            for (int j = 0; j < items.numItems; j++) {
                auto itemName = "Item_" + std::to_string(j);
                root[name]["Items"][itemName]["form"] = items.form[j] ? items.form[j]->formID : 0;
                root[name]["Items"][itemName]["hasExtra_first"] = items.hasExtra[j].first;
                root[name]["Items"][itemName]["numEnch"] = items.numEnch[j];
                root[name]["Items"][itemName]["extraData_first"] = items.hasExtra[j].first ? items.extraData[j].first->GetFormID() : 0;
                root[name]["Items"][itemName]["hasExtra_second"] = items.hasExtra[j].second;
                root[name]["Items"][itemName]["extraData_second"] = items.extraData[j].second;
            }
        }
    }

    {
        auto size = GetCycleEquipsetList().size();
        root["CycleEquipset Size"] = size;
        for (int i = 0; i < size; i++) {
            auto name = "CycleEquipset_" + std::to_string(i);
            auto equipset = mCycleEquipset[i];
            
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
        }
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
        for (int i = 0; i < size; i++) {
            std::string name = "Equipset_" + std::to_string(i);

            int32_t mOrder = root[name].get("mOrder", 0).asInt();
            std::string mName = root[name].get("mName", "").asString();

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
                auto& left = equipment->mLeft;
                left.option = root[name].get("lOption", 0).asInt();
                RE::FormID formID = root[name].get("lForm", 0).asInt();
                left.form = left.option == 2 ? RE::TESForm::LookupByID<RE::TESForm>(formID) : nullptr;
                left.hasExtra.first = root[name].get("lHasExtra_first", false).asBool();
                left.numEnch = root[name].get("lNumEnch", 0).asInt();
                RE::FormID enchantID = root[name].get("lExtraData_first", 0).asInt();
                RE::EnchantmentItem* enchantForm = left.hasExtra.first ? RE::TESForm::LookupByID<RE::EnchantmentItem>(enchantID) : nullptr;
                left.extraData.first = left.hasExtra.first && enchantForm ? enchantForm : nullptr;
                left.hasExtra.second = root[name].get("lHasExtra_second", false).asBool();
                left.extraData.second = root[name].get("lExtraData_second", 0.0f).asFloat();
                RE::ExtraDataList* xList;
                xList = Extra::SearchExtraList(left.form, left.numEnch, left.extraData.first, left.extraData.second);
                left.xList = xList;
            }
            {
                auto& right = equipment->mRight;
                right.option = root[name].get("rOption", 0).asInt();
                RE::FormID formID = root[name].get("rForm", 0).asInt();
                right.form = right.option == 2 ? RE::TESForm::LookupByID<RE::TESForm>(formID) : nullptr;
                right.hasExtra.first = root[name].get("rHasExtra_first", false).asBool();
                right.numEnch = root[name].get("rNumEnch", 0).asInt();
                RE::FormID enchantID = root[name].get("rExtraData_first", 0).asInt();
                RE::EnchantmentItem* enchantForm = right.hasExtra.first ? RE::TESForm::LookupByID<RE::EnchantmentItem>(enchantID) : nullptr;
                right.extraData.first = right.hasExtra.first && enchantForm ? enchantForm : nullptr;
                right.hasExtra.second = root[name].get("rHasExtra_second", false).asBool();
                right.extraData.second = root[name].get("rExtraData_second", 0.0f).asFloat();
                RE::ExtraDataList* xList;
                xList = Extra::SearchExtraList(right.form, right.numEnch, right.extraData.first, right.extraData.second);
                right.xList = xList;
            }
            {
                auto& shout = equipment->mShout;
                shout.option = root[name].get("sOption", 0).asInt();
                RE::FormID formID = root[name].get("sForm", 0).asInt();
                shout.form = shout.option == 2 ? RE::TESForm::LookupByID<RE::TESForm>(formID) : nullptr;
            }

            auto& items = equipment->mItems;
            items.numItems = root[name].get("mNumItems", 0).asInt();
            for (int j = 0; j < items.numItems; j++) {
                auto itemName = "Item_" + std::to_string(j);
                RE::FormID formID = root[name]["Items"][itemName].get("form", 0).asInt();
                RE::TESForm* form = formID != 0 ? RE::TESForm::LookupByID<RE::TESForm>(formID) : nullptr;
                if (form) {
                    items.form.push_back(form);
                }
                bool hasItemsExtraEnch = root[name]["Items"][itemName].get("hasExtra_first", false).asBool();
                uint32_t numItemsEnch = root[name]["Items"][itemName].get("numEnch", 0).asInt();
                items.numEnch.push_back(numItemsEnch);
                RE::FormID enchantID = root[name]["Items"][itemName].get("extraData_first", 0).asInt();
                RE::EnchantmentItem* enchantForm = hasItemsExtraEnch ? RE::TESForm::LookupByID<RE::EnchantmentItem>(enchantID) : nullptr;
                bool hasItemsExtraTemp = root[name]["Items"][itemName].get("hasExtra_second", false).asBool();
                items.hasExtra.push_back(std::make_pair(hasItemsExtraEnch, hasItemsExtraTemp));
                float itemsExtraTemp = hasItemsExtraTemp ? root[name]["Items"][itemName].get("extraData_second", 0).asFloat() : 0.0f;
                items.extraData.push_back(std::make_pair(enchantForm, itemsExtraTemp));
                RE::ExtraDataList* xList;
                xList = Extra::SearchExtraList(items.form[j], items.numEnch[j], items.extraData[j].first, items.extraData[j].second);
                items.xList.push_back(xList);  
            }
            items.numItems = items.form.size();

            manager->NewEquipset(mOrder, mName, hotkey, option, widget, equipment);
        }
    }
    {
        size_t size;
        size = root.get("CycleEquipset Size", 0).asInt();
        for (int i = 0; i < size; i++) {
            std::string name = "CycleEquipset_" + std::to_string(i);

            int32_t mOrder = root[name].get("mOrder", 0).asInt();
            std::string mName = root[name].get("mName", "").asString();

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
        }
    }
}