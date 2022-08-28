#pragma once

namespace Scaleform {
    class WidgetMenu;
}

class WidgetHandler {
private:
    using WidgetMenu = Scaleform::WidgetMenu;

public:
    enum eIDType {
        Widget,
        Text
    };

    static WidgetHandler* GetSingleton() {
        static WidgetHandler singleton;
        return std::addressof(singleton);
    }

    void RefreshWidgetMenu()
	{
		auto task = SKSE::GetTaskInterface();
		task->AddTask([this]() {
			_refreshWidgetMenu = true;
		});
	}

    void OpenWidgetMenu();
    void CloseWidgetMenu();

    void LoadWidget(std::string _path, int32_t _x, int32_t _y, int32_t _width, int32_t _height, int32_t _alpha);
    void LoadText(std::string _text, std::string _font, int32_t _size, int32_t _x, int32_t _y);
    void UnloadWidget(int32_t _id);
    void UnloadText(int32_t _id);

    void AddID(int32_t _id, eIDType _type);
    void RemoveID(int32_t _id);
    std::vector<std::pair<int32_t, eIDType>> GetIDList();

    void SetText(int32_t _id, std::string _text);
    void SetSize(int32_t _id, int32_t _width, int32_t _height);
    void SetPos(int32_t _id, int32_t _x, int32_t _y);
    void SetAlpha(int32_t _id, int32_t _alpha);
    void SetMenuVisible(bool _visible);
    void SetMenuAlpha(int32_t _alpha);

    void Animate(int32_t _id);
    void ProcessAnimate();

    void DissolveIn();
    void ProcessDissolveIn();
    void DissolveOut();
    void ProcessDissolveOut();

protected:
    friend class WidgetMenu;

    void ProcessWidgetMenu(WidgetMenu& a_menu);

private:

    using WidgetTasklet = std::function<void(WidgetMenu&)>;
    using Lock = std::recursive_mutex;
	using Locker = std::lock_guard<Lock>;

    WidgetHandler() {}
    WidgetHandler(const WidgetHandler&) = delete;
    WidgetHandler(WidgetHandler&&) = delete;

    ~WidgetHandler() = default;

    WidgetHandler& operator=(const WidgetHandler&) = delete;
    WidgetHandler& operator=(WidgetHandler&&) = delete;

    void AddWidgetMenuTask(WidgetTasklet a_task);

    std::vector<WidgetTasklet> _WidgetMenuTaskQueue;

    mutable Lock _lock;
    bool _refreshWidgetMenu{false};
    bool _bVisible{true};
    std::vector<std::pair<int32_t, eIDType>> mIDHolder;
    std::vector<std::tuple<int32_t, std::string>> mText;
    std::vector<std::tuple<int32_t, int32_t, int32_t>> mSize;
    std::vector<std::tuple<int32_t, int32_t, int32_t>> mPos;
    std::vector<std::tuple<int32_t, int32_t>> mAlpha;
    std::vector<int32_t> mAnimate;
    bool mDissolveIn;
    bool mDissolveOut;
};