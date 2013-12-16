
#include "jdksmidi/world.h"
#include "jdksmidi/track.h"
#include "jdksmidi/multitrack.h"
#include "jdksmidi/filereadmultitrack.h"
#include "jdksmidi/fileread.h"
#include "jdksmidi/fileshow.h"

using namespace jdksmidi;


void DumpMIDITimedBigMessage ( const MIDITimedBigMessage *msg )
{
    if ( msg )
    {
        char msgbuf[1024];
        fprintf ( stdout, "%8ld : %s\n", msg->GetTime(), msg->MsgToText ( msgbuf ) );

        if ( msg->IsSystemExclusive() )
        {
            fprintf ( stdout, "\tSYSEX length: %d\n", msg->GetSysEx()->GetLength() );
        }
    }
}

void DumpMIDITrack ( MIDITrack *t )
{
    MIDITimedBigMessage *msg;

    for ( int i = 0; i < t->GetNumEvents(); ++i )
    {
        msg = t->GetEventAddress ( i );
        DumpMIDITimedBigMessage ( msg );
    }
}

void DumpAllTracks ( MIDIMultiTrack *mlt )
{
    for ( int i = 0; i < mlt->GetNumTracks(); ++i )
    {
        if ( mlt->GetTrack ( i )->GetNumEvents() > 0 )
        {
            fprintf ( stdout, "DUMP OF TRACK #%2d:\n", i );
            DumpMIDITrack ( mlt->GetTrack ( i ) );
            fprintf ( stdout, "\n" );
        }
    }
}


void DumpMIDIMultiTrack ( MIDIMultiTrack *mlt )
{
    MIDIMultiTrackIterator i ( mlt );
    const MIDITimedBigMessage *msg;
    i.GoToTime ( 0 );

    do
    {
        int trk_num;

        if ( i.GetCurEvent ( &trk_num, &msg ) )
        {
            fprintf ( stdout, "#%2d - ", trk_num );
            DumpMIDITimedBigMessage ( msg );
        }
    }
    while ( i.GoToNextEvent() );
}



int main ( int argc, char **argv )
{
    if ( argc > 1 )
    {
        MIDIFileReadStreamFile rs ( argv[1] );
        MIDIMultiTrack tracks;
        MIDIFileReadMultiTrack track_loader ( &tracks );
        MIDIFileRead reader ( &rs, &track_loader );
        reader.Parse();
        DumpMIDIMultiTrack ( &tracks );
    }

    return 0;
}
