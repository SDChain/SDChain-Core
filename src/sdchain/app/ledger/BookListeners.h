//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_LEDGER_BOOKLISTENERS_H_INCLUDED
#define SDCHAIN_APP_LEDGER_BOOKLISTENERS_H_INCLUDED

#include <sdchain/net/InfoSub.h>
#include <memory>
#include <mutex>

namespace sdchain {

/** Listen to public/subscribe messages from a book. */
class BookListeners
{
public:
    using pointer = std::shared_ptr<BookListeners>;

    BookListeners()
    {
    }

    /** Add a new subscription for this book
    */
    void
    addSubscriber(InfoSub::ref sub);

    /** Stop publishing to a subscriber
    */
    void
    removeSubscriber(std::uint64_t sub);

    /** Publish a transaction to subscribers

        Publish a transaction to clients subscribed to changes on this book.
        Uses havePublished to prevent sending duplicate transactions to clients
        that have subscribed to multiple books.

        @param jvObj JSON transaction data to publish
        @param havePublished InfoSub sequence numbers that have already
                             published this transaction.

    */
    void
    publish(Json::Value const& jvObj, hash_set<std::uint64_t>& havePublished);

private:
    std::recursive_mutex mLock;

    hash_map<std::uint64_t, InfoSub::wptr> mListeners;
};

}  // namespace sdchain

#endif
