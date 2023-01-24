#include "Serialize.h"
#include "EquipsetManager.h"
#include "Equipment.h"

#include <filesystem>
#include <toml++/toml.h>

static void WriteString(SKSE::SerializationInterface* serde, const std::string& _data) {
    if (!serde) return;

    uint32_t length = _data.length();
    serde->WriteRecordData(&length, sizeof(length));
    serde->WriteRecordData(_data.c_str(), length);
}

static void ReadString(SKSE::SerializationInterface* serde, std::string* _dataOut) {
    if (!serde) return;

    uint32_t length;
    serde->ReadRecordData(&length, sizeof(length));

    _dataOut->resize(length);
    serde->ReadRecordData(const_cast<char*>(_dataOut->c_str()), length);
}

static uint16_t GetVersionMajor() {
    auto plugin = SKSE::PluginDeclaration::GetSingleton();
    if (!plugin) return 0;

    return plugin->GetVersion().major();
}

namespace Serialize {
    const auto EquipsetRecord = _byteswap_ulong('HSER');
    const std::filesystem::path equipset_path = "Data/SKSE/Plugins/UIHS/Equipset.toml";

    bool ExportEquipset(Type _type, SKSE::SerializationInterface* serde) {
        auto manager = EquipsetManager::GetSingleton();
        if (!manager) return false;

        if (_type == Type::SAVE && !serde) return false;

        const auto& equipsetVec = manager->equipsetVec;

        auto mainTbl = toml::table{
            {"Init",
             toml::table{
                 {"equipset_count", (uint32_t)equipsetVec.size()},
             }},
        };

        for (int i = 0; i < equipsetVec.size(); i++) {
            if (equipsetVec[i]->type == Equipset::TYPE::NORMAL) {
                auto equipset = static_cast<NormalSet*>(equipsetVec[i]);

                toml::array itemsArr;
                for (auto item : equipset->items) {
                    itemsArr.push_back(item.Pack());
                }

                auto tbl = toml::table{
                    {"equipset_type", static_cast<uint32_t>(equipset->type)},
                    {"equipset_name", equipset->name},
                    {"equipset_hotkey", equipset->hotkey},
                    {"equipset_modifier1", equipset->modifier1},
                    {"equipset_modifier2", equipset->modifier2},
                    {"equipset_modifier3", equipset->modifier3},
                    {"equipset_order", equipset->order},
                    {"equipset_equipSound", equipset->equipSound},
                    {"equipset_toggleEquip", equipset->toggleEquip},
                    {"equipset_reEquip", equipset->reEquip},
                    {"equipset_widgetIcon", equipset->widgetIcon.Pack()},
                    {"equipset_widgetName", equipset->widgetName.Pack()},
                    {"equipset_widgetHotkey", equipset->widgetHotkey.Pack()},
                    {"equipset_lefthand", equipset->lefthand.Pack()},
                    {"equipset_righthand", equipset->righthand.Pack()},
                    {"equipset_shout", equipset->shout.Pack()},
                    {"equipset_itemsCount", (uint32_t)equipset->items.size()},
                    {"equipset_itemsArr", itemsArr},
                };

                mainTbl.insert(std::to_string(i), tbl);

            } else if (equipsetVec[i]->type == Equipset::TYPE::POTION) {
                auto equipset = static_cast<PotionSet*>(equipsetVec[i]);

                toml::array itemsArr;
                for (auto item : equipset->items) {
                    itemsArr.push_back(item.Pack());
                }

                auto tbl = toml::table{
                    {"equipset_type", static_cast<uint32_t>(equipset->type)},
                    {"equipset_name", equipset->name},
                    {"equipset_hotkey", equipset->hotkey},
                    {"equipset_modifier1", equipset->modifier1},
                    {"equipset_modifier2", equipset->modifier2},
                    {"equipset_modifier3", equipset->modifier3},
                    {"equipset_order", equipset->order},
                    {"equipset_equipSound", equipset->equipSound},
                    {"equipset_calcDuration", equipset->calcDuration},
                    {"equipset_widgetIcon", equipset->widgetIcon.Pack()},
                    {"equipset_widgetName", equipset->widgetName.Pack()},
                    {"equipset_widgetAmount", equipset->widgetAmount.Pack()},
                    {"equipset_health", equipset->health.Pack()},
                    {"equipset_magicka", equipset->magicka.Pack()},
                    {"equipset_stamina", equipset->stamina.Pack()},
                    {"equipset_itemsCount", (uint32_t)equipset->items.size()},
                    {"equipset_itemsArr", itemsArr},
                };

                mainTbl.insert(std::to_string(i), tbl);

            } else if (equipsetVec[i]->type == Equipset::TYPE::CYCLE) {
                auto equipset = static_cast<CycleSet*>(equipsetVec[i]);

                toml::array itemsArr;
                for (auto item : equipset->items) {
                    itemsArr.push_back(item);
                }

                auto tbl = toml::table{
                    {"equipset_type", static_cast<uint32_t>(equipset->type)},
                    {"equipset_name", equipset->name},
                    {"equipset_hotkey", equipset->hotkey},
                    {"equipset_modifier1", equipset->modifier1},
                    {"equipset_modifier2", equipset->modifier2},
                    {"equipset_modifier3", equipset->modifier3},
                    {"equipset_order", equipset->order},
                    {"equipset_cyclePersist", equipset->cyclePersist},
                    {"equipset_cycleExpire", equipset->cycleExpire},
                    {"equipset_cycleReset", equipset->cycleReset},
                    {"equipset_widgetIcon", equipset->widgetIcon.Pack()},
                    {"equipset_widgetName", equipset->widgetName.Pack()},
                    {"equipset_widgetHotkey", equipset->widgetHotkey.Pack()},
                    {"equipset_itemsCount", (uint32_t)equipset->items.size()},
                    {"equipset_itemsArr", itemsArr},
                };

                if (_type == Type::SAVE) {
                    tbl.insert("equipset_cycleIndex", equipset->cycleIndex);
                    tbl.insert("equipset_isCycleInit", equipset->isCycleInit);
                }

                mainTbl.insert(std::to_string(i), tbl);
            }
        }

        if (_type == Type::FILE) {
            std::ofstream f(equipset_path);
            if (!f.is_open()) {
                logger::error("Failed to export Equipset!");
                return false;
            }

            f << mainTbl;
        } else if (_type == Type::SAVE) {
            std::stringstream f;
            f << mainTbl;
            WriteString(serde, f.str());
        }

        return true;
    }

