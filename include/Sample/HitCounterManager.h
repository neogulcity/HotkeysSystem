#pragma once

#include <RE/Skyrim.h>

namespace Sample {
#pragma warning(push)
#pragma warning(disable : 4251)
    /**
     * The class which tracks hit count information.
     *
     * <p>
     * All aspects of this are done in a singleton since SKSE cannot create instances of new Papyrus types such as a
     * hit counter.
     * </p>
     *
     * <p>
     * Hit count information must be tracked in the SKSE cosave when the game is save and loaded. Therefore the main
     * <code>SKSEQuery_Load</code> call must be sure to initialize the serialization handlers such that they will call
     * this class when it needs to save or load data.
     * </p>
     */
    class __declspec(dllexport) HitCounterManager {
    public:
        /**
         * Get the singleton instance of the <code>HitCounterManager</code>.
         */
        [[nodiscard]] static HitCounterManager& GetSingleton() noexcept;

        /**
         * Tells the manager that a particular actor should have its hits tracked.
         *
         * @param actor The actor to track.
         * @return <code>true</code> if <code>actor</code> is a valid actor and is not already tracked, otherwise
         *         <code>false</code>.
         */
        bool Track(RE::Actor* actor);

        /**
         * Tells the manager that a particular actor should no longer have its hits tracked.
         *
         * @param actor The actor to no longer track.
         * @return <code>true</code> if <code>actor</code> is a valid actor and was being tracked, otherwise
         *         <code>false</code>.
         */
        bool Untrack(RE::Actor* actor);

        /**
         * Register a hit has occurred on an actor.
         *
         * <p>
         * This will have no effect if the actor is not valid or if the actor is not currently tracked.
         * </p>
         *
         * @param target The actor that was hit.
         */
        inline void RegisterHit(RE::Actor* target) noexcept { Increment(target); }

        /**
         * Increment the hit count on an actor.
         *
         * <p>
         * This will have no effect if the actor is not valid or if the actor is not currently tracked.
         * </p>
         *
         * @param target The actor whose hit count should be incremented.
         * @param by The amount by which to increment the hit count.
         */
        void Increment(RE::Actor* target, int32_t by = 1);

        /**
         * Gets the current hit count for an actor.
         *
         * @param target The actor whose hit count should be returned.
         * @return Empty if the actor is not tracked, otherwise the number of hits that have been registered.
         */
        std::optional<int32_t> GetHitCount(RE::Actor* target) const noexcept;

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
        HitCounterManager() = default;

        mutable std::mutex _lock;
        std::unordered_map<RE::Actor*, int32_t> _hitCounts;
        std::unordered_set<RE::Actor*> _trackedActors;
    };
#pragma warning(pop)
}  // namespace Sample
