#pragma once
// Stub: no FLAC decoder on console.
#include <stdint.h>
#include <stddef.h>
typedef int16_t drflac_int16;
typedef uint64_t drflac_uint64;
typedef struct {
    uint32_t channels;
    uint32_t sampleRate;
    drflac_uint64 totalPCMFrameCount;
} drflac;
static inline drflac* drflac_open_memory(const void* d, size_t n, const void* a) {
    (void)d; (void)n; (void)a; return 0;
}
static inline drflac_uint64 drflac_read_pcm_frames_s16(drflac* f, drflac_uint64 c, drflac_int16* o) {
    (void)f; (void)c; (void)o; return 0;
}
static inline void drflac_close(drflac* f) { (void)f; }
