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

    bool IsFavorited(RE::BSSimpleList<RE::ExtraDataList*>* _extraLists)
    {
        if (!_extraLists) {
            return false;
        }

        for (const auto& xList : *_extraLists) {
            if (xList && xList->HasType<RE::ExtraHotkey>()) {
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

    RE::ExtraDataList* SearchExtraList(RE::TESForm* _form, uint32_t _numEnch, RE::EnchantmentItem* _ench, float _health)
    {
        RE::ExtraDataList* result = nullptr;

        if (!_form) {
            return result;
        }

        auto playerref = RE::PlayerCharacter::GetSingleton();
        if (!playerref) {
            return result;
        }

        auto inv = playerref->GetInventory();
        for (const auto& [item, data] : inv) {
            const auto& [numItem, entry] = data;
            if (!item->Is(RE::FormType::Weapon, RE::FormType::Armor)) {
                continue;
            }

            if (numItem > 0 && item->GetFormID() == _form->GetFormID()) {
                RE::EnchantmentItem* formEnchantment = nullptr;

                if (item->formType == RE::FormType::Weapon) {
                    auto weapon = item->As<RE::TESObjectWEAP>();
                    formEnchantment = weapon && weapon->formEnchanting ? weapon->formEnchanting : nullptr;
                } else if (item->formType == RE::FormType::Armor) {
                    auto armor = item->As<RE::TESObjectARMO>();
                    formEnchantment = armor && armor->formEnchanting ? armor->formEnchanting : nullptr;
                }

                if (formEnchantment) {
                    auto extraLists = entry->extraLists;
                    if (extraLists) {
                        for (auto& xList : *extraLists) {
                            if (Extra::IsTempered(xList)) {
                                if (!_ench && _health != 0.0f) {
                                    bool bTemp = _health == Extra::GetHealth(xList) ? true : false;
                                    result = bTemp ? xList : nullptr;
                                }
                            }
                        }
                    }
                } else {
                    auto extraLists = entry->extraLists;
                    if (extraLists) {
                        for (auto& xList : *extraLists) {
                            if (Extra::IsEnchanted(xList) && Extra::IsTempered(xList)) {
                                if (_ench && _health != 0.0f) {
                                    bool bNumEnch = _numEnch == Extra::GetNumEnchantment(xList) ? true : false;
                                    bool bEnch = _ench == Extra::GetEnchantment(xList) ? true : false;
                                    bool bTemp = _health == Extra::GetHealth(xList) ? true : false;
                                    result = bNumEnch && bEnch && bTemp ? xList : nullptr;
                                }
                            } else if (Extra::IsEnchanted(xList) && !Extra::IsTempered(xList)) {
                                if (_ench && _health == 0.0f) {
                                    bool bNumEnch = _numEnch == Extra::GetNumEnchantment(xList) ? true : false;
                                    bool bEnch = _ench == Extra::GetEnchantment(xList) ? true : false;
                                    result = bNumEnch && bEnch ? xList : nullptr;
                                }
                            } else if (!Extra::IsEnchanted(xList) && Extra::IsTempered(xList)) {
                                if (!_ench && _health != 0.0f) {
                                    bool bTemp = _health == Extra::GetHealth(xList) ? true : false;
                                    result = bTemp ? xList : nullptr;
                                }
                            }
                        }
                    }
                }
            }
        }
        
        return result;
    }
}