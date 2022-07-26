# CommonLibSSE Sample Plugin
A sample SKSE plugin developed in C++. This project is meant to be used both as a template for modern SKSE plugin
development, when starting a new project, or as a tutorial project for those new to CommonLibSSE development who want a
complete and modern project from which to learn.

## Table of Contents
* [Getting Started](#getting-started)
  * [Environment Setup](#environment-setup)
    * [Installing Visual Studio](#installing-visual-studio)
    * [Installing Git](#installing-git)
    * [Vcpkg Install and Configuration](#vcpkg-install-and-configuration)
  * [Cloning the Repository](#cloning-the-repository)
    * [Importing the Project into Your IDE](#importing-the-project-into-your-ide)
      * [Visual Studio](#visual-studio)
      * [Visual Studio Code](#visual-studio-code)
* [Understanding the Project](#understanding-the-project)
  * [Build Features](#build-features)
    * [Vcpkg Integration](#vcpkg-integration)
    * [Multi-Runtime Builds](#multi-runtime-builds)
    * [Clang Support](#clang-support)
    * [Automatic Deployment](#automatic-deployment)
    * [Unit Testing](#unit-testing)
    * [DLL Metadata](#dll-metadata)
    * [Miscellaneous Elements](#miscellaneous-elements)
  * [Plugin Structure](#plugin-structure)
    * [Plugin Initialization](#plugin-initialization)
    * [Logging](#logging)
    * [Configuration](#configuration)
    * [Messaging and Lifecycle Events](#messaging-and-lifecycle-events)
    * [Papyrus Bindings](#papyrus-bindings)
    * [Papyrus Development](#papyrus-development)
    * [Serialization (the SKSE Cosave)](#serialization-the-skse-cosave)
    * [Function Hooks](#function-hooks)
  * [Other Features](#other-features)
    * [Source Code Formatting](#source-code-formatting)
    * [Deploying a FOMOD](#deploying-a-fomod)
    * [Licensing](#licensing)

## Getting Started
### Environment Setup
#### Installing Visual Studio
To do Windows development you will need to install [Visual Studio](https://visualstudio.microsoft.com/). The Community
Edition is free to install, but you must create a Visual Studio account. During install you will be presented with
the components you wish to install for development. The only one required for SKSE development is "Desktop development
with C++". Select it and leave the detailed options on the right untouched unless you really know what you are doing.

![Visual Studio Installer](docs/visual-studio-install.png)

#### Installing/Configuring Visual Studio Code
The Visual Studio installer includes the Visual Studio IDE as well as the development tools needed for C++ development.
However, many SKSE developers use Papyrus as well, since SKSE can be used to add new Papyrus bindings. This is typically
done using Visual Studio Code as it has advanced plugins for Papyrus development other IDEs lack. You can [download
Visual Studio Code](https://visualstudio.microsoft.com/) for free at the same site you used to get Visual Studio.

Once installed, open Visual Studio Code. On the left-hand side of the window find the button called Extensions and click
it (or press `Ctrl+Shift+X`). Search for "Papyrus" in the search bar at the top of the panel and find the extension
called "Papyrus" by Joel Day. Click "Install" to install the extension. You will now be able to setup Papyrus
development workspaces in VS Code.

![Visual Studio Code Papyrus Extension Install](docs/vscode-papyrus-install.png)

#### Installing Git
If you do not already have Git installed, [download and install it](https://gitforwindows.org/) (you do not need to
worry about the specific configuration options during install).

#### Vcpkg Install and Configuration
Vcpkg is a package manager for C/C++ libraries, which makes integrating third-party libraries into your project easy. It
is also installed with Git. Clone Vcpkg and then set it up by running the following commands:

```commandline
git clone https://github.com/microsoft/vcpkg
.\vcpkg\bootstrap-vcpkg.bat
.\vcpkg\vcpkg integrate install
```

This project allows for using default Vcpkg configuration when none is externally specified (e.g. from the command line
or when built as a dependency via Vcpkg). This makes development in your dev environment simpler. To auto-detect Vcpkg
you must set an environment variable `VCPKG_ROOT` to the path to your Vcpkg install. To do so open Control
Panel and go to System. On the left-hand side click About. You will now see an option on the right-hand side of the
window for "Advanced system settings". You will get a window with a number of options; click "Environment Variables".

![Environment Variables Button](docs/vcpkg-system-properties.png)

In the environment variables screen click New and enter `VCPKG_ROOT` as the variable name. For the value,
enter the full path to your Vcpkg install. Note that this variable is not picked up by currently-running applications,
until they are restarted.

![Environment Variables Settings](docs/vcpkg-env.png)

### Cloning the Repository
Clone this repository to your local machine by running
the following command at the command line (using Command Prompt, Powershell, or Windows Terminal):

```commandline
git clone https://gitlab.com/colorglass/commonlibsse-sample-plugin.git
```

This will create a directory called `commonlibsse-sample-plugin` with a clone of this project.

### Importing the Project into Your IDE
#### Visual Studio
Open Visual Studio. You will be presented with a launch screen that lets you select the project to work on. You want to
select "Open a local folder".

![Visual Studio Import](docs/visual-studio-import-folder.png)

Find the folder to which you cloned this repository and select that folder. Visual Studio should open and you will
shortly see a screen that should look roughly like the following (note your window may vary in the visual theme, icons,
and placement of the panels, such as the file tree being on the right side rather than left; this is because my own
installation I used to take these screenshots is not fresh and has been customized).

![Visual Studio Project Imported](docs/visual-studio-freshly-imported.png)

Visual Studio will begin to import the CMake configuration. CMake is build system used by this project, and it is
configured in the `CMakeLists.txt` file. Visual Studio will automatically begin to download and build all the
dependencies needed to build this project, and import the projects settings. This will take some time. If you do not
see the "Output" panel, look around the edge of the screen for a tab that says "Output" and click it to see the output
from the CMake import process. Wait until it is done. If you open files before it is complete Visual Studio cannot tell
you yet if there are any problems, and you will see a warning along the top of the file that C++ Intellisense is not yet
available.

![Visual Studio CMake Import In Progress](docs/visual-studio-cmake-processing.png)

Once Visual Studio has completed the import of the project, you can now do development. You will now have an option to
build the project under the Build menu. Use `Build->Build All` (or `Ctrl+Shift+B`) to build the project.

![Visual Studio CMake Import In Progress](docs/visual-studio-cmake-success.png)

Build the project with `Build->Build All` (or `Ctrl+Shift+B`); if all has gone well you should see a notification that
the build was successful in your status bar, and in the output panel if it is visible. Congratulations, you've built
your first SKSE plugin! You can find the DLL in the project directory under `build/FDGESamplePlugin.dll`.

![Visual Studio CMake Import In Progress](docs/visual-studio-build-success.png)

#### Visual Studio Code
The sample project comes with configuration out-of-the-box for doing Papyrus development in Visual Studio Code. Doing so
requires that the Skyrim vanilla script sources, and the SKSE sources, are available. Therefore, you should load your
project for C++ development in Visual Studio first, and wait until the CMake configuration has completed, before doing
Papyrus development. The Vcpkg repository from the Skyrim NG project includes Vcpkg ports that automatically extract the
necessary Papyrus script sources as a part of the CMake configuration process.

One the CMake configuration is done, in Visual Studio Code go to `File -> Open Workspace From File...` and find the
project file `CommonLibSSESamplePlugin.code-workspace` and open it. This gives you a Visual Studio Code workspace with
everything prepared for Papyrus development.

## Understanding the Project
### Build Features
#### Vcpkg Integration
Like many SKSE projects, this sample project uses Vcpkg to manage dependencies that are needed to build and run the
project. However one advanced feature seen here is the use of Vcpkg to manage even Skyrim-oriented dependencies.
Traditionally projects like CommonLibSSE were included via a Git submodule. This has a number of disadvantages. It
subjects CommonLibSSE to the build configuration for your project. It also requires you to list all the transitive
dependencies in your own `vcpkg.json` file.

To solve this problem the Skyrim NG project has produced a public repository, available for all in the Skyrim and
Fallout 4 communities, to use for their development. This repository includes a modern head of CommonLibSSE
development (called `commonlibsse-ng`). This version of CommonLibSSE uniquely is capable of working with any version of
Skyrim, not only at build-time, but at runtime as well. This library is also available in a precompiled form as
`commonlibsse-ng-prebuilt`, which is being used here to save time. The resulting DLL can be used with Skyrim SE, AE, and
Skyrim VR.

```json
{
    "registries": [
        {
            "kind": "git",
            "repository": "https://gitlab.com/colorglass/vcpkg-colorglass",
            "baseline": "59ebdd824b295fad4effcdccfe6e6aaa47ff4764",
            "packages": [
              // ...
            ]
        }
    ]
}
```

The use of CommonLibSSE NG by default lets this sample project work with Skyrim SE, AE, and VR in a single build.

Furthermore, this Vcpkg repository includes the ability to build and link to SKSE itself, as well as the ability to
deploy the original Bethesda script sources and SKSE versions of those sources. Using the `bethesda-skyrim-scripts`
port will cause Vcpkg to find your Skyrim installation via the registry and extract the script sources locally into
your project build directory, allowing you to do local Papyrus development. SKSE scripts are also download and extracted
when using the `skse` port's `scripts` feature.

The availability of these projects is handled by the `vcpkg-configuration.json` file, which brings in the Skyrim NG
repository hosted by Color-Glass Studios, and is a big step forward in streamlining the development process.

#### Multi-Runtime Builds
A major problem with developing for modern Skyrim is the fragmentation of Skyrim runtimes between pre-AE executables,
post-AE executables, and Skyrim VR. This project demonstrates how to achieve support for all three in a single codebase.
The version of CommonLibSSE used in this project is a fork from the Skyrim NG project called CommonLibSSE NG, which
allows a single version of the compiled library to work with Skyrim AE, SE, or VR. Therefore, your resulting DLL will be
able to work with any version of Skyrim. Users will not need to choose a correct version of the DLL to download and
install.

#### Clang Support
If used with CommonLibSSE NG as the CommonLibSSE fork (the default choice), then it is possible to build the project
with the Clang compiler. Two CMake presets for Clang, a debug and release build, are included. Clang-CL must be
available in `PATH` to build with Clang. This can be done easily by installing Clang from the Visual Studio Installer.
However, this version is currently out of date, and so it is recommended to instead install LLVM from the LLVM GitHub
releases page, and add its `bin` directory to `PATH` yourself.

#### Automatic Deployment
When building the sample project, build results are automatically deployed to `contrib/Distribution`. This directory
has the FOMOD installer for the project. DLL and PDB files are copied automatically. In addition, the CMake clean action
has been extended to clean this files in the FOMOD directory. The project also integrates with the Papyrus extension for
Visual Studio Code. When performing a build of the Papyrus scripts the result will be copied to the appropriate
directory for Papyrus scripts (the compiled scripts are also cleaned by a CMake clean).

You can also incrementally build to a mod directory in Mod Organizer 2. The CMake build is configured to deploy the DLL
and PDB files to an MO2 mod directory if one has been specified by environment variable. The variable, called
`CommonLibSSESamplePluginTargets`, should be set to point to the base directory for the MO2 mod you want to deploy the
files to (do not include `SKSE/Plugins` at the end). This allows you to simply build after making changes and
immediately be able to run Skyrim from MO2 to see the results. This variable is treated as a semicolon-separated list,
so if you are multi-targeting you can deploy to multiple locations, such as separate AE, SE, and VR mod lists.

#### Unit Testing
The project comes with built-in support for running unit tests with Catch2. The build produces an executable with all
Catch2 unit tests; running this executable will run the tests. See `test/HitCounterManager.cpp` for an example. Catch2
has native support in the CLion IDE, and support can be added to Visual Studio and Visual Studio Code with extensions in
their respective marketplaces. In addition, the `CMakePresets.json` includes CTest presets. CTest is a part of CMake and
is a test runner that integrates with other testing frameworks. Tests can also be run via CTest, which is supported
natively in Visual Studio, Visual Studio Code, and CLion.

You can run the tests from within your IDE to see results in its UI, or simply execute the test executable it builds,
called `CommonLibSSESamplePluginTests.exe`. CommonLibSSE NG enhances CommonLibSSE to enable the tests to run without
being within a Skyrim environment. However, some care must be taken. As unit tests, and not integration tests, it is
important to not exercise code that interacts with the Skyrim runtime. Avoid function hooks or calling Skyrim
functions. Unit tests are best used to test the Skyrim-independent backend to your code, as is done in this sample where
the `HitCounterManager` is tested.

It is also possible to perform certain degrees of integration testing. In the CommonLibSSE NG project, the convention is
to tag integration tests which require interaction with the Skyrim engine, but do not require the engine to initialize
(i.e. does not require it to even run its `main()` function), with the tag `[integation]`. Those which require the
engine to be initialized are tagged `[e2e]` (meaning end-to-end). Integration tests require interacting with a Skyrim
engine that is loaded into the test executable dynamically, rather than in an SKSE plugin loaded into a Skyrim process.
This is a feature supported currently only in the CommonLibSSE NG fork and is an advanced topic not covered in this
sample.

#### DLL Metadata
This project comes with a `version.rc.in` file which generates metadata for your output. This embeds things like your
project's name, version number, and licensing into the DLL file, so it shows up in the Windows properties dialog for
the DLL. The metadata comes from the CMake project definition. The same information is also used to inject the correct
name and version number into a header file (`PluginInfo.h`) to be used in declaring the plugin to SKSE.

#### Miscellaneous Elements
The CMake configuration for the project addresses common issues with C++ development with Visual Studio.

```cmake
if (WIN32)
  add_compile_definitions(
          UNICODE
          _UNICODE
          NOMINMAX
          _AMD64_
          WIN32_LEAN_AND_MEAN
  )
endif ()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_definitions(
            __cpp_lib_char8_t
            __cpp_consteval
            __cpp_lib_format
    )
endif ()
```

Interprocedural optimizations are enabled whenever possible, which improves performance by further optimizing the
output at link-time:

```cmake
check_ipo_supported(RESULT USE_IPO OUTPUT IPO_OUTPUT)
if (USE_IPO)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
else ()
    message(WARNING "Interprocedural optimizations are not supported.")
endif ()
```

CMake targets are generated for install, allowing the project to be consumed by other CMake projects:

```cmake
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/Sample"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

install(TARGETS ${PROJECT_NAME}
        DESTINATION "${CMAKE_INSTALL_LIBDIR}")
```

This allows another project that wants to depend on this one to configure itself automatically, like so:

```cmake
find_package(CommonLibSSESamplePlugin CONFIG REQUIRED)
# ...
target_link_libraries(${PROJECT_NAME} PRIVATE CommonLibSSESamplePlugin)
```

### Plugin Structure
#### Plugin Initialization
When SKSE starts, it searches `Data\SKSE\Plugins` for DLL files. Each DLL file is inspected to determine if it is a
proper SKSE plugin. The way in which this is done differs between SE/VR and AE versions of SKSE, and this project is
designed to support all of these cases.

For AE versions of the executable, SKSE looks for static data in the DLL with the plugin metadata, in a structure named
`SKSEPlugin_Version`. In CommonLibSSE NG this can be defined declaratively, for a simpler syntax, with the
`SKSE::PluginDeclaration` class, but a simpler way is to use the CMake function:

```cmake
add_commonlibsse_plugin(${PROJECT_NAME} SOURCES ${headers} ${sources})
```

This function also generates an `SKSEPlugin_Version` object, as well as `SKSEPlugin_Query` function, which is the SE-era
SKSE's way of identifying an SKSE plugin. The generated function will configure the metadata SKSE sees to be identical
to what is in `SKSEPlugin_Version`. This particular sample specifies that it uses the Address Library, by not specifying
any other compatibility mode (Address Library is the default). It is possible to specify only specific Skyrim versions
are acceptable if this is changed, e.g.:

```c++
add_commonlibsse_plugin(${PROJECT_NAME}
        COMPATIBLE_RUNTIMES 1.6.353 1.6.343
        SOURCES ${headers} ${sources}
)
```

It is *strongly* encouraged that you use Address Library whenever possible.

Once valid SKSE plugins have been identified, SKSE will call their `SKSEPlugin_Load` functions one at a time. This
function must also be present or the SKSE plugin will not be loaded, and the function must have a particular signature.
This is simplified in CommonLibSSE NG by using the `SKSEPluginLoad` macro:

```c++
SKSEPluginLoad(const SKSE::LoadInterface* skse) {
    ...
}
```

Like `SKSEPlugin_Query`, this function must return `true` or the plugin will not be loaded. It is in this function that
the basic initialization of your plugin should be performed. Do not perform any operation here that depends on other
plugins (which may not be loaded), or which do multithreading (which can cause a deadlock during DLL initialization).
Instead, you can use the SKSE messaging system (discussed below) to perform such operations at later stages in Skyrim
startup.

Note that in this sample project, during initialization, there is a call to `SKSE::Init(skse)` in the load function.
This is a CommonLibSSE function which initializes it's various interfaces that allow interacting with SKSE. As a general
rule your load function should initialize logging before all else (to maximize how much code can have logging),
followed by this function. Calls to other interfaces such as messaging, serialization, Papyrus binding, etc. all require
this function to be called first.

#### Logging
Logging is an important way to capture information that helps you debug issues with your plugin, especially when it is
running on end-users' games. CommonLibSSE bundles spdlog, a popular logging library, and provides some convience
functions for invoking it. It otherwise leaves configuration up to you. More advanced logging systems are available
from frameworks like Fully Dynamic Game Engine.

The simple configuration used in this project creates a file sink, which writes log output to a file in the standard
SKSE logging location (`Documents\My Games\Skyrim Special Edition\SKSE`, or `Skyrim VR` if playing Skyrim VR). On each
run the file is replaced with a new log. A useful default format is provided. The logging level defaults to `info`, and
the flush level (the level of a log event that forces all log output to be written to file immediately) is `trace`,
which is the lowest level (therefore causing any log event to be written immediately).

The logging configuration here has a special behavior if SKSE is run in debug mode and a debugger is attached at the
time that SKSE starts. Instead of writing to file, it will write to the Windows debug channel, which allows the attached
debugger to see the messages. The log events will then show up in your IDE console instead of a log file.

#### Configuration
Many SKSE plugins have configuration options, controlled via a config file. This plugin has a basic YAML config file
that can customize the log levels of the logger. The parsing for the log file is handled by the Articuno serialization
library, a next-generation serialization library that can easily map YAML, JSON, TOML, and other formats to C++
classes. You can find the config logic in `src/Config.h` and `src/Config.cpp`. The logger initialization function gets
the config object to query the log levels it should use.

#### Messaging and Lifecycle Events
SKSE plugins can exchange information without strong coupling using SKSE as a message bus. This allows a plugin to
broadcast a message with a particular message ID and arbitrary data attached to it, for which other plugins can register
to listen. It is not common for plugins to broadcast information this way, however SKSE itself also broadcasts messages
which are widely used to hook into Skyrim's lifecycle.

When Skyrim starts, SKSE will begin by querying for SKSE plugins and then calling each plugin's `SKSEPlugin_Load`
function. One all load functions are called it will signal a `SKSE::MessagingInterface::kPostLoad` message. Once all
of those handlers have run, it will signal an `SKSE::MessagingInterface::kPostPostLoad` message. After that it waits
until Skyrim has found all of its source files and initialized inputs, and signal
`SKSE::MessagingInterface::kInputLoaded`. Finally, after all the ESM/ESL/ESP plugins are loaded, it will signal
`SKSE::MessagingInterface::kDataLoaded` (warning: this will not fire again if the user reloads their load order using
the in-game mod menu; such cases can be caught using addon frameworks such as Fully Dynamic Game Engine). In general,
it is safe to begin using multithreaded operations and interacting with other plugins after
`SKSE::MessagingInterface::kPostLoad` fires, and safe to start interacting with forms after
`SKSE::MessagingInterface::kDataLoaded` fires.

In addition to these messages, there are several other messages related to gameplay:
* `SKSE::MessagingInterface::kNewGame`: the user has started a new game by selecting New Game at the main menu.
* `SKSE::MessagingInterface::kSaveGame`: the user has saved their game.
* `SKSE::MessagingInterface::kDeleteGame`: the user has deleted a save game.
* `SKSE::MessagingInterface::kPostLoadGame`: the user has loaded a saved game.
* `SKSE::MessagingInterface::kPreLoadGame`: the user has selected a saved game to load, but it has not loaded yet.

In this sample project we initialize the function hooks in `kDataLoaded`; this is not necessary, as this can be done
in `SKSEPlugin_Load`, but it is done here as a demonstration of how messaging works.

#### Papyrus Bindings
You can add new Papyrus functions that are implemented in native code using SKSE. The sample project starts this process
in the load time of the plugin, when it gets the Papyrus interfaces with `SKSE::GetPapyrusInterface()`. This call
returns an interface that can be used to call a registration callback. When Skyrim is still loading, the Papyrus virtual
machine is not yet ready to register native functions. The registration callback will be called to register your native
functions after the VM is initialized. It is possible to register functions any time after the VM is initialized, using
the `RE::BSScript::IVirtualMachine` interface (or the `RE::BSScript::Internal::VirtualMachine` singleton object), but
the most common way to register functions using the registration callbacks:

```c++
if (SKSE::GetPapyrusInterface()->Register(Sample::RegisterHitCounter)) {
    log::debug("Papyrus functions bound.");
} else {
    stl::report_and_fail("Failure to register Papyrus bindings.");
}
```

Your registration callback should be a function which accepts either a `RE::BSScript::IVirtualMachine*` or a
`RE::BSScript::Internal::VirtualMachine*` argument and returns a boolean, with `true` indicating success. During this
callback you can register a native function with the VM's `RegisterFunction` call:

```c++
bool Sample::RegisterHitCounter(IVirtualMachine* vm) {
    vm->RegisterFunction("StartCounting", PapyrusClass, StartCounting);
    vm->RegisterFunction("StopCounting", PapyrusClass, StopCounting);
    vm->RegisterFunction("GetTotalHitCounters", PapyrusClass, GetTotalHitCounters);
    vm->RegisterFunction("Increment", PapyrusClass, Increment);
    vm->RegisterFunction("GetCount", PapyrusClass, GetCount);

    return true;
}
```

Registering a function accepts the name of the function, the name of the script class, and finally a pointer to the
function that will be executed to handle the function. Normally, SKSE cannot handle instance functions, only global
functions (unless you are using Fully Dynamic Game Engine). The first argument of your callback function takes the
Papyrus `self` argument, but for global functions it accepts an argument of type `RE::StaticFunctionTag*` instead, and
this will therefore be your `self` argument for all functions. It is also possible to precede this argument with two
others, one of type `RE::BSScript::IVirtualMachine*` (or `RE::BSScript::Internal::VirtualMachine*`), and a second of
type `RE::VMStackID`. This is an advanced case that can be used to reflect on the call stack that was used to invoke
your function, and it is not demonstrated in this project.

The subsequent arguments to your function will map to the Papyrus function that are passed in. These arguments can be
mapped to primitive Papyrus types like `Bool` (a C++ `bool`), `Int` (a C++ `int32_t`), `Float` (a C++ `float_t`), or
a `String` (which can map to a C++ `std::string`, `std::string_view`, or `RE::BSFixedString`), or they can be a `Form`,
`ActiveMagicEffect`, or `Alias`. If the argument is a form, your argument can be any CommonLibSSE form pointer type that
is compatible with the Papyrus type, e.g. a `RE::TESForm*`, `RE::TESObjectWEAP*`, `RE::Actor*`, etc. `ActiveMagicEffect`
maps to `RE::ActiveEffect*`, and `Alias` maps to `RE::BGSBaseAlias*` or its child classes.

Papyrus arrays can be mapped to a C++ type of a generic container with an element type that can be bound to the type of
the array elements in Papyrus. The container can be any type which is "array-like", such as `std::vector`, `std::list`,
or any custom container that implements similar functions and constructors.

Similar rules apply to your return types, which convert from C++ back to the Papyrus types.

```c++
// Example Papyrus binding.
int32_t GetCount(StaticFunctionTag*, Actor* actor) {
    if (!actor) {
        return 0;
    }
    return HitCounterManager::GetSingleton().GetHitCount(actor).value_or(0);
}
```

#### Papyrus Development
To work with your Papyrus scripts, use the Papyrus project view in Visual Studio Code. You an press `Ctrl+Shift+B` to
activate the build task. The sample project has three build tasks, one to build the Papyrus scripts in debug mode, one
to build the in release mode (with optimizations), and one to build tests (in debug mode only; this builds the script
that lets you demo the functionality in-game). You can select the proper build type to perform that build, which will
populate the proper script folders in the FOMOD directory (`contrib/Distribution`) with the resulting scripts so that
they can be included in the final FOMOD archive.

#### Serialization (the SKSE Cosave)
Our sample project is tracking how many times each actor is hit, but the state of our plugin will be reverted after
every game load. We need it to track the hit counts in the save game, and restore that count when the game is loaded.
SKSE provides a *cosave* for this purpose. SKSE plugins add their own custom data to the cosave, which is stored in a
separate file alongside the original Skyrim save file. Each plugin can register one callback to handle the cosave for
saving, loading, and reverting the game to its default state (used when a new game starts or a a save is about to be
loaded).

Each plugin must use a unique ID for its content in the save file.

#### Function Hooks
Function hooking is the act of intercepting a function, or a function call site, and replacing the functionality with
your own. This is a common way of performing advanced operations that Skyrim's normal interfaces through Papyrus do not
support. Function hooking and reverse engineering is a broad topic and will not be covered in its totality in this
tutorial, but how it can be accomplished with CommonLibSSE will be.

Hooking a function is usually done in one of two ways: either the function itself is hooked, or a specific call or
branch instruction is hooked. The former will replace a function completely throughout the application. The later will
cause a single call to be redirected, but will not affect other calls to the function. SKSE and CommonLibSSE provide
functionality for the later case -- hooking function call sites -- but not the former, hooking functions themselves.
There are other libraries easily available which can perform function hooking. Because call site hooking is what is
targeted by CommonLibSSE, that is what is used in this project (note that sometimes either method can be used, but often
you specifically want to use one method or the other).

An important concept for hooking functions is a *trampoline*. When you hook a function you are overwriting the
executable code in memory. This necessarily replaces existing code. When doing this, a trampoline is a function that
copies the overwritten code as well as a call to the point where the code would have continued after those instructions
were executed. This takes memory, and so we must allocate memory for it. CommonLibSSE has a `SKSE::Trampoline` type for
this. As many trampolines as you want can be created, but it's common to use a singleton instance with enough space for
all generated code as needed. We do this allocation in `Main.cpp`:

```c++
void InitializeHooking() {
    auto& trampoline = SKSE::GetTrampoline();
    trampoline.create(64);
    // ...
}
```

This allocates 64 bytes of space within the singleton trampoline, enough for the use within this sample project. You can
enable trace level logging to see how much space is used by your trampoline in your logs and find out if you need to
allocate more space.

To hook a call site, you must find the call or branch instruction you want to replace with your own, and then call
`trampoline.write_call<5>` for calls or `trampoline.write_branch<5>` for branches (note: the `5` here indicates that the
instruction being replaced is 5 bytes long; there can also be `write_call<6>` and `write_branch<6>` for 6-byte
instructions, although this is rare). The first argument should be the address of the instruction being replaced, and
the second is the address of the function that will be called in its place. The result is an address offset to the
trampoline function generated.

```c++
OriginalPopulateHitData = trampoline.write_call<5>(fn.address(), reinterpret_cast<uintptr_t>(PopulateHitData));
```

Whenever possible with SKSE development we want to work with Address Library IDs. This allows the resulting DLL to be
portable across different Skyrim executable versions, even though the exact memory offsets where we would hook will
change between releases. That is because Address Library assigns IDs, which persist across releases, to each function
and other objects in the executable. These IDs are used to dynamically lookup the true memory offset at runtime, using
Address Library's databases. We see this being done in this project:

```c++
  int32_t* PopulateHitData(Actor* target, char* unk0);

  REL::Relocation<decltype(PopulateHitData)>& GetHookedFunction() noexcept {
      static REL::Relocation<decltype(PopulateHitData)> value(REL::RelocationID(42832, 44001).address() + 0x42);
      return value;
  }

  REL::Relocation<decltype(PopulateHitData)> OriginalPopulateHitData;
```

We use the `REL::Relocation` type to get a strongly-typed reference to a memory address. The memory is looked up from
Address Library using a unique ID that is persistent across Skyrim releases so that we don't need to update the DLL for
each release. There are two separate lineages of Address Library IDs, one for SE/VR, and one for AE. The function
`RelocationID` takes the SE/VR ID in the first argument and the AE argument in the second. When using CommonLibSSE NG,
this choice is resolved at runtime based on the Skyrim executable currently in use, allowing a single DLL to work across
all versions of Skyrim.

In this case= we are actually hooking a call to another function which occurs `0x42` bytes past the start of the
function. For that reason we need to add an additional `0x42` to the address the Address Library IDs will resolve to.
The signature of that function is `int32_t*(Actor*, char*)`, and we define the function that will intercept that call as
well as using that type for the `REL::Relocation` that maps to that call site. We also keep a second `REL::Relocation`
which we assign when we make the `write_call<5>` call, which points to the resulting trampoline function. We can use
this to call the trampoline, which in effect makes the original function call. This way we can intercept the call while
still letting it proceed normally, instead of completely replacing the original call.

In this case the function we intercepted is a call done while populating data related to hitting something. Whenever
this function is called, it is because something hit an actor. This lets us get the actor and increment its hit count
for our sample hit counter.

```c++
int32_t* PopulateHitData(Actor* target, char* unk0) {
    HitCounterManager::GetSingleton().RegisterHit(target);
    return OriginalPopulateHitData(target, unk0);
}
```

As we see we intercept the function call Skyrim was already doing to see the target of the hit, and can therefore
increment it's hit count. After that we pass the call along to the original function, letting it continue as it
originally did.

### Other Features
#### Source Code Formatting
The project ships with `.clang-format` file, which defines the source code formatting standard. Clang's format tool can
automatically format your source code, and this tool is also integrated into common IDE's. Using your IDE's automatic
formatting function should apply these rules, making them universal across all IDE's.

This specific file uses a widely used standard for code formatting, but many such standards exist and many plugin
authors use different ones. You can customize the file to find the style that you want for your plugin. The
[Clang-Format Configurator](https://zed0.co.uk/clang-format-configurator/) is a useful tool for generating a file.

#### Deploying a FOMOD
Under `contrib/Distribution` is the FOMOD structure for a deployable mod that could be posted to e.g. Nexus Mods. The
FOMOD configuration itself is located at `contrib/Distribution/fomod`. The other directories contain various portions of
what will be installed. The FOMOD structure is designed to also allow the Papyrus sources held here to be the used for
Papyrus development, while compiled Papyrus scripts and output DLL and PDB files are treated as build artifacts (i.e.
they are removed on clean, and ignored by Git).

To produce a FOMOD, build the full project. This includes building all the CMake build profiles, producing both debug
and release builds for AE, SE, and VR. Then in Visual Studio Code run the debug, release, and test builds for the
Papyrus scripts. This will populate all the output files necessary. Finally, produce an archive of the contents of
`contrib/Distribution` with a format supported by common mod managers (7zip is recommended, using minimum compression
for testing and maximum compression for a build uploaded to Nexus).

The FOMOD included in this template has two pages with two choices to make: the first page selects which Skyrim
runtime to target, and the second lets you select whether to install a debug or release build. Skyrim VR is
automatically detected by the presence of its additional `SkyrimVR.esm` file, and therefore when installing for Skyrim
VR it is the only choice available. If `SkyrimVR.esm` is not found then the Skyrim VR option is disabled, and the user
has a choice between installing for AE or SE (which one is in use cannot be auto-detected with existing FOMOD features).

#### Licensing
Licensing is a commonly overlooked concern in the modding community. It is common to find projects with no license or an
arbitrary license with no legal evaluation. In the Skyrim NG project we strongly recommend choosing an open source
license for your project. This allows others to learn from and reuse your code for further development, and ensures that
projects that are abandoned by their owners can be continued and maintained by others.

This sample project uses Apache License 2.0, a high quality liberal open source license preferred by many large
enterprises that use open source. It is similar to another popular license, the MIT license, but modernized to cover
concerns such as patents and retaliation in the event of a copyright lawsuit. Other excellent options are the GPL
license (if you want a strong "copyleft" license) or the LGPL license.

Which license you decide your project should use, it is important that you put its text in the `LICENSE` file, and
update `version.rc.in` to include your license. This ensures the license restrictions are clear to any who use your
project and that the license can be detected and displayed properly on sites like GitHub, GitLab, and other code hosting
services.

If you are keeping the Apache License 2.0 `LICENSE` file for your project and using this sample plugin as a template, be
sure to update line 190 with your custom information.
