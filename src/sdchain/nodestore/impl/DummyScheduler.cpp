//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/nodestore/DummyScheduler.h>

namespace sdchain {
namespace NodeStore {

DummyScheduler::DummyScheduler ()
{
}

DummyScheduler::~DummyScheduler ()
{
}

void
DummyScheduler::scheduleTask (Task& task)
{
    // Invoke the task synchronously.
    task.performScheduledTask();
}

void
DummyScheduler::scheduledTasksStopped ()
{
}

void
DummyScheduler::onFetch (const FetchReport& report)
{
}

void
DummyScheduler::onBatchWrite (const BatchWriteReport& report)
{
}

}
}
