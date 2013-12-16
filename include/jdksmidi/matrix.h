
#ifndef JDKSMIDI_MATRIX_H
#define JDKSMIDI_MATRIX_H


#include "jdksmidi/midi.h"
#include "jdksmidi/msg.h"

namespace jdksmidi
{

/// This class implements a matrix to keep track of notes on and hold pedal for every channel.
/// It is used by MIDIDriver and MIDISequencerTrackState
class  MIDIMatrix
{
public:

    /// The constructor creates an empty matrix
    MIDIMatrix();

    /// The destructor
    virtual ~MIDIMatrix();

    /// Processes the given MIDI message upgrading the matrix
    virtual bool Process ( const MIDIMessage &m );

    /// Resets the matrix (no notes on, no pedal hold)
    virtual void Clear();

    /// Returns the total number of notes on
    int GetTotalCount() const
    {
        return total_count;
    }

    /// Returns the number of notes on for given channel (channels from 0 to 15)
    int GetChannelCount ( int channel ) const
    {
        return channel_count[channel];
    }

    /// Gets the number of notes on given the channel and the note MIDI value
    int GetNoteCount ( int channel, int note ) const
    {
        return note_on_count[channel][note];
    }

    /// Returns *true* if pedal is holding on guven channel
    bool GetHoldPedal ( int channel ) const
    {
        return hold_pedal[channel];
    }


protected:

    /// Decrements the note count (currently _m_ is ignored)
    virtual void DecNoteCount ( const MIDIMessage &m, int channel, int note );

    /// Increments the note count (currently _m_ is ignored)
    virtual void IncNoteCount ( const MIDIMessage &m, int channel, int note );

    /// Clear the note count and the pedal on the given channel
    virtual void ClearChannel ( int channel );

    /// It is called by Process() for non note and non pedal messages (currently does nothing)
    virtual void OtherMessage ( const MIDIMessage &m );

    /// Sets the note count
    void SetNoteCount ( unsigned char chan, unsigned char note, unsigned char val )
    {
        note_on_count[chan][note] = val;
    }

    /// Sets the channel note count
    void SetChannelCount ( unsigned char chan, int val )
    {
        channel_count[chan] = val;
    }

private:
    unsigned char note_on_count[16][128];
    int channel_count[16];
    bool hold_pedal[16];
    int total_count;
};

}


#endif


