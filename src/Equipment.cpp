#include "Equipment.h"
#include "EquipsetManager.h"
#include "WidgetHandler.h"
#include "Config.h"
#include "Actor.h"
#include "Translate.h"

#include <filesystem>
#include <toml++/toml.h>

const std::filesystem::path equipment_path = "Data/SKSE/Plugins/UIHS/Equipment.toml";

EquipmentManager::EquipmentManager() {
    auto manager = EquipsetManager::GetSingleton();
    if (!manager) logger::error("Failed to get Equipset Manager.");

    for (int i = 0; i < 32; i++) {
        EquipmentArmor initArmor;
        initArmor.type = EquipmentBase::Type::ARMOR;
        initArmor.slotid = fmt::format("_SLOT{}", i + 30);
        initArmor.widgetID.background = manager->AssignWidgetID();
        initArmor.widgetID.icon = manager->AssignWidgetID();
        initArmor.widgetID.text1 = manager->AssignWidgetID();
        this->armor.push_back(initArmor);
    }
    for (int i = 0; i < 2; i++) {
        EquipmentWeapon initWeapon;
        initWeapon.type = EquipmentBase::Type::WEAPON;
        initWeapon.widgetID.background = manager->AssignWidgetID();
        initWeapon.widgetID.icon = manager->AssignWidgetID();
        initWeapon.widgetID.text1 = manager->AssignWidgetID();
        if (i == 0) {
            initWeapon.isLeft = true;
            this->lefthand = initWeapon;
        } else if (i == 1) {
            initWeapon.isLeft = false;
            this->righthand = initWeapon;
        }
    }

    EquipmentBase initShout;
    initShout.type = EquipmentBase::Type::SHOUT;
    initShout.widgetID.background = manager->AssignWidgetID();
    initShout.widgetID.icon = manager->AssignWidgetID();
    initShout.widgetID.text1 = manager->AssignWidgetID();
    this->shout = initShout;
}

std::string GetArmorWidgetName(const std::string& _slot) {
    auto ts = Translator::GetSingleton();
    if (!ts) logger::error("Faield to get Translator");

    std::string result = TRANSLATE("_NOT_EQUIPPED");

    if (_slot.length() != 7) return result;

    auto player = RE::PlayerCharacter::GetSingleton();
    if (!player) return result;

    int slot = std::stoi(_slot.substr(5, std::string::npos)) - 30;

    auto inv = player->GetInventory();
    for (const auto& [item, data] : inv) {
        const auto& [numItem, entry] = data;
        if (numItem > 0 && item->Is(RE::FormType::Armor)) {
            auto armor = item->As<RE::TESObjectARMO>();
            if (!armor) continue;

            auto extraLists = entry->extraLists;
            if (!extraLists) continue;

            bool doNext = false;
            for (auto& _xList : *extraLists) {
                auto isWorn = _xList->HasType(RE::ExtraDataType::kWorn);
                if (isWorn) {
                    doNext = true;
                    break;
                }
            }

            if (!doNext) continue;

            auto armor_slot = static_cast<uint32_t>(armor->GetSlotMask());
            int flag = 1;
            if (armor_slot & (flag << slot)) {
                result = item->GetName();
            }
        }
    }

    return result;
}

void EquipmentManager::NotifyArmor(bool _isEquip, const std::vector<uint32_t>& _slot, const std::string& _name) {
    auto widgetHandler = WidgetHandler::GetSingleton();
    if (!widgetHandler) return;

    auto ts = Translator::GetSingleton();
    if (!ts) return;

    if (_isEquip) {
        for (auto elem : _slot) {
            if (this->armor[elem].widgetName.enable) {
                widgetHandler->SetText(this->armor[elem].widgetID.text1, _name);
            }
        }
    } else {
        for (auto elem : _slot) {
            if (this->armor[elem].widgetName.enable) {
                widgetHandler->SetText(this->armor[elem].widgetID.text1, TRANSLATE("_NOT_EQUIPPED"));
            }
        }
    }
}

