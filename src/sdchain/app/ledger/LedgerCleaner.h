//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_LEDGER_LEDGERCLEANER_H_INCLUDED
#define SDCHAIN_APP_LEDGER_LEDGERCLEANER_H_INCLUDED

#include <sdchain/app/main/Application.h>
#include <sdchain/json/json_value.h>
#include <sdchain/core/Stoppable.h>
#include <sdchain/beast/utility/PropertyStream.h>
#include <sdchain/beast/utility/Journal.h>
#include <memory>

namespace sdchain {
namespace detail {

/** Check the ledger/transaction databases to make sure they have continuity */
class LedgerCleaner
    : public Stoppable
    , public beast::PropertyStream::Source
{
protected:
    explicit LedgerCleaner (Stoppable& parent);

public:
    /** Destroy the object. */
    virtual ~LedgerCleaner () = 0;

    /** Start a long running task to clean the ledger.
        The ledger is cleaned asynchronously, on an implementation defined
        thread. This function call does not block. The long running task
        will be stopped if the Stoppable stops.

        Thread safety:
            Safe to call from any thread at any time.

        @param parameters A Json object with configurable parameters.
    */
    virtual void doClean (Json::Value const& parameters) = 0;
};

std::unique_ptr<LedgerCleaner>
make_LedgerCleaner (Application& app,
    Stoppable& parent, beast::Journal journal);

} // detail
} // sdchain

#endif
