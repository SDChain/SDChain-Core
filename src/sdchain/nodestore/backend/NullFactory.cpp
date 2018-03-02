//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/basics/contract.h>
#include <sdchain/nodestore/Factory.h>
#include <sdchain/nodestore/Manager.h>
#include <memory>

namespace sdchain {
namespace NodeStore {

class NullBackend : public Backend
{
public:
    NullBackend ()
    {
    }

    ~NullBackend ()
    {
    }

    std::string
    getName() override
    {
        return std::string ();
    }

    void
    close() override
    {
    }

    Status
    fetch (void const*, std::shared_ptr<NodeObject>*) override
    {
        return notFound;
    }

    bool
    canFetchBatch() override
    {
        return false;
    }

    std::vector<std::shared_ptr<NodeObject>>
    fetchBatch (std::size_t n, void const* const* keys) override
    {
        Throw<std::runtime_error> ("pure virtual called");
        return {};
    }

    void
    store (std::shared_ptr<NodeObject> const& object) override
    {
    }

    void
    storeBatch (Batch const& batch) override
    {
    }

    void
    for_each (std::function <void(std::shared_ptr<NodeObject>)> f) override
    {
    }

    int
    getWriteLoad () override
    {
        return 0;
    }

    void
    setDeletePath() override
    {
    }

    void
    verify() override
    {
    }

    /** Returns the number of file handles the backend expects to need */
    int
    fdlimit() const override
    {
        return 0;
    }

private:
};

//------------------------------------------------------------------------------

class NullFactory : public Factory
{
public:
    NullFactory()
    {
        Manager::instance().insert(*this);
    }

    ~NullFactory()
    {
        Manager::instance().erase(*this);
    }

    std::string
    getName () const
    {
        return "none";
    }

    std::unique_ptr <Backend>
    createInstance (
        size_t,
        Section const&,
        Scheduler&, beast::Journal)
    {
        return std::make_unique <NullBackend> ();
    }
};

static NullFactory nullFactory;

}
}
