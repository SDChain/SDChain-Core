//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/misc/ValidatorKeys.h>

#include <sdchain/app/misc/Manifest.h>
#include <sdchain/basics/Log.h>
#include <sdchain/core/Config.h>
#include <sdchain/core/ConfigSections.h>
#include <beast/core/detail/base64.hpp>

namespace sdchain {
ValidatorKeys::ValidatorKeys(Config const& config, beast::Journal j)
{
    if (config.exists(SECTION_VALIDATOR_TOKEN) &&
        config.exists(SECTION_VALIDATION_SEED))
    {
        configInvalid_ = true;
        JLOG(j.fatal()) << "Cannot specify both [" SECTION_VALIDATION_SEED
                           "] and [" SECTION_VALIDATOR_TOKEN "]";
        return;
    }

    if (config.exists(SECTION_VALIDATOR_TOKEN))
    {
        if (auto const token = ValidatorToken::make_ValidatorToken(
                config.section(SECTION_VALIDATOR_TOKEN).lines()))
        {
            auto const pk = derivePublicKey(
                KeyType::secp256k1, token->validationSecret);
            auto const m = Manifest::make_Manifest(
                beast::detail::base64_decode(token->manifest));

            if (! m || pk != m->signingKey)
            {
                configInvalid_ = true;
                JLOG(j.fatal())
                    << "Invalid token specified in [" SECTION_VALIDATOR_TOKEN "]";
            }
            else
            {
                secretKey = token->validationSecret;
                publicKey = pk;
                manifest = std::move(token->manifest);
            }
        }
        else
        {
            configInvalid_ = true;
            JLOG(j.fatal())
                << "Invalid token specified in [" SECTION_VALIDATOR_TOKEN "]";
        }
    }
    else if (config.exists(SECTION_VALIDATION_SEED))
    {
        auto const seed = parseBase58<Seed>(
            config.section(SECTION_VALIDATION_SEED).lines().front());
        if (!seed)
        {
            configInvalid_ = true;
            JLOG(j.fatal()) <<
                "Invalid seed specified in [" SECTION_VALIDATION_SEED "]";
        }
        else
        {
            secretKey = generateSecretKey(KeyType::secp256k1, *seed);
            publicKey = derivePublicKey(KeyType::secp256k1, secretKey);
        }
    }
}
}  // namespace sdchain
