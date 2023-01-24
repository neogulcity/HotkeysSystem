#pragma once

#include <unordered_map>

class InputHandler : public RE::BSTEventSink<RE::InputEvent*> {
private:
    using EventResult = RE::BSEventNotifyControl;

    bool isModifier1{false};
    bool isModifier2{false};
    bool isModifier3{false};
    std::unordered_map<uint32_t, uint32_t> keyMap;
    std::unordered_map<uint32_t, uint32_t> padMap;

    uint32_t GetImGuiKey(const uint32_t& _scanCode, RE::INPUT_DEVICE _device);

public:
    static void Register();

    virtual RE::BSEventNotifyControl ProcessEvent(RE::InputEvent* const* _event,
                                                  RE::BSTEventSource<RE::InputEvent*>* _eventSource) override;

public:
    static InputHandler* GetSingleton() {
        static InputHandler listener;
        return std::addressof(listener);
    }

private:
    InputHandler();
    InputHandler(const InputHandler&) = delete;
    InputHandler(InputHandler&&) = delete;

    ~InputHandler() = default;

    InputHandler& operator=(const InputHandler&) = delete;
    InputHandler& operator=(InputHandler&&) = delete;
};

class CharEvent : public RE::InputEvent {
public:
    uint32_t keyCode;  // 18 (ascii code)
};