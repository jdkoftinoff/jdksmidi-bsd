
#include "jdksmidi/world.h"
#include "jdksmidi/sysex.h"

#ifndef DEBUG_MDSYSEX
# define DEBUG_MDSYSEX 0
#endif

#if DEBUG_MDSYSEX
# undef DBG
# define DBG(a) a
#endif

namespace jdksmidi
{

MIDISystemExclusive::MIDISystemExclusive ( int size_ )
{
    ENTER ( "MIDISystemExclusive::MIDISystemExclusive" );
    buf = new uchar[size_];

    if ( buf )
        max_len = size_;

    else
        max_len = 0;

    cur_len = 0;
    chk_sum = 0;
    deletable = true;
}

MIDISystemExclusive::MIDISystemExclusive ( const MIDISystemExclusive &e )
{
    buf = new unsigned char [e.max_len];
    max_len = e.max_len;
    cur_len = e.cur_len;
    chk_sum = e.chk_sum;
    deletable = true;

    for ( int i = 0; i < cur_len; ++i )
    {
        buf[i] = e.buf[i];
    }
}

MIDISystemExclusive::~MIDISystemExclusive()
{
    ENTER ( "MIDISystemExclusive::~MIDISystemExclusive" );

    if ( deletable )
    {
        jdks_safe_delete_array( buf );
    }
}

bool operator == ( const MIDISystemExclusive &e1, const MIDISystemExclusive &e2 )
{
    if ( e1.cur_len != e2.cur_len )
        return false;

    if ( e1.cur_len == 0 )
        return true;

    return memcmp( e1.buf, e2.buf, e1.cur_len ) == 0 ;
}

}
