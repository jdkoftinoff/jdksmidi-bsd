
#include "jdksmidi/world.h"
#include "jdksmidi/track.h"
#include "jdksmidi/multitrack.h"
#include "jdksmidi/filereadmultitrack.h"
#include "jdksmidi/fileread.h"
#include "jdksmidi/fileshow.h"
#include "jdksmidi/sequencer.h"
#include "jdksmidi/manager.h"
#include "jdksmidi/driver.h"
#include "jdksmidi/driverwin32.h"

using namespace jdksmidi;

int main ( int argc, char **argv )
{
#ifdef WIN32

    if ( argc > 1 )
    {
        MIDIFileReadStreamFile rs ( argv[1] );
        MIDIMultiTrack tracks ( 64 );
        MIDIFileReadMultiTrack track_loader ( &tracks );
        MIDIFileRead reader ( &rs, &track_loader );
        MIDISequencerGUIEventNotifierText gui ( stdout );
        MIDISequencer seq ( &tracks, &gui );
        MIDIDriverWin32 driver ( 128 );
        MIDIManager mgr ( &driver, &gui );
        reader.Parse();
        driver.StartTimer ( 20 );
        driver.OpenMIDIOutPort ( MIDI_MAPPER );
        seq.GoToZero();
        mgr.SetSeq ( &seq );
        mgr.SetTimeOffset ( timeGetTime() );
        mgr.SeqPlay();
        getchar();
        mgr.SeqStop();
    }

    return 0;
#endif
}


