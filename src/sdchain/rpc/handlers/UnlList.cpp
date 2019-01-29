//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2014 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/main/Application.h>
#include <sdchain/app/misc/ValidatorList.h>
#include <sdchain/protocol/JsonFields.h>
#include <sdchain/rpc/Context.h>
#include <sdchain/basics/make_lock.h>

namespace sdchain {

Json::Value doUnlList (RPC::Context& context)
{
    auto lock = make_lock(context.app.getMasterMutex());
    Json::Value obj (Json::objectValue);

    context.app.validators().for_each_listed (
        [&unl = obj[jss::unl]](
            PublicKey const& publicKey,
            bool trusted)
        {
            Json::Value node (Json::objectValue);

            node[jss::pubkey_validator] = toBase58(
                TokenType::TOKEN_NODE_PUBLIC, publicKey);
            node[jss::trusted] = trusted;

            unl.append (node);
        });

    return obj;
}

} // sdchain
