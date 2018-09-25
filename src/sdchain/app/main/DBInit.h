//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_APP_DATA_DBINIT_H_INCLUDED
#define SDCHAIN_APP_DATA_DBINIT_H_INCLUDED

namespace sdchain {

// VFALCO TODO Tidy these up into a class with functions and return types.
extern const char* TxnDBInit[];
extern const char* LedgerDBInit[];
extern const char* WalletDBInit[];

// VFALCO TODO Figure out what these counts are for
extern int TxnDBCount;
extern int LedgerDBCount;
extern int WalletDBCount;

} // sdchain

#endif
