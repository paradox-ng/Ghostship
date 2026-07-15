#include "Saves.h"

#include "sm64.h"
#include "port/ShipInit.hpp"
#include "port/data/SaveConversion.h"

#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

extern "C" struct SaveBuffer gSaveBuffer;

// Resolve at call time, not static-init time. Context::InitPaths (which sets the base
// dir, e.g. sd:/Ghostship/) runs during engine startup, AFTER this translation unit's
// globals are constructed - a global const here would bake in an unresolved relative
// "saves" path and land the folder outside the game directory (at the SD root). All
// callers run after startup, so a function always sees the resolved base dir.
static fs::path SavesPath() {
    return fs::path(Ship::Context::GetPathRelativeToAppDirectory("saves", "sm64"));
}

static void Init() {
    // Create saves directory if it doesn't exist
    if (!fs::exists(SavesPath())) {
        fs::create_directories(SavesPath());
    }
}

extern "C" {
void RestoreMainMenuData(int32_t srcSlot) {
    int32_t destSlot = srcSlot ^ 1;

    bcopy(&gSaveBuffer.menuData[srcSlot], &gSaveBuffer.menuData[destSlot], sizeof(gSaveBuffer.menuData[destSlot]));
}

void RestoreSaveFileData(int32_t fileIndex, int32_t srcSlot) {
    int32_t destSlot = srcSlot ^ 1;

    bcopy(&gSaveBuffer.files[fileIndex][srcSlot], &gSaveBuffer.files[fileIndex][destSlot],
          sizeof(gSaveBuffer.files[fileIndex][destSlot]));
}

void SaveFileDoSave(int32_t fileIndex) {
    std::ofstream file(SavesPath() / ("save_" + std::to_string(fileIndex) + ".json"), std::ios::out);
    if (!file.is_open()) {
        return;
    }

    json j = gSaveBuffer.files[fileIndex][0];
    file << j.dump(1);
    file.close();
}

bool ShouldLoadOldSaveFile(void) {
    return fs::exists(Ship::Context::GetPathRelativeToAppDirectory("default.sav"));
}

void SaveFileLoadAll(void) {
    auto oldSave = Ship::Context::GetPathRelativeToAppDirectory("default.sav");
    if (fs::exists(oldSave)) {
        for (int32_t fileIndex = 0; fileIndex < NUM_SAVE_FILES; fileIndex++) {
            SaveFileDoSave(fileIndex);
        }
        // Move old save files to backup
        fs::rename(oldSave, Ship::Context::GetPathRelativeToAppDirectory("default.sav.bak"));
        return;
    }

    // Read save files
    for (int32_t fileIndex = 0; fileIndex < NUM_SAVE_FILES; fileIndex++) {
        fs::path filepath = SavesPath() / ("save_" + std::to_string(fileIndex) + ".json");
        if (!fs::exists(filepath)) {
            continue;
        }

        std::ifstream file(filepath, std::ios::in);
        if (!file.is_open()) {
            continue;
        }

        json j;
        file >> j;
        // Migrate Existing Saves to Include Vanilla ShipSaveData.
        if (!j.contains("shipSaveData")) {
            j["shipSaveData"] = json::object();
            j["shipSaveData"]["features"] = ShipSaveFeatures{ .achievements = false, .rando = false };
        }

        gSaveBuffer.files[fileIndex][0] = j.get<struct SaveFile>();
        file.close();
    }

    // Read global save file
    fs::path globalpath = SavesPath() / "global.json";
    if (fs::exists(globalpath)) {
        std::ifstream file(globalpath, std::ios::in);
        if (file.is_open()) {
            json j;
            file >> j;
            gSaveBuffer.menuData[0] = j.get<struct MainMenuSaveData>();
            file.close();
        }
    }
}

void SaveMainMenuData(void) {
    std::ofstream file(SavesPath() / "global.json", std::ios::out);
    if (!file.is_open()) {
        return;
    }

    json j = gSaveBuffer.menuData[0];
    file << j.dump(1);
    file.close();
}
}

static RegisterShipInitFunc initFunc(Init);