std::string GetWeaponWidgetName(bool _isLeft) {
    auto ts = Translator::GetSingleton();
    if (!ts) logger::error("Faield to get Translator");

    std::string result = TRANSLATE("_UNARMED");

    auto player = RE::PlayerCharacter::GetSingleton();
    if (!player) return result;

    auto hand = player->GetEquippedObject(_isLeft);
    if (!hand) return result;

    result = hand->GetName();

    return result;
}

std::string GetWeaponWidgetPath(bool _isLeft) {
    std::string result = "";
    std::string id = ID::Null;
    std::vector<std::string> keyword;
    RE::FormID formid = 0U;

    auto player = RE::PlayerCharacter::GetSingleton();
    if (!player) return result;

    auto config = ConfigHandler::GetSingleton();
    if (!config) return result;

    auto TESDataHandler = RE::TESDataHandler::GetSingleton();
    if (!TESDataHandler) return result;

    auto hand = player->GetEquippedObject(_isLeft);

    if (!hand) {
        id = ID::Unarmed;
    } else {
        auto weapon = hand->As<RE::TESObjectWEAP>();
        if (weapon) {
            auto type = weapon->GetWeaponType();
            switch (type) {
                case RE::WEAPON_TYPE::kHandToHandMelee:
                    id = ID::Unarmed;
                    break;
                case RE::WEAPON_TYPE::kOneHandSword:
                    id = ID::Sword;
                    break;
                case RE::WEAPON_TYPE::kOneHandDagger:
                    id = ID::Dagger;
                    break;
                case RE::WEAPON_TYPE::kOneHandAxe:
                    id = ID::Axe;
                    break;
                case RE::WEAPON_TYPE::kOneHandMace:
                    id = ID::Mace;
                    break;
                case RE::WEAPON_TYPE::kTwoHandSword:
                    id = ID::Greatsword;
                    break;
                case RE::WEAPON_TYPE::kTwoHandAxe:
                    id = ID::Battleaxe;
                    break;
                case RE::WEAPON_TYPE::kBow:
                    id = ID::Bow;
                    break;
                case RE::WEAPON_TYPE::kStaff:
                    id = ID::Staff;
                    break;
                case RE::WEAPON_TYPE::kCrossbow:
                    id = ID::Crossbow;
                    break;
            }
        }
        auto spell = hand->As<RE::SpellItem>();
        if (spell) {
            auto type = spell->GetSpellType();
            if (type == RE::MagicSystem::SpellType::kSpell) {
                auto skillType = spell->GetAssociatedSkill();
                switch (skillType) {
                    case RE::ActorValue::kAlteration:
                        id = ID::Alteration;
                        break;
                    case RE::ActorValue::kConjuration:
                        id = ID::Conjuration;
                        break;
                    case RE::ActorValue::kDestruction:
                        id = ID::Destruction;
                        break;
                    case RE::ActorValue::kIllusion:
                        id = ID::Illusion;
                        break;
                    case RE::ActorValue::kRestoration:
                        id = ID::Restoration;
                        break;
                }
            }
        }

        auto armor = hand->As<RE::TESObjectARMO>();
        if (armor) {
            if (armor->IsShield()) {
                id = ID::Shield;
            }
        }

        auto light = hand->As<RE::TESObjectLIGH>();
        if (light) {
            id = ID::Torch;
        }

        auto keywordForm = hand->As<RE::BGSKeywordForm>();
        if (keywordForm) {
            auto keywords = keywordForm->GetKeywords();
            for (auto elem : keywords) {
                auto str = static_cast<std::string>(elem->formEditorID);
                keyword.push_back(str);
            }
        }

        formid = hand->GetFormID();
    }

    // First, assign default equipment widget.
    for (auto& elem : config->eq_widgetVec) {
        if (elem.id == id) {
            result = elem.path;
        }
    }

    // Second, assign cusotm equipment widget by keyword.
    for (auto& elem : config->eq_widgetVec) {
        for (auto& elemkeyword : keyword) {
            if (elem.keyword == elemkeyword) {
                result = elem.path;
            }
        }
    }

    // Third, assign cusotm equipment widget by formid.
    if (formid != 0) {
        for (auto& elem : config->eq_widgetVec) {
            auto compare = TESDataHandler->LookupFormID(elem.formid, elem.modname);
            if (formid == compare) {
                result = elem.path;
                break;
            }
        }
    }

    return result;
}

