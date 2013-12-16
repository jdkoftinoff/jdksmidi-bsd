
#include "jdksmidi/world.h"
#include "jdksmidi/track.h"
#include "jdksmidi/multitrack.h"
#include "jdksmidi/filereadmultitrack.h"
#include "jdksmidi/fileread.h"
#include "jdksmidi/fileshow.h"
#include "jdksmidi/sequencer.h"
#include "jdksmidi/manager.h"
#include "jdksmidi/driverdump.h"

using namespace jdksmidi;

void DumpTrackNames ( MIDISequencer *seq )
{
    fprintf ( stdout, "TEMPO = %f\n",
              seq->GetTrackState ( 0 )->tempobpm
            );

    for ( int i = 0; i < seq->GetNumTracks(); ++i )
    {
        fprintf ( stdout, "TRK #%2d : NAME = '%s'\n",
                  i,
                  seq->GetTrackState ( i )->track_name
                );
    }
}


void PlayDumpManager ( MIDIManager *mgr )
{
    MIDISequencer *seq = mgr->GetSeq();
    double pretend_clock_time = 0.0;
    seq->GoToTime ( ( unsigned long ) pretend_clock_time );
    mgr->SeqPlay();
    // simulate a clock going forward with 10ms resolution for 1 minute

    for ( pretend_clock_time = 0.0; pretend_clock_time < 60.0 * 1000.0; pretend_clock_time += 100 )
    {
        mgr->GetDriver()->TimeTick ( ( unsigned long ) pretend_clock_time );
    }

    mgr->SeqStop();
    mgr->GetDriver()->AllNotesOff();
}


int main ( int argc, char **argv )
{
    if ( argc > 1 )
    {
        MIDIFileReadStreamFile rs ( argv[1] );
        MIDIMultiTrack tracks ( 64 );
        MIDIFileReadMultiTrack track_loader ( &tracks );
        MIDIFileRead reader ( &rs, &track_loader );
        MIDISequencerGUIEventNotifierText gui ( stdout );
        MIDISequencer seq ( &tracks, &gui );
        MIDIDriverDump driver ( 128, stdout );
        MIDIManager mgr ( &driver, &gui );
        reader.Parse();
        seq.GoToZero();
        mgr.SetSeq ( &seq );
        DumpTrackNames ( &seq );
        PlayDumpManager ( &mgr );
    }

    return 0;
}
