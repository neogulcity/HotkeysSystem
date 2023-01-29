#include "WidgetHandler.h"
#include "Scaleform/WidgetMenu.h"
#include "Config.h"

void WidgetHandler::AddWidgetMenuTask(WidgetTasklet a_task) {
    OpenWidgetMenu();
    Locker locker(_lock);
    _WidgetMenuTaskQueue.push_back(std::move(a_task));
}

void WidgetHandler::ProcessWidgetMenu(WidgetMenu& a_menu) {
    if (!_WidgetMenuTaskQueue.empty()) {
        for (auto& task : _WidgetMenuTaskQueue) {
            task(a_menu);
        }
        _WidgetMenuTaskQueue.clear();
    }

    if (_refreshWidgetMenu) {
        a_menu.RefreshUI();
    }

    _refreshWidgetMenu = false;
}

void WidgetHandler::OpenWidgetMenu() {
    auto msgQ = RE::UIMessageQueue::GetSingleton();
    if (!msgQ) return;
    
    msgQ->AddMessage(WidgetMenu::MenuName(), RE::UI_MESSAGE_TYPE::kShow, nullptr);
}

void WidgetHandler::CloseWidgetMenu() {
    auto msgQ = RE::UIMessageQueue::GetSingleton();
    if (!msgQ) return;

    msgQ->AddMessage(WidgetMenu::MenuName(), RE::UI_MESSAGE_TYPE::kHide, nullptr);
}

void WidgetHandler::SetMenuVisible(bool _visible) {
    AddWidgetMenuTask([_visible](WidgetMenu& a_menu) {
        a_menu.SetMenuVisible(_visible); 
    });
}

void WidgetHandler::LoadWidget(uint32_t _id, std::string _path, int32_t _x, int32_t _y, int32_t _width, int32_t _height,
                               int32_t _alpha) {
    AddWidgetMenuTask([_id, _path, _x, _y, _width, _height, _alpha](WidgetMenu& a_menu) {
        a_menu.LoadWidget(_id, _path, _x, _y, _width, _height, _alpha);
    });
}

void WidgetHandler::UnloadWidget(uint32_t _id) {
    AddWidgetMenuTask([_id](WidgetMenu& a_menu) {
        a_menu.UnloadWidget(_id);
    });
}

void WidgetHandler::LoadText(uint32_t _id, std::string _text, std::string _font, int32_t _x, int32_t _y, int32_t _align,
                             int32_t _size, int32_t _alpha, bool _shadow) {
    AddWidgetMenuTask([_id, _text, _font, _x, _y, _align, _size, _alpha, _shadow](WidgetMenu& a_menu) {
        a_menu.LoadText(_id, _text, _font, _x, _y, _align, _size, _alpha, _shadow);
    });
}

void WidgetHandler::UnloadText(uint32_t _id) {
    AddWidgetMenuTask([_id](WidgetMenu& a_menu) {
        a_menu.UnloadText(_id);
    });
}

void WidgetHandler::SetText(uint32_t _id, std::string _text) {
    AddWidgetMenuTask([_id, _text](WidgetMenu& a_menu) {
        a_menu.SetText(_id, _text);
    });
}

void WidgetHandler::SetMenuAlpha(uint32_t _alpha) {
    AddWidgetMenuTask([_alpha](WidgetMenu& a_menu) {
        a_menu.SetMenuAlpha(_alpha);
    });
}

void WidgetHandler::MenuFadeIn() {
    AddWidgetMenuTask([](WidgetMenu& a_menu) {
        a_menu.MenuFadeIn();
    });
}

void WidgetHandler::MenuFadeOut() {
    AddWidgetMenuTask([](WidgetMenu& a_menu) {
        a_menu.MenuFadeOut();
    });
}

void WidgetHandler::ProcessFadeIn() {
    auto config = ConfigHandler::GetSingleton();
    if (!config) return;

    if (config->Widget.General.animType == (uint32_t)Config::AnimType::FADE) {
        this->MenuFadeIn();
    } else if (config->Widget.General.animType == (uint32_t)Config::AnimType::INSTANT) {
        this->SetMenuAlpha(100);
    }
}

void WidgetHandler::ProcessFadeOut() {
    auto config = ConfigHandler::GetSingleton();
    if (!config) return;

    if (config->Widget.General.animDelay != 0.0f &&
        config->Widget.General.displayMode == (uint32_t)Config::DisplayType::INCOMBAT) {
        if (this->expireProgress.load() != 0.0f && this->expireProgress.load() < config->Widget.General.animDelay) {
            this->SetExpireProgress(0.01f);
        } else {
            this->StartExpireTimer();
        }
    }
}

void WidgetHandler::SetExpireProgress(const float& _amount) {
    expireProgress.store(_amount);
}

void WidgetHandler::StartExpireTimer() {
    auto config = ConfigHandler::GetSingleton();
    if (!config) return;

    if (expireProgress.load() != 0.0f && expireProgress.load() < config->Widget.General.animDelay) return;

    expireProgress.store(0.01f);
    expire_future = std::async(std::launch::async, &WidgetHandler::ExpireFunc, this);
}

void WidgetHandler::CloseExpireTimer() {
    if (expireProgress.load() == 0.0f) return;

    shouldCloseExpire.store(true);
}

bool WidgetHandler::ExpireFunc() {
    auto config = ConfigHandler::GetSingleton();
    if (!config) return true;

    auto player = RE::PlayerCharacter::GetSingleton();
    if (!player) return true;

    while (!shouldCloseExpire.load() && expireProgress.load() < config->Widget.General.animDelay) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (config->Widget.General.displayMode == (uint32_t)Config::DisplayType::INCOMBAT &&
            player->IsInCombat()) {
            continue;
        }
        expireProgress.fetch_add(0.01f);
    }

    if (!shouldCloseExpire.load()) {
        if (config->Widget.General.animType == (uint32_t)Config::AnimType::FADE) {
            this->MenuFadeOut();
        } else if (config->Widget.General.animType == (uint32_t)Config::AnimType::INSTANT) {
            this->SetMenuAlpha(0);
        }
    }
    shouldCloseExpire.store(false);
    expireProgress.store(0.0f);

    return true;
}