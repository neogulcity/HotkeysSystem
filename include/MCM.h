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

    enum class eListType {
        Widget,
        Weapon,
        Shout,
        Items,
        CycleItems,
        Equipsets
    };

    class DataHolder {
    public:
        [[nodiscard]] static DataHolder& GetSingleton() noexcept;

        struct Setting {
            bool mFavor;           // Favorited only option
            eSortType mSort;       // Equipset sorting type
            int32_t mModifier[3];  // Modifier keycode
        };

        struct List {
            std::vector<std::pair<std::string, std::string>> mWidgetList;  // first: Widget type, second: Widget path
            std::vector<std::tuple<std::string, RE::TESForm*, RE::ExtraDataList*>> mWeaponList;   // first: Weapon name, second: Weapon form, third: Extradatalist
            std::vector<std::pair<std::string, RE::TESForm*>> mShoutList;         // Shout list
            std::vector<std::tuple<std::string, RE::TESForm*, RE::ExtraDataList*>> mItemsList;    // Items list
            std::vector<std::string> mCycleItemsList;  // CycleItems list
        };

        Setting setting;
        List list;

    private:
        DataHolder() = default;
    };

    void Init_WidgetList();
    void Init_WeaponList();
    void Init_ShoutList();
    void Init_ItemsList();
    void Init_CycleItemsList();
    void ClearList();
}