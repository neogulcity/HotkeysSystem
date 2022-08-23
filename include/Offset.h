#pragma once

RE::BGSEquipSlot* GetRightHandSlot()
{
	using func_t = decltype(GetRightHandSlot);
    if (REL::Module::IsAE()) {
        REL::Relocation<func_t> func{REL::ID(23151)};
        return func();
    }
	else {
        REL::Relocation<func_t> func{REL::ID(23151)};
        return func();
	}
}

RE::BGSEquipSlot* GetLeftHandSlot()
{
	using func_t = decltype(GetLeftHandSlot);
    if (REL::Module::IsAE()) {
        REL::Relocation<func_t> func{REL::ID(23150)};
        return func();
    } else {
        REL::Relocation<func_t> func{REL::ID(23150)};
        return func();
    }
}