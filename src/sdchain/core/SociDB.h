//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2012-2015 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_SOCIDB_H_INCLUDED
#define SDCHAIN_SOCIDB_H_INCLUDED

/** An embedded database wrapper with an intuitive, type-safe interface.

    This collection of classes let's you access embedded SQLite databases
    using C++ syntax that is very similar to regular SQL.

    This module requires the @ref beast_sqlite external module.
*/

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated"
#endif

#include <sdchain/basics/Log.h>
#include <sdchain/core/JobQueue.h>
#define SOCI_USE_BOOST
#include <soci/soci.h>
#include <string>
#include <cstdint>
#include <vector>

namespace sqlite_api {
    struct sqlite3;
}

namespace sdchain {

template <class T, class C>
T rangeCheckedCast (C c)
{
    if ((c > std::numeric_limits<T>::max ()) ||
        (!std::numeric_limits<T>::is_signed && c < 0) ||
        (std::numeric_limits<T>::is_signed &&
         std::numeric_limits<C>::is_signed &&
         c < std::numeric_limits<T>::lowest ()))
    {
        JLOG (debugLog().error()) << "rangeCheckedCast domain error:"
          << " value = " << c
          << " min = " << std::numeric_limits<T>::lowest ()
          << " max: " << std::numeric_limits<T>::max ();
    }

    return static_cast<T>(c);
}

class BasicConfig;

/**
   SociConfig is used when a client wants to delay opening a soci::session after
   parsing the config parameters. If a client want to open a session
   immediately, use the free function "open" below.
 */
class SociConfig
{
    std::string connectionString_;
    soci::backend_factory const& backendFactory_;
    SociConfig(std::pair<std::string, soci::backend_factory const&> init);

public:
    SociConfig(BasicConfig const& config,
               std::string const& dbName);
    std::string connectionString () const;
    void open (soci::session& s) const;
};

/**
   Open a soci session.

   @param s Session to open.

   @param config Parameters to pick the soci backend and how to connect to that
                 backend.

   @param dbName Name of the database. This has different meaning for different
                 backends. Sometimes it is part of a filename (sqlite3),
                 other times it is a database name (postgresql).
*/
void open (soci::session& s,
           BasicConfig const& config,
           std::string const& dbName);

/**
 *  Open a soci session.
 *
 *  @param s Session to open.
 *  @param beName Backend name.
 *  @param connectionString Connection string to forward to soci::open.
 *         see the soci::open documentation for how to use this.
 *
 */
void open (soci::session& s,
           std::string const& beName,
           std::string const& connectionString);

size_t getKBUsedAll (soci::session& s);
size_t getKBUsedDB (soci::session& s);

void convert (soci::blob& from, std::vector<std::uint8_t>& to);
void convert (soci::blob& from, std::string& to);
void convert (std::vector<std::uint8_t> const& from, soci::blob& to);
void convert (std::string const& from, soci::blob& to);

class Checkpointer
{
  public:
    virtual ~Checkpointer() = default;
};

/** Returns a new checkpointer which makes checkpoints of a
    soci database every checkpointPageCount pages, using a job on the job queue.

    The Checkpointer contains references to the session and job queue
    and so must outlive them both.
 */
std::unique_ptr <Checkpointer> makeCheckpointer (soci::session&, JobQueue&, Logs&);

} // sdchain

// Do not remove this dead code. It forces `scons vcxproj` to include version.h.
#if 0
#include "version.h"
#endif

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#endif
