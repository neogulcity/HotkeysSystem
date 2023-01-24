#include "Combat.h"
#include "WidgetHandler.h"

void CombatHandler::Register() {
    auto source = RE::ScriptEventSourceHolder::GetSingleton();
    if (!source) return;

    auto combat = CombatHandler::GetSingleton();
    if (!combat) return;

    source->AddEventSink(combat);
    logger::info("{} Registered.", typeid(RE::TESCombatEvent).name());
}

CombatHandler::EventResult CombatHandler::ProcessEvent(const RE::TESCombatEvent* _event,
                                                       RE::BSTEventSource<RE::TESCombatEvent>* _eventSource) {
    if (!_event) return EventResult::kContinue;

    auto source = _event->actor ? _event->actor->As<RE::Actor>() : nullptr;
    auto target = _event->targetActor ? _event->targetActor->As<RE::Actor>() : nullptr;

    if (!source || !target) return RE::BSEventNotifyControl::kContinue;
    if (target->GetFormID() != 0x14) return RE::BSEventNotifyControl::kContinue;

    auto widgetHandler = WidgetHandler::GetSingleton();
    if (!widgetHandler) return EventResult::kContinue;

    widgetHandler->MenuFadeIn();
    widgetHandler->ProcessFadeOut();

    return EventResult::kContinue;
}
