//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2017 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_MISC_VALIDATOR_KEYS_H_INCLUDED
#define SDCHAIN_APP_MISC_VALIDATOR_KEYS_H_INCLUDED

#include <sdchain/beast/utility/Journal.h>
#include <sdchain/protocol/PublicKey.h>
#include <sdchain/protocol/SecretKey.h>
#include <string>

namespace sdchain {

class Config;

/** Validator keys and manifest as set in configuration file.  Values will be
    empty if not configured as a validator or not configured with a manifest.
*/
class ValidatorKeys
{
public:
    PublicKey publicKey;
    SecretKey secretKey;
    std::string manifest;
    ValidatorKeys(Config const& config, beast::Journal j);

    bool configInvalid() const
    {
        return configInvalid_;
    }

private:
    bool configInvalid_ = false; //< Set to true if config was invalid
};

}  // namespace sdchain

#endif
