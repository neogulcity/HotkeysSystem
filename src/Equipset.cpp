#include "Actor.h"
#include "Equipset.h"
#include "EquipsetManager.h"
#include "MCM.h"
#include "Offset.h"
#include "WidgetHandler.h"

RE::TESForm* GetDummyDagger()
{
    RE::FormID ID = 0x20163;
    RE::TESForm* dummyDagger = RE::TESForm::LookupByID<RE::TESForm>(ID);
    if (!dummyDagger) {
        log::error("Failed to lookup DummyDagger FormID.");
    }
    return dummyDagger;
}

[[maybe_unused]] RE::TESForm* GetDummyShout()
{
    RE::FormID ID = 0x55;
    return RE::TESForm::LookupByID<RE::TESForm>(ID);
}

std::vector<RE::TESForm*> GetAllEquippedItems()
{
	std::vector<RE::TESForm*> result;

	auto playerref = RE::PlayerCharacter::GetSingleton();
    if (!playerref) {
        log::error("Unable to get playerref.");
        return result;
    }

	auto inv = playerref->GetInventory();
	for (const auto& [item, data] : inv) {
		if (item->Is(RE::FormType::LeveledItem)) {
			continue;
		}
		const auto& [numItems, entry] = data;
		if (numItems > 0 && entry->IsWorn()) {
			result.push_back(item);
		}
	}

	return result;
}

void EquipItem(RE::TESForm* _form, RE::BGSEquipSlot* _slot, bool _sound, RE::ExtraDataList* _xList, bool _queue = true, bool _force = false)
{
    if (!_form) {
        log::warn("Warning. Trying to equip nullptr form.");
        return;
    }

    auto equipManager = RE::ActorEquipManager::GetSingleton();
    if (!equipManager) {
        log::error("Unable to get ActorEquipManager.");
        return;
    }

    auto playerref = RE::PlayerCharacter::GetSingleton();
    if (!playerref) {
        log::error("Unable to get playerref.");
        return;
    }

    auto playerbase = playerref->GetActorBase();
    if (!playerbase) {
        log::error("Unable to get PlayerActorBase.");
        return;
    }

    // Check FormType Spell
    if (_form->Is(RE::FormType::Spell)) {
        RE::SpellItem* spell = _form->As<RE::SpellItem>();
        if (spell) {
            if (Actor::HasMagic(playerref, spell) || Actor::HasMagic(playerbase, spell)) {
                equipManager->EquipSpell(playerref, spell, _slot);
            }
        }

    // Check FormType Shout
    } else if (_form->Is(RE::FormType::Shout)) {
        RE::TESShout* shout = _form->As<RE::TESShout>();
        if (shout && Actor::HasShout(playerbase, shout)) {
            equipManager->EquipShout(playerref, shout);
        }

    // Items
    } else {
        if (_form->GetFormType() == RE::FormType::Light) {
            if (Actor::HasItem(playerref, _form)) {
                RE::TESBoundObject* object = _form->As<RE::TESBoundObject>();
                equipManager->EquipObject(playerref, object, nullptr, 1U, _slot, _queue, _force, _sound, false);
            }
        } else if (Actor::HasItem(playerref, _form) || _form->formID == GetDummyDagger()->formID) {
            RE::TESBoundObject* object = _form->As<RE::TESBoundObject>();

            equipManager->EquipObject(playerref, object, _xList, 1U, _slot, _queue, _force, _sound, false);
        }
    }
}

// Unequip Item
void UnequipItem(RE::TESForm* _form, RE::BGSEquipSlot* _slot, bool _sound, RE::ExtraDataList* _xList, bool _queue = false, bool _force = false) {
    if (!_form) {
        log::warn("Warning. Trying to unequip nullptr form.");
        return;
    }

    RE::ActorEquipManager* equipManager = RE::ActorEquipManager::GetSingleton();
    if (!equipManager) {
        log::error("Unable to get ActorEquipManager.");
        return;
    }

    auto playerref = RE::PlayerCharacter::GetSingleton();
    if (!playerref) {
        log::error("Unable to get playerref.");
        return;
    }

    if (_form->Is(RE::FormType::Shout)) {
        // Need to figure out Unequip Shout

    } else {
        RE::TESBoundObject* object = _form->As<RE::TESBoundObject>();
        equipManager->UnequipObject(playerref, object, _xList, 1U, _slot, _queue, _force, _sound, false, nullptr);
    }
}

