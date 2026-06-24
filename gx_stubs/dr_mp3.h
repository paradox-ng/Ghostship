#pragma once
// Stub: no MP3 decoder on console.
#include <stdint.h>
#include <stddef.h>
typedef int16_t drmp3_int16;
typedef uint64_t drmp3_uint64;
typedef struct { uint32_t channels; uint32_t sampleRate; } drmp3_config;
typedef struct { uint32_t channels; uint32_t sampleRate; } drmp3;
static inline int drmp3_init_memory(drmp3* m, const void* d, size_t n, const void* a) {
    (void)d; (void)n; (void)a;
    if (m) { m->channels = 0; m->sampleRate = 0; }
    return 0;
}
static inline void drmp3_uninit(drmp3* m) { (void)m; }
static inline drmp3_uint64 drmp3_get_pcm_frame_count(drmp3* m) { (void)m; return 0; }
static inline drmp3_uint64 drmp3_read_pcm_frames_s16(drmp3* m, drmp3_uint64 f, drmp3_int16* o) {
    (void)m; (void)f; (void)o; return 0;
}
static inline drmp3_int16* drmp3_open_memory_and_read_pcm_frames_s16(const void* d, size_t n, drmp3_config* c,
                                                                     drmp3_uint64* tf, const void* a) {
    (void)d; (void)n; (void)a;
    if (c) { c->channels = 0; c->sampleRate = 0; }
    if (tf) { *tf = 0; }
    return 0;
}
static inline drmp3_int16* drmp3_open_file_and_read_pcm_frames_s16(const char* p, drmp3_config* c, drmp3_uint64* tf,
                                                                   const void* a) {
    (void)p; (void)a;
    if (c) { c->channels = 0; c->sampleRate = 0; }
    if (tf) { *tf = 0; }
    return 0;
}
static inline void drmp3_free(void* p, const void* a) { (void)p; (void)a; }
