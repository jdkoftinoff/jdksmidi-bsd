
#ifndef JDKSMIDI_SONG_H
#define JDKSMIDI_SONG_H

#include "jdksmidi/multitrack.h"
#include "jdksmidi/sequencer.h"
#include "jdksmidi/filereadmultitrack.h"

namespace jdksmidi
{

class MIDISong
{
public:
    MIDISong ( int max_tracks );
    virtual ~MIDISong();

    bool Load ( const char *fname );

// bool Save( const char *fname );

    MIDIMultiTrack *GetMultiTrack()
    {
        return multitrack;
    }

    const MIDIMultiTrack *GetMultiTrack() const
    {
        return multitrack;
    }

    MIDISequencer *GetSeq()
    {
        return sequencer;
    }

    const MIDISequencer *GetSeq() const
    {
        return sequencer;
    }

protected:

    MIDIMultiTrack track;
    MIDISequencer seq;

    char title;
};
}

#endif
