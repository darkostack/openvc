#ifndef PERIPH_CPUID_H
#define PERIPH_CPUID_H

#ifdef __cplusplus
extern "C" {
#endif

#if CPUID_LEN
void cpuid_get(void *id);
#endif

#ifdef __cplusplus
}
#endif

#endif /* PERIPH_CPUID_H */
