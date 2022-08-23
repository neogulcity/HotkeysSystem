#include "Equipset.h"
#include "EquipsetManager.h"
#include "MCM.h"
#include "Offset.h"
#include "WidgetHandler.h"

RE::TESForm* GetDummyDagger()
{
    RE::FormID ID = 0x20163;
    return RE::TESForm::LookupByID<RE::TESForm>(ID);
}

RE::TESForm* GetDummyShout()
{
    RE::FormID ID = 0x55;
    return RE::TESForm::LookupByID<RE::TESForm>(ID);
}

RE::TESForm* GetEquippedShout(RE::Actor* _actor)
{
    if (!_actor) {
        return nullptr;
    }

    return _actor->selectedPower;
}

std::vector<RE::TESForm*> GetAllEquippedItems()
{
	std::vector<RE::TESForm*> result;

	auto playerref = RE::PlayerCharacter::GetSingleton();
    if (!playerref) {
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


bool HasItem(RE::TESForm* _form)
{
    if (!_form) {
        return false;
    }

    if (_form == GetDummyDagger()) {
        return true;
    }

    auto playerref = RE::PlayerCharacter::GetSingleton();
    if (!playerref) {
        return false;
    }

    auto inv = playerref->GetInventory();
    for (const auto& [item, data] : inv) {
        const auto& [numItem, entry] = data;

        if (numItem > 0 && item->GetFormID() == _form->GetFormID()) {
			return true;
        }
    }

    return false;
}

void EquipItem(RE::TESForm* _form, RE::BGSEquipSlot* _slot, bool _sound, RE::ExtraDataList* _xList, bool _queue = true, bool _force = false)
{
    if (!_form) {
        return;
    }

    RE::ActorEquipManager* equipManager = RE::ActorEquipManager::GetSingleton();
    if (!equipManager) {
        return;
    }

    auto playerref = RE::PlayerCharacter::GetSingleton();
    if (!playerref) {
        return;
    }

    // Check FormType Spell
    if (_form->Is(RE::FormType::Spell)) {
        RE::SpellItem* spell = _form->As<RE::SpellItem>();

        equipManager->EquipSpell(playerref, spell, _slot);

        // Check FormType Shout
    } else if (_form->Is(RE::FormType::Shout)) {
        RE::TESShout* shout = _form->As<RE::TESShout>();

        equipManager->EquipShout(playerref, shout);

        // Items
    } else {
        if (_form->GetFormType() == RE::FormType::Light) {
            RE::TESBoundObject* object = _form->As<RE::TESBoundObject>();

            equipManager->EquipObject(playerref, object, nullptr, 1U, _slot, _queue, _force, _sound, false);
        } else if (HasItem(_form)) {
            RE::TESBoundObject* object = _form->As<RE::TESBoundObject>();

            equipManager->EquipObject(playerref, object, _xList, 1U, _slot, _queue, _force, _sound, false);
        }
    }
}

// Unequip Item
void UnequipItem(RE::TESForm* _form, RE::BGSEquipSlot* _slot, bool _sound, RE::ExtraDataList* _xList, bool _queue = false, bool _force = false) {
    if (!_form) {
        return;
    }

    RE::ActorEquipManager* equipManager = RE::ActorEquipManager::GetSingleton();
    if (!equipManager) {
        return;
    }

    auto playerref = RE::PlayerCharacter::GetSingleton();
    if (!playerref) {
        return;
    }

    if (_form->Is(RE::FormType::Shout)) {
        // Need to figure out Unequip Shout // Shout Unequip 은 아직 확인이 더 필요.

    } else {
        RE::TESBoundObject* object = _form->As<RE::TESBoundObject>();
        equipManager->UnequipObject(playerref, object, _xList, 1U, _slot, _queue, _force, _sound, false, nullptr);
    }
}

void Equipset::Equip()
{
    auto playerref = RE::PlayerCharacter::GetSingleton();
    if (!playerref) {
        return;
    }
    
    bool UnequipLeft = false, UnequipRight = false, UnequipShout = false;
    bool EquipLeft = false, EquipRight = false, EquipShout = false;
    std::vector<bool> UnequipItems(mEquipment->mItems.numItems, false);
    std::vector<bool> EquipItems(mEquipment->mItems.numItems, false);

    UnequipLeft = mEquipment->mLeft.option == static_cast<int32_t>(MCM::eAction::Unequip) ? true : UnequipLeft;
    UnequipRight = mEquipment->mRight.option == static_cast<int32_t>(MCM::eAction::Unequip) ? true : UnequipRight;
    UnequipShout = mEquipment->mShout.option == static_cast<int32_t>(MCM::eAction::Unequip) ? true : UnequipShout;
    
    RE::TESForm* equippedRight = playerref->GetEquippedObject(false);
    RE::TESForm* equippedLeft = playerref->GetEquippedObject(true);
    RE::TESForm* equippedShout = GetEquippedShout(playerref);

    // Toggle equip/unequip option Off & Re equip option On
    if (!mOption->mToggleEquip && mOption->mReEquip) {
        if (mEquipment->mLeft.option == static_cast<int32_t>(MCM::eAction::Equip)) {
            UnequipLeft = true;
            EquipLeft = true;
        }

        if (mEquipment->mRight.option == static_cast<int32_t>(MCM::eAction::Equip)) {
            UnequipRight = true;
            EquipRight = true;
        }

        if (mEquipment->mShout.option == static_cast<int32_t>(MCM::eAction::Equip)) {
            UnequipShout = true;
            EquipShout = true;
        }

        for (int i = 0; i < mEquipment->mItems.numItems; ++i) {
            UnequipItems[i] = true;
            EquipItems[i] = true;
        }
    }
    else {
        // Toggle equip/unequip option Off & Re equip option Off
        if (mEquipment->mLeft.option == static_cast<int32_t>(MCM::eAction::Equip)) {
            if (!equippedLeft)
                EquipLeft = true;

            else if (equippedLeft->GetFormID() != mEquipment->mLeft.form->GetFormID())
                EquipLeft = true;
        }
        if (mEquipment->mRight.option == static_cast<int32_t>(MCM::eAction::Equip)) {
            if (!equippedRight)
                EquipRight = true;

            else if (equippedRight->GetFormID() != mEquipment->mRight.form->GetFormID())
                EquipRight = true;
        }
        if (mEquipment->mShout.option == static_cast<int32_t>(MCM::eAction::Equip)) {
            if (!equippedShout)
                EquipShout = true;

            else if (equippedShout->GetFormID() != mEquipment->mShout.form->GetFormID())
                EquipShout = true;
        }

        uint32_t count = 0;
        if (mEquipment->mItems.numItems > 0) {
            std::vector<RE::TESForm*> items = GetAllEquippedItems();
            std::vector<RE::FormID> itemsID(mEquipment->mItems.numItems, false);
            std::vector<bool> isEquipped;

            for (const auto& elem : items) {
                if (elem) {
                    itemsID.push_back(elem->GetFormID());
                }
            }

            for (uint32_t i = 0; i < mEquipment->mItems.numItems; i++) {
                for (int j = 0; j < items.size(); ++j) {
                    if (itemsID[j] == mEquipment->mItems.form[i]->GetFormID()) {
                        isEquipped[i] = true;
                        ++count;
                    }
                }
            }

            for (uint32_t i = 0; i < mEquipment->mItems.numItems; i++) {
                if (!isEquipped[i]) {
                    EquipItems[i] = true;
                }
            }
        }

        if (mOption->mToggleEquip) {
            // Toggle equip/unequip option On & Re equip option Off
            if (!EquipRight && !EquipLeft && !EquipShout && count == mEquipment->mItems.numItems) {
                UnequipLeft = true;
                UnequipRight = true;
                UnequipShout = true;
                for (uint32_t i = 0; i < mEquipment->mItems.numItems; i++) {
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
                for (uint32_t i = 0; i < mEquipment->mItems.numItems; i++) {
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
    for (uint32_t i = 0; i < mEquipment->mItems.numItems; i++) {
        if (UnequipItems[i]) {
            UnequipItem(mEquipment->mItems.form[i], nullptr, mOption->mSound, mEquipment->mItems.xList[i]);
        }
    }

    if (EquipLeft && mEquipment->mLeft.option == static_cast<int32_t>(MCM::eAction::Equip)) {
        EquipItem(mEquipment->mLeft.form, GetLeftHandSlot(), mOption->mSound, mEquipment->mLeft.xList);
    }

    if (EquipRight && mEquipment->mRight.option == static_cast<int32_t>(MCM::eAction::Equip)) {
        EquipItem(mEquipment->mRight.form, GetRightHandSlot(), mOption->mSound, mEquipment->mRight.xList);
    }

    if (EquipShout && mEquipment->mShout.option == static_cast<int32_t>(MCM::eAction::Equip)) {
        EquipItem(mEquipment->mShout.form, nullptr, mOption->mSound, nullptr);
    }

    for (uint32_t i = 0; i < mEquipment->mItems.numItems; i++) {
        if (EquipItems[i]) {
            if (mEquipment->mItems.form[i]->GetFormType() == RE::FormType::Light) {
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

            EquipItem(mEquipment->mItems.form[i], nullptr, mOption->mSound, mEquipment->mItems.xList[i]);
        }
    }

    auto widgetHandler = WidgetHandler::GetSingleton();
    if (!widgetHandler) {
        return;
    }

    auto manager = &UIHS::EquipsetManager::GetSingleton();
    if (!manager) {
        return;
    }

    auto dataHolder = &MCM::DataHolder::GetSingleton();
    if (!dataHolder) {
        return;
    }

    if (!dataHolder->setting.mWidgetActive) {
        return;
    }

    MCM::eWidgetDisplay type = static_cast<MCM::eWidgetDisplay>(dataHolder->widget.mDisplay);
    if (type == MCM::eWidgetDisplay::InCombat) {
        if (dataHolder->widget.mDelay != 0.0f) {
            manager->DissolveOut_Function();
            if (!manager->IsThreadWorking()) {
                manager->SetDissolveTimer();
            }
            else {
                manager->SetRemain(dataHolder->widget.mDelay + 1.0f);
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
        return;
    }

    auto playerref = RE::PlayerCharacter::GetSingleton();
    if (!playerref) {
        return;
    }

    auto equipset = manager->SearchEquipsetByName(mCycleItems[mCycleIndex.first]);
    if (!equipset) {
        return;
    }

    // Cycle persist option On and it's not first time to execute cycle EquipSet
    if (mOption->mPersist && mCycleIndex.second != -1) {
        bool IsChanged = false;
        std::vector<RE::TESForm*> items = GetAllEquippedItems();
        std::vector<RE::FormID> itemsID(mCycleItems.size(), false);
        
        for (const auto& elem : items) {
            if (elem) {
                itemsID.push_back(elem->GetFormID());
            }
        }

        if (equipset->mEquipment->mLeft.option == static_cast<int32_t>(MCM::eAction::Equip) && !IsChanged) {
            RE::TESForm* form = playerref->GetEquippedObject(true);

            if (!form) {
                IsChanged = true;
            }

            else if (form->GetFormID() != equipset->mEquipment->mLeft.form->GetFormID()) {
                IsChanged = true;
            }
        }

        if (equipset->mEquipment->mRight.option == static_cast<int32_t>(MCM::eAction::Equip)) {
            RE::TESForm* form = playerref->GetEquippedObject(false);

            if (!form) {
                IsChanged = true;
            }

            else if (form->GetFormID() != equipset->mEquipment->mRight.form->GetFormID()) {
                IsChanged = true;
            }
        }

        if (equipset->mEquipment->mShout.option == static_cast<int32_t>(MCM::eAction::Equip) && !IsChanged) {
            RE::TESForm* form = GetEquippedShout(playerref);

            if (!form) {
                IsChanged = true;
            }

            else if (form->GetFormID() != equipset->mEquipment->mShout.form->GetFormID()) {
                IsChanged = true;
            }
        }

        if (!IsChanged) {
            for (uint32_t i = 0; i < equipset->mEquipment->mItems.numItems; ++i) {
                int count = 0;

                for (int j = 0; j < items.size(); j++) {
                    if (itemsID[j] == equipset->mEquipment->mItems.form[i]->GetFormID()) {
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

            auto prevEquipset = manager->SearchEquipsetByName(mCycleItems[mCycleIndex.first]);
            if (prevEquipset) {
                prevEquipset->Equip();
                mCycleIndex.second = mCycleIndex.first;
                mCycleIndex.first = mCycleIndex.first >= mCycleItems.size() - 1 ? 0 : ++mCycleIndex.first;

                if (mOption->mExpire != 0.0f) {
                    SetExpireTimer();
                }
            }
        }

        // Equipped weapons, spells, shout, items are not changed so, Go forward.
        else {
            equipset->Equip();
            mCycleIndex.second = mCycleIndex.first;
            mCycleIndex.first = mCycleIndex.first >= mCycleItems.size() - 1 ? 0 : ++mCycleIndex.first;

            if (mOption->mExpire != 0.0f) {
                SetExpireTimer();
            }
        }
    }
    // Cycle persist option Off or It's first time to execute cycle EquipSet
    else {
        equipset->Equip();
        mCycleIndex.second = mCycleIndex.first;
        mCycleIndex.first = mCycleIndex.first >= mCycleItems.size() - 1 ? 0 : ++mCycleIndex.first;

        if (mOption->mExpire != 0.0f) {
            SetExpireTimer();
        }
    }

    auto widgetHandler = WidgetHandler::GetSingleton();
    if (!widgetHandler) {
        return;
    }

    auto dataHolder = &MCM::DataHolder::GetSingleton();
    if (!dataHolder) {
        return;
    }

    if (!dataHolder->setting.mWidgetActive) {
        return;
    }

    MCM::eWidgetDisplay type = static_cast<MCM::eWidgetDisplay>(dataHolder->widget.mDisplay);
    if (type == MCM::eWidgetDisplay::InCombat) {
        if (dataHolder->widget.mDelay != 0.0f) {
            manager->DissolveOut_Function();
            if (!manager->IsThreadWorking()) {
                manager->SetDissolveTimer();
            }
            else {
                manager->SetRemain(dataHolder->widget.mDelay + 1.0f);
            }
        }
    }    

    if (mWidget->mDisplayWidget && mWidget->mWidgetID.size() > 0) {
        widgetHandler->SetAlpha(mWidget->mWidgetID[mCycleIndex.second], 0);
        widgetHandler->Animate(mWidget->mWidgetID[mCycleIndex.first]);
    }
    if (mWidget->mDisplayName && mWidget->mNameID.size() > 0) {
        widgetHandler->SetAlpha(mWidget->mNameID[mCycleIndex.second], 0);
        widgetHandler->Animate(mWidget->mNameID[mCycleIndex.first]);
    }
    if (mWidget->mDisplayHotkey && mWidget->mHotkeyID != -1) {
        widgetHandler->SetAlpha(mWidget->mHotkeyID, 0);
        widgetHandler->Animate(mWidget->mHotkeyID);
    }
}

void CycleEquipset::Expire_Function()
{
    auto UI = RE::UI::GetSingleton();
    if (!UI) {
        return;
    }

    mCloseExpire = false;

    mRemain = mOption->mExpire;
    while (!mCloseExpire && mRemain > 0) {
        mRemain = !UI->GameIsPaused() ? mRemain - 0.1f : mRemain;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    Locker locker(_lock);

    if (!mCloseExpire) {
        mCycleIndex.first = 0;
        mCycleIndex.second = -1;
    }

    mRemain = 0.0f;
    mCloseExpire = true;
}

void CycleEquipset::SetExpireTimer()
{
    if (mOption->mExpire == 0.0f) {
        return;
    }

    if (!mCloseExpire) {
        mRemain = mOption->mExpire;
        return;
    }

    std::thread expireThread = std::thread(&CycleEquipset::Expire_Function, this);
    expireThread.detach();
}

void CycleEquipset::Reset_Function()
{
    auto UI = RE::UI::GetSingleton();
    if (!UI) {
        return;
    }

    mCloseReset = false;

    float num = mOption->mReset;
    while (!mCloseReset && num > 0) {
        num = !UI->GameIsPaused() ? num - 0.01f : num;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    Locker locker(_lock);

    if (!mCloseReset) {
        mCycleIndex.first = 0;
        mCycleIndex.second = -1;
        Equip();
    }

    mCloseReset = true;
}

void CycleEquipset::SetResetTimer()
{
    if (mOption->mReset == 0.0f) {
        return;
    }

    if (!mCloseReset) {
        return;
    }

    std::thread resetThread = std::thread(&CycleEquipset::Reset_Function, this);
    resetThread.detach();
}