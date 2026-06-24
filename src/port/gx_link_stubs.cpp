// Console link stubs for the desktop-only GUI and asset-extractor subsystems.
//
// The ImGui menu (src/port/ui) and the ROM asset extractor (GameExtractor, which
// needs Torch/Companion) are not built for console. Engine.cpp still references
// their entry points, so provide inert definitions here: the menu does nothing, and
// extraction always fails (assets come pre-built in the .o2r on the SD card).

#include "ui/GhostshipGui.hpp"
#include "GameExtractor.h"
#include "mods/achievements/Achievements.h"

#include <unordered_map>

// The achievements feature (UI + api) is not built for console, but the save layer
// references the global list; provide it empty.
std::unordered_map<std::string, Achievement> gAchievementList;

// The dev console (ImGui) and randomizer menu are not built for console; provide
// their entry points as no-ops so the boot path links.
void DevConsole_Init(void) {}

namespace Rando {
void Init() {}
} // namespace Rando

namespace GhostshipGui {
std::shared_ptr<GhostshipMenu> mGhostshipMenu = nullptr;

void SetupHooks() {}
void SetupMenu() {}
void SetupGuiElements() {}
void Draw() {}
void Destroy() {}
void RegisterPopup(std::string /*title*/, std::string /*message*/, std::string /*button1*/, std::string /*button2*/,
                   std::function<void()> /*button1callback*/, std::function<void()> /*button2callback*/) {}
size_t PopupsQueued() {
    return 0;
}
UIWidgets::Colors GetMenuThemeColor() {
    return UIWidgets::Colors::Red;
}
} // namespace GhostshipGui

// GameExtractor: no runtime ROM extraction on console.
bool GameExtractor::GenAssetFile() {
    return false;
}
std::optional<std::string> GameExtractor::ValidateChecksum() const {
    return std::nullopt;
}
bool GameExtractor::RunStandalone(std::string /*rom*/) {
    return false;
}
bool GameExtractor::SelectGameFromUI() {
    return false;
}
void GameExtractor::SetSearchPath(const std::string& path) {
    mSearchPath = path;
}
void GameExtractor::GetRoms(std::vector<std::string>& roms) {
    roms.clear();
}
std::string GameExtractor::GetRomPath() {
    return "";
}
bool GameExtractor::Parse(std::atomic<size_t>& /*assetCount*/, std::string /*appShortName*/) {
    return false;
}
bool GameExtractor::GenerateOTR(std::string /*appShortName*/) {
    return false;
}
bool GameExtractor::GenerateOTR(std::atomic<size_t>& /*assetCount*/, std::string /*appShortName*/) {
    return false;
}
void GameExtractor::WritePortVersion() {}
