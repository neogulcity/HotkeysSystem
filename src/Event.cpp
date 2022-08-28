#include "Event.h"
#include "EquipsetManager.h"


RE::BSEventNotifyControl OnCombatEvent::ProcessEvent(const RE::TESCombatEvent* a_event, RE::BSTEventSource<RE::TESCombatEvent>* a_eventSource)
{
    if (!a_event || !a_eventSource) {
        return RE::BSEventNotifyControl::kContinue;
    }

    auto source = a_event->actor ? a_event->actor->As<RE::Actor>() : nullptr;
    auto target = a_event->targetActor ? a_event->targetActor->As<RE::Actor>() : nullptr;

    if (!source || !target) {
        return RE::BSEventNotifyControl::kContinue;
    }

    auto playerref = RE::PlayerCharacter::GetSingleton();
    if (!playerref) {
        return RE::BSEventNotifyControl::kContinue;
    }

    if (target->GetFormID() != playerref->GetFormID()) {
        return RE::BSEventNotifyControl::kContinue;
    }

    auto manager = &UIHS::EquipsetManager::GetSingleton();
    if (!manager) {
        return RE::BSEventNotifyControl::kContinue;
    }

    auto dataHolder = MCM::DataHolder::GetSingleton();
    if (!dataHolder) {
        return RE::BSEventNotifyControl::kContinue;
    }

    if (!dataHolder->setting->mWidgetActive) {
        return RE::BSEventNotifyControl::kContinue;
    }
    
    MCM::eWidgetDisplay type = static_cast<MCM::eWidgetDisplay>(dataHolder->widget->mDisplay);
    if (type == MCM::eWidgetDisplay::InCombat) {
        if (dataHolder->widget->mDelay != 0.0f) {
            manager->DissolveOut_Function();
            if (!manager->IsThreadWorking()) {
                manager->SetDissolveTimer();
            }
            else {
                manager->SetRemain(dataHolder->widget->mDelay + 1.0f);
            }
        }
    }

    return RE::BSEventNotifyControl::kContinue;
}

bool OnCombatEvent::RegisterEvent()
{
    static OnCombatEvent g_combateventhandler;

    auto ScriptEventSource = RE::ScriptEventSourceHolder::GetSingleton();

    if (!ScriptEventSource) {
        return false;
    }

    ScriptEventSource->AddEventSink(&g_combateventhandler);

    log::info("Registered OnCombatEvent");

    return true;
}