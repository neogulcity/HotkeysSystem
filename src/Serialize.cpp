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

void EquipsetManager::OnRevert(SerializationInterface*) {
    std::unique_lock lock(GetSingleton()._lock);
}

void EquipsetManager::OnGameSaved(SerializationInterface* serde) {
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
        auto left = equipment->mLeft;
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

        auto right = equipment->mRight;
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

        auto shout = equipment->mShout;
        serde->WriteRecordData(&shout.option, sizeof(shout.option));                                                    // Equipset::Equipment::Shout::option
        if (shout.option == static_cast<int32_t>(MCM::eAction::Equip)) {
            serde->WriteRecordData(&shout.form->formID, sizeof(shout.form->formID));                                    // Equipset::Equipment::Shout::form
        }

        Equipment::Items items = equipment->mItems;
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
        serde->WriteRecordData(&equipset->mCycleIndex, sizeof(equipset->mCycleIndex));                                  // CycleEquipset::CycleIndex
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
                // Equipset::mName
                std::string mName;
                size_t nameSize;
                serde->ReadRecordData(&nameSize, sizeof(nameSize));                                                             // Equipset::mName.length()
                for (; nameSize > 0; --nameSize) {
                    char name;
                    serde->ReadRecordData(&name, sizeof(name));                                                                 // Equipset::mName
                    mName += name;
                }

                // Equipset::Hotkey
                Hotkey* hotkey = new Hotkey;
                serde->ReadRecordData(&hotkey->mKeyCode, sizeof(hotkey->mKeyCode));                                            // Equipset::Hotkey::mKeycode
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
                auto left = equipment->mLeft;
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

                auto right = equipment->mRight;
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

                auto shout = equipment->mShout;
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

                Equipment::Items items = equipment->mItems;
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

                manager->NewEquipset(mName, hotkey, option, widget, equipment);
            }
        }
        if (type == CycleEquipsetRecord) {
            // First read how many items follow in this record, so we know how many times to iterate.
            size_t equipsetSize;
            serde->ReadRecordData(&equipsetSize, sizeof(equipsetSize));
            // Iterate over the remaining data in the record.
            for (int i = 0; i < equipsetSize; i++) {
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
                serde->ReadRecordData(&hotkey->mKeyCode, sizeof(hotkey->mKeyCode));                                            // CycleEquipset::Hotkey::mKeycode
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
                serde->ReadRecordData(&index, sizeof(index));                                                                  // CycleEquipset::CycleIndex

                auto manager = &GetSingleton();
                if (!manager) {
                    log::error("Unable to get EquipsetManager");
                    return;
                }

                manager->NewCycleEquipset(mName, hotkey, option, widget, items, index);
            }
        }
        else {
            log::warn("Unknown record type in cosave.");
            __assume(false);
        }
    }
}