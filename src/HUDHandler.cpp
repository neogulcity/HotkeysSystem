#include "HUDHandler.h"
#include "EquipsetManager.h"
#include "WidgetHandler.h"
#include "MCM.h"

void HUDHandler::Register()
{
	auto ui = RE::UI::GetSingleton();
	ui->AddEventSink<RE::MenuOpenCloseEvent>(HUDHandler::GetSingleton());
	logger::info("Registered MenuOpenCloseEvent");
}

void InitWidget()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	auto manager = &UIHS::EquipsetManager::GetSingleton();
	manager->InitWidget();

	auto widgetHandler = WidgetHandler::GetSingleton();
	if (!widgetHandler) {
		return;
	}

	auto dataHolder = MCM::DataHolder::GetSingleton();
	if (!dataHolder) {
		return;
	}

	auto playerref = RE::PlayerCharacter::GetSingleton();
	if (!playerref) {
		return;
	}

	MCM::eWidgetDisplay type = static_cast<MCM::eWidgetDisplay>(dataHolder->widget->mDisplay);
	if (type == MCM::eWidgetDisplay::InCombat) {
		if (!playerref->IsInCombat()) {
			widgetHandler->SetMenuAlpha(0);
		}
	}
}

HUDHandler::EventResult HUDHandler::ProcessEvent(const RE::MenuOpenCloseEvent* _event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*)
{
	if (!_event) {
		return EventResult::kContinue;
	}

	auto intfcStr = RE::InterfaceStrings::GetSingleton();
	if (!intfcStr) {
		return EventResult::kContinue;
	}

	auto widgetHandler = WidgetHandler::GetSingleton();
	if (!widgetHandler) {
		return EventResult::kContinue;
	}
	
	auto dataHolder = MCM::DataHolder::GetSingleton();
	if (!dataHolder) {
		return EventResult::kContinue;
	}

	if (!dataHolder->setting->mWidgetActive) {
		return EventResult::kContinue;
	}

	if (!RE::UI::GetSingleton()->IsMenuOpen(RE::HUDMenu::MENU_NAME)) {
		return EventResult::kContinue;
	}

	if (_event->menuName == intfcStr->loadingMenu) {
		if (!_event->opening) {
			auto thread = std::thread(InitWidget);
			thread.detach();
		}
	}

	if (_event->menuName == intfcStr->mapMenu ||
		_event->menuName == intfcStr->inventoryMenu ||
		_event->menuName == intfcStr->magicMenu ||
		_event->menuName == intfcStr->tweenMenu ||
		_event->menuName == intfcStr->dialogueMenu ||
		_event->menuName == intfcStr->barterMenu ||
		_event->menuName == intfcStr->craftingMenu) {
	
		if (_event->opening) {
			widgetHandler->SetMenuVisible(false);
		}
		else {
			widgetHandler->SetMenuVisible(true);
		}
	}

	return EventResult::kContinue;
}