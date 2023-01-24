#pragma once

class EquipHandler : public RE::BSTEventSink<RE::TESEquipEvent> {
private:
    using EventResult = RE::BSEventNotifyControl;

public:
    static void Register();

    virtual EventResult ProcessEvent(const RE::TESEquipEvent* _event,
                                     RE::BSTEventSource<RE::TESEquipEvent>* _eventSource) override;

public:
    static EquipHandler* GetSingleton() {
        static EquipHandler listener;
        return std::addressof(listener);
    }

private:
    EquipHandler() {}
    EquipHandler(const EquipHandler&) = delete;
    EquipHandler(EquipHandler&&) = delete;

    ~EquipHandler() = default;

    EquipHandler& operator=(const EquipHandler&) = delete;
    EquipHandler& operator=(EquipHandler&&) = delete;
};