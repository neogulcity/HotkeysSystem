#pragma once

class HUDHandler : public RE::BSTEventSink<RE::MenuOpenCloseEvent> {
private:
	using EventResult = RE::BSEventNotifyControl;

public:
	static HUDHandler* GetSingleton()
	{
		static HUDHandler singleton;
		return std::addressof(singleton);
	}

	static void Register();

	virtual EventResult ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;

private:

	HUDHandler() {}
	HUDHandler(const HUDHandler&) = delete;
	HUDHandler(HUDHandler&&) = delete;

	~HUDHandler() = default;

	HUDHandler& operator=(const HUDHandler&) = delete;
	HUDHandler& operator=(HUDHandler&&) = delete;

};