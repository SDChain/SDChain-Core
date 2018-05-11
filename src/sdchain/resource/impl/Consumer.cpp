//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/resource/Consumer.h>
#include <sdchain/resource/impl/Entry.h>
#include <sdchain/resource/impl/Logic.h>
#include <cassert>

namespace sdchain {
namespace Resource {

Consumer::Consumer (Logic& logic, Entry& entry)
    : m_logic (&logic)
    , m_entry (&entry)
{
}

Consumer::Consumer ()
    : m_logic (nullptr)
    , m_entry (nullptr)
{
}

Consumer::Consumer (Consumer const& other)
    : m_logic (other.m_logic)
    , m_entry (nullptr)
{
    if (m_logic && other.m_entry)
    {
        m_entry = other.m_entry;
        m_logic->acquire (*m_entry);
    }
}

Consumer::~Consumer()
{
    if (m_logic && m_entry)
        m_logic->release (*m_entry);
}

Consumer& Consumer::operator= (Consumer const& other)
{
    // remove old ref
    if (m_logic && m_entry)
        m_logic->release (*m_entry);

    m_logic = other.m_logic;
    m_entry = other.m_entry;

    // add new ref
    if (m_logic && m_entry)
        m_logic->acquire (*m_entry);

    return *this;
}

std::string Consumer::to_string () const
{
    if (m_logic == nullptr)
        return "(none)";

    return m_entry->to_string();
}

bool Consumer::isUnlimited () const
{
    if (m_entry)
        return m_entry->isUnlimited();

    return false;
}

Disposition Consumer::disposition() const
{
    Disposition d = ok;
    if (m_logic && m_entry)
        d =  m_logic->charge(*m_entry, Charge(0));

    return d;
}

Disposition Consumer::charge (Charge const& what)
{
    Disposition d = ok;

    if (m_logic && m_entry)
        d = m_logic->charge (*m_entry, what);

    return d;
}

bool Consumer::warn ()
{
    assert (m_entry != nullptr);
    return m_logic->warn (*m_entry);
}

bool Consumer::disconnect ()
{
    assert (m_entry != nullptr);
    return m_logic->disconnect (*m_entry);
}

int Consumer::balance()
{
    assert (m_entry != nullptr);
    return m_logic->balance (*m_entry);
}

Entry& Consumer::entry()
{
    assert (m_entry != nullptr);
    return *m_entry;
}

std::ostream& operator<< (std::ostream& os, Consumer const& v)
{
    os << v.to_string();
    return os;
}

}
}
