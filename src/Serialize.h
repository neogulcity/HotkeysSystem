#pragma once

namespace Serialize {
    enum class Type {
        SAVE,
        FILE
    };
    void OnGameSaved(SKSE::SerializationInterface* serde);
    void OnRevert(SKSE::SerializationInterface* serde);
    void OnGameLoaded(SKSE::SerializationInterface* serde);

    bool ExportEquipset(Type _type, SKSE::SerializationInterface* serde = nullptr);
    bool ImportEquipset(Type _type, SKSE::SerializationInterface* serde = nullptr);
}  // namespace Serialize