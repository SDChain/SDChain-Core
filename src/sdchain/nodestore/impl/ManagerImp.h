//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_NODESTORE_MANAGERIMP_H_INCLUDED
#define SDCHAIN_NODESTORE_MANAGERIMP_H_INCLUDED

#include <sdchain/nodestore/Manager.h>

namespace sdchain {
namespace NodeStore {

class ManagerImp : public Manager
{
private:
    std::mutex mutex_;
    std::vector<Factory*> list_;

public:
    static
    ManagerImp&
    instance();

    static
    void
    missing_backend();

    ManagerImp();

    ~ManagerImp();

    Factory*
    find (std::string const& name);

    void
    insert (Factory& factory) override;

    void
    erase (Factory& factory) override;

    std::unique_ptr <Backend>
    make_Backend (
        Section const& parameters,
        Scheduler& scheduler,
        beast::Journal journal) override;

    std::unique_ptr <Database>
    make_Database (
        std::string const& name,
        Scheduler& scheduler,
        int readThreads,
        Stoppable& parent,
        Section const& backendParameters,
        beast::Journal journal) override;

    std::unique_ptr <DatabaseRotating>
    make_DatabaseRotating (
        std::string const& name,
        Scheduler& scheduler,
        std::int32_t readThreads,
        Stoppable& parent,
        std::shared_ptr <Backend> writableBackend,
        std::shared_ptr <Backend> archiveBackend,
        beast::Journal journal) override;
};

}
}

#endif
