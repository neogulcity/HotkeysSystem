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
        SelectList,
        Equipset,
        Font
    };

    enum class eWidgetDisplay {
        Always,
        InCombat
    };

    class DataHolder {
    public:
        static DataHolder* GetSingleton()
	    {
		    static DataHolder singleton;
		    return std::addressof(singleton);
	    }

        struct Widget {
            std::string mFont;
            int32_t mFontSize;
            int32_t mSize;
            int32_t mAlpha;
            int32_t mDisplay;
            float mDelay;
        };

        struct Setting {
            int32_t mModifier[3];
            bool mFavor;           // Favorited only option
            eSortType mSort;       // Equipset sorting type
            bool mWidgetActive;
        };

        struct List {
            std::vector<std::pair<std::string, std::string>> mWidgetList;  // first: Widget type, second: Widget path
            std::vector<std::tuple<std::string, RE::TESForm*, RE::ExtraDataList*>> mWeaponList;   // first: Weapon name, second: Weapon form, third: Extradatalist
            std::vector<std::pair<std::string, RE::TESForm*>> mShoutList;         // Shout list
            std::vector<std::tuple<std::string, RE::TESForm*, RE::ExtraDataList*>> mItemsList;    // Items list
            std::vector<std::string> mCycleItemsList;  // CycleItems list
            std::vector<std::string> mFontList;  // Font list
        };

        Widget* widget;
        Setting* setting;
        List* list;

    private:
        DataHolder() {
            widget = new Widget;
            setting = new Setting;
            list = new List;

            assert(widget);
            assert(setting);
            assert(list);
        }

        ~DataHolder() {
            delete widget;
            delete setting;
            delete list;
        }

	    DataHolder(const DataHolder&) = delete;
	    DataHolder(DataHolder&&) = delete;

	    DataHolder& operator=(const DataHolder&) = delete;
	    DataHolder& operator=(DataHolder&&) = delete;
    };

    void Init_WidgetList();
    void Init_WeaponList();
    void Init_ShoutList();
    void Init_ItemsList();
    void Init_CycleItemsList();
    void Init_FontList();
    void SetSetting(std::vector<std::string> _data);
    void SetWidgetData(std::vector<std::string> _data);
    void SaveSetting(std::vector<std::string> _data);
    std::vector<std::string> LoadSetting();
    void ClearList();
    bool IsInventoryInit();
}