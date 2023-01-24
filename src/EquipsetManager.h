#pragma once

#include "Equipset.h"

class EquipsetManager {
private:
    uint32_t widgetIndexHolder{1U};
    uint32_t sortOrderHolder{1U};

public:
    std::vector<Equipset*> equipsetVec;

public:
    enum class VALID_TYPE {
        GOOD,
        NAME_BLANK,
        NAME_CONFLICT,
        HOTKEY_CONFLICT
    };
    void Create(const NormalSet& _equipset, bool _assignOrder);
    void Create(const PotionSet& _equipset, bool _assignOrder);
    void Create(const CycleSet& _equipset, bool _assignOrder);
    void Remove(Equipset* _equipset);
    void RemoveAll();
    std::string GetNamePreset();
    std::pair<VALID_TYPE, std::string> IsCreateValid(const std::string& _name, const uint32_t& _hotkey, bool _modifier1, bool _modifier2, bool _modifier3);
    std::pair<VALID_TYPE, std::string> IsEditValid(Equipset* _equipset, const std::string& _name, const uint32_t& _hotkey, bool _modifier1, bool _modifier2, bool _modifier3);
    void ProcessEquip(const uint32_t& _code, bool _modifier1, bool _modifier2, bool _modifier3);
    void CalculateKeydown(const uint32_t& _code, bool _modifier1, bool _modifier2, bool _modifier3, float _time);
    Equipset* SearchEquipsetByName(const std::string& _name);
    void ExportEquipsets();
    void ImportEquipsets();
    void SyncSortOrder();
    void CreateAllWidget();
    void RemoveAllWidget();
    uint32_t AssignWidgetID();
    uint32_t AssignSortOrder();

public:
    static EquipsetManager* GetSingleton() {
        static EquipsetManager singleton;
        return std::addressof(singleton);
    }

private:
    EquipsetManager() {}
    EquipsetManager(const EquipsetManager&) = delete;
    EquipsetManager(EquipsetManager&&) = delete;

    ~EquipsetManager() = default;

    EquipsetManager& operator=(const EquipsetManager&) = delete;
    EquipsetManager& operator=(EquipsetManager&&) = delete;
};