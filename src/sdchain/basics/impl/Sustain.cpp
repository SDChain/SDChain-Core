//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/basics/Sustain.h>
#include <sdchain/beast/core/CurrentThreadName.h>
#include <boost/format.hpp>

// For Sustain Linux variants
// VFALCO TODO Rewrite Sustain to use beast::Process
#ifdef __linux__
#include <sys/types.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <unistd.h>
#endif
#ifdef __FreeBSD__
#include <sys/types.h>
#include <sys/wait.h>
#endif

namespace sdchain {

#ifdef __unix__

static auto const sleepBeforeWaiting = 10;
static auto const sleepBetweenWaits = 1;

static pid_t pManager = static_cast<pid_t> (0);
static pid_t pChild = static_cast<pid_t> (0);

static void pass_signal (int a)
{
    kill (pChild, a);
}

static void stop_manager (int)
{
    kill (pChild, SIGINT);
    _exit (0);
}

bool HaveSustain ()
{
    return true;
}

std::string StopSustain ()
{
    if (getppid () != pManager)
        return "";

    kill (pManager, SIGHUP);
    return "Terminating monitor";
}

static
bool checkChild(pid_t pid, int options)
{
    int i;

    if (waitpid (pChild, &i, options) == -1)
        return false;

    return kill (pChild, 0) == 0;
}

std::string DoSustain ()
{
    pManager = getpid ();
    signal (SIGINT, stop_manager);
    signal (SIGHUP, stop_manager);
    signal (SIGUSR1, pass_signal);
    signal (SIGUSR2, pass_signal);

    // Number of times the child has exited in less than
    // 15 seconds.
    int fastExit = 0;

    for (auto childCount = 1; ; ++childCount)
    {
        pChild = fork ();

        if (pChild == -1)
            _exit (0);

        auto cc = std::to_string (childCount);
        if (pChild == 0)
        {
            beast::setCurrentThreadName ("sdchaind: main");
            signal (SIGINT, SIG_DFL);
            signal (SIGHUP, SIG_DFL);
            signal (SIGUSR1, SIG_DFL);
            signal (SIGUSR2, SIG_DFL);
            return "Launching child " + cc;
        }

        beast::setCurrentThreadName (("sdchaind: #" + cc).c_str());

        sleep (sleepBeforeWaiting);

        // If the child has already terminated count this
        // as a fast exit and an indication that something
        // went wrong:
        if (!checkChild (pChild, WNOHANG))
        {
            if (++fastExit == 5)
                _exit (0);
        }
        else
        {
            fastExit = 0;

            while (checkChild (pChild, 0))
                sleep(sleepBetweenWaits);

            (void)rename ("core",
                ("core." + std::to_string(pChild)).c_str());
        }
    }
}

#else

bool HaveSustain ()
{
    return false;
}

std::string DoSustain ()
{
    return "";
}

std::string StopSustain ()
{
    return "";
}

#endif

} // sdchain
