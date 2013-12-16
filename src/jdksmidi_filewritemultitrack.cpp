
#include "jdksmidi/world.h"
#include "jdksmidi/filewritemultitrack.h"

namespace jdksmidi
{

MIDIFileWriteMultiTrack::MIDIFileWriteMultiTrack (
    const MIDIMultiTrack *mlt_,
    MIDIFileWriteStream *strm_
)
    :
    multitrack ( mlt_ ),
    writer ( strm_ )
{
}

MIDIFileWriteMultiTrack::~MIDIFileWriteMultiTrack()
{
}

bool MIDIFileWriteMultiTrack::Write ( int num_tracks, int division )
{
    if ( !PreWrite() )
        return false;

    // first, write the header.
    writer.WriteFileHeader ( ( num_tracks > 1 )? 1:0, num_tracks, division );
    // now write each track

    for ( int i = 0; i < num_tracks; ++i )
    {
        const MIDITrack *t = multitrack->GetTrack ( i );

        if ( !t || !t->EventsOrderOK() ) // time of events out of order: t->SortEventsOrder() must be done externally
            return false;

        writer.WriteTrackHeader ( 0 ); // will be rewritten later

        const MIDITimedBigMessage *ev;
        MIDIClockTime ev_time = 0;

        for ( int event_num = 0; event_num < t->GetNumEvents(); ++event_num )
        {
            ev = t->GetEventAddress ( event_num );
            if ( !ev )
                return false;

            // don't write to midifile NoOp msgs
            if ( ev->IsNoOp() )
                continue;

            ev_time = ev->GetTime();

            // ignore all msgs after EndOfTrack
            if ( ev->IsDataEnd() )
              break;

            // write all other msgs
            writer.WriteEvent ( *ev );

            if ( writer.ErrorOccurred() )
                return false;
        }

        writer.WriteEndOfTrack ( ev_time );
        writer.RewriteTrackLength();
    }

    if ( !PostWrite() )
        return false;

    return true;
}


bool MIDIFileWriteMultiTrack::PreWrite()
{
    return true;
}

bool MIDIFileWriteMultiTrack::PostWrite()
{
    return true;
}



}
