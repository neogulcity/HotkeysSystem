#include <SKSE/SKSE.h>
#include <Sample/HitCounterManager.h>

using namespace RE;
using namespace Sample;
using namespace SKSE;

namespace {
    // These four-character record types, which store data in the SKSE cosave, are little-endian. That means they are
    // reversed from the character order written here. Using the byteswap functions reverses them back to the order
    // the characters are written in the source code.
    inline const auto TrackedActorsRecord = _byteswap_ulong('TACT');
    inline const auto HitCountsRecord = _byteswap_ulong('HITC');
}

HitCounterManager& HitCounterManager::GetSingleton() noexcept {
    static HitCounterManager instance;
    return instance;
}

bool HitCounterManager::Track(Actor* target) {
    if (!target) {
        return false;
    }
    std::unique_lock lock(_lock);
    return _trackedActors.emplace(target).second;
}

bool HitCounterManager::Untrack(Actor* target) {
    if (!target) {
        return false;
    }
    std::unique_lock lock(_lock);
    return _trackedActors.erase(target);
}

void HitCounterManager::Increment(Actor* target, int32_t by) {
    if (!target) {
        return;
    }
    std::unique_lock lock(_lock);
    if (!_trackedActors.contains(target)) {
        return;
    }
    auto result = _hitCounts.try_emplace(target, by);
    if (!result.second) {
        result.first->second += by;
    }
}

void HitCounterManager::OnRevert(SerializationInterface*) {
    std::unique_lock lock(GetSingleton()._lock);
    GetSingleton()._hitCounts.clear();
    GetSingleton()._trackedActors.clear();
}

std::optional<int32_t> HitCounterManager::GetHitCount(Actor* target) const noexcept {
    std::unique_lock lock(_lock);
    auto result = _hitCounts.find(target);
    if (result == _hitCounts.end()) {
        return {};
    }
    return result->second;
}

void HitCounterManager::OnGameLoaded(SerializationInterface* serde) {
    std::uint32_t type;
    std::uint32_t size;
    std::uint32_t version;
    // To load records from a cosave, use <code>GetNextRecordInfo</code> to iterate from one record to the next.
    // You will be given records in the order they were written, but otherwise you do not look up a record by its name.
    // Instead check the result of each iteration to find out which record it is and handle it appropriately.
    //
    // If you make breaking changes to your data format, you can increase the version number used when writing the data
    // out and check that number here to handle previous versions.
    while (serde->GetNextRecordInfo(type, version, size)) {
        if (type == HitCountsRecord) {
            // First read how many items follow in this record, so we know how many times to iterate.
            std::size_t hitCountsSize;
            serde->ReadRecordData(&hitCountsSize, sizeof(hitCountsSize));
            // Iterate over the remaining data in the record.
            for (; hitCountsSize > 0; --hitCountsSize) {
                RE::FormID actorFormID;
                serde->ReadRecordData(&actorFormID, sizeof(actorFormID));
                // When reading back a form ID from a save, that form ID may no longer be valid because the user's
                // load order may have changed. During the load call, it is possible to use
                // <code>ResolveFormID</code> to attempt to find the new form ID, based on the user's current load
                // order and the load order that was recorded in the save file.
                RE::FormID newActorFormID;
                if (!serde->ResolveFormID(actorFormID, newActorFormID)) {
                    log::warn("Actor ID {:X} could not be found after loading the save.", actorFormID);
                    continue;
                }
                int32_t hitCount;
                serde->ReadRecordData(&hitCount, sizeof(hitCount));
                auto* actor = TESForm::LookupByID<Actor>(newActorFormID);
                if (actor) {
                    GetSingleton()._hitCounts.try_emplace(actor, hitCount);
                } else {
                    log::warn("Actor ID {:X} could not be found after loading the save.", newActorFormID);
                }
            }
        } else if (type == TrackedActorsRecord) {
            std::size_t trackedActorsSize;
            serde->ReadRecordData(&trackedActorsSize, sizeof(trackedActorsSize));
            for (; trackedActorsSize > 0; --trackedActorsSize) {
                RE::FormID actorFormID;
                serde->ReadRecordData(&actorFormID, sizeof(actorFormID));
                RE::FormID newActorFormID;
                if (!serde->ResolveFormID(actorFormID, newActorFormID)) {
                    log::warn("Actor ID {:X} could not be found after loading the save.", actorFormID);
                    continue;
                }
                auto* actor = TESForm::LookupByID<Actor>(newActorFormID);
                if (actor) {
                    GetSingleton()._trackedActors.emplace(actor);
                } else {
                    log::warn("Actor ID {:X} could not be found after loading the save.", newActorFormID);
                }
            }
        } else {
            log::warn("Unknown record type in cosave.");
            __assume(false);
        }
    }
}

void HitCounterManager::OnGameSaved(SerializationInterface* serde) {
    std::unique_lock lock(GetSingleton()._lock);
    // To write data open a record with a given name. The name must be unique within your mod, and has a version number
    // assigned (in this case 0). You can increase the version number if making breaking format change from a previous
    // version of your mod, so that the load handler can know which version of the format is being used.
    if (!serde->OpenRecord(HitCountsRecord, 0)) {
        log::error("Unable to open record to write cosave data.");
        return;
    }
    // First, write the number of items that will be written in this record. That way when we load the data, we know how
    // many times to iterate reading in items.
    auto hitCountsSize = GetSingleton()._hitCounts.size();
    serde->WriteRecordData(&hitCountsSize, sizeof(hitCountsSize));
    for (auto& count : GetSingleton()._hitCounts) {
        serde->WriteRecordData(&count.first->formID, sizeof(count.first->formID));
        serde->WriteRecordData(&count.second, sizeof(count.second));
    }

    if (!serde->OpenRecord(TrackedActorsRecord, 0)) {
        log::error("Unable to open record to write cosave data.");
        return;
    }
    // Again, write the number of items so we know how many times to iterate when loading.
    auto trackedActorsCount = GetSingleton()._trackedActors.size();
    serde->WriteRecordData(&trackedActorsCount, sizeof(trackedActorsCount));
    for (auto* actor : GetSingleton()._trackedActors) {
        serde->WriteRecordData(&actor->formID, sizeof(actor->formID));
    }
}
