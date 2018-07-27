//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/paths/cursor/SDChainLiquidity.h>
#include <sdchain/basics/Log.h>
#include <tuple>

namespace sdchain {
namespace path {

TER PathCursor::liquidity () const
{
    TER resultCode = tecPATH_DRY;
    PathCursor pc = *this;

    pathState_.resetView (sdchainCalc_.view);

    for (pc.nodeIndex_ = pc.nodeSize(); pc.nodeIndex_--; )
    {
        JLOG (j_.trace())
            << "reverseLiquidity>"
            << " nodeIndex=" << pc.nodeIndex_
            << ".issue_.account=" << to_string (pc.node().issue_.account);

        resultCode = pc.reverseLiquidity();

        if (!pc.node().transferRate_)
            return tefINTERNAL;

        JLOG (j_.trace())
            << "reverseLiquidity< "
            << "nodeIndex=" << pc.nodeIndex_
            << " resultCode=" << transToken (resultCode)
            << " transferRate_=" << *pc.node().transferRate_
            << ": " << resultCode;

        if (resultCode != tesSUCCESS)
            break;
    }

    // VFALCO-FIXME this generates errors
    // JLOG (j_.trace())
    //     << "nextIncrement: Path after reverse: " << pathState_.getJson ();

    if (resultCode != tesSUCCESS)
        return resultCode;

    pathState_.resetView (sdchainCalc_.view);

    for (pc.nodeIndex_ = 0; pc.nodeIndex_ < pc.nodeSize(); ++pc.nodeIndex_)
    {
        JLOG (j_.trace())
            << "forwardLiquidity> nodeIndex=" << nodeIndex_;

        resultCode = pc.forwardLiquidity();
        if (resultCode != tesSUCCESS)
            return resultCode;

        JLOG (j_.trace())
            << "forwardLiquidity<"
            << " nodeIndex:" << pc.nodeIndex_
            << " resultCode:" << resultCode;

        if (pathState_.isDry())
            resultCode = tecPATH_DRY;
    }
    return resultCode;
}

} // path
} // sdchain
