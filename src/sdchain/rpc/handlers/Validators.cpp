//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2014 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/main/Application.h>
#include <sdchain/app/misc/ValidatorList.h>
#include <sdchain/rpc/Context.h>

namespace sdchain {

Json::Value
doValidators(RPC::Context& context)
{
    return context.app.validators().getJson();
}

}  // namespace sdchain
