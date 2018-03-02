//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_NODESTORE_SCHEDULER_H_INCLUDED
#define SDCHAIN_NODESTORE_SCHEDULER_H_INCLUDED

#include <sdchain/nodestore/Task.h>
#include <chrono>

namespace sdchain {
namespace NodeStore {

/** Contains information about a fetch operation. */
struct FetchReport
{
    std::chrono::milliseconds elapsed;
    bool isAsync;
    bool wentToDisk;
    bool wasFound;
};

/** Contains information about a batch write operation. */
struct BatchWriteReport
{
    std::chrono::milliseconds elapsed;
    int writeCount;
};

/** Scheduling for asynchronous backend activity

    For improved performance, a backend has the option of performing writes
    in batches. These writes can be scheduled using the provided scheduler
    object.

    @see BatchWriter
*/
class Scheduler
{
public:
    virtual ~Scheduler() = default;

    /** Schedules a task.
        Depending on the implementation, the task may be invoked either on
        the current thread of execution, or an unspecified implementation-defined
        foreign thread.
    */
    virtual void scheduleTask (Task& task) = 0;

    /** Reports completion of a fetch
        Allows the scheduler to monitor the node store's performance
    */
    virtual void onFetch (FetchReport const& report) = 0;

    /** Reports the completion of a batch write
        Allows the scheduler to monitor the node store's performance
    */
    virtual void onBatchWrite (BatchWriteReport const& report) = 0;
};

}
}

#endif
