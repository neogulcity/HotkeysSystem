#pragma once

#include "Equipset.h"

class DrawHelper {
private:
    std::atomic<bool> reload = false;
    std::atomic<bool> reload_name = true;

public:
    inline void NotifyReload(bool value) { reload.store(value); }
    inline void NotifyReloadName(bool value) { reload_name.store(value); }
    inline bool ShouldReload() { return reload.load(); }
    inline bool ShouldReloadName() { return reload_name.load(); }

public:
    static DrawHelper* GetSingleton() {
        static DrawHelper singleton;
        return std::addressof(singleton);
    }

private:
    DrawHelper() {}
    DrawHelper(const DrawHelper&) = delete;
    DrawHelper(DrawHelper&&) = delete;

    ~DrawHelper() = default;

    DrawHelper& operator=(const DrawHelper&) = delete;
    DrawHelper& operator=(DrawHelper&&) = delete;
};

namespace Draw {
    void HelpMarker(const std::string& _desc);
    void TextCenter(const std::string& _msg);
    void InputButton(uint32_t* _key, const std::string& _id, const std::string& _button,
                                const std::string& _label);
    void SliderInt(const std::string& _label, int* _value, const int& _min, const int& _max, const std::string& _format,
                   int flags = 0);
    void SliderFloat(const std::string& _label, float* _value, const float& _min, const float& _max, const std::string& _format = "%.3f",
                   int flags = 0);
    void Combo(const std::vector<std::string>& _items, uint32_t* _current, const std::string& _label);
    void ComboIcon(std::string* _icon_type, const std::string& _label);
    void PopupConflict(const std::string& _title, const std::string& _name, std::string* _conflictName);

    bool CreateNormal();
    void ShowNormal(NormalSet* _normalSet, const uint32_t& _id);

    bool CreatePotion();
    void ShowPotion(PotionSet* _potionSet, const uint32_t& _id);

    bool CreateCycle();
    void ShowCycle(CycleSet* _cycleSet, const uint32_t& _id);
}