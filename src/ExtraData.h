#pragma once

namespace Extra {
    constexpr const char* ENCHNONE{"_ENCHNONE_"};

    uint32_t GetEnchNum(const RE::TESBoundObject* _obj, const RE::ExtraDataList* _xList);
    std::string GetEnchName(const RE::TESBoundObject* _obj, const RE::ExtraDataList* _xList);
    float GetTempValue(const RE::ExtraDataList* _xList);
    bool IsFavorited(const RE::ExtraDataList* _xList);
    bool IsMagicFavorited(const RE::TESForm* _form);

    RE::ExtraDataList* SearchExtraDataList(const std::string& _name, const uint32_t& _enchNum, const std::string& _enchName, const float& _tempVal);
}