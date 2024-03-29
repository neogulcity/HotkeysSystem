#include "Config.h"
#include "Render.h"
#include "Gui/GuiMenu.h"
#include "Translate.h"
#include "Serialize.h"

#include "Event/Combat.h"
#include "Event/Container.h"
#include "Event/Equip.h"
#include "Event/Input.h"

#include "Scaleform/Scaleform.h"
#include "HUDHandler.h"

using namespace RE::BSScript;
using namespace SKSE;
using namespace SKSE::log;
using namespace SKSE::stl;

void postInitCallback() {
    auto menu = GuiMenu::GetSingleton();
    if (!menu) return;

    DXGIPresentHook::callback_mutex.lock();
    DXGIPresentHook::pre_callbacks.push_back([menu]() { 
        menu->LoadFont();
    });
    DXGIPresentHook::mid_callbacks.push_back([menu]() { 
        menu->DrawMain();
    });
    DXGIPresentHook::callback_mutex.unlock();
    menu->NotifyInit();
}

template <class T>
void write_thunk_call() {
    auto& trampoline = SKSE::GetTrampoline();
    REL::Relocation<std::uintptr_t> hook{T::id, T::offset};
    T::func = trampoline.write_call<5>(hook.address(), T::thunk);
}

namespace {
    /**
     * Setup logging.
     *
     * <p>
     * Logging is important to track issues. CommonLibSSE bundles functionality for spdlog, a common C++ logging
     * framework. Here we initialize it, using values from the configuration file. This includes support for a debug
     * logger that shows output in your IDE when it has a debugger attached to Skyrim, as well as a file logger which
     * writes data to the standard SKSE logging directory at <code>Documents/My Games/Skyrim Special Edition/SKSE</code>
     * (or <code>Skyrim VR</code> if you are using VR).
     * </p>
     */
    void InitializeLogging() {
        auto path = log_directory();
        if (!path) {
            report_and_fail("Unable to lookup SKSE logs directory.");
        }
        *path /= PluginDeclaration::GetSingleton()->GetName();
        *path += L".log";

        if (IsDebuggerPresent()) {
            auto msvc_sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
            msvc_sink->set_level(spdlog::level::debug);

            auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
            file_sink->set_level(spdlog::level::trace);

            auto log = std::make_shared<spdlog::logger>("Global",
                                                        std::initializer_list<spdlog::sink_ptr>{msvc_sink, file_sink});
            log->set_level(spdlog::level::trace);
            log->flush_on(spdlog::level::level_enum::trace);

            spdlog::set_default_logger(std::move(log));
            spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [%t] [%s:%#] %v");
        } else {
            auto log = std::make_shared<spdlog::logger>(
                "Global", std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true));

            spdlog::level::level_enum level = spdlog::level::level_enum::info;

            log->set_level(level);
            log->flush_on(level);

            spdlog::set_default_logger(std::move(log));
            spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [%t] [%s:%#] %v");
        }
    }

    /**
     * Initialize the SKSE cosave system for our plugin.
     *
     * <p>
     * SKSE comes with a feature called a <em>cosave</em>, an additional save file kept alongside the original Skyrim
     * save file. SKSE plugins can write their own data to this file, and load it again when the save game is loaded,
     * allowing them to keep custom data along with a player's save. Each plugin must have a unique ID, which is four
     * characters long (similar to the record names used by forms in ESP files). Note however this is little-endian, so
     * technically the 'SMPL' here ends up as 'LPMS' in the save file, unless we use a byte order swap.
     * </p>
     *
     * <p>
     * There can only be one serialization callback for save, revert (called on new game and before a load), and load
     * for the entire plugin.
     * </p>
     */
    void InitializeSerialization() {
        log::trace("Initializing cosave serialization...");
        auto* serde = GetSerializationInterface();
        serde->SetUniqueID(_byteswap_ulong('UIHS'));
        serde->SetSaveCallback(Serialize::OnGameSaved);
        serde->SetRevertCallback(Serialize::OnRevert);
        serde->SetLoadCallback(Serialize::OnGameLoaded);
        log::trace("Cosave serialization initialized.");
    }

    /**
     * Initialize the trampoline space for function hooks.
     *
     * <p>
     * Function hooks are one of the most powerful features available to SKSE developers, allowing you to replace
     * functions with your own, or replace a function call with a call to another function. However, to do this, we
     * need a code snippet that replicates the first instructions of the original code we overwrote, in order to be
     * able to call back to the original control flow with all the same functionality.
     * </p>
     *
     * <p>
     * CommonLibSSE comes with functionality to allocate trampoline space, including a common singleton space we can
     * access from anywhere. While this is not necessarily the most advanced use of trampolines and hooks, this will
     * suffice for our demo project.
     * </p>
     */
    void InitializeD3DHooking() {
        logger::trace("Initializing trampoline...");
        auto& trampoline = GetTrampoline();
        trampoline.create(28);

        D3DInitHook::post_init_callbacks.push_back(postInitCallback);
        write_thunk_call<D3DInitHook>();
        write_thunk_call<DXGIPresentHook>();
        logger::trace("Trampoline initialized.");
    }

    /**
     * Register to listen for messages.
     *
     * <p>
     * SKSE has a messaging system to allow for loosely coupled messaging. This means you don't need to know about or
     * link with a message sender to receive their messages. SKSE itself will send messages for common Skyrim lifecycle
     * events, such as when SKSE plugins are done loading, or when all ESP plugins are loaded.
     * </p>
     *
     * <p>
     * Here we register a listener for SKSE itself (because we have not specified a message source). Plugins can send
     * their own messages that other plugins can listen for as well, although that is not demonstrated in this example
     * and is not common.
     * </p>
     *
     * <p>
     * The data included in the message is provided as only a void pointer. It's type depends entirely on the type of
     * message, and some messages have no data (<code>dataLen</code> will be zero).
     * </p>
     */
    void InitializeMessaging() {
        if (!GetMessagingInterface()->RegisterListener([](MessagingInterface::Message* message) {
            switch (message->type) {
                // Skyrim lifecycle events.
                case MessagingInterface::kPostLoad: // Called after all plugins have finished running SKSEPlugin_Load.
                    // It is now safe to do multithreaded operations, or operations against other plugins.
                case MessagingInterface::kPostPostLoad: // Called after all kPostLoad message handlers have run.
                case MessagingInterface::kInputLoaded: // Called when all game data has been found.
                    break;
                case MessagingInterface::kDataLoaded: // All ESM/ESL/ESP plugins have loaded, main menu is now active.
                    // It is now safe to access form data.
                   
                    GuiMenu::GetSingleton()->NotifyFontReload();
                    InputHandler::Register();
                    HUDHandler::Register();
                    EquipHandler::Register();
                    CombatHandler::Register();
                    ContainerHandler::Register();
                    Scaleform::Register();
                    Translator::GetSingleton()->Load();
                    break;

                // Skyrim game events.
                case MessagingInterface::kNewGame: // Player starts a new game from main menu.
                case MessagingInterface::kPreLoadGame: // Player selected a game to load, but it hasn't loaded yet.
                    // Data will be the name of the loaded save.
                case MessagingInterface::kPostLoadGame: // Player's selected save game has finished loading.
                    // Data will be a boolean indicating whether the load was successful.
                case MessagingInterface::kSaveGame: // The player has saved a game.
                    // Data will be the save name.
                case MessagingInterface::kDeleteGame: // The player deleted a saved game from within the load menu.
                    break;
            }
        })) {
            stl::report_and_fail("Unable to register message listener.");
        }
    }
}

/**
 * This if the main callback for initializing your SKSE plugin, called just before Skyrim runs its main function.
 *
 * <p>
 * This is your main entry point to your plugin, where you should initialize everything you need. Many things can't be
 * done yet here, since Skyrim has not initialized and the Windows loader lock is not released (so don't do any
 * multithreading). But you can register to listen for messages for later stages of Skyrim startup to perform such
 * tasks.
 * </p>
 */
SKSEPluginLoad(const LoadInterface* skse) {
    InitializeLogging();

    auto* plugin = PluginDeclaration::GetSingleton();
    auto version = plugin->GetVersion();
    logger::info("{} {} is loading...", plugin->GetName(), version);


    Init(skse);
    InitializeD3DHooking();
    InitializeMessaging();
    InitializeSerialization();

    logger::info("{} has finished loading.", plugin->GetName());
    return true;
}
