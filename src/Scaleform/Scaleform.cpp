#include "Scaleform/Scaleform.h"
#include "Scaleform/WidgetMenu.h"

namespace Scaleform {
    void Register()
    {
        WidgetMenu::Register();
        logger::info("Registered UIHS_Widget Movie");
    }
}