//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>

// This has to be included early to prevent an obscure MSVC compile error
#include <boost/asio/deadline_timer.hpp>

#include <sdchain/protocol/JsonFields.h>
#include <sdchain/rpc/RPCHandler.h>
#include <sdchain/rpc/handlers/Handlers.h>

#include <sdchain/rpc/handlers/AccountCurrenciesHandler.cpp>
#include <sdchain/rpc/handlers/AccountInfo.cpp>
#include <sdchain/rpc/handlers/AccountLines.cpp>
#include <sdchain/rpc/handlers/AccountChannels.cpp>
#include <sdchain/rpc/handlers/AccountObjects.cpp>
#include <sdchain/rpc/handlers/AccountOffers.cpp>
#include <sdchain/rpc/handlers/AccountTx.cpp>
#include <sdchain/rpc/handlers/AccountTxOld.cpp>
#include <sdchain/rpc/handlers/AccountTxSwitch.cpp>
#include <sdchain/rpc/handlers/BlackList.cpp>
#include <sdchain/rpc/handlers/BookOffers.cpp>
#include <sdchain/rpc/handlers/CanDelete.cpp>
#include <sdchain/rpc/handlers/Connect.cpp>
#include <sdchain/rpc/handlers/ConsensusInfo.cpp>
#include <sdchain/rpc/handlers/Feature1.cpp>
#include <sdchain/rpc/handlers/Fee1.cpp>
#include <sdchain/rpc/handlers/FetchInfo.cpp>
#include <sdchain/rpc/handlers/GatewayBalances.cpp>
#include <sdchain/rpc/handlers/GetCounts.cpp>
#include <sdchain/rpc/handlers/LedgerHandler.cpp>
#include <sdchain/rpc/handlers/LedgerAccept.cpp>
#include <sdchain/rpc/handlers/LedgerCleanerHandler.cpp>
#include <sdchain/rpc/handlers/LedgerClosed.cpp>
#include <sdchain/rpc/handlers/LedgerCurrent.cpp>
#include <sdchain/rpc/handlers/LedgerData.cpp>
#include <sdchain/rpc/handlers/LedgerEntry.cpp>
#include <sdchain/rpc/handlers/LedgerHeader.cpp>
#include <sdchain/rpc/handlers/LedgerRequest.cpp>
#include <sdchain/rpc/handlers/LogLevel.cpp>
#include <sdchain/rpc/handlers/LogRotate.cpp>
#include <sdchain/rpc/handlers/NoSDChainCheck.cpp>
#include <sdchain/rpc/handlers/OwnerInfo.cpp>
