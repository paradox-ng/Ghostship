#pragma once
// Stub: no Vorbis decoder on console.
#include <stddef.h>
typedef struct { unsigned int sample_rate; int channels; } stb_vorbis_info;
typedef struct stb_vorbis stb_vorbis;
static inline stb_vorbis* stb_vorbis_open_memory(const unsigned char* d, int n, int* err, const void* a) {
    (void)d; (void)n; (void)a; if (err) { *err = 1; } return 0;
}
static inline stb_vorbis_info stb_vorbis_get_info(stb_vorbis* v) { (void)v; stb_vorbis_info i = {0, 0}; return i; }
static inline unsigned int stb_vorbis_stream_length_in_samples(stb_vorbis* v) { (void)v; return 0; }
static inline void stb_vorbis_close(stb_vorbis* v) { (void)v; }
static inline int stb_vorbis_decode_memory(const unsigned char* d, int n, int* ch, int* sr, short** out) {
    (void)d; (void)n; if (ch) { *ch = 0; } if (sr) { *sr = 0; } if (out) { *out = 0; } return -1;
}
