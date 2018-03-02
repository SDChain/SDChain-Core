//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_NODESTORE_DUMMYSCHEDULER_H_INCLUDED
#define SDCHAIN_NODESTORE_DUMMYSCHEDULER_H_INCLUDED

#include <sdchain/nodestore/Scheduler.h>

namespace sdchain {
namespace NodeStore {

/** Simple NodeStore Scheduler that just peforms the tasks synchronously. */
class DummyScheduler : public Scheduler
{
public:
    DummyScheduler ();
    ~DummyScheduler ();
    void scheduleTask (Task& task) override;
    void scheduledTasksStopped ();
    void onFetch (FetchReport const& report) override;
    void onBatchWrite (BatchWriteReport const& report) override;
};

}
}

#endif
