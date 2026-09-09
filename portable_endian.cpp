#include"portable_endian.h"
#include<cstdint>
unsigned int my_htole32(unsigned int x)
{
 #if ISO_BIG_ENDIAN
		return
 (
  ((x>>24)& 0x0000000FF) |
  ((x>>8)&  0x0000FF00)  |
  ((x<<8)&  0x00FF0000) |
  ((x<<24)&0xFF000000)
 );
 #else
	return x;
 #endif
}
