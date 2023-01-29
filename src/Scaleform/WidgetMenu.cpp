#include "WidgetMenu.h"

#include "WidgetHandler.h"

namespace Scaleform {
    void WidgetMenu::Register() {
        auto ui = RE::UI::GetSingleton();
        if (!ui) return;

        ui->Register(MENU_NAME, Creator);
        logger::info("{} Registered."sv, MENU_NAME);
    }

    void WidgetMenu::RefreshUI() {
        //auto widgetHandler = WidgetHandler::GetSingleton();

        if (uiMovie) {
            //if (widgetHandler->mDissolveIn) {
            //    _widget.Invoke("DissolveIn");
            //    widgetHandler->ProcessDissolveIn();
            //}

            //if (widgetHandler->mDissolveOut) {
            //    _widget.Invoke("DissolveOut");
            //    widgetHandler->ProcessDissolveOut();
            //}

            //while (widgetHandler->mAnimate.size() > 0) {
            //    const auto& elem = widgetHandler->mAnimate;
            //    int32_t id = elem[0];
            //    RE::GFxValue arg[1];
            //    arg[0].SetNumber(id);
            //    _widget.Invoke("Animate", nullptr, arg, 1);
            //    widgetHandler->ProcessAnimate();
            //}
        }
    }

    void WidgetMenu::ProcessDelegate() {
        WidgetHandler::GetSingleton()->ProcessWidgetMenu(*this);
    }

    void WidgetMenu::OnOpen() {
        RefreshUI();
        ProcessDelegate();
    }

    void WidgetMenu::OnClose() {
        //
    }

    void WidgetMenu::SetMenuVisible(bool _visible) {
        const auto prev = _view->GetVisible();
        if (prev != _visible) {
            _view->SetVisible(_visible);
        }
    }

    void WidgetMenu::LoadWidget(uint32_t _id, std::string _path, int32_t _x, int32_t _y, int32_t _width, int32_t _height,
                                int32_t _alpha) {
        if (!uiMovie) return;

        RE::GFxValue args[7];
        args[0].SetNumber(_id);
        args[1].SetString(_path);
        args[2].SetNumber(_x);
        args[3].SetNumber(_y);
        args[4].SetNumber(_width);
        args[5].SetNumber(_height);
        args[6].SetNumber(_alpha);
        _widget.Invoke("LoadWidget", nullptr, args, 7);
    }

    void WidgetMenu::UnloadWidget(uint32_t _id) {
        if (!uiMovie) return;

        RE::GFxValue args[1];
        args[0].SetNumber(_id);
        _widget.Invoke("UnloadWidget", nullptr, args, 1);
    }

    void WidgetMenu::LoadText(uint32_t _id, std::string _text, std::string _font, int32_t _x, int32_t _y,
                              int32_t _align, int32_t _size, int32_t _alpha, bool _shadow) {
        if (!uiMovie) return;

        RE::GFxValue args[9];
        args[0].SetNumber(_id);
        args[1].SetString(_text);
        args[2].SetString(_font);
        args[3].SetNumber(_x);
        args[4].SetNumber(_y);
        args[5].SetNumber(_align);
        args[6].SetNumber(_size);
        args[7].SetNumber(_alpha);
        args[8].SetBoolean(_shadow);
        _widget.Invoke("LoadText", nullptr, args, 9);
    }

    void WidgetMenu::UnloadText(uint32_t _id) {
        if (!uiMovie) return;

        RE::GFxValue args[1];
        args[0].SetNumber(_id);
        _widget.Invoke("UnloadText", nullptr, args, 1);
    }

    void WidgetMenu::SetText(uint32_t _id, std::string _text) {
        if (!uiMovie) return;

        RE::GFxValue args[2];
        args[0].SetNumber(_id);
        args[1].SetString(_text);
        _widget.Invoke("SetText", nullptr, args, 2);
    }

    void WidgetMenu::SetMenuAlpha(uint32_t _alpha) {
        if (!uiMovie) return;

        RE::GFxValue args[1];
        args[0].SetNumber(_alpha);
        _widget.Invoke("SetMenuAlpha", nullptr, args, 1);
    }

    void WidgetMenu::MenuFadeIn() {
        if (!uiMovie) return;

        _widget.Invoke("MenuFadeIn");
    }

    void WidgetMenu::MenuFadeOut() {
        if (!uiMovie) return;

        _widget.Invoke("MenuFadeOut");
    }
}  // namespace Scaleform


