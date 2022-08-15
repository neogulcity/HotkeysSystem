#include "EquipsetManager.h"
#include "MCM.h"

using namespace RE;
using namespace UIHS;
using namespace SKSE;

namespace UIHS {
    // These four-character record types, which store data in the SKSE cosave, are little-endian. That means they are
    // reversed from the character order written here. Using the byteswap functions reverses them back to the order
    // the characters are written in the source code.
    inline const auto EquipsetRecord = _byteswap_ulong('HSER');
    inline const auto CycleEquipsetRecord = _byteswap_ulong('HSCR');
}

EquipsetManager& EquipsetManager::GetSingleton() noexcept {
    static EquipsetManager instance;
    return instance;
}

void EquipsetManager::NewEquipset(std::string _name, MCM::Hotkey* _hotkey, MCM::Option* _option, MCM::Widget* _widget, MCM::Equipment* _equipment) {
    mEquipset.push_back(new Equipset(_name, _hotkey, _option, _widget, _equipment));
}

void EquipsetManager::Display() {
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

        auto widget = equipset->mWidget;
        log::debug("Icon Type: {}", widget->mIcon);
        log::debug("H Pos: {}", widget->mHpos);
        log::debug("V Pos: {}", widget->mVpos);
        log::debug("Display Icon: {}", widget->mDisplayIcon);
        log::debug("Display Name: {}", widget->mDisplayName);
        log::debug("Display Hotkey: {}", widget->mDisplayHotkey);

        auto equipment = equipset->mEquipment;
        log::debug("Lefthand Option: {}", equipment->mLeftOpt);
        if (equipment->mLeftOpt == static_cast<int32_t>(MCM::eAction::Equip)) {
            auto left = equipment->mLefthand;
            bool IsEnchanted = equipment->mHasLeftExtra.first;
            bool IsTempered = equipment->mHasLeftExtra.second;
            if (IsEnchanted && IsTempered) {
                log::debug("[Lefthand] {}({:X}) - {}: {} - {}", left->GetName(), left->formID, equipment->mNumLeftEnch, equipment->mLeftExtra.first->GetName(), equipment->mLeftExtra.second);
            } else if (!IsEnchanted && IsTempered) {
                log::debug("[Lefthand] {}({:X}) - None - {}", left->GetName(), left->formID, equipment->mLeftExtra.second);
            } else if (IsEnchanted && !IsTempered) {
                log::debug("[Lefthand] {}({:X}) - {}: {} - None", left->GetName(), left->formID, equipment->mNumLeftEnch, equipment->mLeftExtra.first->GetName());
            } else {
                log::debug("[Lefthand] {}({:X}) - None - None", left->GetName(), left->formID);
            }
        }
        log::debug("Righthand Option: {}", equipment->mRightOpt);
        if (equipment->mRightOpt == static_cast<int32_t>(MCM::eAction::Equip)) {
            auto right = equipment->mRighthand;
            bool IsEnchanted = equipment->mHasRightExtra.first;
            bool IsTempered = equipment->mHasRightExtra.second;
            if (IsEnchanted && IsTempered) {
                log::debug("[Righthand] {}({:X}) - {}: {} - {}", right->GetName(), right->formID, equipment->mNumRightEnch, equipment->mRightExtra.first->GetName(), equipment->mRightExtra.second);
            } else if (!IsEnchanted && IsTempered) {
                log::debug("[Righthand] {}({:X}) - None - {}", right->GetName(), right->formID, equipment->mRightExtra.second);
            } else if (IsEnchanted && !IsTempered) {
                log::debug("[Righthand] {}({:X}) - {}: {} - None", right->GetName(), right->formID, equipment->mNumRightEnch, equipment->mRightExtra.first->GetName());
            } else {
                log::debug("[Righthand] {}({:X}) - None - None", right->GetName(), right->formID);
            }
        }
        log::debug("Shout Option: {}", equipment->mShoutOpt);
        if (equipment->mShoutOpt == static_cast<int32_t>(MCM::eAction::Equip)) {
            log::debug("Shout: {}({:X})", equipment->mShout->GetName(), equipment->mShout->formID);
        }

        log::debug("Contained Items");

        int size = equipment->mItems.size();
        for (int i = 0; i < size; i++) {
            auto item = equipment->mItems[i];
            bool IsEnchanted = equipment->mHasItemsExtra[i].first;
            bool IsTempered = equipment->mHasItemsExtra[i].second;
            if (IsEnchanted && IsTempered) {
                log::debug("[Items] {}({:X}) - {}: {} - {}", item->GetName(), item->formID, equipment->mNumItemsEnch[i], equipment->mItemsExtra[i].first->GetName(), equipment->mItemsExtra[i].second);
            } else if (!IsEnchanted && IsTempered) {
                log::debug("[Items] {}({:X}) - None - {}", item->GetName(), item->formID, equipment->mItemsExtra[i].second);
            } else if (IsEnchanted && !IsTempered) {
                log::debug("[Items] {}({:X}) - {}: {} - None", item->GetName(), item->formID, equipment->mNumItemsEnch[i], equipment->mItemsExtra[i].first->GetName());
            } else {
                log::debug("[Items] {}({:X}) - None - None", item->GetName(), item->formID);
            }
        }
    }
}

