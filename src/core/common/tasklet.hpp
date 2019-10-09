#ifndef TASKLET_HPP_
#define TASKLET_HPP_

#include "openvc-core-config.h"

#include <stdio.h>

#include <openvc/error.h>
#include <openvc/tasklet.h>

#include "common/locator.hpp"

namespace vc {

class TaskletScheduler;

class Tasklet : public InstanceLocator, public OwnerLocator
{
    friend class TaskletScheduler;

public:
    typedef void (*Handler)(Tasklet &aTasklet);

    Tasklet(Instance &aInstance, Handler aHandler, void *aOwner);

    vcError Post(void);

private:
    void RunTask(void) { mHandler(*this); }

    Handler  mHandler;
    Tasklet *mNext;
};

class TaskletScheduler
{
public:
    TaskletScheduler(void);

    vcError Post(Tasklet &aTasklet);

    bool AreTaskletsPending(void) { return mHead != NULL; }

    void ProcessQueuedTasklets(void);

private:
    Tasklet *PopTasklet(void);
    Tasklet *mHead;
    Tasklet *mTail;
};

} // namespace vc

#endif // TASKLET_HPP_
