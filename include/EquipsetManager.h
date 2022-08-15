#pragma once

#include "Equipset.h"

namespace UIHS {
    class __declspec(dllexport) EquipsetManager {
    public:

        [[nodiscard]] static EquipsetManager& GetSingleton() noexcept;

        void NewEquipset(std::string _name, MCM::Hotkey* _hotkey, MCM::Option* _option, MCM::Widget* _widget, MCM::Equipment* _equipment);
        void Display();
        RE::BSFixedString GetNamePrefix();


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
    };
}
