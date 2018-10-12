//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_MAIN_NODEIDENTITY_H_INCLUDED
#define SDCHAIN_APP_MAIN_NODEIDENTITY_H_INCLUDED

#include <sdchain/app/main/Application.h>
#include <sdchain/protocol/PublicKey.h>
#include <sdchain/protocol/SecretKey.h>
#include <utility>

namespace sdchain {

/** The cryptographic credentials identifying this server instance. */
std::pair<PublicKey, SecretKey>
loadNodeIdentity (Application& app);

} // sdchain

#endif
