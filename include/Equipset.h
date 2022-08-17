#pragma once

namespace MCM {
    enum class eAction;
}

using namespace SKSE;

namespace UIHS {
    class EquipsetManager;
}

class CycleEquipset;

class Hotkey {
public:
    Hotkey() : mKeyCode(-1) {
        mModifier[0] = false;
        mModifier[1] = false;
        mModifier[2] = false;
    }

    Hotkey(int32_t _keycode, bool _modifier[3]) : mKeyCode(_keycode) {
        mModifier[0] = _modifier[0];
        mModifier[1] = _modifier[1];
        mModifier[2] = _modifier[2];
    }

    int32_t mKeyCode;   // Hotkey keycode
    bool mModifier[3];  // Modifier key
};

class Option {
public:
    Option() : mSound(false), mToggleEquip(false), mReEquip(false), mBeast(false) {}

    bool mSound;        // Equip sound
    bool mToggleEquip;  // Toggle equip/unequip
    bool mReEquip;      // Re equip
    bool mBeast;        // Beast Hotkey
};

class CycleOption {
public:
    CycleOption() : mPersist(false), mExpire(0.0f), mReset(0.0f), mBeast(false) {}

    bool mPersist;  // Cycle Persist
    float mExpire;  // Cycle Expire
    float mReset;   // Cycle Reset
    bool mBeast;    // Beast Hotkey
};

class Widget {
public:
    Widget() : mWidget(""), mHpos(0), mVpos(0), mDisplayWidget(false), mDisplayName(false), mDisplayHotkey(false) {}

    std::string mWidget;  // Widget icon type
    int32_t mHpos;        // Widget horizontal position
    int32_t mVpos;        // Widget vertical position
    bool mDisplayWidget;  // Enable widget equipset icon on hud
    bool mDisplayName;    // Enable widget equipset name on hud
    bool mDisplayHotkey;  // Enable widget equipset hotkey on hud
};

class Equipment {
public:
    Equipment() {
        mLeft.option = 0;
        mLeft.numEnch = 0;
        mLeft.form = nullptr;
        mLeft.xList = nullptr;
        mLeft.hasExtra = std::make_pair(false, false);
        mLeft.extraData = std::make_pair(nullptr, 0.0f);

        mRight.option = 0;
        mRight.numEnch = 0;
        mRight.form = nullptr;
        mRight.xList = nullptr;
        mRight.hasExtra = std::make_pair(false, false);
        mRight.extraData = std::make_pair(nullptr, 0.0f);

        mShout.option = 0;
        mShout.form = nullptr;
    }

    struct Weapon {
        int32_t option;                                    // Equip behavior option
        uint32_t numEnch;                                  // Number of enchantment
        RE::TESForm* form;                                 // Form
        RE::ExtraDataList* xList;                          // ExtraDataList
        std::pair<bool, bool> hasExtra;                    // first: Enchanted bool, second: Tempered bool
        std::pair<RE::EnchantmentItem*, float> extraData;  // first: Enchantment, second: Tempered value
    };

    struct Shout {
        int32_t option;     // Equip behavior option
        RE::TESForm* form;  // Shout/power form
    };

    struct Items {
        uint32_t numItems;                                              // Number of items
        std::vector<uint32_t> numEnch;                                  // Number of enchantment
        std::vector<RE::TESForm*> form;                                 // Form
        std::vector<RE::ExtraDataList*> xList;                          // ExtraDataList
        std::vector<std::pair<bool, bool>> hasExtra;                    // first: Enchanted bool, second: Tempered bool
        std::vector<std::pair<RE::EnchantmentItem*, float>> extraData;  // first: Enchantment, second: Tempered value
    };

    Weapon mLeft;
    Weapon mRight;
    Shout mShout;
    Items mItems;
};

class Equipset {
public:
    friend class CycleEquipset;
    friend class UIHS::EquipsetManager;

    Equipset(std::string _name, Hotkey* _hotkey, Option* _option, Widget* _widget, Equipment* _equipment)
        : mName(_name),
          mHotkey(_hotkey),
          mOption(_option),
          mWidget(_widget),
          mEquipment(_equipment) {}

    ~Equipset() {
        delete mHotkey;
        delete mOption;
        delete mWidget;
        delete mEquipment;
    }

private:
    std::string mName; // Equipset name
    Hotkey* mHotkey;
    Option* mOption;
    Widget* mWidget;
    Equipment* mEquipment;
};

class CycleEquipset {
public:
    friend class UIHS::EquipsetManager;

    CycleEquipset(std::string _name, Hotkey* _hotkey, CycleOption* _option, Widget* _widget, std::vector<std::string> _cycleItems, uint32_t _cycleIndex)
        : mName(_name),
          mHotkey(_hotkey),
          mOption(_option),
          mWidget(_widget),
          mCycleItems(_cycleItems),
          mCycleIndex(_cycleIndex){}

private:
    std::string mName;  // Equipset name
    std::vector<std::string> mCycleItems; // Conatined Equipsets name.
    Hotkey* mHotkey;
    CycleOption* mOption;
    Widget* mWidget;
    uint32_t mCycleIndex;
};