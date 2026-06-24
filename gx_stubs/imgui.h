#pragma once
// No-op ImGui stub for the GX/console build. The full ImGui UI is stripped: the UI
// .cpp are not compiled, but the game's non-UI translation units (Engine.cpp and the
// UI headers they pull) reference ImGui types, enums, and a handful of calls. This
// header provides the value types, opaque handles, enum constants, and inline no-op
// functions so those units compile and link to nothing - no UI is rendered.

#include <stdint.h>
#include <stddef.h>

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
typedef unsigned short ImWchar;
typedef int ImGuiCol;
typedef int ImGuiStyleVar;
typedef int ImGuiHoveredFlags;
typedef int ImGuiSliderFlags;
typedef int ImGuiComboFlags;
typedef int ImGuiButtonFlags;
typedef int ImGuiInputFlags;
typedef int ImGuiInputTextFlags;
typedef int ImGuiWindowFlags;
typedef int ImGuiTableFlags;
typedef int ImGuiTreeNodeFlags;
typedef int ImGuiSelectableFlags;
typedef int ImGuiCond;
typedef void* ImTextureID;

// Enum constants referenced by the compiled headers (values are arbitrary; nothing
// reads them on console).
enum {
    ImGuiButtonFlags_None = 0,
    ImGuiCol_Text = 0,
    ImGuiCol_Border,
    ImGuiCol_FrameBg,
    ImGuiCol_PlotHistogram,
    ImGuiCol_TitleBgActive,
    ImGuiCol_ModalWindowDimBg,
    ImGuiHoveredFlags_AllowWhenDisabled = 0,
    ImGuiInputTextFlags_ReadOnly = 1 << 14,
    ImGuiStyleVar_FramePadding = 0,
    ImGuiStyleVar_FrameRounding,
    ImGuiStyleVar_ItemSpacing,
    ImGuiWindowFlags_NoMove = 1 << 2,
    ImGuiWindowFlags_NoResize = 1 << 1,
    ImGuiWindowFlags_NoScrollbar = 1 << 3,
    ImGuiWindowFlags_NoSavedSettings = 1 << 8,
    ImGuiWindowFlags_AlwaysAutoResize = 1 << 6,
};

// FontAwesome icon ranges/glyphs (used when building the icon font; inert here).
#define ICON_MIN_FA 0xe000
#define ICON_MAX_16_FA 0xf8ff
#define ICON_FA_EXCLAMATION_TRIANGLE "\xef\x81\xb1"

struct ImFont;
struct ImDrawList;
struct ImGuiViewport;
struct ImGuiContext;

struct ImGuiInputTextCallbackData {
    void* UserData = nullptr;
    char* Buf = nullptr;
    int BufTextLen = 0;
    int BufSize = 0;
    ImWchar EventChar = 0;
};

struct ImFontConfig {
    int OversampleH = 1;
    int OversampleV = 1;
    bool PixelSnapH = false;
    float SizePixels = 0.0f;
    bool FontDataOwnedByAtlas = true;
    bool MergeMode = false;
    float GlyphMinAdvanceX = 0.0f;
    ImFontConfig() = default;
};

struct ImFontAtlas {
    ImFont* AddFontDefault(const ImFontConfig* = nullptr) { return nullptr; }
    ImFont* AddFontFromMemoryTTF(void*, int, float, const ImFontConfig* = nullptr, const ImWchar* = nullptr) {
        return nullptr;
    }
    ImFont* AddFontFromMemoryCompressedBase85TTF(const char*, float, const ImFontConfig* = nullptr,
                                                 const ImWchar* = nullptr) {
        return nullptr;
    }
};

struct ImGuiIO {
    ImFontAtlas* Fonts = nullptr;
    ImFont* FontDefault = nullptr;
    float FontGlobalScale = 1.0f;
};

struct ImGuiStyle {
    ImVec2 FramePadding{ 0.0f, 0.0f };
    void ScaleAllSizes(float) {}
};

struct ImGuiTextFilter {
    ImGuiTextFilter() = default;
    bool Draw(const char* = nullptr, float = 0.0f) { return false; }
    bool PassFilter(const char*, const char* = nullptr) const { return true; }
    void Clear() {}
};

namespace ImGui {
inline ImGuiIO& GetIO() {
    static ImGuiIO io;
    static ImFontAtlas atlas;
    io.Fonts = &atlas;
    return io;
}
inline ImGuiStyle& GetStyle() {
    static ImGuiStyle style;
    return style;
}
inline double GetTime() { return 0.0; }
inline float GetCursorPosX() { return 0.0f; }
inline ImVec2 GetContentRegionAvail() { return ImVec2(); }
inline ImVec2 CalcTextSize(const char*, const char* = nullptr, bool = false, float = -1.0f) { return ImVec2(); }

inline void Text(const char*, ...) {}
inline void SetTooltip(const char*, ...) {}
inline bool Button(const char*, const ImVec2& = ImVec2()) { return false; }
inline bool Selectable(const char*, bool = false, ImGuiSelectableFlags = 0, const ImVec2& = ImVec2()) { return false; }
inline void SameLine(float = 0.0f, float = -1.0f) {}
inline void NewLine() {}
inline void Spacing() {}
inline void AlignTextToFramePadding() {}
inline void BeginGroup() {}
inline void EndGroup() {}
inline void BeginDisabled(bool = true) {}
inline void EndDisabled() {}
inline bool BeginCombo(const char*, const char*, ImGuiComboFlags = 0) { return false; }
inline void EndCombo() {}
inline bool BeginPopupModal(const char*, bool* = nullptr, ImGuiWindowFlags = 0) { return false; }
inline void EndPopup() {}
inline void OpenPopup(const char*, ImGuiInputFlags = 0) {}
inline bool IsPopupOpen(const char*, ImGuiInputFlags = 0) { return false; }
inline bool IsItemHovered(ImGuiHoveredFlags = 0) { return false; }
inline void SetNextItemWidth(float) {}
inline void ProgressBar(float, const ImVec2& = ImVec2(-1, 0), const char* = nullptr) {}
inline bool InputTextMultiline(const char*, char*, size_t, const ImVec2& = ImVec2(), ImGuiInputTextFlags = 0,
                               int (*)(ImGuiInputTextCallbackData*) = nullptr, void* = nullptr) {
    return false;
}
inline void PushID(const char*) {}
inline void PushID(int) {}
inline void PopID() {}
inline void PushStyleColor(ImGuiCol, ImU32) {}
inline void PushStyleColor(ImGuiCol, const ImVec4&) {}
inline void PopStyleColor(int = 1) {}
inline void PushStyleVar(ImGuiStyleVar, float) {}
inline void PushStyleVar(ImGuiStyleVar, const ImVec2&) {}
inline void PopStyleVar(int = 1) {}
} // namespace ImGui
