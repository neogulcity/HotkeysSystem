#pragma once

class OnCombatEvent : public RE::BSTEventSink<RE::TESCombatEvent> {
    OnCombatEvent() = default;
    ~OnCombatEvent() = default;
    OnCombatEvent(const OnCombatEvent&) = delete;
    OnCombatEvent(OnCombatEvent&&) = delete;
    OnCombatEvent& operator=(const OnCombatEvent&) = delete;
    OnCombatEvent& operator=(OnCombatEvent&&) = delete;

public:

    virtual RE::BSEventNotifyControl ProcessEvent(const RE::TESCombatEvent* a_event, RE::BSTEventSource<RE::TESCombatEvent>* a_eventSource) override;
    static bool RegisterEvent();
};