void Equipset::Equip()
{
    auto playerref = RE::PlayerCharacter::GetSingleton();
    if (!playerref) {
        log::error("Unable to get playerref.");
        return;
    }
    
    bool UnequipLeft = false, UnequipRight = false, UnequipShout = false;
    bool EquipLeft = false, EquipRight = false, EquipShout = false;
    std::vector<bool> UnequipItems(mEquipment->mItems.size(), false);
    std::vector<bool> EquipItems(mEquipment->mItems.size(), false);

    UnequipLeft = mEquipment->mLeft->option == static_cast<int32_t>(MCM::eAction::Unequip) ? true : UnequipLeft;
    UnequipRight = mEquipment->mRight->option == static_cast<int32_t>(MCM::eAction::Unequip) ? true : UnequipRight;
    UnequipShout = mEquipment->mShout->option == static_cast<int32_t>(MCM::eAction::Unequip) ? true : UnequipShout;
    
    RE::TESForm* equippedRight = playerref->GetEquippedObject(false);
    RE::TESForm* equippedLeft = playerref->GetEquippedObject(true);
    RE::TESForm* equippedShout = Actor::GetEquippedShout(playerref);

    // Toggle equip/unequip option Off & Re equip option On
    if (!mOption->mToggleEquip && mOption->mReEquip) {
        if (mEquipment->mLeft->option == static_cast<int32_t>(MCM::eAction::Equip)) {
            UnequipLeft = true;
            EquipLeft = true;
        }

        if (mEquipment->mRight->option == static_cast<int32_t>(MCM::eAction::Equip)) {
            UnequipRight = true;
            EquipRight = true;
        }

        if (mEquipment->mShout->option == static_cast<int32_t>(MCM::eAction::Equip)) {
            UnequipShout = true;
            EquipShout = true;
        }

        for (int i = 0; i < mEquipment->mItems.size(); ++i) {
            UnequipItems[i] = true;
            EquipItems[i] = true;
        }
    }
    else {
        // Toggle equip/unequip option Off & Re equip option Off
        if (mEquipment->mLeft->option == static_cast<int32_t>(MCM::eAction::Equip)) {
            if (!equippedLeft)
                EquipLeft = true;

            else if (equippedLeft->GetFormID() != mEquipment->mLeft->form->GetFormID())
                EquipLeft = true;
        }
        if (mEquipment->mRight->option == static_cast<int32_t>(MCM::eAction::Equip)) {
            if (!equippedRight)
                EquipRight = true;

            else if (equippedRight->GetFormID() != mEquipment->mRight->form->GetFormID())
                EquipRight = true;
        }
        if (mEquipment->mShout->option == static_cast<int32_t>(MCM::eAction::Equip)) {
            if (!equippedShout)
                EquipShout = true;

            else if (equippedShout->GetFormID() != mEquipment->mShout->form->GetFormID())
                EquipShout = true;
        }
        uint32_t count = 0;
        if (mEquipment->mItems.size() > 0) {
            std::vector<RE::TESForm*> items = GetAllEquippedItems();
            std::vector<RE::FormID> itemsID(mEquipment->mItems.size(), 0);
            std::vector<bool> isEquipped(mEquipment->mItems.size(), 0);

            for (const auto& elem : items) {
                if (elem) {
                    itemsID.push_back(elem->GetFormID());
                }
            }

            for (uint32_t i = 0; i < mEquipment->mItems.size(); i++) {
                for (int j = 0; j < items.size(); ++j) {
                    if (itemsID[j] == mEquipment->mItems[j]->form->GetFormID()) {
                        isEquipped[i] = true;
                        ++count;
                    }
                }
            }

            for (uint32_t i = 0; i < mEquipment->mItems.size(); i++) {
                if (!isEquipped[i]) {
                    EquipItems[i] = true;
                }
            }
        }
        if (mOption->mToggleEquip) {
            // Toggle equip/unequip option On & Re equip option Off
            if (!EquipRight && !EquipLeft && !EquipShout && count == mEquipment->mItems.size()) {
                UnequipLeft = true;
                UnequipRight = true;
                UnequipShout = true;
                for (uint32_t i = 0; i < mEquipment->mItems.size(); i++) {
                    UnequipItems[i] = true;
                }
            }

            // Toggle equip/unequip option On & Re equip option On
            else if (mOption->mReEquip) {
                UnequipLeft = true;
                UnequipRight = true;
                UnequipShout = true;
                EquipLeft = true;
                EquipRight = true;
                EquipShout = true;
                for (uint32_t i = 0; i < mEquipment->mItems.size(); i++) {
                    UnequipItems[i] = true;
                    EquipItems[i] = true;
                }
            }
        }
    }

    auto DummyDagger = GetDummyDagger();
    //auto DummyShout = GetDummyShout();

    if (UnequipRight) {
        if (DummyDagger) {
            EquipItem(DummyDagger, GetRightHandSlot(), false, nullptr, false, true);
            UnequipItem(DummyDagger, GetRightHandSlot(), false, nullptr, false, true);
        }
    }
    if (UnequipLeft) {
        if (DummyDagger) {
            EquipItem(DummyDagger, GetLeftHandSlot(), false, nullptr, false, true);
            UnequipItem(DummyDagger, GetLeftHandSlot(), false, nullptr, false, true);
        }
    }
    /*
    if (UnequipShout) {
        if (DummyShout) {
            EquipItem(DummyShout, nullptr, false, nullptr, false, true);
            UnequipItem(DummyShout, nullptr, false, nullptr, false, true);
        }
    }
    */

    for (uint32_t i = 0; i < mEquipment->mItems.size(); i++) {
        if (UnequipItems[i]) {
            UnequipItem(mEquipment->mItems[i]->form, nullptr, mOption->mSound, mEquipment->mItems[i]->xList);
        }
    }

    if (EquipLeft && mEquipment->mLeft->option == static_cast<int32_t>(MCM::eAction::Equip)) {
        EquipItem(mEquipment->mLeft->form, GetLeftHandSlot(), mOption->mSound, mEquipment->mLeft->xList);
    }

    if (EquipRight && mEquipment->mRight->option == static_cast<int32_t>(MCM::eAction::Equip)) {
        EquipItem(mEquipment->mRight->form, GetRightHandSlot(), mOption->mSound, mEquipment->mRight->xList);
    }
    if (EquipShout && mEquipment->mShout->option == static_cast<int32_t>(MCM::eAction::Equip)) {
        EquipItem(mEquipment->mShout->form, nullptr, mOption->mSound, nullptr);
    }

    for (uint32_t i = 0; i < mEquipment->mItems.size(); i++) {
        if (EquipItems[i]) {
            if (mEquipment->mItems[i]->form->GetFormType() == RE::FormType::Light) {
                RE::TESForm* form = playerref->GetEquippedObject(false);
                if (form) {
                    RE::WEAPON_TYPE type = form->As<RE::TESObjectWEAP>()->GetWeaponType();
                    if (type == RE::WEAPON_TYPE::kTwoHandSword || type == RE::WEAPON_TYPE::kTwoHandAxe ||
                        type == RE::WEAPON_TYPE::kBow || type == RE::WEAPON_TYPE::kCrossbow) {
                        EquipItem(DummyDagger, GetRightHandSlot(), false, nullptr, false, true);
                        UnequipItem(DummyDagger, GetRightHandSlot(), false, nullptr, false, true);
                    }
                }
            }

            EquipItem(mEquipment->mItems[i]->form, nullptr, mOption->mSound, mEquipment->mItems[i]->xList);
        }
    }

    auto widgetHandler = WidgetHandler::GetSingleton();
    if (!widgetHandler) {
        log::error("Unable to get WidgetHandler.");
        return;
    }

    auto manager = &UIHS::EquipsetManager::GetSingleton();
    if (!manager) {
        log::error("Unable to get EquipsetManager.");
        return;
    }

    auto dataHolder = MCM::DataHolder::GetSingleton();
    if (!dataHolder) {
        log::error("Unable to get DataHolder.");
        return;
    }

    if (!dataHolder->setting->mWidgetActive) {
        return;
    }

    MCM::eWidgetDisplay type = static_cast<MCM::eWidgetDisplay>(dataHolder->widget->mDisplay);
    if (type == MCM::eWidgetDisplay::InCombat) {
        if (dataHolder->widget->mDelay != 0.0f) {
            manager->DissolveOut_Function();
            if (!manager->IsThreadWorking()) {
                manager->SetDissolveTimer();
            }
            else {
                manager->SetRemain(dataHolder->widget->mDelay + 1.0f);
            }
        }
    }   

    if (mWidget->mDisplayWidget && mWidget->mWidgetID.size() > 0) {
        widgetHandler->Animate(mWidget->mWidgetID[0]);
    }
    if (mWidget->mDisplayName && mWidget->mNameID.size() > 0) {
        widgetHandler->Animate(mWidget->mNameID[0]);
    }
    if (mWidget->mDisplayHotkey && mWidget->mHotkeyID != -1) {
        widgetHandler->Animate(mWidget->mHotkeyID);
    }
}

