
#include "jdksmidi/world.h"

#include "jdksmidi/matrix.h"

#ifndef DEBUG_MDMATRIX
# define DEBUG_MDMATRIX 0
#endif

#if DEBUG_MDMATRIX
# undef DBG
# define DBG(a) a
#endif


namespace jdksmidi
{

MIDIMatrix::MIDIMatrix()
{
    ENTER ( "MIDIMatrix::MIDIMatrix()" );

    for ( int channel = 0; channel < 16; channel++ )
    {
        channel_count[channel] = 0;
        hold_pedal[channel] = false;

        for ( unsigned char note = 0; note < 128; note++ )
            note_on_count[channel][note] = 0;
    }

    total_count = 0;
}

MIDIMatrix::~MIDIMatrix()
{
    ENTER ( "MIDIMatrix::~MIDIMatrix()" );
}


void  MIDIMatrix::DecNoteCount ( const MIDIMessage &, int channel, int note )
{
    ENTER ( "MIDIMatrix::DecNoteCount()" );

    if ( note_on_count[channel][note] > 0 )
    {
        --note_on_count[channel][note];
        --channel_count[channel];
        --total_count;
    }
}

void  MIDIMatrix::IncNoteCount ( const MIDIMessage &, int channel, int note )
{
    ENTER ( "MIDIMatrix::IncNoteCount()" );
    ++note_on_count[channel][note];
    ++channel_count[channel];
    ++total_count;
}

void MIDIMatrix::OtherMessage ( const MIDIMessage & )
{
    ENTER ( "MIDIMatrix::OtherMessage()" );
}


bool MIDIMatrix::Process ( const MIDIMessage &m )
{
    ENTER ( "MIDIMatrix::Process()" );
    bool status = false;

    if ( m.IsChannelMsg() )
    {
        int channel = m.GetChannel();
        int note = m.GetNote();

        if ( m.IsAllNotesOff() )
        {
            ClearChannel ( channel );
            status = true;
        }

        else if ( m.IsNoteOn() )
        {
            if ( m.GetVelocity() != 0 )
                IncNoteCount ( m, channel, note );

            else
                DecNoteCount ( m, channel, note );

            status = true;
        }

        else if ( m.IsNoteOff() )
        {
            DecNoteCount ( m, channel, note );
            status = true;
        }

        else if ( m.IsControlChange() && m.GetController() == C_DAMPER )
        {
            if ( m.GetControllerValue() & 0x40 )
            {
                hold_pedal[channel] = true;
            }

            else
            {
                hold_pedal[channel] = false;
            }
        }

        else
            OtherMessage ( m );
    }

    return status;
}

void MIDIMatrix::Clear()
{
    ENTER ( "MIDIMatrix::Clear()" );

    for ( int channel = 0; channel < 16; ++channel )
    {
        ClearChannel ( channel );
    }

    total_count = 0;
}

void MIDIMatrix::ClearChannel ( int channel )
{
    ENTER ( "MIDIMatrix::ClearChannel()" );

    for ( int note = 0; note < 128; ++note )
    {
        total_count -= note_on_count[channel][note];
        note_on_count[channel][note] = 0;
    }

    channel_count[channel] = 0;
    hold_pedal[channel] = 0;
}



}
