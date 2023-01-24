#pragma once

#include <Windows.h>

#include <functional>
#include <mutex>
#include <vector>

// Credit: SlavicPotato
struct WndProcHook {
    static LRESULT thunk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static inline WNDPROC func;
};

using FuncVector = std::vector<std::function<void()>>;

struct D3DInitHook {
    static void thunk();
    static inline REL::Relocation<decltype(thunk)> func;

    static constexpr auto id = REL::RelocationID(75595, 77226);
    static constexpr auto offset = REL::VariantOffset(0x9, 0x275, 0x00);  // VR unknown

    static inline std::atomic<bool> initialized = false;

    static inline FuncVector post_init_callbacks;
};

struct DXGIPresentHook {
    static void thunk(std::uint32_t a_p1);
    static inline REL::Relocation<decltype(thunk)> func;

    static constexpr auto id = REL::RelocationID(75461, 77246);
    static constexpr auto offset = REL::Offset(0x9);

    static inline std::mutex callback_mutex;
    static inline FuncVector pre_callbacks;
    static inline FuncVector mid_callbacks;
};