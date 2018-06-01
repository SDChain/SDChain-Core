//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2014 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/tx/impl/CancelTicket.h>
#include <sdchain/basics/Log.h>
#include <sdchain/protocol/Feature.h>
#include <sdchain/protocol/Indexes.h>
#include <sdchain/ledger/View.h>

namespace sdchain {

TER
CancelTicket::preflight (PreflightContext const& ctx)
{
    if (! ctx.rules.enabled(featureTickets))
        return temDISABLED;

    auto const ret = preflight1 (ctx);
    if (!isTesSuccess (ret))
        return ret;

    return preflight2 (ctx);
}

TER
CancelTicket::doApply ()
{
    uint256 const ticketId = ctx_.tx.getFieldH256 (sfTicketID);

    // VFALCO This is highly suspicious, we're requiring that the
    //        transaction provide the return value of getTicketIndex?
    SLE::pointer sleTicket = view().peek (keylet::ticket(ticketId));

    if (!sleTicket)
        return tecNO_ENTRY;

    auto const ticket_owner =
        sleTicket->getAccountID (sfAccount);

    bool authorized =
        account_ == ticket_owner;

    // The target can also always remove a ticket
    if (!authorized && sleTicket->isFieldPresent (sfTarget))
        authorized = (account_ == sleTicket->getAccountID (sfTarget));

    // And finally, anyone can remove an expired ticket
    if (!authorized && sleTicket->isFieldPresent (sfExpiration))
    {
        using tp = NetClock::time_point;
        using d = tp::duration;
        auto const expiration = tp{d{sleTicket->getFieldU32(sfExpiration)}};

        if (view().parentCloseTime() >= expiration)
            authorized = true;
    }

    if (!authorized)
        return tecNO_PERMISSION;

    std::uint64_t const hint (sleTicket->getFieldU64 (sfOwnerNode));

    auto viewJ = ctx_.app.journal ("View");
    TER const result = dirDelete (ctx_.view (), false, hint,
        keylet::ownerDir (ticket_owner), ticketId, false, (hint == 0), viewJ);

    adjustOwnerCount(view(), view().peek(
        keylet::account(ticket_owner)), -1, viewJ);
    ctx_.view ().erase (sleTicket);

    return result;
}

}
