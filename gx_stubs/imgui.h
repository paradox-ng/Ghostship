#pragma once
// Minimal ImGui type stub for the GX/console build. The full ImGui UI is stripped
// (see docs/INTEGRATION.md / libultragx); only the handful of value types the game's
// non-UI headers reference (colour/vector structs, the opaque font/draw handles) are
// declared here so those headers compile UI-free. No ImGui functions - the UI .cpp
// are not compiled.

#include <stdint.h>

struct ImVec2 {
    float x = 0.0f, y = 0.0f;
    ImVec2() = default;
    ImVec2(float _x, float _y) : x(_x), y(_y) {}
};

struct ImVec4 {
    float x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;
    ImVec4() = default;
    ImVec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
};

typedef uint32_t ImU32;
typedef uint32_t ImGuiID;
typedef int ImGuiCol;
typedef int ImGuiStyleVar;
typedef int ImGuiHoveredFlags;
typedef int ImGuiSliderFlags;
typedef int ImGuiComboFlags;
typedef int ImGuiButtonFlags;
typedef int ImGuiInputFlags;
typedef void* ImTextureID;

// Opaque handles the game's headers hold as pointers (no UI rendered on console).
struct ImFont;
struct ImDrawList;
struct ImGuiViewport;
struct ImGuiInputTextCallbackData;
struct ImGuiContext;