    bool ImportEquipset(Type _type, SKSE::SerializationInterface* serde) {
        try {
            auto manager = EquipsetManager::GetSingleton();
            if (!manager) return false;

            if (_type == Type::SAVE && !serde) return false;

            toml::table tbl;
            if (_type == Type::FILE) {
                tbl = toml::parse_file(equipset_path.c_str());
            } else if (_type == Type::SAVE) {
                std::string data;
                ReadString(serde, &data);
                tbl = toml::parse(data);
            }

            uint32_t equipset_count = tbl["Init"]["equipset_count"].value_or<uint32_t>(0);

            for (int i = 0; i < equipset_count; i++) {
                auto id = std::to_string(i);
                auto type = tbl[id]["equipset_type"].value_or<int>(-1);
                if (type == -1) continue;

                if (static_cast<Equipset::TYPE>(type) == Equipset::TYPE::NORMAL) {
                    NormalSet equipset;
                    equipset.type = Equipset::TYPE::NORMAL;
                    equipset.name = tbl[id]["equipset_name"].value_or<std::string>("");
                    equipset.hotkey = tbl[id]["equipset_hotkey"].value_or<uint32_t>(0);
                    equipset.modifier1 = tbl[id]["equipset_modifier1"].value_or<bool>(false);
                    equipset.modifier2 = tbl[id]["equipset_modifier2"].value_or<bool>(false);
                    equipset.modifier3 = tbl[id]["equipset_modifier3"].value_or<bool>(false);
                    equipset.order = tbl[id]["equipset_order"].value_or<uint32_t>(0);
                    equipset.equipSound = tbl[id]["equipset_equipSound"].value_or<bool>(false);
                    equipset.toggleEquip = tbl[id]["equipset_toggleEquip"].value_or<bool>(false);
                    equipset.reEquip = tbl[id]["equipset_reEquip"].value_or<bool>(false);

                    auto packedWidgetIcon = tbl[id]["equipset_widgetIcon"].value_or<std::string>("");
                    auto widgetIcon = WidgetIcon::Unpack(packedWidgetIcon);
                    equipset.widgetIcon = widgetIcon;

                    auto packedWidgetName = tbl[id]["equipset_widgetName"].value_or<std::string>("");
                    auto widgetName = WidgetText::Unpack(packedWidgetName);
                    equipset.widgetName = widgetName;

                    auto packedWidgetHotkey = tbl[id]["equipset_widgetHotkey"].value_or<std::string>("");
                    auto widgetHotkey = WidgetText::Unpack(packedWidgetHotkey);
                    equipset.widgetHotkey = widgetHotkey;

                    auto packedLefthand = tbl[id]["equipset_lefthand"].value_or<std::string>("");
                    auto lefthand = DataWeapon::Unpack(packedLefthand);
                    equipset.lefthand = lefthand;

                    auto packedRighthand = tbl[id]["equipset_righthand"].value_or<std::string>("");
                    auto righthand = DataWeapon::Unpack(packedRighthand);
                    equipset.righthand = righthand;

                    auto packedShout = tbl[id]["equipset_shout"].value_or<std::string>("");
                    auto shout = DataShout::Unpack(packedShout);
                    equipset.shout = shout;

                    std::vector<DataArmor> items;
                    auto itemsCount = tbl[id]["equipset_itemsCount"].value_or<uint32_t>(0);
                    for (int j = 0; j < itemsCount; j++) {
                        auto packedItem = tbl[id]["equipset_itemsArr"][j].value_or<std::string>("");
                        auto item = DataArmor::Unpack(packedItem);
                        items.push_back(item);
                    }
                    equipset.items = items;

                    manager->Create(equipset, false);

                } else if (static_cast<Equipset::TYPE>(type) == Equipset::TYPE::POTION) {
                    PotionSet equipset;
                    equipset.type = Equipset::TYPE::POTION;
                    equipset.name = tbl[id]["equipset_name"].value_or<std::string>("");
                    equipset.hotkey = tbl[id]["equipset_hotkey"].value_or<uint32_t>(0);
                    equipset.modifier1 = tbl[id]["equipset_modifier1"].value_or<bool>(false);
                    equipset.modifier2 = tbl[id]["equipset_modifier2"].value_or<bool>(false);
                    equipset.modifier3 = tbl[id]["equipset_modifier3"].value_or<bool>(false);
                    equipset.order = tbl[id]["equipset_order"].value_or<uint32_t>(0);
                    equipset.equipSound = tbl[id]["equipset_equipSound"].value_or<bool>(false);
                    equipset.calcDuration = tbl[id]["equipset_calcDuration"].value_or<bool>(false);

                    auto packedWidgetIcon = tbl[id]["equipset_widgetIcon"].value_or<std::string>("");
                    auto widgetIcon = WidgetIcon::Unpack(packedWidgetIcon);
                    equipset.widgetIcon = widgetIcon;

                    auto packedWidgetName = tbl[id]["equipset_widgetName"].value_or<std::string>("");
                    auto widgetName = WidgetText::Unpack(packedWidgetName);
                    equipset.widgetName = widgetName;

                    auto packedWidgetAmount = tbl[id]["equipset_widgetAmount"].value_or<std::string>("");
                    auto widgetAmount = WidgetText::Unpack(packedWidgetAmount);
                    equipset.widgetAmount = widgetAmount;

                    auto packedHealth = tbl[id]["equipset_health"].value_or<std::string>("");
                    auto health = DataPotion::Unpack(packedHealth);
                    equipset.health = health;

                    auto packedMagicka = tbl[id]["equipset_magicka"].value_or<std::string>("");
                    auto magicka = DataPotion::Unpack(packedMagicka);
                    equipset.magicka = magicka;

                    auto packedStamina = tbl[id]["equipset_stamina"].value_or<std::string>("");
                    auto stamina = DataPotion::Unpack(packedStamina);
                    equipset.stamina = stamina;

                    std::vector<DataPotion> items;
                    auto itemsCount = tbl[id]["equipset_itemsCount"].value_or<uint32_t>(0);
                    for (int j = 0; j < itemsCount; j++) {
                        auto packedItem = tbl[id]["equipset_itemsArr"][j].value_or<std::string>("");
                        auto item = DataPotion::Unpack(packedItem);
                        items.push_back(item);
                    }
                    equipset.items = items;

                    manager->Create(equipset, false);
                } else if (static_cast<Equipset::TYPE>(type) == Equipset::TYPE::CYCLE) {
                    CycleSet equipset;
                    equipset.type = Equipset::TYPE::CYCLE;
                    equipset.name = tbl[id]["equipset_name"].value_or<std::string>("");
                    equipset.hotkey = tbl[id]["equipset_hotkey"].value_or<uint32_t>(0);
                    equipset.modifier1 = tbl[id]["equipset_modifier1"].value_or<bool>(false);
                    equipset.modifier2 = tbl[id]["equipset_modifier2"].value_or<bool>(false);
                    equipset.modifier3 = tbl[id]["equipset_modifier3"].value_or<bool>(false);
                    equipset.order = tbl[id]["equipset_order"].value_or<uint32_t>(0);
                    equipset.cyclePersist = tbl[id]["equipset_cyclePersist"].value_or<bool>(false);
                    equipset.cycleExpire = tbl[id]["equipset_cycleExpire"].value_or<float>(0.0f);
                    equipset.cycleReset = tbl[id]["equipset_cycleReset"].value_or<float>(0.0f);

                    auto packedWidgetIcon = tbl[id]["equipset_widgetIcon"].value_or<std::string>("");
                    auto widgetIcon = WidgetIcon::Unpack(packedWidgetIcon);
                    equipset.widgetIcon = widgetIcon;

                    auto packedWidgetName = tbl[id]["equipset_widgetName"].value_or<std::string>("");
                    auto widgetName = WidgetText::Unpack(packedWidgetName);
                    equipset.widgetName = widgetName;

                    auto packedWidgetHotkey = tbl[id]["equipset_widgetHotkey"].value_or<std::string>("");
                    auto widgetHotkey = WidgetText::Unpack(packedWidgetHotkey);
                    equipset.widgetHotkey = widgetHotkey;

                    std::vector<std::string> items;
                    auto itemsCount = tbl[id]["equipset_itemsCount"].value_or<uint32_t>(0);
                    for (int j = 0; j < itemsCount; j++) {
                        auto item = tbl[id]["equipset_itemsArr"][j].value_or<std::string>("");
                        items.push_back(item);
                    }
                    equipset.items = items;

                    if (_type == Type::SAVE) {
                        equipset.cycleIndex = tbl[id]["equipset_cycleIndex"].value_or<uint32_t>(0);
                        equipset.isCycleInit = tbl[id]["equipset_isCycleInit"].value_or<bool>(false);
                    }

                    manager->Create(equipset, false);
                }
            }

            logger::info("Equipset loaded.");
        } catch (const toml::parse_error& err) {
            logger::warn("Failed to parse Equipset file.\nError: {}", err.description());

            return false;
        }

        return true;
    }

    void OnGameSaved(SKSE::SerializationInterface* serde) {
        auto manager = EquipsetManager::GetSingleton();
        if (!manager) return;

        if (!serde->OpenRecord(EquipsetRecord, GetVersionMajor())) {
            logger::error("Unable to open record to write cosave data.");
            return;
        }

        ExportEquipset(Type::SAVE, serde);
    }

    void OnRevert(SKSE::SerializationInterface* serde) {
        auto manager = EquipsetManager::GetSingleton();
        if (!manager) return;

        manager->RemoveAll();
    }

    void OnGameLoaded(SKSE::SerializationInterface* serde) {
        auto manager = EquipsetManager::GetSingleton();
        if (!manager) return;

        auto equipment = EquipmentManager::GetSingleton();
        if (!equipment) return;

        uint32_t type;
        uint32_t size;
        uint32_t version;
        std::string packedData;

        while (serde->GetNextRecordInfo(type, version, size)) {
            if (type == EquipsetRecord && version == GetVersionMajor()) {
                ImportEquipset(Type::SAVE, serde);
                manager->SyncSortOrder();
            } else {
                logger::warn("Unknown record type in cosave.");
                __assume(false);
            }
        }
    }
}  // namespace Serialize