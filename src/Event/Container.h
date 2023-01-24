#pragma once

class ContainerHandler : public RE::BSTEventSink<RE::TESContainerChangedEvent> {
private:
    using EventResult = RE::BSEventNotifyControl;

public:
    static void Register();

    virtual EventResult ProcessEvent(const RE::TESContainerChangedEvent* _event,
                                     RE::BSTEventSource<RE::TESContainerChangedEvent>* _eventSource) override;

public:
    static ContainerHandler* GetSingleton() {
        static ContainerHandler listener;
        return std::addressof(listener);
    }

private:
    ContainerHandler() {}
    ContainerHandler(const ContainerHandler&) = delete;
    ContainerHandler(ContainerHandler&&) = delete;

    ~ContainerHandler() = default;

    ContainerHandler& operator=(const ContainerHandler&) = delete;
    ContainerHandler& operator=(ContainerHandler&&) = delete;
};