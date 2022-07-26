#pragma once

#include <RE/Skyrim.h>

namespace Sample {
    bool RegisterHitCounter(RE::BSScript::IVirtualMachine* vm);

    void InitializeHook(SKSE::Trampoline& trampoline);
}
