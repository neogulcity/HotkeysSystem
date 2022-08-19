#pragma once

RE::BGSEquipSlot* GetRightHandSlot()
{
	using func_t = decltype(GetRightHandSlot);
	REL::Relocation<func_t> func{ REL::ID(23151) };
	return func();
}

RE::BGSEquipSlot* GetLeftHandSlot()
{
	using func_t = decltype(GetLeftHandSlot);
	REL::Relocation<func_t> func{ REL::ID(23150) };
	return func();
}