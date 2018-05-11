//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_RESOURCE_DISPOSITION_H_INCLUDED
#define SDCHAIN_RESOURCE_DISPOSITION_H_INCLUDED

namespace sdchain {
namespace Resource {

/** The disposition of a consumer after applying a load charge. */
enum Disposition
{
    /** No action required. */
    ok

    /** Consumer should be warned that consumption is high. */
    ,warn

    /** Consumer should be disconnected for excess consumption. */
    ,drop
};

}
}

#endif
