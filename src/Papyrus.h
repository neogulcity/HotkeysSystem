#pragma once

using namespace RE;
using namespace RE::BSScript;
using namespace REL;
using namespace SKSE;

void Exec(StaticFunctionTag*);
RE::BGSEquipSlot* GetRightHandSlot();
void EquipItem(RE::TESForm*, RE::BGSEquipSlot*, bool, RE::ExtraDataList*, bool, bool);

namespace UIHS {
    bool RegisterFuncs(IVirtualMachine* vm);
}