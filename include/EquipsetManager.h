#pragma once

#include "Equipset.h"
#include "MCM.h"

namespace UIHS {
    class __declspec(dllexport) EquipsetManager {
    public:

        [[nodiscard]] static EquipsetManager& GetSingleton() noexcept;

        void NewEquipset(uint32_t _order, std::string _name, Hotkey* _hotkey, Option* _option, Widget* _widget, Equipment* _equipment);
        void NewCycleEquipset(uint32_t _order, std::string _name, Hotkey* _hotkey, CycleOption* _option, Widget* _widget, std::vector<std::string> _cycleItems, std::pair<uint32_t, int32_t> _cycleIndex);
        void EditEquipset(std::string _name, std::vector<std::string> _data);
        void RemoveEquipset(std::string _name);
        void Display();
        void DisplayCycle();
        Equipset* SearchEquipsetByName(std::string _name);
        CycleEquipset* SearchCycleEquipsetByName(std::string _name);
        const RE::BSFixedString GetNamePrefix();
        RE::BSFixedString GetKeyConflict(int32_t _key, std::vector<bool> _modifier, bool _beast);
        std::vector<std::string> GetEquipsetList();
        std::vector<std::string> GetCycleEquipsetList();
        std::vector<std::string> GetAllEquipsetList();
        std::vector<RE::BSFixedString> GetEquipsetData(RE::BSFixedString _name);
        uint32_t GetIndexFromList(std::string _name, MCM::eListType _type);
        uint32_t GetIndexFromList(RE::TESForm* _form, RE::ExtraDataList* _xList, MCM::eListType _type);
        bool IsNameConflict(RE::BSFixedString _name);
        bool IsCycleEquipset(RE::BSFixedString _name);
        std::vector<int32_t> GetKeycodeList();
        uint32_t GetAllEquipsetSize();
        std::vector<std::string> GetSortedEquipsetList();
        std::vector<std::string> GetAllSortedEquipsetList();
        void Exec(int32_t _code, bool _modifier1, bool _modifier2, bool _modifier3);
        void CalculateKeydown(int32_t _code, bool _modifier1, bool _modifier2, bool _modifier3, float _time);
        void InitWidget();
        void InitWidgetNext();
        void ClearWidget();
        void SaveEquipsetData();
        void LoadEquipsetData();
        void RemoveAllEquipset();

        void SetDissolveTimer();
        bool DissolveIn_Function();
        void DissolveOut_Function();

        float GetRemain();
        void SetRemain(float _param);

        bool IsThreadClosing();
        void SetThreadClose(bool _param);

        bool IsThreadWorking();
        void SetThreadWorking(bool _param);

        /**
         * The serialization handler for reverting game state.
         *
         * <p>
         * This is called as the handler for revert. Revert is called by SKSE on a plugin that is registered for
         * serialization handling on a new game or before a save game is loaded. It should be used to revert the state
         * of the plugin back to its default.
         * </p>
        */
        static void OnRevert(SKSE::SerializationInterface*);

        /**
         * The serialization handler for saving data to the cosave.
         *
         * @param serde The serialization interface used to write data.
         */
        static void OnGameSaved(SKSE::SerializationInterface* serde);

        /**
         * The serialization handler for loading data from a cosave.
         *
         * @param serde  The serialization interface used to read data.
         */
        static void OnGameLoaded(SKSE::SerializationInterface* serde);

    private:
        EquipsetManager() = default;

        mutable std::mutex _lock;

        std::vector<Equipset*> mEquipset;
        std::vector<CycleEquipset*> mCycleEquipset;
        bool mThreadClose{false};
        bool mThreadWorking{false};
        float mRemain{0.0f};
        std::future<bool> mThreadHandle;
    };
}