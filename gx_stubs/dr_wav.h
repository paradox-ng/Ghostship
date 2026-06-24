#pragma once
// Stub: no WAV decoder on console. init returns failure so the factory bails.
#include <stdint.h>
#include <stddef.h>
typedef int16_t drwav_int16;
typedef uint64_t drwav_uint64;
typedef struct {
    uint32_t channels;
    uint32_t sampleRate;
    uint32_t bitsPerSample;
    drwav_uint64 totalPCMFrameCount;
} drwav;
static inline int drwav_init_memory(drwav* w, const void* d, size_t n, const void* a) {
    (void)d; (void)n; (void)a;
    if (w) { w->channels = 0; w->sampleRate = 0; w->bitsPerSample = 0; w->totalPCMFrameCount = 0; }
    return 0;
}
static inline drwav_uint64 drwav_read_pcm_frames_s16(drwav* w, drwav_uint64 f, drwav_int16* o) {
    (void)w; (void)f; (void)o; return 0;
}
static inline void drwav_uninit(drwav* w) { (void)w; }
