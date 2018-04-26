//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/resource/Charge.h>

namespace sdchain {
namespace Resource {

Charge const feeInvalidRequest    (  10, "malformed request"      );
Charge const feeRequestNoReply    (   1, "unsatisfiable request"  );
Charge const feeInvalidSignature  ( 100, "invalid signature"      );
Charge const feeUnwantedData      (  15, "useless data"           );
Charge const feeBadData           (  20, "invalid data"           );

Charge const feeInvalidRPC        (  10, "malformed RPC"          );
Charge const feeReferenceRPC      (   2, "reference RPC"          );
Charge const feeExceptionRPC      (  10, "exceptioned RPC"        );
Charge const feeLightRPC          (   5, "light RPC"              ); // DAVID: Check the cost
Charge const feeLowBurdenRPC      (  20, "low RPC"                );
Charge const feeMediumBurdenRPC   (  40, "medium RPC"             );
Charge const feeHighBurdenRPC     ( 300, "heavy RPC"              );

Charge const feeLightPeer         (1, "trivial peer request"      );
Charge const feeLowBurdenPeer     (2, "simple peer request"       );
Charge const feeMediumBurdenPeer  (50, "moderate peer request"    );
Charge const feeHighBurdenPeer    (250, "heavy peer request"      );

Charge const feeNewTrustedNote    (  10, "trusted note"           );
Charge const feeNewValidTx        (  10, "valid tx"               );
Charge const feeSatisfiedRequest  (  10, "needed data"            );

Charge const feeWarning           ( 200, "received warning"       );
Charge const feeDrop              ( 300, "dropped"                );

}
}
