//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_PEERFINDER_SIM_PREDICATES_H_INCLUDED
#define SDCHAIN_PEERFINDER_SIM_PREDICATES_H_INCLUDED

namespace sdchain {
namespace PeerFinder {
namespace Sim {

/** UnaryPredicate, returns `true` if the 'to' node on a Link matches. */
/** @{ */
template <typename Node>
class is_remote_node_pred
{
public:
    is_remote_node_pred (Node const& node)
        : node (node)
        { }
    template <typename Link>
    bool operator() (Link const& l) const
        { return &node == &l.remote_node(); }
private:
    Node const& node;
};

template <typename Node>
is_remote_node_pred <Node> is_remote_node (Node const& node)
{
    return is_remote_node_pred <Node> (node);
}

template <typename Node>
is_remote_node_pred <Node> is_remote_node (Node const* node)
{
    return is_remote_node_pred <Node> (*node);
}
/** @} */

//------------------------------------------------------------------------------

/** UnaryPredicate, `true` if the remote address matches. */
class is_remote_endpoint
{
public:
    explicit is_remote_endpoint (beast::IP::Endpoint const& address)
        : m_endpoint (address)
        { }
    template <typename Link>
    bool operator() (Link const& link) const
    {
        return link.remote_endpoint() == m_endpoint;
    }
private:
    beast::IP::Endpoint const m_endpoint;
};

}
}
}

#endif
