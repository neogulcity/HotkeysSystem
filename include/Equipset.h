#pragma once

namespace MCM {
    enum class eAction;
    class Hotkey;
    class Option;
    class Widget;
    class Equipment;
}

using namespace SKSE;

namespace UIHS {
    class EquipsetManager;
}

class CycleEquipset;

class Equipset {
public:
    friend class CycleEquipset;
    friend class UIHS::EquipsetManager;

    Equipset(std::string _name, MCM::Hotkey* _hotkey, MCM::Option* _option, MCM::Widget* _widget, MCM::Equipment* _equipment)
        : next(nullptr),
          mName(_name),
          mHotkey(_hotkey),
          mOption(_option),
          mWidget(_widget),
          mEquipment(_equipment) {}

private:
    Equipset* next;

    std::string mName; // Equipset name
    MCM::Hotkey* mHotkey;
    MCM::Option* mOption;
    MCM::Widget* mWidget;
    MCM::Equipment* mEquipment;
};

class CycleEquipset {
public:
    CycleEquipset()
    : last(nullptr)
    {}
    void push_back(Equipset*);
    void pop_back();

private:
    Equipset* last;

    std::string mstrName;
    std::string mstrIcon;
    int32_t miHotkey;
    int32_t miModifierA;
    int32_t miModifierB;
    int32_t miModifierC;
    int32_t miHpos;
    int32_t miVpos;
    bool mbPersist;
    bool mbExpire;
    bool mbReset;
    bool mbDisplayIcon;
    bool mbDisplayName;
    bool mbDisplayHotkey;
};