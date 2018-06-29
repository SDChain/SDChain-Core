//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_PATHS_TYPES_H_INCLUDED
#define SDCHAIN_APP_PATHS_TYPES_H_INCLUDED

namespace sdchain {

// account id, issue.
using AccountIssue = std::pair <AccountID, Issue>;

// Map of account, issue to node index.
namespace path {

using NodeIndex = unsigned int;

}

using AccountIssueToNodeIndex = hash_map <AccountIssue, path::NodeIndex>;

} // sdchain

#endif
