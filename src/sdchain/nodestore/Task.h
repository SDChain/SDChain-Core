//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_NODESTORE_TASK_H_INCLUDED
#define SDCHAIN_NODESTORE_TASK_H_INCLUDED

namespace sdchain {
namespace NodeStore {

/** Derived classes perform scheduled tasks. */
struct Task
{
    virtual ~Task() = default;

    /** Performs the task.
        The call may take place on a foreign thread.
    */
    virtual void performScheduledTask() = 0;
};

}
}

#endif
