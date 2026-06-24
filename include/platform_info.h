#ifndef PLATFORM_INFO_H
#define PLATFORM_INFO_H

#include <stdint.h>
#define IS_64_BIT (UINTPTR_MAX == 0xFFFFFFFFFFFFFFFFU)
#if (defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)) || defined(__BIG_ENDIAN__)
// Define with a value (not just presence): the audio engine uses `#if IS_BIG_ENDIAN`,
// which on a big-endian target would otherwise be `#if` with an empty token. Desktop
// ports are little-endian and never define this, so they never hit that.
#define IS_BIG_ENDIAN 1
#endif
#define DOUBLE_SIZE_ON_64_BIT(size) ((size) * (sizeof(void *) / 2))

#endif // PLATFORM_INFO_H
