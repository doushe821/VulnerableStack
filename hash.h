#ifndef DJB2_HASH_INCLUDED
#define DJB2_HASH_INCLUDED

#include <stdlib.h>
#include <math.h>

#ifndef NDEBUG
typedef u_int64_t hash_t;
#endif

hash_t hash(const void* data, size_t size);

#endif