#pragma once

// Minimal SDL2 stand-in for the console build. The game uses SDL only for one
// desktop affordance: a "trust this mod author?" message box. libultragx has no
// SDL; this header provides just enough of the SDL_MessageBox* surface to compile,
// and SDL_ShowMessageBox auto-selects the default ("Yes") so the console boot is
// not blocked by a dialog that cannot be shown.

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SDL_arraysize(array) (sizeof(array) / sizeof((array)[0]))

enum {
    SDL_MESSAGEBOX_ERROR = 0x00000010,
    SDL_MESSAGEBOX_WARNING = 0x00000020,
    SDL_MESSAGEBOX_INFORMATION = 0x00000040,
};

enum {
    SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT = 0x00000001,
    SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT = 0x00000002,
};

enum {
    SDL_MESSAGEBOX_COLOR_BACKGROUND = 0,
    SDL_MESSAGEBOX_COLOR_TEXT,
    SDL_MESSAGEBOX_COLOR_BUTTON_BORDER,
    SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND,
    SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED,
    SDL_MESSAGEBOX_COLOR_MAX,
};

typedef struct {
    uint32_t flags;
    int buttonid;
    const char* text;
} SDL_MessageBoxButtonData;

typedef struct {
    uint8_t r, g, b;
} SDL_MessageBoxColor;

typedef struct {
    SDL_MessageBoxColor colors[SDL_MESSAGEBOX_COLOR_MAX];
} SDL_MessageBoxColorScheme;

typedef struct {
    uint32_t flags;
    void* window;
    const char* title;
    const char* message;
    int numbuttons;
    const SDL_MessageBoxButtonData* buttons;
    const SDL_MessageBoxColorScheme* colorScheme;
} SDL_MessageBoxData;

// No console UI; pick the return-key default button (the "Yes"/trust choice) and
// report success so the caller proceeds.
static inline int SDL_ShowMessageBox(const SDL_MessageBoxData* messageboxdata, int* buttonid) {
    int picked = 1;
    if (messageboxdata != NULL) {
        for (int i = 0; i < messageboxdata->numbuttons; i++) {
            if (messageboxdata->buttons[i].flags & SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT) {
                picked = messageboxdata->buttons[i].buttonid;
                break;
            }
        }
    }
    if (buttonid != NULL) {
        *buttonid = picked;
    }
    return 0;
}

#ifdef __cplusplus
}
#endif
