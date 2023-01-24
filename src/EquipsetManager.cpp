#include "EquipsetManager.h"
#include "Translate.h"
#include "Config.h"
#include "Gui/GuiMenu.h"
#include "WidgetHandler.h"
#include "Serialize.h"

void EquipsetManager::Create(const NormalSet& _equipset, bool _assignOrder) {
	NormalSet* newSet = new NormalSet;
	*newSet = _equipset;

    newSet->widgetID.background = AssignWidgetID();
    newSet->widgetID.icon = AssignWidgetID();
    newSet->widgetID.text1 = AssignWidgetID();
    newSet->widgetID.text2 = AssignWidgetID();
    if (_assignOrder) newSet->order = AssignSortOrder();

	Equipset* equipset = newSet;
	equipsetVec.push_back(equipset);

    newSet->CreateWidget();
}

void EquipsetManager::Create(const PotionSet& _equipset, bool _assignOrder) {
    PotionSet* newSet = new PotionSet;
    *newSet = _equipset;

    newSet->AssignAutoPotion();

    newSet->widgetID.background = AssignWidgetID();
    newSet->widgetID.icon = AssignWidgetID();
    newSet->widgetID.text1 = AssignWidgetID();
    newSet->widgetID.text2 = AssignWidgetID();
    if (_assignOrder) newSet->order = AssignSortOrder();

    Equipset* equipset = newSet;
    equipsetVec.push_back(equipset);

    newSet->CreateWidget();
}

void EquipsetManager::Create(const CycleSet& _equipset, bool _assignOrder) {
    CycleSet* newSet = new CycleSet;
    *newSet = _equipset;

    newSet->widgetID.background = AssignWidgetID();
    newSet->widgetID.icon = AssignWidgetID();
    newSet->widgetID.text1 = AssignWidgetID();
    newSet->widgetID.text2 = AssignWidgetID();
    if (_assignOrder) newSet->order = AssignSortOrder();

    Equipset* equipset = newSet;
    equipsetVec.push_back(equipset);

    newSet->CreateWidget();
}

void EquipsetManager::Remove(Equipset* _equipset) {
    if (!_equipset) return;

    for (int i = 0; i < equipsetVec.size(); i++) {
        if (_equipset->name == equipsetVec[i]->name) {
            equipsetVec.erase(equipsetVec.begin() + i);
            delete _equipset;
        }
    }
}

void EquipsetManager::RemoveAll() {
    for (auto equipset : equipsetVec) {
        delete equipset;
    }

    equipsetVec.clear();
}


std::string EquipsetManager::GetNamePreset() {
	auto ts = Translator::GetSingleton();
	if (!ts) return "";

	std::string result = TRANSLATE("_EQUIPSET_NAME_PRESET");

	int suffix = 1;
    while (true) {
		bool next = false;
        for (int i = 0; i < equipsetVec.size(); i++) {
            if (equipsetVec[i]->name == fmt::format("{}{}", result, suffix)) {
                next = true;
            }
        }

		if (!next) {
            result = fmt::format("{}{}", result, suffix);
			break;
		}
		suffix++;
    }

	return result;
}

std::pair<EquipsetManager::VALID_TYPE, std::string> EquipsetManager::IsCreateValid(const std::string& _name,
                                                                                   const uint32_t& _hotkey,
                                                                                   bool _modifier1, bool _modifier2,
                                                                                   bool _modifier3) {
    if (_name == "") {
        return std::make_pair<VALID_TYPE, std::string>(VALID_TYPE::NAME_BLANK, "");
    }

    for (auto equipset : equipsetVec) {
        if (equipset->name == _name) {
            return std::make_pair<VALID_TYPE, std::string>(VALID_TYPE::NAME_CONFLICT, (std::string)_name);
        }
    }

    if (_hotkey != 0) {
        for (auto equipset : equipsetVec) {
            if (equipset->hotkey == _hotkey &&
                equipset->modifier1 == _modifier1 &&
                equipset->modifier2 == _modifier2 &&
                equipset->modifier3 == _modifier3) {
                return std::make_pair<VALID_TYPE, std::string>(VALID_TYPE::HOTKEY_CONFLICT, (std::string)equipset->name);
            }
        }
    }

    return std::make_pair<VALID_TYPE, std::string>(VALID_TYPE::GOOD, "");
}

std::pair<EquipsetManager::VALID_TYPE, std::string> EquipsetManager::IsEditValid(Equipset* _equipset,
                                                                                 const std::string& _name,
                                                                                 const uint32_t& _hotkey,
                                                                                 bool _modifier1, bool _modifier2,
                                                                                 bool _modifier3) {
    if (_name == "") {
        return std::make_pair<VALID_TYPE, std::string>(VALID_TYPE::NAME_BLANK, "");
    }

    for (auto equipset : equipsetVec) {
        if (equipset == _equipset) continue;

        if (equipset->name == _name) {
            return std::make_pair<VALID_TYPE, std::string>(VALID_TYPE::NAME_CONFLICT, (std::string)_name);
        }
    }

    if (_hotkey != 0) {
        for (auto equipset : equipsetVec) {
            if (equipset == _equipset) continue;

            if (equipset->hotkey == _hotkey && equipset->modifier1 == _modifier1 && equipset->modifier2 == _modifier2 &&
                equipset->modifier3 == _modifier3) {
                return std::make_pair<VALID_TYPE, std::string>(VALID_TYPE::HOTKEY_CONFLICT,
                                                               (std::string)equipset->name);
            }
        }
    }

    return std::make_pair<VALID_TYPE, std::string>(VALID_TYPE::GOOD, "");
}

