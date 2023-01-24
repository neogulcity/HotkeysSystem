#pragma once

class HUDHandler : public RE::BSTEventSink<RE::MenuOpenCloseEvent> {
private:
    using EventResult = RE::BSEventNotifyControl;

public:
    static void Register();

    virtual EventResult ProcessEvent(const RE::MenuOpenCloseEvent* _event,
                                     RE::BSTEventSource<RE::MenuOpenCloseEvent>* _eventSource) override;

public:
    static HUDHandler* GetSingleton() {
        static HUDHandler singleton;
        return std::addressof(singleton);
    }

private:
    HUDHandler() {}
    HUDHandler(const HUDHandler&) = delete;
    HUDHandler(HUDHandler&&) = delete;

    ~HUDHandler() = default;

    HUDHandler& operator=(const HUDHandler&) = delete;
    HUDHandler& operator=(HUDHandler&&) = delete;
};