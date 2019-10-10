#ifndef PLATFORM_POSIX_H_
#define PLATFORM_POSIX_H_

#include <openvc-core-config.h>
#include <openvc/config.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#define POLL poll

#include <openvc/instance.h>

#include "openvc-core-config.h"

enum
{
    WELLKNOWN_NODE_ID = 34,
};

extern uint32_t gNodeId;

void platformAlarmInit(uint32_t aSpeedUpFactor);

void platformAlarmUpdateTimeout(struct timeval *aTimeout);

void platformAlarmProcess(vcInstance *aInstance);

int32_t platformAlarmGetNext(void);

uint64_t platformAlarmGetNow(void);

void platformAlarmAdvanceNow(uint64_t aDelta);

void platformRandomInit(void);

void platformUartUpdateFdSet(fd_set *aReadFdSet, fd_set *aWriteFdSet, fd_set *aErrorFdSet, int *aMaxFd);

void platformUartProcess(void);

void platformUartRestore(void);

#endif // PLATFORM_POSIX_H_
