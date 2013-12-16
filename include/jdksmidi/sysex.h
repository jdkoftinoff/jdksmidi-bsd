
#ifndef JDKSMIDI_SYSEX_H
#define JDKSMIDI_SYSEX_H

#include "jdksmidi/midi.h"

namespace jdksmidi
{

class  MIDISystemExclusive
{
public:
    MIDISystemExclusive ( int size = 384 );

    MIDISystemExclusive ( const MIDISystemExclusive &e );


    MIDISystemExclusive (
        unsigned char *buf_,
        int max_len_,
        int cur_len_,
        bool deletable_
    )
    {
        buf = buf_;
        max_len = max_len_;
        cur_len = cur_len_;
        chk_sum = 0;
        deletable = deletable_;
    }

    virtual ~MIDISystemExclusive();

    friend bool operator == ( const MIDISystemExclusive &e1, const MIDISystemExclusive &e2 );

    void Clear()
    {
        cur_len = 0;
        chk_sum = 0;
    }
    void ClearChecksum()
    {
        chk_sum = 0;
    }

    void PutSysByte ( unsigned char b ) // does not add to chksum
    {
        if ( cur_len < max_len )
            buf[cur_len++] = b;
    }

    void PutByte ( unsigned char b )
    {
        PutSysByte ( b );
        chk_sum += b;
    }

    void PutEXC()
    {
        PutSysByte ( SYSEX_START_N );
    }
    void PutEOX()
    {
        PutSysByte ( SYSEX_END );
    }

    // low nibble first
    void PutNibblizedByte ( unsigned char b )
    {
        PutByte ( ( unsigned char ) ( b & 0xf ) );
        PutByte ( ( unsigned char ) ( b >> 4 ) );
    }

    // high nibble first
    void PutNibblizedByte2 ( unsigned char b )
    {
        PutByte ( ( unsigned char ) ( b >> 4 ) );
        PutByte ( ( unsigned char ) ( b & 0xf ) );
    }

    void PutChecksum()
    {
        PutByte ( ( unsigned char ) ( chk_sum & 0x7f ) );
    }

    unsigned char GetChecksum() const
    {
        return ( unsigned char ) ( chk_sum & 0x7f );
    }

    int GetLengthSE() const
    {
        return cur_len;
    }

    int GetLength() const
    {
        return GetLengthSE();
    }

    unsigned char GetData ( int i ) const
    {
        return buf[i];
    }

    bool IsFull() const
    {
        return cur_len >= max_len;
    }

    unsigned char *GetBuf()
    {
        return buf;
    }

    const unsigned char *GetBuf() const
    {
        return buf;
    }

private:

    unsigned char *buf;
    int max_len;
    int cur_len;
    unsigned char  chk_sum;
    bool deletable;
};
}

#endif