void EquipmentManager::NotifyWeapon(bool _isEquip, RE::TESForm* _weapon) {
    auto player = RE::PlayerCharacter::GetSingleton();
    if (!player) return;

    auto config = ConfigHandler::GetSingleton();
    if (!config) return;

    auto widgetHandler = WidgetHandler::GetSingleton();
    if (!widgetHandler) return;

    auto ts = Translator::GetSingleton();
    if (!ts) return;

    if (!_weapon) return;

    auto lefthand = player->GetEquippedObject(true);
    auto righthand = player->GetEquippedObject(false);
    if (_isEquip) {
        if (lefthand && lefthand->GetFormID() == _weapon->GetFormID()) {
            widgetHandler->UnloadWidget(this->lefthand.widgetID.icon);
            this->lefthand.CreateWidgetIcon();
            widgetHandler->SetText(this->lefthand.widgetID.text1, _weapon->GetName());
        }
        if (righthand && righthand->GetFormID() == _weapon->GetFormID()) {
            widgetHandler->UnloadWidget(this->righthand.widgetID.icon);
            this->righthand.CreateWidgetIcon();
            widgetHandler->SetText(this->righthand.widgetID.text1, _weapon->GetName());
        }
    } else {
        if (!lefthand) {
            widgetHandler->UnloadWidget(this->lefthand.widgetID.icon);
            this->lefthand.CreateWidgetIcon();
            widgetHandler->SetText(this->lefthand.widgetID.text1, TRANSLATE("_UNARMED"));
        }
        if (!righthand) {
            widgetHandler->UnloadWidget(this->righthand.widgetID.icon);
            this->righthand.CreateWidgetIcon();
            widgetHandler->SetText(this->righthand.widgetID.text1, TRANSLATE("_UNARMED"));
        }
    }
}

std::string GetShoutWidgetName() {
    auto ts = Translator::GetSingleton();
    if (!ts) logger::error("Faield to get Translator");

    std::string result = TRANSLATE("_NOT_EQUIPPED");

    auto player = RE::PlayerCharacter::GetSingleton();
    if (!player) return result;

    auto shout = Actor::GetEquippedShout(player);
    if (!shout) return result;

    result = shout->GetName();

    return result;
}

std::string GetShoutWidgetPath(bool _unequip = false) {
    std::string result = "";
    std::string id = ID::Null;
    std::vector<std::string> keyword;
    RE::FormID formid = 0U;

    auto player = RE::PlayerCharacter::GetSingleton();
    if (!player) return result;

    auto config = ConfigHandler::GetSingleton();
    if (!config) return result;

    auto TESDataHandler = RE::TESDataHandler::GetSingleton();
    if (!TESDataHandler) return result;

    if (_unequip) {
        for (auto& elem : config->eq_widgetVec) {
            if (elem.id == id) {
                result = elem.path;
            }
        }
        return result;
    }

    auto shout = Actor::GetEquippedShout(player);

    if (!shout) {
        id = ID::Null;
    } else {
        auto spell = shout->As<RE::SpellItem>();
        if (spell) {
            auto type = spell->GetSpellType();
            if (type == RE::MagicSystem::SpellType::kPower ||
                type == RE::MagicSystem::SpellType::kLesserPower) {
                id = ID::Power;
            }
        }

        if (shout->Is(RE::FormType::Shout)) {
            id = ID::Shout;
        }

        auto keywordForm = shout->As<RE::BGSKeywordForm>();
        if (keywordForm) {
            auto keywords = keywordForm->GetKeywords();
            for (auto elem : keywords) {
                auto str = static_cast<std::string>(elem->formEditorID);
                keyword.push_back(str);
            }
        }

        auto formid = shout->GetFormID();
    }

    // First, assign default equipment widget.
    for (auto& elem : config->eq_widgetVec) {
        if (elem.id == id) {
            result = elem.path;
        }
    }

    // Second, assign cusotm equipment widget by keyword.
    for (auto& elem : config->eq_widgetVec) {
        for (auto& elemkeyword : keyword) {
            if (elem.keyword == elemkeyword) {
                result = elem.path;
            }
        }
    }

    // Third, assign cusotm equipment widget by formid.
    if (formid != 0) {
        for (auto& elem : config->eq_widgetVec) {
            auto compare = TESDataHandler->LookupFormID(elem.formid, elem.modname);
            if (formid == compare) {
                result = elem.path;
                break;
            }
        }
    }

    return result;
}

