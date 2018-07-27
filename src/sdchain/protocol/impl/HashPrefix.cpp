//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/protocol/HashPrefix.h>

namespace sdchain {

// The prefix codes are part of the SDChain protocol
// and existing codes cannot be arbitrarily changed.

HashPrefix const HashPrefix::transactionID               ('T', 'X', 'N');
HashPrefix const HashPrefix::txNode                      ('S', 'N', 'D');
HashPrefix const HashPrefix::leafNode                    ('M', 'L', 'N');
HashPrefix const HashPrefix::innerNode                   ('M', 'I', 'N');
HashPrefix const HashPrefix::innerNodeV2                 ('I', 'N', 'R');
HashPrefix const HashPrefix::ledgerMaster                ('L', 'W', 'R');
HashPrefix const HashPrefix::txSign                      ('S', 'T', 'X');
HashPrefix const HashPrefix::txMultiSign                 ('S', 'M', 'T');
HashPrefix const HashPrefix::validation                  ('V', 'A', 'L');
HashPrefix const HashPrefix::proposal                    ('P', 'R', 'P');
HashPrefix const HashPrefix::manifest                    ('M', 'A', 'N');
HashPrefix const HashPrefix::paymentChannelClaim         ('C', 'L', 'M');

} // sdchain
