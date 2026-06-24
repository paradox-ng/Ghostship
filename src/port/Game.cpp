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
    // Dolphin's emulated SD write path deadlocks after ~24 fopen/write/close cycles,
    // and bootflush runs on the main thread, so an over-budget flush freezes the game
    // itself. Cap the number of real flushes so logging can never be the thing that
    // hangs; later checkpoints just accumulate in RAM (visible if we lower the cap).
    static int flushes = 0;
    if (flushes >= 18) {
        return;
    }
    flushes++;
    FILE* f = fopen("sd:/ghostship_boot.log", "w");
    if (f) {
        fwrite(g_bootbuf, 1, g_bootlen, f);
        fflush(f);
        fclose(f);
    }
}

// Bring-up toggle: 0 disables audio synthesis + skips audio resource loading (fast
// init for the gdb stub, to isolate the render path).
int g_audio_enabled = 0;

// BISECT: 1 = skip all GX triangle submission (read by the GX backend). Lets us test
// whether the frame-1 render hang is in the triangle/GP path vs. elsewhere.
extern "C" { int g_gx_skip_draw = 0; }
// BISECT: 1 = set GX state per draw but submit NO geometry (GX_Begin/verts/GX_End).
extern "C" { int g_gx_skip_geom = 0; }
// BISECT: stop DrawTriangles after stage N (1=TEV 2=+vtxdesc 3=+mtx 4=+chan 5=full).
extern "C" { int g_gx_stop_at = 5; }

void alloc_pool() {
    // Console fit: the desktop pool is 32 MB (u64[4M]), which alone overruns the
    // GameCube's 24 MB (and Wii MEM1). Use 8 MB so the whole static image fits in
    // MEM1 with room for the heap/stack. Tunable; large levels may need more.
    static u64 pool[512 * 1024]; // 4 MB (reduced to leave MEM1 heap for the in-RAM o2r)
    main_pool_init(pool, pool + sizeof(pool) / sizeof(pool[0]));
    gEffectsMemoryPool = mem_pool_init(0x4000, MEMORY_POOL_LEFT);
}

extern "C" void exec_display_list(SPTask* spTask) {
    static int e = 0;
    bool t = false; // BISECT: fine trace off so the frame heartbeat keeps the flush budget
    if (t) { char b[24]; snprintf(b, sizeof(b), "ex%d enter", e); bootlog(b); bootflush(); }
    GameEngine::ProcessGfxCommands((Gfx*)spTask->task.t.data_ptr);
    if (t) { char b[24]; snprintf(b, sizeof(b), "ex%d done", e); bootlog(b); bootflush(); }
    e++;
}

void push_frame() {
    // Frame-loop bring-up trace: flush each sub-step for the first 3 frames so the
    // last SD line pinpoints which call hangs (init logging is RAM-only to leave the
    // libfat write budget for this).
    static int pf = 0;
    bool ft = false; // off; DrawTriangles per-call trace gets the whole flush budget
    char b[28];
    if (ft) { snprintf(b, sizeof(b), "f%d StartAudioFrame", pf); bootlog(b); bootflush(); }
    GameEngine::StartAudioFrame();
    if (ft) { snprintf(b, sizeof(b), "f%d StartFrame", pf); bootlog(b); bootflush(); }
    GameEngine::Instance->StartFrame();
    if (ft) { snprintf(b, sizeof(b), "f%d thread5_iter", pf); bootlog(b); bootflush(); }
    thread5_iteration();
    if (ft) { snprintf(b, sizeof(b), "f%d EndAudioFrame", pf); bootlog(b); bootflush(); }
    GameEngine::EndAudioFrame();
    pf++;
}

#ifdef _WIN32
int SDL_main(int argc, char** argv) {
#else
int main(int argc, char* argv[]) {
#endif
    // Init logs accumulate in RAM only (no SD writes) so the libfat write-deadlock
    // budget is spent entirely on the frame loop below.
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
    bootflush();
    int frame = 0;
    while (WindowIsRunning()) {
        push_frame();
        frame++;
        if (frame % 10 == 0) { // minimal SD logging to avoid the libfat hang
            char b[24];
            snprintf(b, sizeof(b), "frame %d done", frame);
            bootlog(b);
            bootflush();
        }
    }
    GameEngine::Instance->Destroy();
    return 0;
}