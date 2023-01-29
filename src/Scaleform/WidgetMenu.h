#pragma once

namespace Scaleform {
    class WidgetMenu : public RE::IMenu {
    private:
        using Super = RE::IMenu;

    public:
        static constexpr std::string_view MenuName() noexcept { return MENU_NAME; }
        static constexpr std::int8_t SortPriority() noexcept { return SORT_PRIORITY; }

        static void Register();
        void RefreshUI();

        void SetMenuVisible(bool _visible);
        void LoadWidget(uint32_t _id, std::string _path, int32_t _x, int32_t _y, int32_t _width, int32_t _height, int32_t _alpha);
        void UnloadWidget(uint32_t _id);
        void LoadText(uint32_t _id, std::string _text, std::string _font, int32_t _x, int32_t _y, int32_t _align, int32_t _size, int32_t _alpha, bool _shadow);
        void UnloadText(uint32_t _id);
        void SetText(uint32_t _id, std::string _text);
        void SetMenuAlpha(uint32_t _alpha);
        void MenuFadeIn();
        void MenuFadeOut();

    protected:
        using UIResult = RE::UI_MESSAGE_RESULTS;

        WidgetMenu() {
            auto menu = static_cast<Super*>(this);
            menu->depthPriority = SortPriority();
            auto scaleformManager = RE::BSScaleformManager::GetSingleton();
            [[maybe_unused]] const auto success =
                scaleformManager->LoadMovieEx(menu, FILE_NAME, [](RE::GFxMovieDef* a_def) -> void {
                    a_def->SetState(RE::GFxState::StateType::kLog, RE::make_gptr<Logger>().get());
                });

            assert(success);
            menuFlags.set(RE::UI_MENU_FLAGS::kAllowSaving);

            _view = menu->uiMovie;
            _view->GetVariable(&_root, "_root");
            _view->GetVariable(&_widget, "_root.WidgetHolder");
        }

        WidgetMenu(const WidgetMenu&) = default;
        WidgetMenu(WidgetMenu&&) = default;

        ~WidgetMenu() = default;

        WidgetMenu& operator=(const WidgetMenu&) = default;
        WidgetMenu& operator=(WidgetMenu&&) = default;

        static RE::stl::owner<RE::IMenu*> Creator() { return new WidgetMenu(); }

        UIResult ProcessMessage(RE::UIMessage& a_message) override {
            using Type = RE::UI_MESSAGE_TYPE;
            switch (*a_message.type) {
                case Type::kShow:
                    OnOpen();
                    return Super::ProcessMessage(a_message);
                case Type::kHide:
                    OnClose();
                    return UIResult::kHandled;
                default:
                    return Super::ProcessMessage(a_message);
            }
        }

        void AdvanceMovie(float a_interval, std::uint32_t a_currentTime) override {
            // SKSE::log::warn("Advance Movie");
            ProcessDelegate();
            // AssignBossTargetsFromQueue();
            // Update();
            Super::AdvanceMovie(a_interval, a_currentTime);
        }

    private:
        class Logger : public RE::GFxLog {
        public:
            void LogMessageVarg(LogMessageType, const char* a_fmt, std::va_list a_argList) override {
                std::string fmt(a_fmt ? a_fmt : "");
                while (!fmt.empty() && fmt.back() == '\n') {
                    fmt.pop_back();
                }

                std::va_list args;
                va_copy(args, a_argList);
                std::vector<char> buf(static_cast<std::size_t>(std::vsnprintf(0, 0, fmt.c_str(), a_argList) + 1));
                std::vsnprintf(buf.data(), buf.size(), fmt.c_str(), args);
                va_end(args);

                logger::info("{}: {}"sv, WidgetMenu::MenuName(), buf.data());
            }
        };

        void OnOpen();
        void OnClose();
        void ProcessDelegate();

        RE::GFxValue _root;
        RE::GFxValue _widget;

        static constexpr std::string_view FILE_NAME{"HotkeysSystem/UIHS_Widget"};
        static constexpr std::string_view MENU_NAME{"UIHS_WidgetMenu"};
        static constexpr std::int8_t SORT_PRIORITY{0};

        RE::GPtr<RE::GFxMovieView> _view;
    };
}  // namespace Scaleform