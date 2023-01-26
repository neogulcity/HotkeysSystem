#include "HUDHandler.h"
#include "WidgetHandler.h"
#include "EquipsetManager.h"
#include "Equipment.h"
#include "Config.h"

void HUDHandler::Register() {
    auto ui = RE::UI::GetSingleton();
    if(!ui) return;

    auto hud = HUDHandler::GetSingleton();
    if (!hud) return;

    ui->AddEventSink<RE::MenuOpenCloseEvent>(hud);
    logger::info("{} Registered.", typeid(RE::MenuOpenCloseEvent).name());
}

HUDHandler::EventResult HUDHandler::ProcessEvent(const RE::MenuOpenCloseEvent* _event,
                                                 RE::BSTEventSource<RE::MenuOpenCloseEvent>* _eventSource) {
    if (!_event) {
        return EventResult::kContinue;
    }

    auto intfcStr = RE::InterfaceStrings::GetSingleton();
    if (!intfcStr) return EventResult::kContinue;

    auto widgetHandler = WidgetHandler::GetSingleton();
    if (!widgetHandler) return EventResult::kContinue;

    auto config = ConfigHandler::GetSingleton();
    if (!config) return EventResult::kContinue;

    if (_event->menuName == intfcStr->loadingMenu && !(_event->opening)) {
        auto task = SKSE::GetTaskInterface();
        if (!task) return EventResult::kContinue;

        task->AddUITask([widgetHandler, config]() {
            auto equipment = EquipmentManager::GetSingleton();
            auto equipset = EquipsetManager::GetSingleton();

            if (equipment && equipset) {
                widgetHandler->OpenWidgetMenu();

                if (config->Widget.General.animDelay != 0.0f &&
                    config->Widget.General.displayMode == (uint32_t)Config::DisplayType::INCOMBAT) {
                    widgetHandler->SetMenuAlpha(0);
                }

                equipment->RemoveAllArmorWidget();
                equipment->RemoveAllWeaponWidget();
                equipment->RemoveAllShoutWidget();
                equipset->RemoveAllWidget();
                equipment->CreateAllArmorWidget();
                equipment->CreateAllWeaponWidget();
                equipment->CreateAllShoutWidget();
                equipset->CreateAllWidget();
            }
        });
        //auto equipment = EquipmentManager::GetSingleton();
        //if (!equipment) return EventResult::kContinue;

        //auto equipset = EquipsetManager::GetSingleton();
        //if (!equipset) return EventResult::kContinue;

        //widgetHandler->OpenWidgetMenu();
        //
        //if (config->Widget.General.animDelay != 0.0f &&
        //    config->Widget.General.displayMode == (uint32_t)Config::DisplayType::INCOMBAT) {
        //    widgetHandler->SetMenuAlpha(0);
        //}

        //equipment->RemoveAllArmorWidget();
        //equipment->RemoveAllWeaponWidget();
        //equipment->RemoveAllShoutWidget();
        //equipset->RemoveAllWidget();
        //equipment->CreateAllArmorWidget();
        //equipment->CreateAllWeaponWidget();
        //equipment->CreateAllShoutWidget();
        //equipset->CreateAllWidget();
    }

    if (_event->menuName == intfcStr->mapMenu || _event->menuName == intfcStr->inventoryMenu ||
        _event->menuName == intfcStr->magicMenu || _event->menuName == intfcStr->tweenMenu ||
        _event->menuName == intfcStr->dialogueMenu || _event->menuName == intfcStr->barterMenu ||
        _event->menuName == intfcStr->craftingMenu || _event->menuName == intfcStr->containerMenu) {
        if (_event->opening) {
            widgetHandler->SetMenuVisible(false);
        } else {
            widgetHandler->SetMenuVisible(true);
        }
    }

    return EventResult::kContinue;
}