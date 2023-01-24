#pragma once

#include "Data.h"

class Equipset {
public:
    enum class TYPE : std::uint8_t {
        NORMAL,
        POTION,
        CYCLE
    };

    TYPE type{TYPE::NORMAL};
	std::string name{""};
    uint32_t hotkey{0U};
    bool modifier1{false};
    bool modifier2{false};
    bool modifier3{false};
    uint32_t order{0U};

    Equipset() {}
    virtual void Equip() = 0;
    virtual void CreateWidget() = 0;
    virtual void RemoveWidget() = 0;
    void CreateWidgetBackground();
    void CreateWidgetIcon();
    void CreateWidgetText1();
    void CreateWidgetText2();
    void SyncEquipset(const std::string& _prevName, const std::string& _curName);
    void SyncWidget();
};

class NormalSet : public Equipset {
public:
    bool equipSound{true};
    bool toggleEquip{false};
    bool reEquip{false};
    DataWeapon lefthand;
    DataWeapon righthand;
    DataShout shout;
    std::vector<DataArmor> items;
    WidgetIcon widgetIcon;
    WidgetText widgetName;
    WidgetText widgetHotkey;
    WidgetID widgetID;

    NormalSet() {}
    virtual void Equip() override;
    virtual void CreateWidget() override;
    virtual void RemoveWidget() override;

    void operator=(const NormalSet& _equipset) {
        this->type = _equipset.type;
        this->name = _equipset.name;
        this->hotkey = _equipset.hotkey;
        this->modifier1 = _equipset.modifier1;
        this->modifier2 = _equipset.modifier2;
        this->modifier3 = _equipset.modifier3;
        this->equipSound = _equipset.equipSound;
        this->toggleEquip = _equipset.toggleEquip;
        this->reEquip = _equipset.reEquip;
        this->widgetIcon = _equipset.widgetIcon;
        this->widgetName = _equipset.widgetName;
        this->widgetHotkey = _equipset.widgetHotkey;
        this->lefthand = _equipset.lefthand;
        this->righthand = _equipset.righthand;
        this->shout = _equipset.shout;
        this->items = _equipset.items;
    }
};

class PotionSet : public Equipset {
public:
    bool equipSound{true};
    bool calcDuration{false};

    DataPotion health;
    DataPotion magicka;
    DataPotion stamina;
    std::vector<DataPotion> items;
    WidgetIcon widgetIcon;
    WidgetText widgetName;
    WidgetText widgetAmount;
    WidgetID widgetID;

    PotionSet() {}
    virtual void Equip() override;
    virtual void CreateWidget() override;
    virtual void RemoveWidget() override;
    void AssignAutoPotion();
    std::string GetPotionName();
    std::string GetPotionAmount();

    void operator=(const PotionSet& _equipset) {
        this->type = _equipset.type;
        this->name = _equipset.name;
        this->hotkey = _equipset.hotkey;
        this->modifier1 = _equipset.modifier1;
        this->modifier2 = _equipset.modifier2;
        this->modifier3 = _equipset.modifier3;
        this->equipSound = _equipset.equipSound;
        this->calcDuration = _equipset.calcDuration;
        this->widgetIcon = _equipset.widgetIcon;
        this->widgetName = _equipset.widgetName;
        this->widgetAmount = _equipset.widgetAmount;
        this->health = _equipset.health;
        this->magicka = _equipset.magicka;
        this->stamina = _equipset.stamina;
        this->items = _equipset.items;
    }
};

class CycleSet : public Equipset {
public:
    bool cyclePersist{false};
    float cycleExpire{0.0f};
    float cycleReset{0.0f};
    std::vector<std::string> items;
    WidgetIcon widgetIcon;
    WidgetText widgetName;
    WidgetText widgetHotkey;
    WidgetID widgetID;
    uint32_t cycleIndex{0U};
    bool isCycleInit{false};

    std::future<bool> expire_future;
    std::atomic<bool> shouldCloseExpire{false};
    std::atomic<float> cycleExpireProgress{0.0f};

    std::future<bool> reset_future;
    std::atomic<bool> shouldCloseReset{false};
    std::atomic<float> cycleResetProgress{0.0f};
    
public:
    void SetExpireProgress(const float& _amount);
    void StartExpireTimer();
    void CloseExpireTimer();
    bool ExpireFunc();

    void SetResetProgress(const float& _amount);
    void StartResetTimer();
    void CloseResetTimer();
    bool ResetFunc();

    CycleSet() {}
    virtual void Equip() override;
    virtual void CreateWidget() override;
    virtual void RemoveWidget() override;

    void operator=(const CycleSet& _cycleset) {
        this->type = _cycleset.type;
        this->name = _cycleset.name;
        this->hotkey = _cycleset.hotkey;
        this->modifier1 = _cycleset.modifier1;
        this->modifier2 = _cycleset.modifier2;
        this->modifier3 = _cycleset.modifier3;
        this->cyclePersist = _cycleset.cyclePersist;
        this->cycleExpire = _cycleset.cycleExpire;
        this->cycleReset = _cycleset.cycleReset;
        this->widgetIcon = _cycleset.widgetIcon;
        this->widgetName = _cycleset.widgetName;
        this->widgetHotkey = _cycleset.widgetHotkey;
        this->items = _cycleset.items;
    }
};