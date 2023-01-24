#include "Data.h"
#include "ExtraData.h"
#include "Config.h"
#include "Actor.h"
#include "Translate.h"
#include "Utility.h"

DataWeapon::DataWeapon() {
    auto ts = Translator::GetSingleton();
    if (!ts) logger::error("Failed to get Translator.");

    type = DATATYPE::NOTHING;
    name = TRANSLATE("_NOTHING");
    enchNum = 0;
    enchName = Extra::ENCHNONE;
    tempVal = 0.0f;
    form = nullptr;
}

std::string DataWeapon::Pack() {
    std::string result;

    result += std::to_string(static_cast<uint32_t>(type)) + Utility::delimiter;
    result += name + Utility::delimiter;
    result += std::to_string(enchNum) + Utility::delimiter;
    result += enchName + Utility::delimiter;
    result += std::to_string(tempVal) + Utility::delimiter;
    result += form ? std::to_string(form->GetFormID()) : "0";
    result += Utility::delimiter;

    return result;
}


DataWeapon DataWeapon::Unpack(const std::string& _packed) {
    std::vector<std::string> dataVec = Utility::Split(_packed, Utility::delimiter);

    assert(dataVec.size() == 6);

    DataWeapon result;
    result.type = static_cast<Data::DATATYPE>(std::stoi(dataVec[0]));
    result.name = dataVec[1];
    result.enchNum = std::stoi(dataVec[2]);
    result.enchName = dataVec[3];
    result.tempVal = std::stof(dataVec[4]);

    RE::FormID id = std::stoi(dataVec[5]);
    auto form = RE::TESForm::LookupByID<RE::TESForm>(id);
    result.form = form;

    return result;
}

DataShout::DataShout() {
    auto ts = Translator::GetSingleton();
    if (!ts) logger::error("Failed to get Translator.");

    type = DATATYPE::NOTHING;
    name = TRANSLATE("_NOTHING");
    form = nullptr;
}

std::string DataShout::Pack() {
    std::string result;

    result += std::to_string(static_cast<uint32_t>(type)) + Utility::delimiter;
    result += name + Utility::delimiter;
    result += form ? std::to_string(form->GetFormID()) : "0";
    result += Utility::delimiter;

    return result;
}

DataShout DataShout::Unpack(const std::string& _packed) {
    std::vector<std::string> dataVec = Utility::Split(_packed, Utility::delimiter);

    assert(dataVec.size() == 3);

    DataShout result;
    result.type = static_cast<Data::DATATYPE>(std::stoi(dataVec[0]));
    result.name = dataVec[1];

    RE::FormID id = std::stoi(dataVec[2]);
    auto form = RE::TESForm::LookupByID<RE::TESForm>(id);
    result.form = form;

    return result;
}

DataArmor::DataArmor() {
    type = DATATYPE::NOTHING;
    name = "";
    enchNum = 0;
    enchName = Extra::ENCHNONE;
    tempVal = 0.0f;
    form = nullptr;
}

std::string DataArmor::Pack() {
    std::string result;

    result += std::to_string(static_cast<uint32_t>(type)) + Utility::delimiter;
    result += name + Utility::delimiter;
    result += std::to_string(enchNum) + Utility::delimiter;
    result += enchName + Utility::delimiter;
    result += std::to_string(tempVal) + Utility::delimiter;
    result += form ? std::to_string(form->GetFormID()) : "0";
    result += Utility::delimiter;

    return result;
}

DataArmor DataArmor::Unpack(const std::string& _packed) {
    std::vector<std::string> dataVec = Utility::Split(_packed, Utility::delimiter);

    assert(dataVec.size() == 6);

    DataArmor result;
    result.type = static_cast<Data::DATATYPE>(std::stoi(dataVec[0]));
    result.name = dataVec[1];
    result.enchNum = std::stoi(dataVec[2]);
    result.enchName = dataVec[3];
    result.tempVal = std::stof(dataVec[4]);

    RE::FormID id = std::stoi(dataVec[5]);
    auto form = RE::TESForm::LookupByID<RE::TESForm>(id);
    result.form = form;

    return result;
}

DataPotion::DataPotion() {
    auto ts = Translator::GetSingleton();
    if (!ts) logger::error("Failed to get Translator.");

    type = DATATYPE::NOTHING;
    name = TRANSLATE("_NOTHING");
    form = nullptr;
}

std::string DataPotion::Pack() {
    std::string result;

    result += std::to_string(static_cast<uint32_t>(type)) + Utility::delimiter;
    result += name + Utility::delimiter;
    result += form ? std::to_string(form->GetFormID()) : "0";
    result += Utility::delimiter;

    return result;
}

