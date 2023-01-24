#pragma once

namespace Scaleform {
    class WidgetMenu;
}

class WidgetHandler {
private:
    using WidgetMenu = Scaleform::WidgetMenu;

    std::future<bool> expire_future;
    std::atomic<bool> shouldCloseExpire{false};
    std::atomic<float> expireProgress{0.0f};

public:
    void RefreshWidgetMenu() {
        auto task = SKSE::GetTaskInterface();
        task->AddTask([this]() { _refreshWidgetMenu = true; });
    }

    void OpenWidgetMenu();
    void CloseWidgetMenu();

    void SetMenuVisible(bool _visible);
    void LoadWidget(uint32_t _id, std::string _path, int32_t _x, int32_t _y, int32_t _width, int32_t _height, int32_t _alpha);
    void UnloadWidget(uint32_t _id);
    void LoadText(uint32_t _id, std::string _text, std::string _font, int32_t _x, int32_t _y, int32_t _align,
                  int32_t _size, int32_t _alpha, bool _shadow);
    void UnloadText(uint32_t _id);
    void Animate(uint32_t _previd, uint32_t _nextid);
    void SetText(uint32_t _id, std::string _text);
    void SetMenuAlpha(uint32_t _alpha);
    void MenuFadeIn();
    void MenuFadeOut();

    void ProcessFadeIn();
    void ProcessFadeOut();
    void SetExpireProgress(const float& _amount);
    void StartExpireTimer();
    void CloseExpireTimer();
    bool ExpireFunc();

protected:
    friend class WidgetMenu;

    void ProcessWidgetMenu(WidgetMenu& a_menu);

private:
    using WidgetTasklet = std::function<void(WidgetMenu&)>;
    using Lock = std::recursive_mutex;
    using Locker = std::lock_guard<Lock>;

    void AddWidgetMenuTask(WidgetTasklet a_task);

    std::vector<WidgetTasklet> _WidgetMenuTaskQueue;

    mutable Lock _lock;
    bool _refreshWidgetMenu{false};
    bool _bVisible{true};

public:
    static WidgetHandler* GetSingleton() {
        static WidgetHandler singleton;
        return std::addressof(singleton);
    }

private:
    WidgetHandler() {}
    WidgetHandler(const WidgetHandler&) = delete;
    WidgetHandler(WidgetHandler&&) = delete;

    ~WidgetHandler() = default;

    WidgetHandler& operator=(const WidgetHandler&) = delete;
    WidgetHandler& operator=(WidgetHandler&&) = delete;
};