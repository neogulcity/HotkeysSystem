#pragma once

#include <mutex>
#include <optional>
#include <string_view>

#include <imgui.h>

class GuiMenu {
private:
    bool imgui_inited = false;
    bool show = false;

    std::atomic<bool> reload_font = false;
    std::atomic<uint32_t> current_opened = 0;
    ImFont* font = nullptr;

    void DrawConfig();
    void DrawEquipment();

public:
    inline void NotifyInit() { imgui_inited = true; }
    inline void NotifyFontReload() { reload_font.store(true); }
    inline uint32_t GetCurrentOpened() { return current_opened.load(); }
    inline void SetCurrentOpened(const uint32_t& _value) { current_opened.store(_value); }

    void Toggle(std::optional<bool> enabled);
    void DrawMain();
    void LoadFont();
    void DisableInput(bool _status);

    inline bool isShow() { return show; }

public:
    static GuiMenu* GetSingleton() {
        static GuiMenu singleton;
        return std::addressof(singleton);
    }

private:
    GuiMenu();
    GuiMenu(const GuiMenu&) = delete;
    GuiMenu(GuiMenu&&) = delete;

    ~GuiMenu() = default;

    GuiMenu& operator=(const GuiMenu&) = delete;
    GuiMenu& operator=(GuiMenu&&) = delete;
};