DataPotion DataPotion::Unpack(const std::string& _packed) {
    std::vector<std::string> dataVec = Utility::Split(_packed, Utility::delimiter);

    assert(dataVec.size() == 3);

    DataPotion result;
    result.type = static_cast<Data::DATATYPE>(std::stoi(dataVec[0]));
    result.name = dataVec[1];

    RE::FormID id = std::stoi(dataVec[2]);
    auto form = RE::TESForm::LookupByID<RE::TESForm>(id);
    result.form = form;

    return result;
}

void DataHandler::Init() {
    Clear();
    InitWeapon();
    InitShout();
    InitArmor();
    InitPotion();
}

void DataHandler::Clear() {
    weapon.clear();
    shout.clear();
    armor.clear();
    potion.clear();
    potion_health.clear();
    potion_magicka.clear();
    potion_stamina.clear();
}

void DataHandler::InitWeapon() {
    auto player = RE::PlayerCharacter::GetSingleton();
    if (!player || !player->IsInitialized()) return;

    auto config = ConfigHandler::GetSingleton();
    if (!config) return;
    
    auto ts = Translator::GetSingleton();
    if (!ts) return;

    bool favorOnly = config->Settings.favorOnly;
    std::vector<Data::DATATYPE> type;
    std::vector<std::string> name;
    std::vector<uint32_t> enchNum;
    std::vector<std::string> enchName;
    std::vector<float> tempVal;
    std::vector<RE::TESForm*> form;

    for (int i = 0; i < 2; i++) {
        Data::DATATYPE _type = i == 0 ? Data::DATATYPE::NOTHING : Data::DATATYPE::UNEQUIP;
        std::string _name = i == 0 ? TRANSLATE("_NOTHING") : TRANSLATE("_UNEQUIP");
        type.push_back(_type);
        name.push_back(_name);
        enchNum.push_back(0);
        enchName.push_back(Extra::ENCHNONE);
        tempVal.push_back(0.0f);
        form.push_back(nullptr);
    }

    auto inv = player->GetInventory();
    for (const auto& [item, data] : inv) {
        const auto& [numItem, entry] = data;
        if (numItem > 0 && item->Is(RE::FormType::Weapon)) {
            uint32_t numExtra = 0;

            auto extraLists = entry->extraLists;
            if (extraLists) {
                for (auto& _xList : *extraLists) {
                    ++numExtra;
                    if (favorOnly && !Extra::IsFavorited(_xList)) continue;

                    type.push_back(Data::DATATYPE::WEAP);
                    name.push_back(item->GetName());
                    enchNum.push_back(Extra::GetEnchNum(item, _xList));
                    enchName.push_back(Extra::GetEnchName(item, _xList));
                    tempVal.push_back(Extra::GetTempValue(_xList));
                    form.push_back(item->As<RE::TESForm>());
                }
            }

            if (favorOnly) continue;

            if (numExtra != numItem) {
                type.push_back(Data::DATATYPE::WEAP);
                name.push_back(item->GetName());
                enchNum.push_back(Extra::GetEnchNum(item, nullptr));
                enchName.push_back(Extra::GetEnchName(item, nullptr));
                tempVal.push_back(0.0f);
                form.push_back(item->As<RE::TESForm>());
            }
        }
    }

    std::vector<RE::SpellItem*> allSpell;

    auto baseSpellSize = Actor::GetBaseSpellCount(player);
    for (int i = 0; i < baseSpellSize; i++) {
        auto spell = Actor::GetBaseNthSpell(player, i);
        if (!spell) continue;

        allSpell.push_back(spell);
    }

    auto raceSpellSize = Actor::GetRaceSpellCount(player);
    for (int i = 0; i < raceSpellSize; i++) {
        auto spell = Actor::GetRaceNthSpell(player, i);
        if (!spell) continue;

        allSpell.push_back(spell);
    }

    auto spellSize = Actor::GetActorSpellCount(player);
    for (int i = 0; i < spellSize; i++) {
        auto spell = Actor::GetActorNthSpell(player, i);
        if (!spell) continue;

        allSpell.push_back(spell);
    }

    for (auto spell : allSpell) {
        auto spellType = spell->GetSpellType();
        if (spellType == RE::MagicSystem::SpellType::kSpell) {
            if (favorOnly && !Extra::IsMagicFavorited(spell)) continue;

            type.push_back(Data::DATATYPE::SPELL);
            name.push_back(spell->GetName());
            enchNum.push_back(0);
            enchName.push_back(Extra::ENCHNONE);
            tempVal.push_back(0.0f);
            form.push_back(spell->As<RE::TESForm>());
        }
    }

    for (int i = 0; i < type.size(); i++) {
        DataWeapon data(type[i], name[i], enchNum[i], enchName[i], tempVal[i], form[i]);
        weapon.push_back(data);
    }
}

