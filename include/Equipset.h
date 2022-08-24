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
    Widget()
        : mWidget(""),
          mHpos(0),
          mVpos(0),
          mDisplayWidget(false),
          mDisplayName(false),
          mDisplayHotkey(false),
          mBackgroundID(-1),
          mHotkeyID(-1) {}

    std::string mWidget;  // Widget icon type
    int32_t mHpos;        // Widget horizontal position
    int32_t mVpos;        // Widget vertical position
    bool mDisplayWidget;  // Enable widget equipset icon on hud
    bool mDisplayName;    // Enable widget equipset name on hud
    bool mDisplayHotkey;  // Enable widget equipset hotkey on hud
    std::vector<int32_t> mWidgetID;
    std::vector<int32_t> mNameID;
    int32_t mHotkeyID;
    int32_t mBackgroundID;
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

    Equipset(uint32_t _order, std::string _name, Hotkey* _hotkey, Option* _option, Widget* _widget, Equipment* _equipment)
        : mOrder(_order),
          mName(_name),
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

    void Equip();

private:
    uint32_t mOrder;
    std::string mName; // Equipset name
    Hotkey* mHotkey;
    Option* mOption;
    Widget* mWidget;
    Equipment* mEquipment;
};

class CycleEquipset {
public:
    friend class UIHS::EquipsetManager;

    CycleEquipset(uint32_t _order, std::string _name, Hotkey* _hotkey, CycleOption* _option, Widget* _widget, std::vector<std::string> _cycleItems, std::pair<uint32_t, int32_t> _cycleIndex)
        : mOrder(_order),
          mName(_name),
          mHotkey(_hotkey),
          mOption(_option),
          mWidget(_widget),
          mCycleItems(_cycleItems),
          mCycleIndex(_cycleIndex),
          mExpireClose(true),
          mExpireWorking(false),
          mResetClose(true),
          mResetWorking(false),
          mRemain(0.0f) {}

    void Equip();
    bool Expire_Function();
    bool Reset_Function();
    void SetExpireTimer();
    void SetResetTimer();

    float GetRemain();
    void SetRemain(float _param);

    bool IsExpireClosing();
    void SetExpireClose(bool _param);
    bool IsExpireWorking();
    void SetExpireWorking(bool _param);

    bool IsResetClosing();
    void SetResetClose(bool _param);
    bool IsResetWorking();
    void SetResetWorking(bool _param);

private:
    using Lock = std::recursive_mutex;
	using Locker = std::lock_guard<Lock>;

    mutable Lock _lock;
    uint32_t mOrder;
    std::string mName;  // Equipset name
    std::vector<std::string> mCycleItems; // Conatined Equipsets name.
    Hotkey* mHotkey;
    CycleOption* mOption;
    Widget* mWidget;
    std::pair<uint32_t, int32_t> mCycleIndex; // first: Current index to equip, second: Previous index

    bool mExpireClose;
    bool mExpireWorking;
    bool mResetClose;
    bool mResetWorking;
    float mRemain;
    std::future<bool> mExpireHandle;
    std::future<bool> mResetHandle;
};

RE::TESForm* GetDummyDagger();
RE::TESForm* GetDummyShout();
RE::TESForm* GetEquippedShout(RE::Actor* _actor);
std::vector<RE::TESForm*> GetAllEquippedItems();
void EquipItem(RE::TESForm* _form, RE::BGSEquipSlot* _slot, bool _sound, RE::ExtraDataList* _xList, bool _queue, bool _force);
void UnequipItem(RE::TESForm* _form, RE::BGSEquipSlot* _slot, bool _sound, RE::ExtraDataList* _xList, bool _queue, bool _force);
