//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/paths/Node.h>
#include <sdchain/app/paths/PathState.h>
#include <sdchain/protocol/JsonFields.h>

namespace sdchain {
namespace path {

// Compare the non-calculated fields.
bool Node::operator== (const Node& other) const
{
    return other.uFlags == uFlags
            && other.account_ == account_
            && other.issue_ == issue_;
}

// This is for debugging not end users. Output names can be changed without
// warning.
Json::Value Node::getJson () const
{
    Json::Value jvNode (Json::objectValue);
    Json::Value jvFlags (Json::arrayValue);

    jvNode[jss::type]  = uFlags;

    bool const hasCurrency = !isSDA (issue_.currency);
    bool const hasAccount = !isSDA (account_);
    bool const hasIssuer = !isSDA (issue_.account);

    if (isAccount() || hasAccount)
        jvFlags.append (!isAccount() == hasAccount ? "account" : "-account");

    if (uFlags & STPathElement::typeCurrency || hasCurrency)
    {
        jvFlags.append ((uFlags & STPathElement::typeCurrency) && hasCurrency
            ? "currency"
            : "-currency");
    }

    if (uFlags & STPathElement::typeIssuer || hasIssuer)
    {
        jvFlags.append ((uFlags & STPathElement::typeIssuer) && hasIssuer
            ? "issuer"
            : "-issuer");
    }

    jvNode["flags"] = jvFlags;

    if (!isSDA (account_))
        jvNode[jss::account] = to_string (account_);

    if (!isSDA (issue_.currency))
        jvNode[jss::currency] = to_string (issue_.currency);

    if (!isSDA (issue_.account))
        jvNode[jss::issuer] = to_string (issue_.account);

    if (saRevRedeem)
        jvNode["rev_redeem"] = saRevRedeem.getFullText ();

    if (saRevIssue)
        jvNode["rev_issue"] = saRevIssue.getFullText ();

    if (saRevDeliver)
        jvNode["rev_deliver"] = saRevDeliver.getFullText ();

    if (saFwdRedeem)
        jvNode["fwd_redeem"] = saFwdRedeem.getFullText ();

    if (saFwdIssue)
        jvNode["fwd_issue"] = saFwdIssue.getFullText ();

    if (saFwdDeliver)
        jvNode["fwd_deliver"] = saFwdDeliver.getFullText ();

    return jvNode;
}

} // path
} // sdchain
