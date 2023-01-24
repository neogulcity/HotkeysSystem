#pragma once

class CombatHandler : public RE::BSTEventSink<RE::TESCombatEvent> {
private:
    using EventResult = RE::BSEventNotifyControl;

public:
    static void Register();

    virtual EventResult ProcessEvent(const RE::TESCombatEvent* _event,
                                     RE::BSTEventSource<RE::TESCombatEvent>* _eventSource) override;

public:
    static CombatHandler* GetSingleton() {
        static CombatHandler listener;
        return std::addressof(listener);
    }

private:
    CombatHandler() {}
    CombatHandler(const CombatHandler&) = delete;
    CombatHandler(CombatHandler&&) = delete;

    ~CombatHandler() = default;

    CombatHandler& operator=(const CombatHandler&) = delete;
    CombatHandler& operator=(CombatHandler&&) = delete;
};