void EquipmentManager::NotifyShout(bool _isEquip, RE::TESForm* _shout) {
    auto player = RE::PlayerCharacter::GetSingleton();
    if (!player) return;

    auto config = ConfigHandler::GetSingleton();
    if (!config) return;

    auto widgetHandler = WidgetHandler::GetSingleton();
    if (!widgetHandler) return;

    auto ts = Translator::GetSingleton();
    if (!ts) return;

    if (!_shout) return;

    auto shout = Actor::GetEquippedShout(player);
    if (_isEquip) {
        if (shout && shout->GetFormID() == _shout->GetFormID()) {
            widgetHandler->UnloadWidget(this->shout.widgetID.icon);
            this->shout.CreateWidgetIcon();
            widgetHandler->SetText(this->shout.widgetID.text1, _shout->GetName());
        }
    } else {
        widgetHandler->UnloadWidget(this->shout.widgetID.icon);
        this->shout.CreateWidgetIcon(true);
        widgetHandler->SetText(this->shout.widgetID.text1, TRANSLATE("_NOT_EQUIPPED"));
    }
}

void EquipmentBase::CreateWidgetBackground() {
    auto config = ConfigHandler::GetSingleton();
    if (!config) return;

    auto widgetHandler = WidgetHandler::GetSingleton();
    if (!widgetHandler) return;

    if (this->type == EquipmentBase::Type::ARMOR) {
        auto armor = static_cast<EquipmentArmor*>(this);
        if (!armor) return;

        if (armor->widgetIcon.enable) {
            auto id = armor->widgetID.background;
            auto path = config->GetWidgetPath(config->Widget.Equipment.Armor.bgType);
            auto offsetX = armor->widgetIcon.offsetX;
            auto offsetY = armor->widgetIcon.offsetY;
            auto width = 1.3f * (float)config->Widget.Equipment.Armor.bgSize;
            auto height = 1.3f * (float)config->Widget.Equipment.Armor.bgSize;
            auto alpha = config->Widget.Equipment.Armor.bgAlpha;

            widgetHandler->LoadWidget(id, path, offsetX, offsetY, width, height, alpha);
        }

    } else if (this->type == EquipmentBase::Type::WEAPON) {
        auto weapon = static_cast<EquipmentWeapon*>(this);
        if (!weapon) return;

        if (weapon->widgetIcon.enable) {
            auto id = weapon->widgetID.background;
            auto path = config->GetWidgetPath(config->Widget.Equipment.Weapon.bgType);
            auto offsetX = weapon->widgetIcon.offsetX;
            auto offsetY = weapon->widgetIcon.offsetY;
            auto width = 1.3f * (float)config->Widget.Equipment.Weapon.bgSize;
            auto height = 1.3f * (float)config->Widget.Equipment.Weapon.bgSize;
            auto alpha = config->Widget.Equipment.Weapon.bgAlpha;

            widgetHandler->LoadWidget(id, path, offsetX, offsetY, width, height, alpha);
        }
    } else if (this->type == EquipmentBase::Type::SHOUT) {
        auto shout = this;

        if (shout->widgetIcon.enable) {
            auto id = shout->widgetID.background;
            auto path = config->GetWidgetPath(config->Widget.Equipment.Shout.bgType);
            auto offsetX = shout->widgetIcon.offsetX;
            auto offsetY = shout->widgetIcon.offsetY;
            auto width = 1.3f * (float)config->Widget.Equipment.Shout.bgSize;
            auto height = 1.3f * (float)config->Widget.Equipment.Shout.bgSize;
            auto alpha = config->Widget.Equipment.Shout.bgAlpha;

            widgetHandler->LoadWidget(id, path, offsetX, offsetY, width, height, alpha);
        }
    }
}

