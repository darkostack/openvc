#include "tasklet.hpp"

#include "common/code_utils.hpp"
#include "common/debug.hpp"
#include "common/instance.hpp"

namespace vc {

Tasklet::Tasklet(Instance &aInstance, Handler aHandler, void *aOwner)
    : InstanceLocator(aInstance)
    , OwnerLocator(aOwner)
    , mHandler(aHandler)
    , mNext(NULL)
{
}

vcError Tasklet::Post(void)
{
    return GetInstance().GetTaskletScheduler().Post(*this);
}

TaskletScheduler::TaskletScheduler(void)
    : mHead(NULL)
    , mTail(NULL)
{
}

vcError TaskletScheduler::Post(Tasklet &aTasklet)
{
    vcError error = VC_ERROR_NONE;

    VerifyOrExit(mTail != &aTasklet && aTasklet.mNext == NULL, error = VC_ERROR_ALREADY);

    VerifyOrExit(&aTasklet.GetInstance().Get<TaskletScheduler>() == this);

    if (mTail == NULL)
    {
        mHead = &aTasklet;
        mTail = &aTasklet;
        vcTaskletsSignalPending(&aTasklet.GetInstance());
    }
    else
    {
        mTail->mNext = &aTasklet;
        mTail        = &aTasklet;
    }

exit:
    return error;
}

Tasklet *TaskletScheduler::PopTasklet(void)
{
    Tasklet *task = mHead;

    if (task != NULL)
    {
        mHead = mHead->mNext;

        if (mHead == NULL)
        {
            mTail = NULL;
        }

        task->mNext = NULL;
    }

    return task;
}

void TaskletScheduler::ProcessQueuedTasklets(void)
{
    Tasklet *tail = mTail;
    Tasklet *cur;

    while ((cur = PopTasklet()) != NULL)
    {
        cur->RunTask();

        // only process tasklets that were queued at the time this method was called
        if (cur == tail)
        {
            if (mHead != NULL)
            {
                vcTaskletsSignalPending(&mHead->GetInstance());
            }

            break;
        }
    }
}

} // namespace vc
