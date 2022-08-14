#pragma once

namespace MCM {
    enum class eAction {
        Nothing,
        Unequip,
        Equip
    };

    enum class eSortType {
        CreateAsc,
        CreateDesc,
        NameAsc,
        NameDesc
    };

    class Hotkey {
    public:
        Hotkey() : mKeyCode(-1) {
            mModifier[0] = -1;
            mModifier[1] = -1;
            mModifier[2] = -1;
        }

        int32_t mKeyCode;      // Hotkey keycode
        int32_t mModifier[3];  // Modifier key
    };

    class Option {
    public:
        Option()
            : mSound(false),
              mToggleEquip(false),
              mReEquip(false) {}

        bool mSound;        // Equip sound
        bool mToggleEquip;  // Toggle equip/unequip
        bool mReEquip;      // Re equip
    };

    class Widget {
    public:
        Widget()
            : mIcon(""),
              mHpos(0),
              mVpos(0),
              mDisplayIcon(false),
              mDisplayName(false),
              mDisplayHotkey(false) {}

        std::string mIcon;    // Widget icon type
        int32_t mHpos;        // Widget horizontal position
        int32_t mVpos;        // Widget vertical position
        bool mDisplayIcon;    // Enable widget equipset icon on hud
        bool mDisplayName;    // Enable widget equipset name on hud
        bool mDisplayHotkey;  // Enable widget equipset hotkey on hud
    };

    class Equipment {
    public:
        Equipment()
            : mLeftOpt(0),
              mRightOpt(0),
              mShoutOpt(0),
              mLefthand(nullptr),
              mRighthand(nullptr),
              mShout(nullptr),
              mHasLeftExtra({false, false}),
              mNumLeftEnch(0),
              mLeftExtra({nullptr, 0.0f}),
              mHasRightExtra({false, false}),
              mNumRightEnch(0),
              mRightExtra({nullptr, 0.0f}) {}

        int32_t mLeftOpt;                                                 // Lefthand equip behavior option
        int32_t mRightOpt;                                                // Righthand equip behavior option
        int32_t mShoutOpt;                                                // Shout equip behavior option
        uint32_t mNumLeftEnch;                                            // Number of lefthand enchantment
        uint32_t mNumRightEnch;                                           // Number of righthand enchantment
        RE::TESForm* mLefthand;                                           // Lighthand form
        RE::TESForm* mRighthand;                                          // Righthand form
        RE::TESForm* mShout;                                              // Shout/power form
        std::pair<bool, bool> mHasLeftExtra;                              // first: Lefthand enchanted bool, second: Lefthand tempered bool
        std::pair<bool, bool> mHasRightExtra;                             // first: Righthand enchanted bool, second: Righthand tempered bool
        std::pair<RE::EnchantmentItem*, float> mLeftExtra;                // first: Lefthand enchantment, second: Lefthand tempered value
        std::pair<RE::EnchantmentItem*, float> mRightExtra;               // first: Righthand enchantment, second: Righthand tempered value
        std::vector<RE::TESForm*> mItems;                                 // Items form
        std::vector<std::pair<bool, bool>> mHasItemsExtra;                // first: Items enchanted bool, second: Items tempered bool
        std::vector<uint32_t> mNumItemsEnch;                              // Number of items enchantment
        std::vector<std::pair<RE::EnchantmentItem*, float>> mItemsExtra;  // first: Items enchantment, second: Items tempered value
    };

    class Setting {
    public:
        [[nodiscard]] static Setting& GetSingleton() noexcept;

        bool mFavor;        // Favorited only option
        eSortType mSort;    // Equipset sorting type

    private:
        Setting() = default;
    };
}