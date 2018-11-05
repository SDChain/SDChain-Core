//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_RPC_HANDLERS_LEDGER_H_INCLUDED
#define SDCHAIN_RPC_HANDLERS_LEDGER_H_INCLUDED

#include <sdchain/app/main/Application.h>
#include <sdchain/app/ledger/LedgerToJson.h>
#include <sdchain/app/ledger/LedgerMaster.h>
#include <sdchain/ledger/ReadView.h>
#include <sdchain/json/Object.h>
#include <sdchain/protocol/JsonFields.h>
#include <sdchain/rpc/Context.h>
#include <sdchain/rpc/Status.h>
#include <sdchain/rpc/impl/Handler.h>
#include <sdchain/rpc/Role.h>

namespace Json {
class Object;
}

namespace sdchain {
namespace RPC {

struct Context;

// ledger [id|index|current|closed] [full]
// {
//    ledger: 'current' | 'closed' | <uint256> | <number>,  // optional
//    full: true | false    // optional, defaults to false.
// }

class LedgerHandler {
public:
    explicit LedgerHandler (Context&);

    Status check ();

    template <class Object>
    void writeResult (Object&);

    static const char* const name()
    {
        return "ledger";
    }

    static Role role()
    {
        return Role::USER;
    }

    static Condition condition()
    {
        return NO_CONDITION;
    }

private:
    Context& context_;
    std::shared_ptr<ReadView const> ledger_;
    std::vector<TxQ::TxDetails> queueTxs_;
    Json::Value result_;
    int options_ = 0;
    LedgerEntryType type_;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
// Implementation.

template <class Object>
void LedgerHandler::writeResult (Object& value)
{
    if (ledger_)
    {
        Json::copyFrom (value, result_);
        addJson (value, {*ledger_, options_, queueTxs_, type_});
    }
    else
    {
        auto& master = context_.app.getLedgerMaster ();
        {
            auto&& closed = Json::addObject (value, jss::closed);
            addJson (closed, {*master.getClosedLedger(), 0});
        }
        {
            auto&& open = Json::addObject (value, jss::open);
            addJson (open, {*master.getCurrentLedger(), 0});
        }
    }
}

} // RPC
} // sdchain

#endif
