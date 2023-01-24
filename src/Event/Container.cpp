#include "Container.h"
#include "Config.h"
#include "EquipsetManager.h"
#include "WidgetHandler.h"

void ContainerHandler::Register() {
    auto source = RE::ScriptEventSourceHolder::GetSingleton();
    if (!source) return;

    auto container = ContainerHandler::GetSingleton();
    if (!container) return;

    source->AddEventSink(container);
    logger::info("{} Registered.", typeid(RE::TESContainerChangedEvent).name());
}

ContainerHandler::EventResult ContainerHandler::ProcessEvent(
    const RE::TESContainerChangedEvent* _event, RE::BSTEventSource<RE::TESContainerChangedEvent>* _eventSource) {
    if (!_event) return EventResult::kContinue;

    if (_event->newContainer != 0x14 && _event->oldContainer != 0x14) return EventResult::kContinue;
    bool isAdded = _event->newContainer == 0x14;

    auto form = RE::TESForm::LookupByID(_event->baseObj);
    if (!form) return EventResult::kContinue;
    
    auto potion = form->As<RE::AlchemyItem>();
    if (!potion) return EventResult::kContinue;

    auto config = ConfigHandler::GetSingleton();
    if (!config) return EventResult::kContinue;

    auto TESDataHandler = RE::TESDataHandler::GetSingleton();
    if (!TESDataHandler) return EventResult::kContinue;

    auto manager = EquipsetManager::GetSingleton();
    if (!manager) return EventResult::kContinue;

    auto widgetHandler = WidgetHandler::GetSingleton();
    if (!widgetHandler) return EventResult::kContinue;

    for (auto equipset : manager->equipsetVec) {
        if (equipset->type != Equipset::TYPE::POTION) continue;
        
        auto potionset = static_cast<PotionSet*>(equipset);
        if (!potionset) continue;

        potionset->AssignAutoPotion();

        if (potionset->widgetIcon.enable) {
            widgetHandler->UnloadWidget(potionset->widgetID.icon);
            potionset->CreateWidgetIcon();
        }

        if (potionset->widgetName.enable) {
            auto name = potionset->GetPotionName();
            widgetHandler->SetText(potionset->widgetID.text1, name);
        }

        if (potionset->widgetAmount.enable) {
            auto amount = potionset->GetPotionAmount();
            widgetHandler->SetText(potionset->widgetID.text2, amount);
        }
    }

    return EventResult::kContinue;
}
