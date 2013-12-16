
#ifndef JDKSMIDI_FILEWRITEMULTITRACK_H
#define JDKSMIDI_FILEWRITEMULTITRACK_H

#include "jdksmidi/filewrite.h"
#include "jdksmidi/multitrack.h"

namespace jdksmidi
{

class MIDIFileWriteMultiTrack
{
public:

    MIDIFileWriteMultiTrack (
        const MIDIMultiTrack *mlt_,
        MIDIFileWriteStream *strm_
    );

    virtual ~MIDIFileWriteMultiTrack();

    bool Write ( int num_tracks, int division );

    bool Write ( int num_tracks )
    {
        return Write ( num_tracks, multitrack->GetClksPerBeat() );
    }
    bool Write()
    {
        return Write ( multitrack->GetNumTracks(), multitrack->GetClksPerBeat() );
    }

    // false argument disable use running status in midi file (true on default)
    void UseRunningStatus( bool use )
    {
        writer.UseRunningStatus( use );
    }

private:
    virtual bool PreWrite();
    virtual bool PostWrite();

    const MIDIMultiTrack *multitrack;
    MIDIFileWrite writer;
};

}


#endif
