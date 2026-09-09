#pragma once
#ifndef MY_HTOLE
#define MY_HTOLE
#include<cstdint>
#if defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__) && \
    (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#define ISO_BIG_ENDIAN 1
#else
#define ISO_BIG_ENDIAN 0
#endif
unsigned int my_htole32(unsigned int x);
#endif
