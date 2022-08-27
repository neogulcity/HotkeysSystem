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
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	manager->InitWidgetNext();
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
	
	auto dataHolder = &MCM::DataHolder::GetSingleton();
	if (!dataHolder) {
		return EventResult::kContinue;
	}

	if (!dataHolder->setting.mWidgetActive) {
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





//RE::BSEventNotifyControl ViewHandler::ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*)
//{
//	auto widgetHandler = WidgetHandler::GetSingleton();
//	if (!widgetHandler) {
//		SKSE::log::error("Unable to get WidgetHandler.");
//		return EventResult::kContinue;
//	}
//
//	auto dataHolder = &MCM::DataHolder::GetSingleton();
//    if (!dataHolder) {
//		SKSE::log::error("Unable to get DataHolder.");
//        return EventResult::kContinue;
//    }
//
//
//	//if (a_event && a_event->menuName == RE::HUDMenu::MENU_NAME) {
//	//	
//	//	if (dataHolder->setting.mWidgetActive) {
//	//		SKSE::log::info("Widget Active.");
//	//		if (a_event->opening) {
//	//			SKSE::log::info("Opening hud.");
//	//			widgetHandler->OpenWidgetMenu();
//	//			auto manager = &UIHS::EquipsetManager::GetSingleton();
//	//			if (!manager) {
//	//				return EventResult::kContinue;
//	//			}
//
//	//			manager->InitWidget();
//	//		}
//	//		else {
//	//			widgetHandler->CloseWidgetMenu();
//	//		}
//	//	}
//	//}
//
//	auto intfcStr = RE::InterfaceStrings::GetSingleton();
//	if (intfcStr &&
//		a_event &&
//		a_event->menuName == intfcStr->mapMenu ||
//		a_event->menuName == intfcStr->inventoryMenu ||
//		a_event->menuName == intfcStr->magicMenu ||
//		a_event->menuName == intfcStr->tweenMenu) {
//		if (!a_event->opening) {
//			Enable();
//		} else {
//			Disable();
//		}
//	}
//
//	return EventResult::kContinue;
//}