void EquipmentBase::CreateWidgetIcon(bool _unequip) {
    auto config = ConfigHandler::GetSingleton();
    if (!config) return;

    auto widgetHandler = WidgetHandler::GetSingleton();
    if (!widgetHandler) return;

    
    if (this->type == EquipmentBase::Type::ARMOR) {
        auto armor = static_cast<EquipmentArmor*>(this);
        if (!armor) return;

        if (armor->widgetIcon.enable) {
            auto id = armor->widgetID.icon;
            std::string path = "";
            auto offsetX = armor->widgetIcon.offsetX;
            auto offsetY = armor->widgetIcon.offsetY;
            auto width = config->Widget.Equipment.Armor.widgetSize;
            auto height = config->Widget.Equipment.Armor.widgetSize;

            for (auto& elem : config->eq_widgetVec) {
                if (elem.id == armor->slotid) {
                    path = elem.path;
                }
            }

            widgetHandler->LoadWidget(id, path, offsetX, offsetY, width, height, 100);
        }
    } else if (this->type == EquipmentBase::Type::WEAPON) {
        auto weapon = static_cast<EquipmentWeapon*>(this);
        if (!weapon) return;

        if (weapon->widgetIcon.enable) {
            auto id = weapon->widgetID.icon;
            auto path = GetWeaponWidgetPath(weapon->isLeft);
            auto offsetX = weapon->widgetIcon.offsetX;
            auto offsetY = weapon->widgetIcon.offsetY;
            auto width = config->Widget.Equipment.Weapon.widgetSize;
            auto height = config->Widget.Equipment.Weapon.widgetSize;

            widgetHandler->LoadWidget(id, path, offsetX, offsetY, width, height, 100);
        }
    } else if (this->type == EquipmentBase::Type::SHOUT) {
        auto shout = this;

        if (shout->widgetIcon.enable) {
            auto id = shout->widgetID.icon;
            auto path = GetShoutWidgetPath(_unequip);
            auto offsetX = shout->widgetIcon.offsetX;
            auto offsetY = shout->widgetIcon.offsetY;
            auto width = config->Widget.Equipment.Shout.widgetSize;
            auto height = config->Widget.Equipment.Shout.widgetSize;

            widgetHandler->LoadWidget(id, path, offsetX, offsetY, width, height, 100);
        }
    }
}

void EquipmentBase::CreateWidgetText1() {
    auto config = ConfigHandler::GetSingleton();
    if (!config) return;

    auto widgetHandler = WidgetHandler::GetSingleton();
    if (!widgetHandler) return;

    if (this->type == EquipmentBase::Type::ARMOR) {
        auto armor = static_cast<EquipmentArmor*>(this);
        if (!armor) return;

        if (armor->widgetIcon.enable && armor->widgetName.enable) {
            auto id = armor->widgetID.text1;
            auto text = GetArmorWidgetName(armor->slotid);
            auto font = config->fontVec[config->Widget.General.font];
            auto offsetX = armor->widgetName.offsetX + armor->widgetIcon.offsetX;
            auto offsetY = armor->widgetName.offsetY + armor->widgetIcon.offsetY;
            auto align = static_cast<uint32_t>(armor->widgetName.align);
            auto size = 0.25f * (float)config->Widget.Equipment.Armor.fontSize;
            auto shadow = config->Widget.Equipment.Armor.fontShadow;

            widgetHandler->LoadText(id, text, font, offsetX, offsetY, align, size, 100, shadow);
        }
    } else if (this->type == EquipmentBase::Type::WEAPON) {
        auto weapon = static_cast<EquipmentWeapon*>(this);
        if (!weapon) return;

        if (weapon->widgetIcon.enable && weapon->widgetName.enable) {
            auto id = weapon->widgetID.text1;
            auto text = GetWeaponWidgetName(weapon->isLeft);
            auto font = config->fontVec[config->Widget.General.font];
            auto offsetX = weapon->widgetName.offsetX + weapon->widgetIcon.offsetX;
            auto offsetY = weapon->widgetName.offsetY + weapon->widgetIcon.offsetY;
            auto align = static_cast<uint32_t>(weapon->widgetName.align);
            auto size = 0.25f * (float)config->Widget.Equipment.Weapon.fontSize;
            auto shadow = config->Widget.Equipment.Weapon.fontShadow;

            widgetHandler->LoadText(id, text, font, offsetX, offsetY, align, size, 100, shadow);
        }
    } else if (this->type == EquipmentBase::Type::SHOUT) {
        auto shout = this;

        if (shout->widgetIcon.enable && shout->widgetName.enable) {
            auto id = shout->widgetID.text1;
            auto text = GetShoutWidgetName();
            auto font = config->fontVec[config->Widget.General.font];
            auto offsetX = shout->widgetName.offsetX + shout->widgetIcon.offsetX;
            auto offsetY = shout->widgetName.offsetY + shout->widgetIcon.offsetY;
            auto align = static_cast<uint32_t>(shout->widgetName.align);
            auto size = 0.25f * (float)config->Widget.Equipment.Shout.fontSize;
            auto shadow = config->Widget.Equipment.Shout.fontShadow;

            widgetHandler->LoadText(id, text, font, offsetX, offsetY, align, size, 100, shadow);
        }
    }
}