void CycleEquipset::Equip()
{
    auto manager = &UIHS::EquipsetManager::GetSingleton();
    if (!manager) {
        log::error("Unable to get EquipsetManager.");
        return;
    }

    auto playerref = RE::PlayerCharacter::GetSingleton();
    if (!playerref) {
        log::error("Unable to get playerref.");
        return;
    }

    if (mCycleItems.size() == 0) {
        return;
    }

    // Cycle persist option On and it's not first time to execute cycle EquipSet
    if (mOption->mPersist && mCycleIndex.second != -1) {
        auto prevEquipset = manager->SearchEquipsetByName(mCycleItems[mCycleIndex.second]);
        if (!prevEquipset) {
            log::warn("Failed to search Equipest by name.");
            return;
        }

        bool IsChanged = false;
        std::vector<RE::TESForm*> items = GetAllEquippedItems();
        std::vector<RE::FormID> itemsID;
        
        for (const auto& elem : items) {
            if (elem) {
                itemsID.push_back(elem->GetFormID());
            } else {
                itemsID.push_back(0);
            }
        }

        if (prevEquipset->mEquipment->mLeft->option == static_cast<int32_t>(MCM::eAction::Equip)) {
            RE::TESForm* form = playerref->GetEquippedObject(true);
            auto left = prevEquipset->mEquipment->mLeft->form;

            if (!form) {
                IsChanged = true;
            }

            else if (left && form->GetFormID() != left->GetFormID()) {
                IsChanged = true;
            }
        }

        if (prevEquipset->mEquipment->mRight->option == static_cast<int32_t>(MCM::eAction::Equip) && !IsChanged) {
            RE::TESForm* form = playerref->GetEquippedObject(false);
            auto right = prevEquipset->mEquipment->mRight->form;

            if (!form) {
                IsChanged = true;
            }

            else if (right && form->GetFormID() != right->GetFormID()) {
                IsChanged = true;
            }
        }

        if (prevEquipset->mEquipment->mShout->option == static_cast<int32_t>(MCM::eAction::Equip) && !IsChanged) {
            RE::TESForm* form = Actor::GetEquippedShout(playerref);
            auto shout = prevEquipset->mEquipment->mShout->form;

            if (!form) {
                IsChanged = true;
            }

            else if (shout && form->GetFormID() != shout->GetFormID()) {
                IsChanged = true;
            }
        }

        if (!IsChanged) {
            for (uint32_t i = 0; i < prevEquipset->mEquipment->mItems.size(); ++i) {
                int count = 0;

                for (int j = 0; j < items.size(); j++) {
                    auto item = prevEquipset->mEquipment->mItems[i]->form;
                    if (item && itemsID[j] == item->GetFormID()) {
                        ++count;
                    }
                }
                if (count == 0) {
                    IsChanged = true;
                    break;
                }
            }
        }

        // Equipped weapons, spells, shout, items Changed so, Turn cycle back and Equip.
        if (IsChanged) {
            mCycleIndex.first = mCycleIndex.first <= 0 ? mCycleItems.size() - 1 : --mCycleIndex.first;

            auto equipset = manager->SearchEquipsetByName(mCycleItems[mCycleIndex.first]);
            if (!equipset) {
                log::warn("Failed to search Equipset by name");
            }
            else {
                equipset->Equip();
                mCycleIndex.second = mCycleIndex.first;
                mCycleIndex.first = mCycleIndex.first >= mCycleItems.size() - 1 ? 0 : ++mCycleIndex.first;

                if (mOption->mExpire != 0.0f) {
                    if (!IsExpireWorking()) {
                        SetExpireTimer();
                    }
                    else {
                        SetRemain(mOption->mExpire);
                    }
                }
            }
        }

        // Equipped weapons, spells, shout, items are not changed so, Go forward.
        else {
            auto equipset = manager->SearchEquipsetByName(mCycleItems[mCycleIndex.first]);
            if (!equipset) {
                log::warn("Failed to search Equipset by name");
            }

            equipset->Equip();
            mCycleIndex.second = mCycleIndex.first;
            mCycleIndex.first = mCycleIndex.first >= mCycleItems.size() - 1 ? 0 : ++mCycleIndex.first;

            if (mOption->mExpire != 0.0f) {
                if (!IsExpireWorking()) {
                    SetExpireTimer();
                }
                else {
                    SetRemain(mOption->mExpire);
                }
            }
        }
    }
    // Cycle persist option Off or It's first time to execute cycle EquipSet
    else {
        auto equipset = manager->SearchEquipsetByName(mCycleItems[mCycleIndex.first]);
        if (!equipset) {
            log::warn("Failed to search Equipset by name");
        }

        equipset->Equip();
        mCycleIndex.second = mCycleIndex.first;
        mCycleIndex.first = mCycleIndex.first >= mCycleItems.size() - 1 ? 0 : ++mCycleIndex.first;

        if (mOption->mExpire != 0.0f) {
            if (!IsExpireWorking()) {
                SetExpireTimer();
            }
            else {
                SetRemain(mOption->mExpire);
            }
        }
    }

    auto widgetHandler = WidgetHandler::GetSingleton();
    if (!widgetHandler) {
        log::error("Unable to get WidgetHandler.");
        return;
    }

    auto dataHolder = MCM::DataHolder::GetSingleton();
    if (!dataHolder) {
        log::error("Unable to get DataHolder.");
        return;
    }

    if (!dataHolder->setting->mWidgetActive) {
        return;
    }

    if (mWidget->mDisplayWidget && mWidget->mWidgetID.size() > 0) {
        widgetHandler->SetAlpha(mWidget->mWidgetID[mCycleIndex.second], 0);
        widgetHandler->SetAlpha(mWidget->mWidgetID[mCycleIndex.first], 100);
        widgetHandler->Animate(mWidget->mWidgetID[mCycleIndex.first]);
    }
    if (mWidget->mDisplayName && mWidget->mNameID.size() > 0) {
        widgetHandler->SetAlpha(mWidget->mNameID[mCycleIndex.second], 0);
        widgetHandler->SetAlpha(mWidget->mNameID[mCycleIndex.first], 100);
        widgetHandler->Animate(mWidget->mNameID[mCycleIndex.first]);
    }
    if (mWidget->mDisplayHotkey && mWidget->mHotkeyID != -1) {
        widgetHandler->SetAlpha(mWidget->mHotkeyID, 0);
        widgetHandler->SetAlpha(mWidget->mHotkeyID, 100);
        widgetHandler->Animate(mWidget->mHotkeyID);
    }
}

