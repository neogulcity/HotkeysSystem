#include "ExtraData.h"

namespace Extra {
    bool IsEnchanted(const RE::TESBoundObject* _obj)
    {
        if (!_obj) {
            return false;
        }

        if (_obj->formType == RE::FormType::Weapon) {
            auto weapon = _obj->As<RE::TESObjectWEAP>();
            return weapon && weapon->formEnchanting;
        } else if (_obj->formType == RE::FormType::Armor) {
            auto armor = _obj->As<RE::TESObjectARMO>();
            return armor && armor->formEnchanting;
        }

        return false;
    }

    bool IsEnchanted(RE::ExtraDataList* _xList)
    {
        if (_xList && _xList->HasType(RE::ExtraDataType::kEnchantment)) {
            return true;
        }

        return false;
    }

    bool IsTempered(RE::ExtraDataList* _xList)
    {
        if (_xList && _xList->HasType(RE::ExtraDataType::kHealth)) {
            return true;
        }

        return false;
    }

    bool HasDisplayName(RE::ExtraDataList* _xList)
    {
        if (_xList && _xList->HasType(RE::ExtraDataType::kTextDisplayData)) {
            return true;
        }

        return false;
    }

    RE::EnchantmentItem* GetEnchantment(const RE::TESBoundObject* _obj)
    {
        RE::EnchantmentItem* result = nullptr;
        if (!_obj) {
            return result;
        }

        if (_obj->formType == RE::FormType::Weapon) {
            auto weapon = _obj->As<RE::TESObjectWEAP>();
            result = weapon && weapon->formEnchanting ? weapon->formEnchanting : result;
        } else if (_obj->formType == RE::FormType::Armor) {
            auto armor = _obj->As<RE::TESObjectARMO>();
            result = armor && armor->formEnchanting ? armor->formEnchanting : result;
        }

        return result;
    }

    RE::EnchantmentItem* GetEnchantment(RE::ExtraDataList* _xList)
    {
        RE::EnchantmentItem* result = nullptr;
        if (!_xList) {
            return result;
        }

        if (IsEnchanted(_xList)) {
            auto xEnch = _xList->GetByType<RE::ExtraEnchantment>();
            result = xEnch && xEnch->enchantment ? xEnch->enchantment : result;
        }

        return result;
    }

    uint32_t GetNumEnchantment(const RE::TESBoundObject* _obj)
    {
        uint32_t result = 0;
        if (!_obj) {
            return result;
        }

        if (IsEnchanted(_obj)) {
            auto xEnch = GetEnchantment(_obj);
            result = xEnch ? xEnch->effects.size() : result;
        }

        return result;
    }

    uint32_t GetNumEnchantment(RE::ExtraDataList* _xList)
    {
        uint32_t result = 0;
        if (!_xList) {
            return result;
        }

        if (IsEnchanted(_xList)) {
            auto xEnch = GetEnchantment(_xList);
            result = xEnch ? xEnch->effects.size() : result;
        }

        return result;
    }

    float GetHealth(RE::ExtraDataList* _xList)
    {
        float result = 0.0f;
        if (!_xList) {
            return result;
        }

        if (IsTempered(_xList)) {
            auto xEnch = _xList->GetByType<RE::ExtraHealth>();
            result = xEnch ? xEnch->health : result;
        }

        return result;
    }

    RE::BSFixedString GetDisplayName(RE::ExtraDataList* _xList)
    {
        RE::BSFixedString result = "";
        if (!_xList) {
            return result;
        }

        if (HasDisplayName(_xList)) {
            auto xName = _xList->GetByType<RE::ExtraTextDisplayData>();
            result = xName ? xName->displayName : result;
        }

        return result;
    }

    bool IsFavorited(RE::ExtraDataList* _xList)
    {
        if (_xList && _xList->HasType<RE::ExtraHotkey>()) {
            return true;
        }

        return false;
    }

    bool IsFavorited(RE::BSSimpleList<RE::ExtraDataList*> _extraLists)
    {
        if (!_extraLists) {
            return false;
        }

        for (const auto& xList : *_extraLists) {
            if (_xList && _xList->HasType<RE::ExtraHotkey>()) {
                return true;
            }
        }

        return false;
    }

    bool IsMagicFavorited(RE::TESForm* _form)
    {
        if (!_form) {
            return false;
        }

        if (_form->Is(RE::FormType::Spell, RE::FormType::Shout)) {
            const auto magicFavorites = RE::MagicFavorites::GetSingleton();
            if (magicFavorites && std::ranges::find(magicFavorites->spells, _form) != magicFavorites->spells.end()) {
                return true;
            }
        }

        return false;
    }

    uint32_t GetNumFavorited(RE::BSSimpleList<RE::ExtraDataList*>* _extraLists)
    {
        uint32_t result = 0;
        if (!_extraLists) {
            return result;
        }

        for (const auto& xList : *_extraLists) {
            if (IsFavorited(xList)) {
                ++result;
            }
        }

        return result;
    }
}