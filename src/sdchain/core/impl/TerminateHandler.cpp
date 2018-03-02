//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012 - 2017 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/core/TerminateHandler.h>
#include <sdchain/basics/Log.h>
#include <sdchain/beast/core/CurrentThreadName.h>

#include <boost/coroutine/exceptions.hpp>
#include <exception>
#include <iostream>
#include <typeinfo>

namespace sdchain {

void terminateHandler()
{
    if (std::current_exception())
    {
        auto const thName =
            beast::getCurrentThreadName().value_or("Unknown");
        try
        {
            throw;
        }
        catch (const std::exception& e)
        {
            auto exName = typeid (e).name();
            std::cerr
                << "Terminating thread " << thName << ": unhandled "
                << exName << " '" << e.what () << "'\n";
            JLOG(debugLog().fatal())
                << "Terminating thread " << thName << ": unhandled "
                << exName << " '" << e.what () << "'\n";
        }
        catch (boost::coroutines::detail::forced_unwind const&)
        {
            std::cerr
                << "Terminating thread " << thName << ": forced_unwind\n";
            JLOG(debugLog().fatal())
                << "Terminating thread " << thName << ": forced_unwind\n";
        }
        catch (...)
        {
            std::cerr
                << "Terminating thread " << thName << ": unknown exception\n";
            JLOG (debugLog().fatal())
                << "Terminating thread " << thName << ": unknown exception\n";
        }
    }
}

}
