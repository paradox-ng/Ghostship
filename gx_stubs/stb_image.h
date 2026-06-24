#pragma once
// Stub: no PNG/image decoder on console. load returns null so the factory bails.
#include <stddef.h>
static inline unsigned char* stbi_load_from_memory(const unsigned char* b, int n, int* x, int* y, int* ch, int want) {
    (void)b; (void)n; (void)ch; (void)want; if (x) { *x = 0; } if (y) { *y = 0; } return 0;
}
static inline const char* stbi_failure_reason(void) { return "stb_image stubbed (no decoder on console)"; }
