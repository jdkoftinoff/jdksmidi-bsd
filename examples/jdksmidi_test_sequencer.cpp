
#ifdef WIN32
#include <windows.h>
#endif

#include "jdksmidi/world.h"
#include "jdksmidi/track.h"
#include "jdksmidi/multitrack.h"
#include "jdksmidi/filereadmultitrack.h"
#include "jdksmidi/fileread.h"
#include "jdksmidi/fileshow.h"
#include "jdksmidi/sequencer.h"
using namespace jdksmidi;

#include <iostream>
using namespace std;

void DumpMIDIBigMessage( MIDITimedBigMessage *msg )
{
    if ( msg )
    {
        char msgbuf[1024];
        fprintf ( stdout, "%s", msg->MsgToText ( msgbuf ) );

        if ( msg->IsSystemExclusive() )
        {
            fprintf ( stdout, "SYSEX length: %d", msg->GetSysEx()->GetLengthSE() );
        }

        fprintf ( stdout, "\n" );
    }
}

void DumpMIDITimedBigMessage( const MIDITimedBigMessage *msg )
{
    if ( msg )
    {
        char msgbuf[1024];

        // note that Sequencer generate SERVICE_BEAT_MARKER in files dump,
        // but files themselves not contain this meta event...
        // see MIDISequencer::beat_marker_msg.SetBeatMarker()
        if ( msg->IsBeatMarker() )
        {
            fprintf ( stdout, "%8ld : %s <------------------>", msg->GetTime(), msg->MsgToText ( msgbuf ) );
        }
        else
        {
            fprintf ( stdout, "%8ld : %s", msg->GetTime(), msg->MsgToText ( msgbuf ) );
        }

        if ( msg->IsSystemExclusive() )
        {
            fprintf ( stdout, "SYSEX length: %d", msg->GetSysEx()->GetLengthSE() );
        }

        fprintf ( stdout, "\n" );
    }
}

void DumpMIDITrack( MIDITrack *t )
{
    MIDITimedBigMessage *msg;

    for ( int i = 0; i < t->GetNumEvents(); ++i )
    {
        msg = t->GetEventAddress ( i );
        DumpMIDITimedBigMessage ( msg );
    }
}

void DumpAllTracks( MIDIMultiTrack *mlt )
{
    fprintf ( stdout , "Clocks per beat: %d\n\n", mlt->GetClksPerBeat() );

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

void DumpMIDIMultiTrack( MIDIMultiTrack *mlt )
{
    MIDIMultiTrackIterator i ( mlt );
    const MIDITimedBigMessage *msg;
    fprintf ( stdout , "Clocks per beat: %d\n\n", mlt->GetClksPerBeat() );
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

void PlayDumpSequencer( MIDISequencer *seq )
{
    float pretend_clock_time = 0.0;
    float next_event_time = 0.0;
    MIDITimedBigMessage ev;
    int ev_track;
    seq->GoToTimeMs ( pretend_clock_time );

    if ( !seq->GetNextEventTimeMs ( &next_event_time ) )
    {
        return;
    }

    // simulate a clock going forward with 10 ms resolution for 1 hour
    float max_time = 3600. * 1000.;
    for ( ; pretend_clock_time < max_time; pretend_clock_time += 10. )
    {
        // find all events that came before or a the current time
        while ( next_event_time <= pretend_clock_time )
        {
            if ( seq->GetNextEvent ( &ev_track, &ev ) )
            {
                // found the event!
                // show it to stdout
                fprintf ( stdout, "tm=%06.0f : evtm=%06.0f :trk%02d : ",
                          pretend_clock_time, next_event_time, ev_track );
                DumpMIDITimedBigMessage ( &ev );
                // now find the next message

                if ( !seq->GetNextEventTimeMs ( &next_event_time ) )
                {
                    // no events left so end
                    fprintf ( stdout, "End\n" );
                    return;
                }
            }
        }
    }
}


int main( int argc, char **argv )
{
    if ( argc > 1 )
    {
        const char *infile_name = argv[1];

        MIDIFileReadStreamFile rs( infile_name );
        MIDIMultiTrack tracks;
        MIDIFileReadMultiTrack track_loader( &tracks );
        MIDIFileRead reader( &rs, &track_loader );

        // set amount of tracks equal to midifile
        tracks.ClearAndResize( reader.ReadNumTracks() );

//      MIDISequencerGUIEventNotifierText notifier( stdout );
//      MIDISequencer seq( &tracks, &notifier );
        MIDISequencer seq ( &tracks );

        // load the midifile into the multitrack object
        if ( !reader.Parse() )
        {
            cerr << "\nError parse file " << infile_name << endl;
            return -1;
        }

        if ( argc > 2 )
        {
            cout << endl;
            int mode = atoi ( argv[2] );
            if ( mode == 0 )
            {
                DumpMIDIMultiTrack( &tracks );
            }
            else // mode = 1
            {
                PlayDumpSequencer( &seq );
            }
        }

//      cout << MultiTrackAsText( tracks ); // new util fun

        double dt = seq.GetMisicDurationInSeconds();

        cout << "\nMisic duration = " << dt << endl;
    }
    else
    {
        cerr << "\nusage:\n    jdkmidi_test_sequencer FILE.mid [0 for DumpMIDIMultiTrack]\n";
        cerr <<           "                                    [1 for PlayDumpSequencer]\n";
        return -1;
    }

    return 0;
}

