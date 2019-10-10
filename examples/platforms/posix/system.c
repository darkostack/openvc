#include "platform-posix.h"

#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <libgen.h>
#include <syslog.h>

#include <openvc/tasklet.h>
#include <openvc/platform/alarm-milli.h>

uint32_t gNodeId = 1;

extern bool gPlatformPseudoResetWasRequested;

static volatile bool gTerminate = false;

static void handleSignal(int aSignal)
{
    VC_UNUSED_VARIABLE(aSignal);

    gTerminate = true;
}

void vcSysInit(int aArgCount, char *aArgVector[])
{
    char *   endptr;
    uint32_t speedUpFactor = 1;

    if (gPlatformPseudoResetWasRequested)
    {
        gPlatformPseudoResetWasRequested = false;
        return;
    }

    if (aArgCount < 2)
    {
        fprintf(stderr, "Syntax:\n    %s NodeId [TimeSpeedUpFactor]\n", aArgVector[0]);
        exit(EXIT_FAILURE);
    }

    openlog(basename(aArgVector[0]), LOG_PID, LOG_USER);
    setlogmask(setlogmask(0) & LOG_UPTO(LOG_NOTICE));

    signal(SIGTERM, &handleSignal);
    signal(SIGHUP, &handleSignal);

    gNodeId = (uint32_t)strtol(aArgVector[1], &endptr, 0);

    if (*endptr != '\0' || gNodeId < 1 || gNodeId >= WELLKNOWN_NODE_ID)
    {
        fprintf(stderr, "Invalid NodeId: %s\n", aArgVector[1]);
        exit(EXIT_FAILURE);
    }

    if (aArgCount > 2)
    {
        speedUpFactor = (uint32_t)strtol(aArgVector[2], &endptr, 0);

        if (*endptr != '\0' || speedUpFactor == 0)
        {
            fprintf(stderr, "Invalid value for TimerSpeedUpFactor: %s\n", aArgVector[2]);
            exit(EXIT_FAILURE);
        }
    }

    platformAlarmInit(speedUpFactor);
    platformRandomInit();
}

bool vcSysPseudoResetWasRequested(void)
{
    return gPlatformPseudoResetWasRequested;
}

void vcSysDeInit(void)
{
}

void vcSysProcessDrivers(vcInstance *aInstance)
{
    fd_set         read_fds;
    fd_set         write_fds;
    fd_set         error_fds;
    int            max_fd = -1;
    struct timeval timeout;
    int            rval;

    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_ZERO(&error_fds);

    platformUartUpdateFdSet(&read_fds, &write_fds, &error_fds, &max_fd);
    platformAlarmUpdateTimeout(&timeout);

    if (vcTaskletsArePending(aInstance))
    {
        timeout.tv_sec  = 0;
        timeout.tv_usec = 0;
    }

    rval = select(max_fd + 1, &read_fds, &write_fds, &error_fds, &timeout);

    if (rval >= 0)
    {
        platformUartProcess();
    }
    else if (errno != EINTR)
    {
        perror("select");
        exit(EXIT_FAILURE);
    }

    platformAlarmProcess(aInstance);

    if (gTerminate)
    {
        exit(0);
    }
}