void DataHandler::InitShout() {
    auto player = RE::PlayerCharacter::GetSingleton();
    if (!player || !player->IsInitialized()) return;

    auto config = ConfigHandler::GetSingleton();
    if (!config) return;

    auto ts = Translator::GetSingleton();
    if (!ts) return;

    bool favorOnly = config->Settings.favorOnly;
    std::vector<Data::DATATYPE> type;
    std::vector<std::string> name;
    std::vector<RE::TESForm*> form;

    type.push_back(Data::DATATYPE::NOTHING);
    name.push_back(TRANSLATE("_NOTHING"));
    form.push_back(nullptr);

    auto basePowerSize = Actor::GetBaseSpellCount(player);
    for (int i = 0; i < basePowerSize; i++) {
        auto power = Actor::GetBaseNthSpell(player, i);
        if (!power) continue;

        auto powerType = power->GetSpellType();
        if (powerType == RE::MagicSystem::SpellType::kPower ||
            powerType == RE::MagicSystem::SpellType::kLesserPower) {
            if (favorOnly && !Extra::IsMagicFavorited(power)) continue;

            type.push_back(Data::DATATYPE::SHOUT);
            name.push_back(power->GetName());
            form.push_back(power->As<RE::TESForm>());
        }
    }

    auto racePowerSize = Actor::GetRaceSpellCount(player);
    for (int i = 0; i < racePowerSize; i++) {
        auto power = Actor::GetRaceNthSpell(player, i);
        if (!power) continue;

        auto powerType = power->GetSpellType();
        if (powerType == RE::MagicSystem::SpellType::kPower ||
            powerType == RE::MagicSystem::SpellType::kLesserPower) {
            if (favorOnly && !Extra::IsMagicFavorited(power)) continue;

            type.push_back(Data::DATATYPE::SHOUT);
            name.push_back(power->GetName());
            form.push_back(power->As<RE::TESForm>());
        }
    }

    auto powerSize = Actor::GetActorSpellCount(player);
    for (int i = 0; i < powerSize; i++) {
        auto power = Actor::GetActorNthSpell(player, i);
        if (!power) continue;

        auto powerType = power->GetSpellType();
        if (powerType == RE::MagicSystem::SpellType::kPower ||
            powerType == RE::MagicSystem::SpellType::kLesserPower) {
            if (favorOnly && !Extra::IsMagicFavorited(power)) continue;

            type.push_back(Data::DATATYPE::SHOUT);
            name.push_back(power->GetName());
            form.push_back(power->As<RE::TESForm>());
        }
    }

    auto baseShoutSize = Actor::GetBaseShoutCount(player);
    for (int i = 0; i < baseShoutSize; i++) {
        auto shout = Actor::GetBaseNthShout(player, i);
        if (!shout) continue;

        if (favorOnly && !Extra::IsMagicFavorited(shout)) continue;

        type.push_back(Data::DATATYPE::SHOUT);
        name.push_back(shout->GetName());
        form.push_back(shout->As<RE::TESForm>());
    }

    auto raceShoutSize = Actor::GetRaceShoutCount(player);
    for (int i = 0; i < raceShoutSize; i++) {
        auto shout = Actor::GetRaceNthShout(player, i);
        if (!shout) continue;

        if (favorOnly && !Extra::IsMagicFavorited(shout)) continue;

        type.push_back(Data::DATATYPE::SHOUT);
        name.push_back(shout->GetName());
        form.push_back(shout->As<RE::TESForm>());
    }

    for (int i = 0; i < type.size(); i++) {
        DataShout data(type[i], name[i], form[i]);
        shout.push_back(data);
    }
}

