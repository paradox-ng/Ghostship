#include <libultraship.h>

#include <fast/interpreter.h>
#include "Engine.h"

extern "C" {
#include "audio/external.h"
#include "game/game_init.h"
#include "sm64.h"
}

#include <cstdio>

// Boot checkpoint logger: appends a line to sd:/ghostship_boot.log (the SD is a
// synced folder under Dolphin, so it reads back on the host). Open/close per call
// so the last line survives a hang + kill. TEMP boot-bring-up instrumentation.
// bootlog accumulates into a RAM buffer only; bootflush() commits it to the SD.
// Per-call SD writes contend with the o2r's open SD handle and hang after a few
// dozen ops, so the main loop flushes once per frame instead.
extern "C" void bootlog(const char* msg);
extern "C" void bootflush(void);
static char g_bootbuf[32768];
static int g_bootlen = 0;
extern "C" void bootlog(const char* msg) {
    int n = snprintf(g_bootbuf + g_bootlen, sizeof(g_bootbuf) - g_bootlen, "%s\n", msg);
    if (n > 0 && g_bootlen + n < (int)sizeof(g_bootbuf)) {
        g_bootlen += n;
    }
}
extern "C" void bootflush(void) {
    FILE* f = fopen("sd:/ghostship_boot.log", "w");
    if (f) {
        fwrite(g_bootbuf, 1, g_bootlen, f);
        fflush(f);
        fclose(f);
    }
}

void alloc_pool() {
    // Console fit: the desktop pool is 32 MB (u64[4M]), which alone overruns the
    // GameCube's 24 MB (and Wii MEM1). Use 8 MB so the whole static image fits in
    // MEM1 with room for the heap/stack. Tunable; large levels may need more.
    static u64 pool[1024 * 1024];
    main_pool_init(pool, pool + sizeof(pool) / sizeof(pool[0]));
    gEffectsMemoryPool = mem_pool_init(0x4000, MEMORY_POOL_LEFT);
}

extern "C" void exec_display_list(SPTask* spTask) {
    GameEngine::ProcessGfxCommands((Gfx*)spTask->task.t.data_ptr);
}

void push_frame() {
    GameEngine::StartAudioFrame();
    GameEngine::Instance->StartFrame();
    thread5_iteration();
    GameEngine::EndAudioFrame();
}

#ifdef _WIN32
int SDL_main(int argc, char** argv) {
#else
int main(int argc, char* argv[]) {
#endif
    bootlog("00 main start");
    GameEngine::Create(argc, argv);
    bootlog("01 Create done");
    alloc_pool();
    bootlog("02 alloc_pool done");
    audio_init();
    bootlog("03 audio_init done");
    sound_init();
    bootlog("04 sound_init done");
    thread5_game_loop();
    bootlog("05 thread5_game_loop done");
    bootflush(); // last SD write before the loop; the loop renders cleanly (AVI = truth)
    while (WindowIsRunning()) {
        push_frame();
    }
    GameEngine::Instance->Destroy();
    return 0;
}