void EquipmentManager::CreateAllArmorWidget() {
    for (auto& elem : this->armor) {
        elem.CreateWidgetBackground();
        elem.CreateWidgetIcon();
        elem.CreateWidgetText1();
    }
}

void EquipmentManager::RemoveAllArmorWidget() {
    auto widgetHandler = WidgetHandler::GetSingleton();
    if (!widgetHandler) return;

    for (auto& elem : this->armor) {
        widgetHandler->UnloadWidget(elem.widgetID.background);
        widgetHandler->UnloadWidget(elem.widgetID.icon);
        widgetHandler->UnloadText(elem.widgetID.text1);
    }
}

void EquipmentManager::CreateAllWeaponWidget() {
    this->lefthand.CreateWidgetBackground();
    this->lefthand.CreateWidgetIcon();
    this->lefthand.CreateWidgetText1();
    this->righthand.CreateWidgetBackground();
    this->righthand.CreateWidgetIcon();
    this->righthand.CreateWidgetText1();
}

void EquipmentManager::RemoveAllWeaponWidget() {
    auto widgetHandler = WidgetHandler::GetSingleton();
    if (!widgetHandler) return;

    widgetHandler->UnloadWidget(this->lefthand.widgetID.background);
    widgetHandler->UnloadWidget(this->lefthand.widgetID.icon);
    widgetHandler->UnloadText(this->lefthand.widgetID.text1);
    widgetHandler->UnloadWidget(this->righthand.widgetID.background);
    widgetHandler->UnloadWidget(this->righthand.widgetID.icon);
    widgetHandler->UnloadText(this->righthand.widgetID.text1);
}

void EquipmentManager::CreateAllShoutWidget() {
    this->shout.CreateWidgetBackground();
    this->shout.CreateWidgetIcon();
    this->shout.CreateWidgetText1();
}

void EquipmentManager::RemoveAllShoutWidget() {
    auto widgetHandler = WidgetHandler::GetSingleton();
    if (!widgetHandler) return;

    widgetHandler->UnloadWidget(this->shout.widgetID.background);
    widgetHandler->UnloadWidget(this->shout.widgetID.icon);
    widgetHandler->UnloadText(this->shout.widgetID.text1);
}

