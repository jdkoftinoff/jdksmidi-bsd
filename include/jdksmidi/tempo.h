
#ifndef JDKSMIDI_TEMPO_H
#define JDKSMIDI_TEMPO_H

//
// This class makes it easy to deal with Tempos as fixed point numbers.
//
// The actual tempo is stored times 256 for 1/256 bpm accuracy.
//
// The default operator int() etc., automatically convert the
// fixed point number so the value is in normal beats per minutes.
//
//

#include "jdksmidi/midi.h"

namespace jdksmidi
{

class  MIDITempo
{
public:
    MIDITempo()
    {
        tempo = 120 << 8;
    }
    MIDITempo ( int a )
    {
        tempo = ( unsigned long ) a << 8;
    }
    MIDITempo ( unsigned int a )
    {
        tempo = ( unsigned long ) a << 8;
    }
    MIDITempo ( long a )
    {
        tempo = ( unsigned long ) a << 8;
    }
    MIDITempo ( unsigned long a )
    {
        tempo = a << 8;
    }
    MIDITempo ( float a )
    {
        tempo = ( unsigned long ) ( a * 256.0 );
    }
    MIDITempo ( const MIDITempo &a )
    {
        tempo = a.GetFullTempo();
    }

    operator short ()
    {
        return ( short ) ( ( tempo + 0x80 ) >> 8 );
    }
    operator unsigned short ()
    {
        return ( unsigned short ) ( ( tempo + 0x80 ) >> 8 );
    }

    operator int ()
    {
        return ( int ) ( ( tempo + 0x80 ) >> 8 );
    }
    operator unsigned int ()
    {
        return ( unsigned int ) ( ( tempo + 0x80 ) >> 8 );
    }
    operator long ()
    {
        return ( long ) ( ( tempo + 0x80 ) >> 8 );
    }
    operator unsigned long ()
    {
        return ( unsigned long ) ( ( tempo + 0x80 ) >> 8 );
    }
    operator float ()
    {
        return ( float ) tempo / 256.0f;
    }
    void operator = ( unsigned short a )
    {
        tempo = ( unsigned long ) a << 8;
    }
    void operator = ( short a )
    {
        tempo = ( unsigned long ) a << 8;
    }

    void operator = ( unsigned int a )
    {
        tempo = ( unsigned long ) a << 8;
    }
    void operator = ( int a )
    {
        tempo = ( unsigned long ) a << 8;
    }
    void operator = ( unsigned long a )
    {
        tempo = ( unsigned long ) a << 8;
    }
    void operator = ( long a )
    {
        tempo = ( unsigned long ) a << 8;
    }

    void operator = ( float a )
    {
        tempo = ( unsigned long ) ( a * 256.0 );
    }

    unsigned long GetFullTempo() const
    {
        return tempo;
    }
    void SetFullTempo ( unsigned long v )
    {
        tempo = v;
    }

    unsigned long GetMIDIFileTempo()
    {
        if ( tempo )
            return ( 60000000L / 256 ) / tempo;

        else
            return ( 60000000L / 256 ) / ( 120 * 256 );
    }

protected:
    unsigned long tempo;
};

}

#endif


