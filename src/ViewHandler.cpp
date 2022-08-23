#include "ViewHandler.h"

#include "WidgetHandler.h"

void ViewHandler::Close()
{
	auto widgetHandler = WidgetHandler::GetSingleton();
	widgetHandler->CloseWidgetMenu();
}