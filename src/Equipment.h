#pragma once

#include "Data.h"

class EquipmentBase {
public:
    enum class Type {
        ARMOR,
        WEAPON,
        SHOUT
    };

    Type type;
    WidgetIcon widgetIcon;
    WidgetText widgetName;
    WidgetID widgetID;

public:
    void CreateWidgetBackground();
    void CreateWidgetIcon(bool _unequip = false);
    void CreateWidgetText1();
};

class EquipmentArmor : public EquipmentBase {
public:
    std::string slotid{""};
};

class EquipmentWeapon : public EquipmentBase {
public:
    bool isLeft{false};
};

class EquipmentManager {
public:
    std::vector<EquipmentArmor> armor;
    EquipmentWeapon lefthand;
    EquipmentWeapon righthand;
    EquipmentBase shout;

public:
    void NotifyArmor(bool _isEquip, const std::vector<uint32_t>& _slot, const std::string& _name);
    void NotifyWeapon(bool _isEquip, RE::TESForm* _weapon);
    void NotifyShout(bool _isEquip, RE::TESForm* _shout);
    void CreateAllArmorWidget();
    void CreateAllWeaponWidget();
    void CreateAllShoutWidget();
    void RemoveAllArmorWidget();
    void RemoveAllWeaponWidget();
    void RemoveAllShoutWidget();

    void Load();
    void Save();

public:
    static EquipmentManager* GetSingleton() {
        static EquipmentManager singleton;
        return std::addressof(singleton);
    }

private:
    EquipmentManager();
    EquipmentManager(const EquipmentManager&) = delete;
    EquipmentManager(EquipmentManager&&) = delete;

    ~EquipmentManager() = default;

    EquipmentManager& operator=(const EquipmentManager&) = delete;
    EquipmentManager& operator=(EquipmentManager&&) = delete;
};

namespace ID {
    const std::string Null = "_NULL";

    const std::string Sword = "_SWORD";
    const std::string Dagger = "_DAGGER";
    const std::string Axe = "_AXE";
    const std::string Mace = "_MACE";
    const std::string Greatsword = "_GREATSWORD";
    const std::string Battleaxe = "_BATTLEAXE";
    const std::string Bow = "_BOW";
    const std::string Staff = "_STAFF";
    const std::string Crossbow = "_CROSSBOW";

    const std::string Alteration = "_ALTERATION";
    const std::string Conjuration = "_CONJURATION";
    const std::string Destruction = "_DESTRUCTION";
    const std::string Illusion = "_ILLUSION";
    const std::string Restoration = "_RESTORATION";
    
    const std::string Unarmed = "_UNARMED";
    const std::string Shield = "_SHIELD";
    const std::string Torch = "_TORCH";

    const std::string Power = "_POWER";
    const std::string Shout = "_SHOUT";
}