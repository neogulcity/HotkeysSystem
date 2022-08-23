#include "Scaleform/WidgetMenu.h"
#include "WidgetHandler.h"

namespace Scaleform {
    void WidgetMenu::Register()
    {
        auto ui = RE::UI::GetSingleton();
        if (ui) {
            ui->Register(MENU_NAME, Creator);
            logger::info("Registered {}"sv, MENU_NAME);
        }
    }

    void WidgetMenu::RefreshUI()
    {
        auto widgetHandler = WidgetHandler::GetSingleton();

        const RE::GFxValue bTrue{true};
        const RE::GFxValue bFalse{false};

        if (uiMovie) {
            if (widgetHandler->mDissolveIn) {
                _widget.Invoke("DissolveIn");
                widgetHandler->ProcessDissolveIn();
            }

            if (widgetHandler->mDissolveOut) {
                _widget.Invoke("DissolveOut");
                widgetHandler->ProcessDissolveOut();
            }

            while (widgetHandler->mText.size() > 0) {
                const auto& elem = widgetHandler->mText;
                int32_t id = std::get<0>(elem[0]);
                std::string text = std::get<1>(elem[1]);
                RE::GFxValue args[2];
                args[0].SetNumber(id);
                args[1].SetString(text);
                _widget.Invoke("SetText", nullptr, args, 2);
                widgetHandler->ProcessText();
            }

            while (widgetHandler->mSize.size() > 0) {
                const auto& elem = widgetHandler->mSize;
                int32_t id = std::get<0>(elem[0]);
                int32_t width = std::get<1>(elem[0]);
                int32_t height = std::get<2>(elem[0]);
                RE::GFxValue args[3];
                args[0].SetNumber(id);
                args[1].SetNumber(width);
                args[2].SetNumber(height);
                _widget.Invoke("SetSize", nullptr, args, 3);
                widgetHandler->ProcessSize();
            }

            while (widgetHandler->mPos.size() > 0) {
                const auto& elem = widgetHandler->mPos;
                int32_t id = std::get<0>(elem[0]);
                int32_t xPos = std::get<1>(elem[0]);
                int32_t yPos = std::get<2>(elem[0]);
                RE::GFxValue args[3];
                args[0].SetNumber(id);
                args[1].SetNumber(xPos);
                args[2].SetNumber(yPos);
                _widget.Invoke("SetPos", nullptr, args, 3);
                widgetHandler->ProcessPos();
            }

            while (widgetHandler->mAlpha.size() > 0) {
                const auto& elem = widgetHandler->mAlpha;
                int32_t id = std::get<0>(elem[0]);
                int32_t alpha = std::get<1>(elem[0]);
                RE::GFxValue args[2];
                args[0].SetNumber(id);
                args[1].SetNumber(alpha);
                _widget.Invoke("SetAlpha", nullptr, args, 2);
                widgetHandler->ProcessAlpha();
            }

            while (widgetHandler->mAnimate.size() > 0) {
                const auto& elem = widgetHandler->mAnimate;
                int32_t id = elem[0];
                RE::GFxValue arg[1];
                arg[0].SetNumber(id);
                _widget.Invoke("Animate", nullptr, arg, 1);
                widgetHandler->ProcessAnimate();
            }
        }
    }

    void WidgetMenu::ProcessDelegate()
	{
		WidgetHandler::GetSingleton()->ProcessWidgetMenu(*this);
	}

    void WidgetMenu::OnOpen()
    {
		RefreshUI();
		ProcessDelegate();
    }

    void WidgetMenu::OnClose()
    {
        if (!uiMovie) {
            return;
        }

        _widget.Invoke("Clear");
    }

    void WidgetMenu::LoadWidget(std::string _path, int32_t _x, int32_t _y)
    {
        auto widgetHandler = WidgetHandler::GetSingleton();
        if (!widgetHandler) {
            return;
        }

        if (!uiMovie) {
            return;
        }

        RE::GFxValue result;
        RE::GFxValue args[3];
        args[0].SetString(_path);
        args[1].SetNumber(_x);
        args[2].SetNumber(_y);
        _widget.Invoke("LoadWidget", &result, args, 3);
        int32_t id = std::stoi(result.GetString());
        widgetHandler->AddID(id, WidgetHandler::eIDType::Widget);
    }

    void WidgetMenu::LoadText(std::string _text, std::string _font, int32_t _size, int32_t _x, int32_t _y)
    {
        auto widgetHandler = WidgetHandler::GetSingleton();
        if (!widgetHandler) {
            return;
        }

        if (!uiMovie) {
            return;
        }

        RE::GFxValue result;
        RE::GFxValue args[5];
        args[0].SetString(_text);
        args[1].SetString(_font);
        args[2].SetNumber(_size);
        args[3].SetNumber(_x);
        args[4].SetNumber(_y);
        _widget.Invoke("LoadText", &result, args, 5);
        int32_t id = std::stoi(result.GetString());
        widgetHandler->AddID(id, WidgetHandler::eIDType::Text);
    }

    void WidgetMenu::UnloadWidget(int32_t _id)
    {
        auto widgetHandler = WidgetHandler::GetSingleton();
        if (!widgetHandler) {
            return;
        }

        if (!uiMovie) {
            return;
        }

        RE::GFxValue arg[1];
        arg[0].SetNumber(_id);
        _widget.Invoke("UnloadWidget", nullptr, arg, 1);
        widgetHandler->RemoveID(_id);
    }

    void WidgetMenu::UnloadText(int32_t _id) {
        auto widgetHandler = WidgetHandler::GetSingleton();
        if (!widgetHandler) {
            return;
        }

        if (!uiMovie) {
            return;
        }

        RE::GFxValue arg[1];
        arg[0].SetNumber(_id);
        _widget.Invoke("UnloadText", nullptr, arg, 1);
        widgetHandler->RemoveID(_id);
    }
}