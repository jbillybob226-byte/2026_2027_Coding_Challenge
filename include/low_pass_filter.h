#ifndef LOW_PASS_FILTER_H
#define LOW_PASS_FILTER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <float.h>
#include <limits.h>

typedef float fp32;

#ifdef __cplusplus
static_assert(sizeof(fp32) * CHAR_BIT == 32 && FLT_RADIX == 2 &&
              FLT_MANT_DIG == 24 && FLT_MAX_EXP == 128,
              "fp32 requires a 32-bit binary float");
#else
_Static_assert(sizeof(fp32) * CHAR_BIT == 32 && FLT_RADIX == 2 &&
               FLT_MANT_DIG == 24 && FLT_MAX_EXP == 128,
               "fp32 requires a 32-bit binary float");
#endif

/* Stateful, non-reentrant filter; init == 1 reseeds the output. */
fp32 low_pass_filter(fp32 x, fp32 alpha, int init);

#ifdef __cplusplus
}
#endif
#endif