void DataHandler::InitArmor() {
    auto player = RE::PlayerCharacter::GetSingleton();
    if (!player || !player->IsInitialized()) return;

    auto playerBase = player->GetActorBase();
    if (!playerBase) return;

    auto config = ConfigHandler::GetSingleton();
    if (!config) return;

    auto ts = Translator::GetSingleton();
    if (!ts) return;

    bool favorOnly = config->Settings.favorOnly;
    std::vector<Data::DATATYPE> type;
    std::vector<std::string> name;
    std::vector<uint32_t> enchNum;
    std::vector<std::string> enchName;
    std::vector<float> tempVal;
    std::vector<RE::TESForm*> form;

    auto inv = player->GetInventory();
    for (const auto& [item, data] : inv) {
        const auto& [numItem, entry] = data;
        if (numItem > 0 && item->Is(RE::FormType::Armor)) {
            uint32_t numExtra = 0;

            auto extraLists = entry->extraLists;
            if (extraLists) {
                for (auto& _xList : *extraLists) {
                    ++numExtra;
                    if (favorOnly && !Extra::IsFavorited(_xList)) continue;

                    type.push_back(Data::DATATYPE::ARMOR);
                    name.push_back(item->GetName());
                    enchNum.push_back(Extra::GetEnchNum(item, _xList));
                    enchName.push_back(Extra::GetEnchName(item, _xList));
                    tempVal.push_back(Extra::GetTempValue(_xList));
                    form.push_back(item->As<RE::TESForm>());
                }
            }

            if (favorOnly) continue;

            for (uint32_t i = numExtra; i < numItem; i++) {
                type.push_back(Data::DATATYPE::ARMOR);
                name.push_back(item->GetName());
                enchNum.push_back(Extra::GetEnchNum(item, nullptr));
                enchName.push_back(Extra::GetEnchName(item, nullptr));
                tempVal.push_back(0.0f);
                form.push_back(item->As<RE::TESForm>());
            }
        }
    }

    for (int i = 0; i < type.size(); i++) {
        DataArmor data(type[i], name[i], enchNum[i], enchName[i], tempVal[i], form[i]);
        armor.push_back(data);
    }
}

