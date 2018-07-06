//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/paths/Credit.h>
#include <sdchain/app/paths/Flow.h>
#include <sdchain/app/paths/impl/AmountSpec.h>
#include <sdchain/app/paths/impl/StrandFlow.h>
#include <sdchain/app/paths/impl/Steps.h>
#include <sdchain/basics/Log.h>
#include <sdchain/protocol/IOUAmount.h>
#include <sdchain/protocol/SDAAmount.h>

#include <boost/container/flat_set.hpp>

#include <numeric>
#include <sstream>

namespace sdchain {

template<class FlowResult>
static
auto finishFlow (PaymentSandbox& sb,
    Issue const& srcIssue, Issue const& dstIssue,
    FlowResult&& f)
{
    path::SDChainCalc::Output result;
    if (f.ter == tesSUCCESS)
        f.sandbox->apply (sb);
    else
        result.removableOffers = std::move (f.removableOffers);

    result.setResult (f.ter);
    result.actualAmountIn = toSTAmount (f.in, srcIssue);
    result.actualAmountOut = toSTAmount (f.out, dstIssue);

    return result;
};

path::SDChainCalc::Output
flow (
    PaymentSandbox& sb,
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
    path::detail::FlowDebugInfo* flowDebugInfo)
{
    Issue const srcIssue = [&] {
        if (sendMax)
            return sendMax->issue ();
        if (!isSDA (deliver.issue ().currency))
            return Issue (deliver.issue ().currency, src);
        return sdaIssue ();
    }();

    Issue const dstIssue = deliver.issue ();

    boost::optional<Issue> sendMaxIssue;
    if (sendMax)
        sendMaxIssue = sendMax->issue ();

    // convert the paths to a collection of strands. Each strand is the collection
    // of account->account steps and book steps that may be used in this payment.
    auto sr = toStrands (sb, src, dst, dstIssue, limitQuality, sendMaxIssue,
        paths, defaultPaths, ownerPaysTransferFee, offerCrossing, j);

    if (sr.first != tesSUCCESS)
    {
        path::SDChainCalc::Output result;
        result.setResult (sr.first);
        return result;
    }

    auto& strands = sr.second;

    if (j.trace())
    {
        j.trace() << "\nsrc: " << src << "\ndst: " << dst
            << "\nsrcIssue: " << srcIssue << "\ndstIssue: " << dstIssue;
        j.trace() << "\nNumStrands: " << strands.size ();
        for (auto const& curStrand : strands)
        {
            j.trace() << "NumSteps: " << curStrand.size ();
            for (auto const& step : curStrand)
            {
                j.trace() << '\n' << *step << '\n';
            }
        }
    }

    const bool srcIsSDA = isSDA (srcIssue.currency);
    const bool dstIsSDA = isSDA (dstIssue.currency);

    auto const asDeliver = toAmountSpec (deliver);

    // The src account may send either sda or iou. The dst account may receive
    // either sda or iou. Since SDA and IOU amounts are represented by different
    // types, use templates to tell `flow` about the amount types.
    if (srcIsSDA && dstIsSDA)
    {
        return finishFlow (sb, srcIssue, dstIssue,
            flow<SDAAmount, SDAAmount> (
                sb, strands, asDeliver.sda, partialPayment, offerCrossing,
                limitQuality, sendMax, j, flowDebugInfo));
    }

    if (srcIsSDA && !dstIsSDA)
    {
        return finishFlow (sb, srcIssue, dstIssue,
            flow<SDAAmount, IOUAmount> (
                sb, strands, asDeliver.iou, partialPayment, offerCrossing,
                limitQuality, sendMax, j, flowDebugInfo));
    }

    if (!srcIsSDA && dstIsSDA)
    {
        return finishFlow (sb, srcIssue, dstIssue,
            flow<IOUAmount, SDAAmount> (
                sb, strands, asDeliver.sda, partialPayment, offerCrossing,
                limitQuality, sendMax, j, flowDebugInfo));
    }

    assert (!srcIsSDA && !dstIsSDA);
    return finishFlow (sb, srcIssue, dstIssue,
        flow<IOUAmount, IOUAmount> (
            sb, strands, asDeliver.iou, partialPayment, offerCrossing,
            limitQuality, sendMax, j, flowDebugInfo));

}

} // sdchain
