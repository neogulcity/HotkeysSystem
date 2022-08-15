#pragma once

namespace Extra {
	bool IsEnchanted(const RE::TESBoundObject* _obj);
    bool IsEnchanted(RE::ExtraDataList* _xList);
    bool IsTempered(RE::ExtraDataList* _xList);
    bool HasDisplayName(RE::ExtraDataList* _xList);

	RE::EnchantmentItem* GetEnchantment(const RE::TESBoundObject* _obj);
    RE::EnchantmentItem* GetEnchantment(RE::ExtraDataList* _xList);

	uint32_t GetNumEnchantment(const RE::TESBoundObject* _obj);
    uint32_t GetNumEnchantment(RE::ExtraDataList* _xList);

	float GetHealth(RE::ExtraDataList* _xList);

	RE::BSFixedString GetDisplayName(RE::ExtraDataList* _xList);

	bool IsFavorited(RE::ExtraDataList* _xList);
    bool IsFavorited(RE::BSSimpleList<RE::ExtraDataList*> _extraList);
    bool IsMagicFavorited(RE::TESForm* _form);
    uint32_t GetNumFavorited(RE::BSSimpleList<RE::ExtraDataList*>* _extraLists);
}