uint32_t EquipsetManager::AssignWidgetID() {
    uint32_t value = widgetIndexHolder;
    widgetIndexHolder++;

    return value;
}

uint32_t EquipsetManager::AssignSortOrder() {
    uint32_t value = sortOrderHolder;
    sortOrderHolder++;

    return value;
}

void EquipsetManager::SyncSortOrder() {
    uint32_t MAX = 0;
    for (auto equipset : equipsetVec) {
        if (equipset->order > MAX) {
            MAX = equipset->order;
        }
    }

    sortOrderHolder = MAX + 1;
}

void EquipsetManager::ProcessEquip(const uint32_t& _code, bool _modifier1, bool _modifier2, bool _modifier3) {
    auto gui = GuiMenu::GetSingleton();
    if (!gui) return;

    auto UI = RE::UI::GetSingleton();
    if (!UI) return;

    auto intfcStr = RE::InterfaceStrings::GetSingleton();
    if (!intfcStr) return;

    if (UI->GameIsPaused() ||
        UI->IsMenuOpen(intfcStr->mapMenu) ||
        UI->IsMenuOpen(intfcStr->inventoryMenu) ||
        UI->IsMenuOpen(intfcStr->magicMenu) ||
        UI->IsMenuOpen(intfcStr->tweenMenu) ||
        UI->IsMenuOpen(intfcStr->dialogueMenu) ||
        UI->IsMenuOpen(intfcStr->barterMenu) ||
        UI->IsMenuOpen(intfcStr->craftingMenu) ||
        UI->IsMenuOpen(intfcStr->containerMenu) ||
        UI->IsMenuOpen(intfcStr->console) ||
        UI->IsMenuOpen("LootMenu")) return;

    if (gui->isShow()) return;

    for (auto equipset : equipsetVec) {
        if (equipset->hotkey == _code &&
            equipset->modifier1 == _modifier1 &&
            equipset->modifier2 == _modifier2 &&
            equipset->modifier3 == _modifier3) {
            if (equipset->type != Equipset::TYPE::CYCLE) {
                equipset->Equip();
                return;
            }

            auto cycleset = static_cast<CycleSet*>(equipset);
            if (cycleset->cycleExpire != 0.0f) {
                if (cycleset->cycleExpireProgress != 0.0f && cycleset->cycleExpireProgress.load() < cycleset->cycleExpire) {
                    cycleset->SetExpireProgress(0.01f);
                } else {
                    cycleset->StartExpireTimer();
                }
            }

            if (cycleset->cycleReset != 0.0f) {
                if (cycleset->cycleResetProgress != 0.0f && cycleset->cycleResetProgress.load() < cycleset->cycleReset) {
                    cycleset->SetResetProgress(0.01f);
                } else {
                    cycleset->StartResetTimer();
                }
            } else {
                cycleset->Equip();
            }

            return;
        }
    }
}

void EquipsetManager::CalculateKeydown(const uint32_t& _code, bool _modifier1, bool _modifier2, bool _modifier3, float _time) {
    auto gui = GuiMenu::GetSingleton();
    if (!gui) return;

    if (gui->isShow()) return;

    for (auto equipset : equipsetVec) {
        if (equipset->hotkey == _code &&
            equipset->modifier1 == _modifier1 &&
            equipset->modifier2 == _modifier2 &&
            equipset->modifier3 == _modifier3) {
            if (equipset->type != Equipset::TYPE::CYCLE) return;

            auto cycleset = static_cast<CycleSet*>(equipset);
            if (cycleset->cycleReset != 0.0f && _time < cycleset->cycleReset) {
                cycleset->CloseResetTimer();
                cycleset->Equip();
            }

            return;
        }
    }
}

Equipset* EquipsetManager::SearchEquipsetByName(const std::string& _name) {
    for (auto elem : equipsetVec) {
        if (elem->name == _name) {
            return elem;
        }
    }

    return nullptr;
}

void EquipsetManager::ImportEquipsets() {
    Serialize::ImportEquipset(Serialize::Type::FILE);
}

void EquipsetManager::ExportEquipsets() {
    Serialize::ExportEquipset(Serialize::Type::FILE);
}

void EquipsetManager::CreateAllWidget() {
    for (auto elem : equipsetVec) {
        elem->CreateWidget();
    }
}

void EquipsetManager::RemoveAllWidget() {
    for (auto elem : equipsetVec) {
        elem->RemoveWidget();
    }
}