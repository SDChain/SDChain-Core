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
#include <beast/core/string.hpp>
#include <map>
#include <memory>
#include <mutex>

namespace sdchain {
namespace NodeStore {

struct MemoryDB
{
    std::mutex mutex;
    bool open = false;
    std::map <uint256 const, std::shared_ptr<NodeObject>> table;
};

class MemoryFactory : public Factory
{
private:
    std::mutex mutex_;
    std::map <std::string, MemoryDB, beast::iless> map_;

public:
    MemoryFactory();
    ~MemoryFactory();

    std::string
    getName() const;

    std::unique_ptr <Backend>
    createInstance (
        size_t keyBytes,
        Section const& keyValues,
        Scheduler& scheduler,
        beast::Journal journal);

    MemoryDB&
    open (std::string const& path)
    {
        std::lock_guard<std::mutex> _(mutex_);
        auto const result = map_.emplace (std::piecewise_construct,
            std::make_tuple(path), std::make_tuple());
        MemoryDB& db = result.first->second;
        if (db.open)
            Throw<std::runtime_error> ("already open");
        return db;
    }
};

static MemoryFactory memoryFactory;

//------------------------------------------------------------------------------

class MemoryBackend : public Backend
{
private:
    using Map = std::map <uint256 const, std::shared_ptr<NodeObject>>;

    std::string name_;
    beast::Journal journal_;
    MemoryDB* db_;

public:
    MemoryBackend (size_t keyBytes, Section const& keyValues,
        Scheduler& scheduler, beast::Journal journal)
        : name_ (get<std::string>(keyValues, "path"))
        , journal_ (journal)
    {
        if (name_.empty())
            Throw<std::runtime_error> ("Missing path in Memory backend");
        db_ = &memoryFactory.open(name_);
    }

    ~MemoryBackend ()
    {
        close();
    }

    std::string
    getName () override
    {
        return name_;
    }

    void
    close() override
    {
        db_ = nullptr;
    }

    //--------------------------------------------------------------------------

    Status
    fetch (void const* key, std::shared_ptr<NodeObject>* pObject) override
    {
        uint256 const hash (uint256::fromVoid (key));

        std::lock_guard<std::mutex> _(db_->mutex);

        Map::iterator iter = db_->table.find (hash);
        if (iter == db_->table.end())
        {
            pObject->reset();
            return notFound;
        }
        *pObject = iter->second;
        return ok;
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
        std::lock_guard<std::mutex> _(db_->mutex);
        db_->table.emplace (object->getHash(), object);
    }

    void
    storeBatch (Batch const& batch) override
    {
        for (auto const& e : batch)
            store (e);
    }

    void
    for_each (std::function <void(std::shared_ptr<NodeObject>)> f) override
    {
        for (auto const& e : db_->table)
            f (e.second);
    }

    int
    getWriteLoad() override
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

    int
    fdlimit() const override
    {
        return 0;
    }
};

//------------------------------------------------------------------------------

MemoryFactory::MemoryFactory()
{
    Manager::instance().insert(*this);
}

MemoryFactory::~MemoryFactory()
{
    Manager::instance().erase(*this);
}

std::string
MemoryFactory::getName() const
{
    return "Memory";
}

std::unique_ptr <Backend>
MemoryFactory::createInstance (
    size_t keyBytes,
    Section const& keyValues,
    Scheduler& scheduler,
    beast::Journal journal)
{
    return std::make_unique <MemoryBackend> (
        keyBytes, keyValues, scheduler, journal);
}

}
}
