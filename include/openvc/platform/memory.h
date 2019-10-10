#ifndef OPENVC_PLATFORM_MEMORY_H_
#define OPENVC_PLATFORM_MEMORY_H_

#ifdef __cplusplus
extern "C" {
#endif

void *vcPlatCAlloc(size_t aNum, size_t aSize);

void otPlatFree(void *aPtr);

#ifdef __cplusplus
}
#endif

#endif /* OPENVC_PLATFORM_MEMORY_H_ */
