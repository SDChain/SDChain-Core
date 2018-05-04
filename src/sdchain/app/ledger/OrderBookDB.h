//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_LEDGER_ORDERBOOKDB_H_INCLUDED
#define SDCHAIN_APP_LEDGER_ORDERBOOKDB_H_INCLUDED

#include <sdchain/app/ledger/AcceptedLedgerTx.h>
#include <sdchain/app/ledger/BookListeners.h>
#include <sdchain/app/main/Application.h>
#include <sdchain/app/misc/OrderBook.h>
#include <mutex>

namespace sdchain {

class OrderBookDB
    : public Stoppable
{
public:
    OrderBookDB (Application& app, Stoppable& parent);

    void setup (std::shared_ptr<ReadView const> const& ledger);
    void update (std::shared_ptr<ReadView const> const& ledger);
    void invalidate ();

    void addOrderBook(Book const&);

    /** @return a list of all orderbooks that want this issuerID and currencyID.
     */
    OrderBook::List getBooksByTakerPays (Issue const&);

    /** @return a count of all orderbooks that want this issuerID and
        currencyID. */
    int getBookSize(Issue const&);

    bool isBookToSDA (Issue const&);

    BookListeners::pointer getBookListeners (Book const&);
    BookListeners::pointer makeBookListeners (Book const&);

    // see if this txn effects any orderbook
    void processTxn (
        std::shared_ptr<ReadView const> const& ledger,
        const AcceptedLedgerTx& alTx, Json::Value const& jvObj);

    using IssueToOrderBook = hash_map <Issue, OrderBook::List>;

private:
    void rawAddBook(Book const&);

    Application& app_;

    // by ci/ii
    IssueToOrderBook mSourceMap;

    // by co/io
    IssueToOrderBook mDestMap;

    // does an order book to SDA exist
    hash_set <Issue> mSDABooks;

    std::recursive_mutex mLock;

    using BookToListenersMap = hash_map <Book, BookListeners::pointer>;

    BookToListenersMap mListeners;

    std::uint32_t mSeq;

    beast::Journal j_;
};

} // sdchain

#endif
