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

#include <sdchain/rpc/handlers/PathFind.cpp>
#include <sdchain/rpc/handlers/PayChanClaim.cpp>
#include <sdchain/rpc/handlers/Peers.cpp>
#include <sdchain/rpc/handlers/Ping.cpp>
#include <sdchain/rpc/handlers/Print.cpp>
#include <sdchain/rpc/handlers/Random.cpp>
#include <sdchain/rpc/handlers/SDChainPathFind.cpp>
#include <sdchain/rpc/handlers/ServerInfo.cpp>
#include <sdchain/rpc/handlers/ServerState.cpp>
#include <sdchain/rpc/handlers/SignFor.cpp>
#include <sdchain/rpc/handlers/SignHandler.cpp>
#include <sdchain/rpc/handlers/Stop.cpp>
#include <sdchain/rpc/handlers/Submit.cpp>
#include <sdchain/rpc/handlers/SubmitMultiSigned.cpp>
#include <sdchain/rpc/handlers/Subscribe.cpp>
#include <sdchain/rpc/handlers/TransactionEntry.cpp>
#include <sdchain/rpc/handlers/Tx.cpp>
#include <sdchain/rpc/handlers/TxHistory.cpp>
#include <sdchain/rpc/handlers/UnlList.cpp>
#include <sdchain/rpc/handlers/Unsubscribe.cpp>
#include <sdchain/rpc/handlers/ValidationCreate.cpp>
#include <sdchain/rpc/handlers/ValidationSeed.cpp>
#include <sdchain/rpc/handlers/Validators.cpp>
#include <sdchain/rpc/handlers/ValidatorListSites.cpp>
#include <sdchain/rpc/handlers/WalletPropose.cpp>
#include <sdchain/rpc/handlers/WalletSeed.cpp>

#include <sdchain/rpc/impl/Handler.cpp>
#include <sdchain/rpc/impl/LegacyPathFind.cpp>
#include <sdchain/rpc/impl/Role.cpp>
#include <sdchain/rpc/impl/RPCHandler.cpp>
#include <sdchain/rpc/impl/RPCHelpers.cpp>
#include <sdchain/rpc/impl/ServerHandlerImp.cpp>
#include <sdchain/rpc/impl/Status.cpp>
#include <sdchain/rpc/impl/TransactionSign.cpp>


