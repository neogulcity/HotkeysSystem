#include "ExtraData.h"

namespace Extra {
    uint32_t GetEnchNum(const RE::TESBoundObject* _obj, const RE::ExtraDataList* _xList) {
        uint32_t result = 0;
        if (!_obj) {
            return result;
        }

        if (_obj->formType == RE::FormType::Weapon) {
            auto weapon = _obj->As<RE::TESObjectWEAP>();
            auto formEnch = weapon && weapon->formEnchanting ? weapon->formEnchanting : nullptr;
            result = formEnch ? formEnch->effects.size() : result;
        } else if (_obj->formType == RE::FormType::Armor) {
            auto armor = _obj->As<RE::TESObjectARMO>();
            auto formEnch = armor && armor->formEnchanting ? armor->formEnchanting : nullptr;
            result = formEnch ? formEnch->effects.size() : result;
        }

        if (!_xList) {
            return result;
        }

        if (!(_xList->HasType(RE::ExtraDataType::kEnchantment))) {
            return result;
        }

        auto xEnch = _xList->GetByType<RE::ExtraEnchantment>();
        auto enchantment = xEnch ? xEnch->enchantment : nullptr;
        result = enchantment ? enchantment->effects.size() : result;

        return result;
    }

    std::string GetEnchName(const RE::TESBoundObject* _obj, const RE::ExtraDataList* _xList) {
        std::string result = ENCHNONE;
        if (!_obj) {
            return result;
        }

        if (_obj->formType == RE::FormType::Weapon) {
            auto weapon = _obj->As<RE::TESObjectWEAP>();
            auto formEnch = weapon && weapon->formEnchanting ? weapon->formEnchanting : nullptr;
            result = formEnch ? formEnch->GetName() : result;
        } else if (_obj->formType == RE::FormType::Armor) {
            auto armor = _obj->As<RE::TESObjectARMO>();
            auto formEnch = armor && armor->formEnchanting ? armor->formEnchanting : nullptr;
            result = formEnch ? formEnch->GetName() : result;
        }

        if (!_xList) {
            return result;
        }

        if (!(_xList->HasType(RE::ExtraDataType::kEnchantment))) {
            return result;
        }

        auto xEnch = _xList->GetByType<RE::ExtraEnchantment>();
        auto enchantment = xEnch ? xEnch->enchantment : nullptr;
        result = enchantment ? enchantment->GetName() : result;

        return result;
    }

    float GetTempValue(const RE::ExtraDataList* _xList) {
        float result = 0.0f;
        if (!_xList) {
            return result;
        }

        if (!(_xList->HasType(RE::ExtraDataType::kHealth))) {
            return result;
        }

        auto xHealth = _xList->GetByType<RE::ExtraHealth>();
        result = xHealth ? xHealth->health : result;

        return result;
    }

    bool IsFavorited(const RE::ExtraDataList* _xList) {
        if (_xList && _xList->HasType<RE::ExtraHotkey>()) {
            return true;
        }

        return false;
    }

    bool IsMagicFavorited(const RE::TESForm* _form) {
        if (!_form) {
            return false;
        }

        if (_form->Is(RE::FormType::Spell, RE::FormType::Shout)) {
            auto magicFavorites = RE::MagicFavorites::GetSingleton();
            if (magicFavorites && std::ranges::find(magicFavorites->spells, _form) != magicFavorites->spells.end()) {
                return true;
            }
        }

        return false;
    }

    RE::ExtraDataList* SearchExtraDataList(const std::string& _name, const uint32_t& _enchNum,
                                           const std::string& _enchName, const float& _tempVal) {
        auto player = RE::PlayerCharacter::GetSingleton();
        if (!player) return nullptr;

        auto inv = player->GetInventory();
        for (const auto& [item, data] : inv) {
            const auto& [numItem, entry] = data;
            if (!(numItem > 0 && item->Is(RE::FormType::Weapon, RE::FormType::Armor))) continue;

            if (item->GetName() != _name) continue;

            auto extraLists = entry->extraLists;
            if (!extraLists) return nullptr;

            for (auto& xList : *extraLists) {
                if (Extra::GetEnchNum(item, xList) == _enchNum &&
                    Extra::GetEnchName(item, xList) == _enchName &&
                    Extra::GetTempValue(xList) == _tempVal) {
                    return xList;
                }
            }
        }

        return nullptr;
    }
}