void EquipmentManager::Load() {
    try {
        auto tbl = toml::parse_file(equipment_path.c_str());
        
        for (int i = 0; i < 32; i++) {
            auto& armor = this->armor[i];
            auto section = fmt::format("Armor{}", i);

            armor.widgetIcon.enable = tbl[section]["icon_enable"].value_or<bool>(false);
            armor.widgetIcon.offsetX = tbl[section]["icon_offsetX"].value_or<int>(0);
            armor.widgetIcon.offsetY = tbl[section]["icon_offsetY"].value_or<int>(0);
            armor.widgetName.enable = tbl[section]["name_enable"].value_or<bool>(false);
            armor.widgetName.align =
                static_cast<WidgetText::ALIGN_TYPE>(tbl[section]["name_align"].value_or<uint32_t>(2));
            armor.widgetName.offsetX = tbl[section]["name_offsetX"].value_or<int>(0);
            armor.widgetName.offsetY = tbl[section]["name_offsetY"].value_or<int>(0);
        }

        for (int i = 0; i < 2; i++) {
            auto& weapon = i == 0 ? this->lefthand : this->righthand;
            auto section = i == 0 ? "Lefthand" : "Righthand";

            weapon.widgetIcon.enable = tbl[section]["icon_enable"].value_or<bool>(false);
            weapon.widgetIcon.offsetX = tbl[section]["icon_offsetX"].value_or<int>(0);
            weapon.widgetIcon.offsetY = tbl[section]["icon_offsetY"].value_or<int>(0);
            weapon.widgetName.enable = tbl[section]["name_enable"].value_or<bool>(false);
            weapon.widgetName.align =
                static_cast<WidgetText::ALIGN_TYPE>(tbl[section]["name_align"].value_or<uint32_t>(2));
            weapon.widgetName.offsetX = tbl[section]["name_offsetX"].value_or<int>(0);
            weapon.widgetName.offsetY = tbl[section]["name_offsetY"].value_or<int>(0);
        }

        {
            this->shout.widgetIcon.enable = tbl["Shout"]["icon_enable"].value_or<bool>(false);
            this->shout.widgetIcon.offsetX = tbl["Shout"]["icon_offsetX"].value_or<int>(0);
            this->shout.widgetIcon.offsetY = tbl["Shout"]["icon_offsetY"].value_or<int>(0);
            this->shout.widgetName.enable = tbl["Shout"]["name_enable"].value_or<bool>(false);
            this->shout.widgetName.align =
                static_cast<WidgetText::ALIGN_TYPE>(tbl["Shout"]["name_align"].value_or<uint32_t>(2));
            this->shout.widgetName.offsetX = tbl["Shout"]["name_offsetX"].value_or<int>(0);
            this->shout.widgetName.offsetY = tbl["Shout"]["name_offsetY"].value_or<int>(0);
        }

        logger::info("Equipment data loaded.");
    } catch (const toml::parse_error& err) {
        logger::warn("Failed to parse equipment file. Using default settings.\nError: {}", err.description());
    }
}

void EquipmentManager::Save() {
    std::ofstream f(equipment_path);
    if (!f.is_open()) {
        logger::error("Failed to save equipment!");
        return;
    }

    toml::table mainTbl;
    
    for (int i = 0; i < 32; i++) {
        auto& armor = this->armor[i];
        auto tbl = toml::table{
            {"icon_enable", armor.widgetIcon.enable},
            {"icon_offsetX", armor.widgetIcon.offsetX},
            {"icon_offsetY", armor.widgetIcon.offsetY},
            {"name_enable", armor.widgetName.enable},
            {"name_align", static_cast<uint32_t>(armor.widgetName.align)},
            {"name_offsetX", armor.widgetName.offsetX},
            {"name_offsetY", armor.widgetName.offsetY},
        };

        mainTbl.insert(fmt::format("Armor{}", i), tbl);
    }

    for (int i = 0; i < 2; i++) {
        auto& weapon = i == 0 ? this->lefthand : this->righthand;
        auto tbl = toml::table{
            {"icon_enable", weapon.widgetIcon.enable},
            {"icon_offsetX", weapon.widgetIcon.offsetX},
            {"icon_offsetY", weapon.widgetIcon.offsetY},
            {"name_enable", weapon.widgetName.enable},
            {"name_align", static_cast<uint32_t>(weapon.widgetName.align)},
            {"name_offsetX", weapon.widgetName.offsetX},
            {"name_offsetY", weapon.widgetName.offsetY},
        };

        std::string weaponstr = i == 0 ? "Lefthand" : "Righthand";
        mainTbl.insert(weaponstr, tbl);
    }

    {
        auto tbl = toml::table{
            {"icon_enable", this->shout.widgetIcon.enable},
            {"icon_offsetX", this->shout.widgetIcon.offsetX},
            {"icon_offsetY", this->shout.widgetIcon.offsetY},
            {"name_enable", this->shout.widgetName.enable},
            {"name_align", static_cast<uint32_t>(this->shout.widgetName.align)},
            {"name_offsetX", this->shout.widgetName.offsetX},
            {"name_offsetY", this->shout.widgetName.offsetY},
        };

        mainTbl.insert("Shout", tbl);
    }

    f << mainTbl;
    logger::info("Equipment saved.");
}