RE::BSFixedString EquipsetManager::GetNamePrefix()
{
    RE::BSFixedString result;

    int size = mEquipset.size();
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
        
        if (isConflict) {
            ++prefix;
        } else {
            result = static_cast<RE::BSFixedString>(name + strPrefix);
            return result;
        }
    }

    return result;
}

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
    auto equipsetSize = manager->mEquipset.size();
    serde->WriteRecordData(&equipsetSize, sizeof(equipsetSize));
    for (int i = 0; i < equipsetSize; i++) {
        auto equipset = manager->mEquipset[i];
 
        // Equipset::mName
        size_t nameSize = equipset->mName.length();                                                                     // Equipset::mName.length()
        serde->WriteRecordData(&nameSize, sizeof(nameSize));
        for (auto& elem : equipset->mName) {
            serde->WriteRecordData(&elem, sizeof(elem));                                                                // Equipset::mName
        }

        // Equipset::MCM::Hotkey
        auto hotkey = equipset->mHotkey;
        serde->WriteRecordData(&hotkey->mKeyCode, sizeof(hotkey->mKeyCode));                                            // Equipset::MCM::Hotkey::mKeyCode
        serde->WriteRecordData(&hotkey->mModifier[0], sizeof(hotkey->mModifier[0]));                                    // Equipset::MCM::Hotkey::mModifier[0]
        serde->WriteRecordData(&hotkey->mModifier[1], sizeof(hotkey->mModifier[1]));                                    // Equipset::MCM::Hotkey::mModifier[1]
        serde->WriteRecordData(&hotkey->mModifier[2], sizeof(hotkey->mModifier[2]));                                    // Equipset::MCM::Hotkey::mModifier[2]

        // Equipset::MCM::Option
        auto option = equipset->mOption;
        serde->WriteRecordData(&option->mSound, sizeof(option->mSound));                                                // Equipset::MCM::Option::mSound
        serde->WriteRecordData(&option->mToggleEquip, sizeof(option->mToggleEquip));                                    // Equipset::MCM::Option::mToggleEquip
        serde->WriteRecordData(&option->mReEquip, sizeof(option->mReEquip));                                            // Equipset::MCM::Option::mReEquip

        // Equipset::MCM::Widget
        auto widget = equipset->mWidget;

        // Equipset::MCM::Widget::mIcon
        size_t iconSize = widget->mIcon.length();                                                                       // Equipset::MCM::Widget::mIcon.length()
        serde->WriteRecordData(&iconSize, sizeof(iconSize));
        for (auto& elem : widget->mIcon) {
            serde->WriteRecordData(&elem, sizeof(elem));                                                                // Equipset::MCM::Widget::mIcon
        }
        serde->WriteRecordData(&widget->mHpos, sizeof(widget->mHpos));                                                  // Equipset::MCM::Widget::mHpos
        serde->WriteRecordData(&widget->mVpos, sizeof(widget->mVpos));                                                  // Equipset::MCM::Widget::mVpos
        serde->WriteRecordData(&widget->mDisplayIcon, sizeof(widget->mDisplayIcon));                                    // Equipset::MCM::Widget::mDisplayIcon
        serde->WriteRecordData(&widget->mDisplayName, sizeof(widget->mDisplayName));                                    // Equipset::MCM::Widget::mDisplayName
        serde->WriteRecordData(&widget->mDisplayHotkey, sizeof(widget->mDisplayHotkey));                                // Equipset::MCM::Widget::mDisplayHotkey

        // Equipset::MCM::Equipment
        auto equipment = equipset->mEquipment;
        serde->WriteRecordData(&equipment->mLeftOpt, sizeof(equipment->mLeftOpt));                                      // Equipset::MCM::Equipment::mLeftOpt
        if (equipment->mLeftOpt == static_cast<int32_t>(MCM::eAction::Equip)) {
            serde->WriteRecordData(&equipment->mLefthand->formID, sizeof(equipment->mLefthand->formID));                // Equipset::MCM::Equipment::mLefthand
            serde->WriteRecordData(&equipment->mHasLeftExtra.first, sizeof(equipment->mHasLeftExtra.first));            // Equipset::MCM::Equipment::mHasLeftExtra.first
            if (equipment->mHasLeftExtra.first) {
                serde->WriteRecordData(&equipment->mNumLeftEnch, sizeof(equipment->mNumLeftEnch));                      // Equipset::MCM::Equipment::mNumLeftEnch
                RE::FormID ID = equipment->mLeftExtra.first->GetFormID();
                serde->WriteRecordData(&ID, sizeof(ID));                                                                // Equipset::MCM::Equipment::mLeftExtra.first
            }
            serde->WriteRecordData(&equipment->mHasLeftExtra.second, sizeof(equipment->mHasLeftExtra.second));          // Equipset::MCM::Equipment::mHasLeftExtra.second
            if (equipment->mHasLeftExtra.second) {
                serde->WriteRecordData(&equipment->mLeftExtra.second, sizeof(equipment->mLeftExtra.second));            // Equipset::MCM::Equipment::mLeftExtra.second
            }
        }
        serde->WriteRecordData(&equipment->mRightOpt, sizeof(equipment->mRightOpt));                                    // Equipset::MCM::Equipment::mRightOpt
        if (equipment->mRightOpt == static_cast<int32_t>(MCM::eAction::Equip)) {
            serde->WriteRecordData(&equipment->mRighthand->formID, sizeof(equipment->mRighthand->formID));              // Equipset::MCM::Equipment::mRighthand
            serde->WriteRecordData(&equipment->mHasRightExtra.first, sizeof(equipment->mHasRightExtra.first));          // Equipset::MCM::Equipment::mHasRightExtra.first
            if (equipment->mHasRightExtra.first) {
                serde->WriteRecordData(&equipment->mNumRightEnch, sizeof(equipment->mNumRightEnch));                    // Equipset::MCM::Equipment::mNumRightEnch
                RE::FormID ID = equipment->mRightExtra.first->GetFormID();
                serde->WriteRecordData(&ID, sizeof(ID));                                                                // Equipset::MCM::Equipment::mRightExtra.first
            }
            serde->WriteRecordData(&equipment->mHasRightExtra.second, sizeof(equipment->mHasRightExtra.second));        // Equipset::MCM::Equipment::mHasRightExtra.second
            if (equipment->mHasRightExtra.second) {
                serde->WriteRecordData(&equipment->mRightExtra.second, sizeof(equipment->mRightExtra.second));          // Equipset::MCM::Equipment::mRightExtra.second
            }
        }
        serde->WriteRecordData(&equipment->mShoutOpt, sizeof(equipment->mShoutOpt));                                    // Equipset::MCM::Equipment::mShoutOpt
        if (equipment->mShoutOpt == static_cast<int32_t>(MCM::eAction::Equip)) {
            serde->WriteRecordData(&equipment->mShout->formID, sizeof(equipment->mShout->formID));                      // Equipset::MCM::Equipment::mShout
        }

        size_t itemsSize = equipment->mItems.size();
        serde->WriteRecordData(&itemsSize, sizeof(itemsSize));                                                          // Equipset::MCM::Equipment::mItems.size()
        for (int j = 0; j < itemsSize; j++) {
            serde->WriteRecordData(&equipment->mItems[j]->formID, sizeof(equipment->mItems[j]->formID));                // Equipset::MCM::Equipment::mItems
            serde->WriteRecordData(&equipment->mHasItemsExtra[j].first, sizeof(equipment->mHasItemsExtra[j].first));    // Equipset::MCM::Equipment::mHasItemsExtra.first
            if (equipment->mHasItemsExtra[j].first) {
                serde->WriteRecordData(&equipment->mNumItemsEnch[j], sizeof(equipment->mNumItemsEnch[j]));              // Equipset::MCM::Equipment::mNumItemsEnch
                RE::FormID ID = equipment->mItemsExtra[j].first->GetFormID();
                serde->WriteRecordData(&ID, sizeof(ID));                                                                // Equipset::MCM::Equipment::mItemsExtra.first
            }
            serde->WriteRecordData(&equipment->mHasItemsExtra[j].second, sizeof(equipment->mHasItemsExtra[j].second));  // Equipset::MCM::Equipment::mHasItemsExtra.second
            if (equipment->mHasItemsExtra[j].second) {
                serde->WriteRecordData(&equipment->mItemsExtra[j].second, sizeof(equipment->mItemsExtra[j].second));    // Equipset::MCM::Equipment::mItemsExtra.second
            }
        }
    }
    
    //if (!serde->OpenRecord(CycleEquipsetRecord, 0)) {
    //    log::error("Unable to open record to write cosave data.");
    //    return;
    //}
    // Again, write the number of items so we know how many times to iterate when loading.
    //auto trackedActorsCount = GetSingleton()._trackedActors.size();
    //serde->WriteRecordData(&trackedActorsCount, sizeof(trackedActorsCount));
    //for (auto* actor : GetSingleton()._trackedActors) {
    //    serde->WriteRecordData(&actor->formID, sizeof(actor->formID));
    //}
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
                serde->ReadRecordData(&nameSize, sizeof(nameSize));                                                          // Equipset::mName.length()
                for (; nameSize > 0; --nameSize) {
                    char name;
                    serde->ReadRecordData(&name, sizeof(name));                                                              // Equipset::mName
                    mName += name;
                }

                // Equipset::MCM::Hotkey
                MCM::Hotkey hotkey;
                serde->ReadRecordData(&hotkey.mKeyCode, sizeof(hotkey.mKeyCode));                                            // Equipset::MCM::Hotkey::mKeycode
                serde->ReadRecordData(&hotkey.mModifier[0], sizeof(hotkey.mModifier[0]));                                    // Equipset::MCM::Hotkey::mModifier[0]
                serde->ReadRecordData(&hotkey.mModifier[1], sizeof(hotkey.mModifier[1]));                                    // Equipset::MCM::Hotkey::mModifier[1]
                serde->ReadRecordData(&hotkey.mModifier[2], sizeof(hotkey.mModifier[2]));                                    // Equipset::MCM::Hotkey::mModifier[2]

                // Equipset::MCM::Option
                MCM::Option option;
                serde->ReadRecordData(&option.mSound, sizeof(option.mSound));                                                // Equipset::MCM::Option::mSound
                serde->ReadRecordData(&option.mToggleEquip, sizeof(option.mToggleEquip));                                    // Equipset::MCM::Option::mToggleEquip
                serde->ReadRecordData(&option.mReEquip, sizeof(option.mReEquip));                                            // Equipset::MCM::Option::mReEquip

                // Equipset::MCM::Widget
                MCM::Widget widget;

                size_t IconSize;
                serde->ReadRecordData(&IconSize, sizeof(IconSize));                                                          // Equipset::MCM::Widget::mIcon.length()
                for (; IconSize > 0; --IconSize) {
                    char name;
                    serde->ReadRecordData(&name, sizeof(name));                                                              // Equipset::MCM::Widget::mIcon
                    widget.mIcon += name;
                }
                serde->ReadRecordData(&widget.mHpos, sizeof(widget.mHpos));                                                  // Equipset::MCM::Widget::mHpos
                serde->ReadRecordData(&widget.mVpos, sizeof(widget.mVpos));                                                  // Equipset::MCM::Widget::mVpos
                serde->ReadRecordData(&widget.mDisplayIcon, sizeof(widget.mDisplayIcon));                                    // Equipset::MCM::Widget::mDisplayIcon
                serde->ReadRecordData(&widget.mDisplayName, sizeof(widget.mDisplayName));                                    // Equipset::MCM::Widget::mDisplayName
                serde->ReadRecordData(&widget.mDisplayHotkey, sizeof(widget.mDisplayHotkey));                                // Equipset::MCM::Widget::mDisplayHotkey

                // Equipset::MCM::Equipment
                MCM::Equipment equipment;
                serde->ReadRecordData(&equipment.mLeftOpt, sizeof(equipment.mLeftOpt));                                      // Equipset::MCM::Equipment::mLeftOpt
                if (equipment.mLeftOpt == static_cast<int32_t>(MCM::eAction::Equip)) {
                    RE::FormID ID, newID;
                    serde->ReadRecordData(&ID, sizeof(ID));                                                                  // Equipset::MCM::Equipment::mLefthand
                    if (!serde->ResolveFormID(ID, newID)) {
                        log::warn("Form ID {:X} could not be found after loading the save.", ID);
                        continue;
                    }
                    equipment.mLefthand = RE::TESForm::LookupByID<RE::TESForm>(newID);
                    serde->ReadRecordData(&equipment.mHasLeftExtra.first, sizeof(equipment.mHasLeftExtra.first));            // Equipset::MCM::Equipment::mHasLeftExtra.first
                    if (equipment.mHasLeftExtra.first) {
                        serde->ReadRecordData(&equipment.mNumLeftEnch, sizeof(equipment.mNumLeftEnch));                      // Equipset::MCM::Equipment::mNumLeftEnch
                        RE::FormID ExtraID, newExtraID;
                        serde->ReadRecordData(&ExtraID, sizeof(ExtraID));                                                    // Equipset::MCM::Equipment::mLeftExtra.first
                        if (!serde->ResolveFormID(ExtraID, newExtraID)) {
                            log::warn("Form ID {:X} could not be found after loading the save.", ExtraID);
                            continue;
                        }
                        equipment.mLeftExtra.first = RE::TESForm::LookupByID<RE::EnchantmentItem>(newExtraID);
                    }
                    serde->ReadRecordData(&equipment.mHasLeftExtra.second, sizeof(equipment.mHasLeftExtra.second));          // Equipset::MCM::Equipment::mHasLeftExtra.second
                    if (equipment.mHasLeftExtra.second) {
                        serde->ReadRecordData(&equipment.mLeftExtra.second, sizeof(equipment.mLeftExtra.second));            // Equipset::MCM::Equipment::mLeftExtra.second
                    }
                }
                serde->ReadRecordData(&equipment.mRightOpt, sizeof(equipment.mRightOpt));                                    // Equipset::MCM::Equipment::mRightOpt
                if (equipment.mRightOpt == static_cast<int32_t>(MCM::eAction::Equip)) {
                    RE::FormID ID, newID;
                    serde->ReadRecordData(&ID, sizeof(ID));                                                                  // Equipset::MCM::Equipment::mRighthand
                    if (!serde->ResolveFormID(ID, newID)) {
                        log::warn("Form ID {:X} could not be found after loading the save.", ID);
                        continue;
                    }
                    equipment.mRighthand = RE::TESForm::LookupByID<RE::TESForm>(newID);
                    serde->ReadRecordData(&equipment.mHasRightExtra.first, sizeof(equipment.mHasRightExtra.first));          // Equipset::MCM::Equipment::mHasRightExtra.first
                    if (equipment.mHasRightExtra.first) {
                        serde->ReadRecordData(&equipment.mNumRightEnch, sizeof(equipment.mNumRightEnch));                    // Equipset::MCM::Equipment::mNumRightEnch
                        RE::FormID ExtraID, newExtraID;
                        serde->ReadRecordData(&ExtraID, sizeof(ExtraID));                                                    // Equipset::MCM::Equipment::mRightExtra.first
                        if (!serde->ResolveFormID(ExtraID, newExtraID)) {
                            log::warn("Form ID {:X} could not be found after loading the save.", ExtraID);
                            continue;
                        }
                        equipment.mRightExtra.first = RE::TESForm::LookupByID<RE::EnchantmentItem>(newExtraID);
                    }
                    serde->ReadRecordData(&equipment.mHasRightExtra.second, sizeof(equipment.mHasRightExtra.second));        // Equipset::MCM::Equipment::mHasRightExtra.second
                    if (equipment.mHasRightExtra.second) {
                        serde->ReadRecordData(&equipment.mRightExtra.second, sizeof(equipment.mRightExtra.second));          // Equipset::MCM::Equipment::mRightExtra.second
                    }
                }
                serde->ReadRecordData(&equipment.mShoutOpt, sizeof(equipment.mShoutOpt));                                    // Equipset::MCM::Equipment::mShoutOpt
                if (equipment.mShoutOpt == static_cast<int32_t>(MCM::eAction::Equip)) {
                    RE::FormID ID, newID;
                    serde->ReadRecordData(&ID, sizeof(ID));                                                                  // Equipset::MCM::Equipment::mShout
                    if (!serde->ResolveFormID(ID, newID)) {
                        log::warn("Form ID {:X} could not be found after loading the save.", ID);
                        continue;
                    }
                    equipment.mShout = RE::TESForm::LookupByID<RE::TESForm>(newID);
                }

                size_t itemsSize;
                serde->ReadRecordData(&itemsSize, sizeof(itemsSize));                                                        // Equipset::MCM::Equipment::mItems.size()
                for (int j = 0; j < itemsSize; j++) {
                    RE::FormID ID, newID;
                    serde->ReadRecordData(&ID, sizeof(ID));                                                                  // Equipset::MCM::Equipment::mItems
                    if (!serde->ResolveFormID(ID, newID)) {
                        log::warn("Form ID {:X} could not be found after loading the save.", ID);
                        continue;
                    }
                    equipment.mItems[j] = RE::TESForm::LookupByID<RE::TESForm>(newID);
                    serde->ReadRecordData(&equipment.mHasItemsExtra[j].first, sizeof(equipment.mHasItemsExtra[j].first));    // Equipset::MCM::Equipment::mHasItemsExtra.first
                    if (equipment.mHasItemsExtra[j].first) {
                        serde->ReadRecordData(&equipment.mNumItemsEnch[j], sizeof(equipment.mNumItemsEnch[j]));              // Equipset::MCM::Equipment::mNumItemsEnch
                        RE::FormID ExtraID, newExtraID;
                        serde->ReadRecordData(&ExtraID, sizeof(ExtraID));                                                    // Equipset::MCM::Equipment::mItemsExtra.first
                        if (!serde->ResolveFormID(ExtraID, newExtraID)) {
                            log::warn("Form ID {:X} could not be found after loading the save.", ExtraID);
                            continue;
                        }
                        equipment.mItemsExtra[j].first = RE::TESForm::LookupByID<RE::EnchantmentItem>(newExtraID);
                    }
                    serde->ReadRecordData(&equipment.mHasItemsExtra[j].second, sizeof(equipment.mHasItemsExtra[j].second));  // Equipset::MCM::Equipment::mHasItemsExtra.second
                    if (equipment.mHasItemsExtra[j].second) {
                        serde->ReadRecordData(&equipment.mItemsExtra[j].second, sizeof(equipment.mItemsExtra[j].second));    // Equipset::MCM::Equipment::mItemsExtra.second
                    }
                }

                auto manager = &GetSingleton();
                if (!manager) {
                    log::error("Unable to get EquipsetManager");
                    return;
                }

                manager->NewEquipset(mName, &hotkey, &option, &widget, &equipment);
            }
        }
        else {
            log::warn("Unknown record type in cosave.");
            __assume(false);
        }
    }
}