//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#ifndef SDCHAIN_SHAMAP_SHAMAPADDNODE_H_INCLUDED
#define SDCHAIN_SHAMAP_SHAMAPADDNODE_H_INCLUDED

#include <string>

namespace sdchain {

// results of adding nodes
class SHAMapAddNode
{
private:
    int mGood;
    int mBad;
    int mDuplicate;

public:
    SHAMapAddNode ();
    void incInvalid ();
    void incUseful ();
    void incDuplicate ();
    void reset ();
    int getGood () const;
    bool isGood () const;
    bool isInvalid () const;
    bool isUseful () const;
    std::string get () const;

    SHAMapAddNode& operator+= (SHAMapAddNode const& n);

    static SHAMapAddNode duplicate ();
    static SHAMapAddNode useful ();
    static SHAMapAddNode invalid ();

private:
    SHAMapAddNode (int good, int bad, int duplicate);
};

inline
SHAMapAddNode::SHAMapAddNode ()
    : mGood (0)
    , mBad (0)
    , mDuplicate (0)
{
}

inline
SHAMapAddNode::SHAMapAddNode (int good, int bad, int duplicate)
    : mGood (good)
    , mBad (bad)
    , mDuplicate (duplicate)
{
}

inline
void
SHAMapAddNode::incInvalid ()
{
    ++mBad;
}

inline
void
SHAMapAddNode::incUseful ()
{
    ++mGood;
}

inline
void
SHAMapAddNode::incDuplicate ()
{
    ++mDuplicate;
}

inline
void
SHAMapAddNode::reset ()
{
    mGood = mBad = mDuplicate = 0;
}

inline
int
SHAMapAddNode::getGood () const
{
    return mGood;
}

inline
bool
SHAMapAddNode::isInvalid () const
{
    return mBad > 0;
}

inline
bool
SHAMapAddNode::isUseful () const
{
    return mGood > 0;
}

inline
SHAMapAddNode&
SHAMapAddNode::operator+= (SHAMapAddNode const& n)
{
    mGood += n.mGood;
    mBad += n.mBad;
    mDuplicate += n.mDuplicate;

    return *this;
}

inline
bool
SHAMapAddNode::isGood () const
{
    return (mGood + mDuplicate) > mBad;
}

inline
SHAMapAddNode
SHAMapAddNode::duplicate ()
{
    return SHAMapAddNode (0, 0, 1);
}

inline
SHAMapAddNode
SHAMapAddNode::useful ()
{
    return SHAMapAddNode (1, 0, 0);
}

inline
SHAMapAddNode
SHAMapAddNode::invalid ()
{
    return SHAMapAddNode (0, 1, 0);
}

inline
std::string
SHAMapAddNode::get () const
{
    std::string ret;
    if (mGood > 0)
    {
        ret.append("good:");
        ret.append(std::to_string(mGood));
    }
    if (mBad > 0)
    {
        if (!ret.empty())
            ret.append(" ");
         ret.append("bad:");
         ret.append(std::to_string(mBad));
    }
    if (mDuplicate > 0)
    {
        if (!ret.empty())
            ret.append(" ");
         ret.append("dupe:");
         ret.append(std::to_string(mDuplicate));
    }
    if (ret.empty ())
        ret = "no nodes processed";
    return ret;
}

}

#endif