void DataHandler::InitPotion() {
    auto player = RE::PlayerCharacter::GetSingleton();
    if (!player || !player->IsInitialized()) return;

    auto config = ConfigHandler::GetSingleton();
    if (!config) return;

    auto ts = Translator::GetSingleton();
    if (!ts) return;

    auto TESDataHandler = RE::TESDataHandler::GetSingleton();
    if (!TESDataHandler) return;

    bool favorOnly = config->Settings.favorOnly;
    
    std::vector<Data::DATATYPE> type;
    std::vector<Data::DATATYPE> type_health;
    std::vector<Data::DATATYPE> type_magicka;
    std::vector<Data::DATATYPE> type_stamina;
    std::vector<std::string> name;
    std::vector<std::string> name_health;
    std::vector<std::string> name_magicka;
    std::vector<std::string> name_stamina;
    std::vector<RE::TESForm*> form;
    std::vector<RE::TESForm*> form_health;
    std::vector<RE::TESForm*> form_magicka;
    std::vector<RE::TESForm*> form_stamina;

    for (int i = 0; i < 3; i++) {
        Data::DATATYPE _type;
        std::string _name;
        if (i == 0) {
            _type = Data::DATATYPE::NOTHING;
            _name = TRANSLATE("_NOTHING");
        } else if (i == 1) {
            _type = Data::DATATYPE::POTION_AUTO_HIGHEST;
            _name = TRANSLATE("_AUTO_HIGHEST");
        } else if (i == 2) {
            _type = Data::DATATYPE::POTION_AUTO_LOWEST;
            _name = TRANSLATE("_AUTO_LOWEST");
        }
        type_health.push_back(_type);
        type_magicka.push_back(_type);
        type_stamina.push_back(_type);
        name_health.push_back(_name);
        name_magicka.push_back(_name);
        name_stamina.push_back(_name);
        form_health.push_back(nullptr);
        form_magicka.push_back(nullptr);
        form_stamina.push_back(nullptr);
    }

    auto inv = player->GetInventory();
    for (const auto& [item, data] : inv) {
        const auto& [numItem, entry] = data;
        if (numItem > 0 && item->Is(RE::FormType::AlchemyItem)) {
            auto potion = item->As<RE::AlchemyItem>();
            if (!potion) continue;

            uint32_t foundType = 0;
            for (auto effect : potion->effects) {
                auto baseEffect = effect->baseEffect;
                if (!baseEffect) break;

                auto formid = baseEffect->GetFormID();
                for (const auto& elem : config->healthVec) {
                    auto compare = TESDataHandler->LookupFormID(elem.formid, elem.modname);
                    if (formid == compare) {
                        foundType = 1;
                        break;
                    }
                }
                for (const auto& elem : config->magickaVec) {
                    auto compare = TESDataHandler->LookupFormID(elem.formid, elem.modname);
                    if (formid == compare) {
                        foundType = 2;
                        break;
                    }
                }
                for (const auto& elem : config->staminaVec) {
                    auto compare = TESDataHandler->LookupFormID(elem.formid, elem.modname);
                    if (formid == compare) {
                        foundType = 3;
                        break;
                    }
                }
            }

            auto extraLists = entry->extraLists;
            if (extraLists) {
                for (auto& _xList : *extraLists) {
                    if (favorOnly && !Extra::IsFavorited(_xList)) continue;

                    switch (foundType) {
                        case 1:
                            type_health.push_back(Data::DATATYPE::POTION);
                            name_health.push_back(item->GetName());
                            form_health.push_back(item->As<RE::TESForm>());
                            break;
                        case 2:
                            type_magicka.push_back(Data::DATATYPE::POTION);
                            name_magicka.push_back(item->GetName());
                            form_magicka.push_back(item->As<RE::TESForm>());
                            break;
                        case 3:
                            type_stamina.push_back(Data::DATATYPE::POTION);
                            name_stamina.push_back(item->GetName());
                            form_stamina.push_back(item->As<RE::TESForm>());
                            break;
                        default:
                            type.push_back(Data::DATATYPE::POTION);
                            name.push_back(item->GetName());
                            form.push_back(item->As<RE::TESForm>());
                            break;
                    }
                }
            }

            if (favorOnly) continue;

            switch (foundType) {
                case 1:
                    type_health.push_back(Data::DATATYPE::POTION);
                    name_health.push_back(item->GetName());
                    form_health.push_back(item->As<RE::TESForm>());
                    break;
                case 2:
                    type_magicka.push_back(Data::DATATYPE::POTION);
                    name_magicka.push_back(item->GetName());
                    form_magicka.push_back(item->As<RE::TESForm>());
                    break;
                case 3:
                    type_stamina.push_back(Data::DATATYPE::POTION);
                    name_stamina.push_back(item->GetName());
                    form_stamina.push_back(item->As<RE::TESForm>());
                    break;
                default:
                    type.push_back(Data::DATATYPE::POTION);
                    name.push_back(item->GetName());
                    form.push_back(item->As<RE::TESForm>());
                    break;
            }
        }
    }

    for (int i = 0; i < type.size(); i++) {
        DataPotion data(type[i], name[i], form[i]);
        potion.push_back(data);
    }
    for (int i = 0; i < type_health.size(); i++) {
        DataPotion data(type_health[i], name_health[i], form_health[i]);
        potion_health.push_back(data);
    }
    for (int i = 0; i < type_magicka.size(); i++) {
        DataPotion data(type_magicka[i], name_magicka[i], form_magicka[i]);
        potion_magicka.push_back(data);
    }
    for (int i = 0; i < type_stamina.size(); i++) {
        DataPotion data(type_stamina[i], name_stamina[i], form_stamina[i]);
        potion_stamina.push_back(data);
    }
}

std::string WidgetIcon::Pack() {
    std::string result;

    result += std::to_string(enable) + Utility::delimiter;
    result += type + Utility::delimiter;
    result += std::to_string(offsetX) + Utility::delimiter;
    result += std::to_string(offsetY) + Utility::delimiter;

    return result;
}

WidgetIcon WidgetIcon::Unpack(const std::string& _packed) {
    std::vector<std::string> dataVec = Utility::Split(_packed, Utility::delimiter);

    assert(dataVec.size() == 4);

    WidgetIcon result;
    result.enable = Utility::to_bool(dataVec[0]);
    result.type = dataVec[1];
    result.offsetX = std::stoi(dataVec[2]);
    result.offsetY = std::stoi(dataVec[3]);

    return result;
}

std::string WidgetText::Pack() {
    std::string result;

    result += std::to_string(enable) + Utility::delimiter;
    result += std::to_string(static_cast<uint32_t>(align)) + Utility::delimiter;
    result += std::to_string(offsetX) + Utility::delimiter;
    result += std::to_string(offsetY) + Utility::delimiter;

    return result;
}

WidgetText WidgetText::Unpack(const std::string& _packed) {
    std::vector<std::string> dataVec = Utility::Split(_packed, Utility::delimiter);

    assert(dataVec.size() == 4);

    WidgetText result;
    result.enable = Utility::to_bool(dataVec[0]);
    result.align = static_cast<WidgetText::ALIGN_TYPE>(std::stoi(dataVec[1]));
    result.offsetX = std::stoi(dataVec[2]);
    result.offsetY = std::stoi(dataVec[3]);

    return result;
}