//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_PATHS_FLOW_H_INCLUDED
#define SDCHAIN_APP_PATHS_FLOW_H_INCLUDED

#include <sdchain/app/paths/impl/Steps.h>
#include <sdchain/app/paths/SDChainCalc.h>
#include <sdchain/protocol/Quality.h>

namespace sdchain
{

namespace path {
namespace detail{
struct FlowDebugInfo;
}
}

/**
  Make a payment from the src account to the dst account

  @param view Trust lines and balances
  @param deliver Amount to deliver to the dst account
  @param src Account providing input funds for the payment
  @param dst Account receiving the payment
  @param paths Set of paths to explore for liquidity
  @param defaultPaths Include defaultPaths in the path set
  @param partialPayment If the payment cannot deliver the entire
           requested amount, deliver as much as possible, given the constraints
  @param ownerPaysTransferFee If true then owner, not sender, pays fee
  @param offerCrossing If true then flow is executing offer crossing, not payments
  @param limitQuality Do not use liquidity below this quality threshold
  @param sendMax Do not spend more than this amount
  @param j Journal to write journal messages to
  @param flowDebugInfo If non-null a pointer to FlowDebugInfo for debugging
  @return Actual amount in and out, and the result code
*/
path::SDChainCalc::Output
flow (PaymentSandbox& view,
    STAmount const& deliver,
    AccountID const& src,
    AccountID const& dst,
    STPathSet const& paths,
    bool defaultPaths,
    bool partialPayment,
    bool ownerPaysTransferFee,
    bool offerCrossing,
    boost::optional<Quality> const& limitQuality,
    boost::optional<STAmount> const& sendMax,
    beast::Journal j,
    path::detail::FlowDebugInfo* flowDebugInfo=nullptr);

}  // sdchain

#endif
