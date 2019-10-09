#include <openvc/config.h>

#include "core/utils/heap.hpp"

#include <stdlib.h>

#include "common/debug.hpp"
#include "utils/wrap_string.h"

#include "test_platform.h"
#include "test_util.h"

/**
 * Verifies single variable allocating and freeing.
 *
 */
void TestAllocateSingle(void)
{
    vc::Utils::Heap heap;

    const size_t totalSize = heap.GetFreeSize();

    {
        void *p = heap.CAlloc(1, 0);
        VerifyOrQuit(p == NULL && totalSize == heap.GetFreeSize(), "TestAllocateSingle allocate 1 x 0 byte failed!\n");
        heap.Free(p);

        p = heap.CAlloc(0, 1);
        VerifyOrQuit(p == NULL && totalSize == heap.GetFreeSize(), "TestAllocateSingle allocate 0 x 1 byte failed!\n");
        heap.Free(p);
    }

    for (size_t size = 1; size <= heap.GetCapacity(); ++size)
    {
        Log("%s allocating %zu bytes...", __func__, size);
        void *p = heap.CAlloc(1, size);
        VerifyOrQuit(p != NULL && !heap.IsClean() && heap.GetFreeSize() + size <= totalSize, "allocating failed!\n");
        memset(p, 0xff, size);
        heap.Free(p);
        VerifyOrQuit(heap.IsClean() && heap.GetFreeSize() == totalSize, "freeing failed!\n");
    }
}

/**
 * Verifies randomly allocating and freeing variables.
 *
 * @param[in]   aSizeLimit  The maximum allocation size.
 * @param[in]   aSeed       The seed for generating random sizes.
 *
 */
void TestAllocateRandomly(size_t aSizeLimit, unsigned int aSeed)
{
    struct Node
    {
        Node * mNext;
        size_t mSize;
    };

    vc::Utils::Heap heap;
    Node            head;
    size_t          nnodes = 0;

    srand(aSeed);

    const size_t totalSize = heap.GetFreeSize();
    Node *       last      = &head;

    do
    {
        size_t size = sizeof(Node) + static_cast<size_t>(rand()) % aSizeLimit;
        Log("TestAllocateRandomly allocating %zu bytes...", size);
        last->mNext = static_cast<Node *>(heap.CAlloc(1, size));

        // No more memory for allocation.
        if (last->mNext == NULL)
        {
            break;
        }

        VerifyOrQuit(last->mNext->mNext == NULL, "TestAllocateRandomly memory not initialized to zero!\n");
        last        = last->mNext;
        last->mSize = size;
        ++nnodes;

        // 50% probability to randomly free a node.
        size_t freeIndex = static_cast<size_t>(rand()) % (nnodes * 2);

        if (freeIndex > nnodes)
        {
            freeIndex /= 2;

            Node *prev = &head;

            while (freeIndex--)
            {
                prev = prev->mNext;
            }

            Node *curr = prev->mNext;
            Log("TestAllocateRandomly freeing %zu bytes...", curr->mSize);
            prev->mNext = curr->mNext;
            heap.Free(curr);

            if (last == curr)
            {
                last = prev;
            }

            --nnodes;
        }
    } while (true);

    last = head.mNext;

    while (last)
    {
        Node *next = last->mNext;
        Log("TestAllocateRandomly freeing %zu bytes...", last->mSize);
        heap.Free(last);
        last = next;
    }

    VerifyOrQuit(heap.IsClean() && heap.GetFreeSize() == totalSize,
                 "TestAllocateRandomly heap not clean after freeing all!\n");
}

/**
 * Verifies allocating and free multiple variables.
 */
void TestAllocateMultiple(void)
{
    for (unsigned int seed = 0; seed < 10; ++seed)
    {
        size_t sizeLimit = (1 << seed);
        Log("TestAllocateRandomly(%zu, %u)...", sizeLimit, seed);
        TestAllocateRandomly(sizeLimit, seed);
    }
}

void RunTimerTests(void)
{
    TestAllocateSingle();
    TestAllocateMultiple();
}

#ifdef ENABLE_TEST_MAIN
int main(void)
{
    RunTimerTests();
    printf("All tests passed\n");
    return 0;
}
#endif
