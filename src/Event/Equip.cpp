#include "Equip.h"
#include "Equipment.h"
#include "WidgetHandler.h"

void EquipHandler::Register() {
    auto source = RE::ScriptEventSourceHolder::GetSingleton();
    if (!source) return;

    auto equip = EquipHandler::GetSingleton();
    if (!equip) return;

    source->AddEventSink(equip);
    logger::info("{} Registered.", typeid(RE::TESEquipEvent).name());
}

EquipHandler::EventResult EquipHandler::ProcessEvent(const RE::TESEquipEvent* _event,
                                                               RE::BSTEventSource<RE::TESEquipEvent>* _eventSource) {
    if (!_event) return EventResult::kContinue;

    auto actor = _event->actor.get();
    if (!actor) return EventResult::kContinue;

    if (actor->GetFormID() != 0x14) return EventResult::kContinue;

    auto formID = _event->baseObject;
    auto form = RE::TESForm::LookupByID<RE::TESForm>(formID);
    if (form) {
        auto equipment = EquipmentManager::GetSingleton();
        if (!equipment) return EventResult::kContinue;

        auto armor = form->As<RE::TESObjectARMO>();
        if (armor) {
            if (armor->IsShield()) {
                equipment->NotifyWeapon(_event->equipped, form);
            } else {
                auto armor_slot = static_cast<uint32_t>(armor->GetSlotMask());
                int flag = 1;
                std::vector<uint32_t> result;
                for (int i = 0; i < 32; i++) {
                    if (armor_slot & (flag << i)) {
                        result.push_back(i);
                    }
                }

                equipment->NotifyArmor(_event->equipped, result, form->GetName());
            }
        }

        auto light = form->As<RE::TESObjectLIGH>();
        if (light) {
            equipment->NotifyWeapon(_event->equipped, form);
        }

        auto weapon = form->As<RE::TESObjectWEAP>();
        if (weapon) {
            equipment->NotifyWeapon(_event->equipped, form);
        }

        auto spell = form->As<RE::SpellItem>();
        if (spell) {
            auto type = spell->GetSpellType();
            if (type == RE::MagicSystem::SpellType::kSpell) {
                equipment->NotifyWeapon(_event->equipped, form);
            }

            if (type == RE::MagicSystem::SpellType::kPower ||
                type == RE::MagicSystem::SpellType::kLesserPower) {
                equipment->NotifyShout(_event->equipped, form);
            }
        }

        if (form->Is(RE::FormType::Shout)) {
            equipment->NotifyShout(_event->equipped, form);
        }

        auto widgetHandler = WidgetHandler::GetSingleton();
        if (!widgetHandler) return EventResult::kContinue;

        widgetHandler->ProcessFadeIn();
        widgetHandler->ProcessFadeOut();
    }


    return EventResult::kContinue;
}