bool CycleEquipset::Expire_Function()
{
    SetExpireWorking(true);
    auto UI = RE::UI::GetSingleton();
    if (!UI) {
        log::error("Unable to get UI.");
        return false;
    }

    auto widgetHandler = WidgetHandler::GetSingleton();
    if (!widgetHandler) {
        log::error("Unable to get WidgetHandler.");
        return false;
    }

    auto dataHolder = MCM::DataHolder::GetSingleton();
    if (!dataHolder) {
        log::error("Unable to get DataHolder.");
        return false;
    }

    auto manager = &UIHS::EquipsetManager::GetSingleton();
    if (!manager) {
        log::error("Unable to get EquipsetManager.");
        return false;
    }

    SetExpireClose(false);

    SetRemain(mOption->mExpire);
    while (!IsExpireClosing() && GetRemain() > 0.0f) {
        float remain = GetRemain();
        remain = !UI->GameIsPaused() ? remain - 0.1f : remain;
        SetRemain(remain);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if (!IsExpireClosing()) {
        MCM::eWidgetDisplay type = static_cast<MCM::eWidgetDisplay>(dataHolder->widget->mDisplay);
        if (type == MCM::eWidgetDisplay::InCombat) {
            if (dataHolder->widget->mDelay != 0.0f) {
                manager->DissolveOut_Function();
                if (manager->IsThreadWorking()) {
                    SetRemain(dataHolder->widget->mDelay + 1.0f);
                }
                else {
                    manager->SetDissolveTimer();
                }
            }
        }

        if (mWidget->mDisplayWidget && mWidget->mWidgetID.size() > 0) {
            widgetHandler->SetAlpha(mWidget->mWidgetID[mCycleIndex.first], 0);
            widgetHandler->SetAlpha(mWidget->mWidgetID[0], 100);
            widgetHandler->Animate(mWidget->mWidgetID[0]);
        }
        if (mWidget->mDisplayName && mWidget->mNameID.size() > 0) {
            widgetHandler->SetAlpha(mWidget->mNameID[mCycleIndex.first], 0);
            widgetHandler->SetAlpha(mWidget->mNameID[0], 100);
            widgetHandler->Animate(mWidget->mNameID[0]);
        }
        if (mWidget->mDisplayHotkey && mWidget->mHotkeyID != -1) {
            widgetHandler->Animate(mWidget->mHotkeyID);
        }
        mCycleIndex.first = 0;
        mCycleIndex.second = -1;
    }

    SetRemain(0.0f);
    SetExpireClose(true);
    SetExpireWorking(false);

    return true;
}

void CycleEquipset::SetExpireTimer()
{
    mExpireHandle = std::async(std::launch::async, &CycleEquipset::Expire_Function, this);
}

bool CycleEquipset::Reset_Function()
{
    SetResetWorking(true);
    auto UI = RE::UI::GetSingleton();
    if (!UI) {
        log::error("Unable to get UI.");
        return false;
    }

    auto widgetHandler = WidgetHandler::GetSingleton();
    if (!widgetHandler) {
        log::error("Unable to get WidgetHandler.");
        return false;
    }

    SetResetClose(false);

    float num = mOption->mReset;
    while (!IsResetClosing() && num > 0.0f) {
        num = !UI->GameIsPaused() ? num - 0.01f : num;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
 
    if (!IsResetClosing()) {
        if (mWidget->mDisplayWidget && mWidget->mWidgetID.size() > 0) {
            widgetHandler->SetAlpha(mWidget->mWidgetID[mCycleIndex.first], 0);
            widgetHandler->SetAlpha(mWidget->mWidgetID[0], 100);
            widgetHandler->Animate(mWidget->mWidgetID[0]);
        }
        if (mWidget->mDisplayName && mWidget->mNameID.size() > 0) {
            widgetHandler->SetAlpha(mWidget->mNameID[mCycleIndex.first], 0);
            widgetHandler->SetAlpha(mWidget->mNameID[0], 100);
            widgetHandler->Animate(mWidget->mNameID[0]);
        }
        if (mWidget->mDisplayHotkey && mWidget->mHotkeyID != -1) {
            widgetHandler->Animate(mWidget->mHotkeyID);
        }

        mCycleIndex.first = 0;
        mCycleIndex.second = -1;
        Equip();
    }

    SetResetClose(true);
    SetResetWorking(false);
    return true;
}

void CycleEquipset::SetResetTimer()
{
    mResetHandle = std::async(std::launch::async, &CycleEquipset::Reset_Function, this);
}

float CycleEquipset::GetRemain()
{
    Locker locker(_lock);
    return mRemain;
}
void CycleEquipset::SetRemain(float _param)
{
    Locker locker(_lock);
    mRemain = _param;
}

bool CycleEquipset::IsExpireClosing()
{
    Locker locker(_lock);
    return mExpireClose;
}

void CycleEquipset::SetExpireClose(bool _param)
{
    Locker locker(_lock);
    mExpireClose = _param;
}

bool CycleEquipset::IsExpireWorking()
{
    Locker locker(_lock);
    return mExpireWorking;
}

void CycleEquipset::SetExpireWorking(bool _param)
{
    Locker locker(_lock);
    mExpireWorking = _param;
}

bool CycleEquipset::IsResetClosing()
{
    Locker locker(_lock);
    return mResetClose;
}

void CycleEquipset::SetResetClose(bool _param)
{
    Locker locker(_lock);
    mResetClose = _param;
}

bool CycleEquipset::IsResetWorking()
{
    Locker locker(_lock);
    return mResetWorking;
}

void CycleEquipset::SetResetWorking(bool _param)
{
    Locker locker(_lock);
    mResetWorking = _param;
}