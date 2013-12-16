
#ifndef JDKSMIDI_FILEREADMULTITRACK_H
#define JDKSMIDI_FILEREADMULTITRACK_H

#include "jdksmidi/midi.h"
#include "jdksmidi/msg.h"
#include "jdksmidi/sysex.h"
#include "jdksmidi/file.h"
#include "jdksmidi/fileread.h"
#include "jdksmidi/multitrack.h"

namespace jdksmidi
{

class MIDIFileReadMultiTrack : public MIDIFileEvents
{
public:
    MIDIFileReadMultiTrack ( MIDIMultiTrack *mlttrk );

    virtual ~MIDIFileReadMultiTrack();


//
// The possible events in a MIDI Files
//

    virtual bool mf_metamisc ( MIDIClockTime time, int type, int len, unsigned char *data );
    virtual bool mf_timesig ( MIDIClockTime time, int, int, int, int );
    virtual bool mf_tempo ( MIDIClockTime time, unsigned char a, unsigned char b, unsigned char c );
    virtual bool mf_keysig ( MIDIClockTime time, int, int );
    virtual bool mf_sqspecific ( MIDIClockTime time, int, unsigned char * );
    virtual bool mf_text ( MIDIClockTime time, int, int, unsigned char * );
    virtual bool mf_eot ( MIDIClockTime time );
    virtual bool mf_sysex ( MIDIClockTime time, int type, int len, unsigned char *s );

//
// the following methods are to be overridden for your specific purpose
//

    virtual void mf_error ( const char * );
    virtual void mf_starttrack ( int trk );
    virtual void mf_endtrack ( int trk );
    virtual void mf_header ( int, int, int );

//
// Higher level dispatch functions
//

    virtual bool ChanMessage ( const MIDITimedMessage &msg );
    // test and sort events temporal order in all tracks
    virtual void SortEventsOrder();

protected:

    // return false if dest_track absent or no space for event
    bool AddEventToMultiTrack ( const MIDITimedMessage &msg, MIDISystemExclusive *sysex, int dest_track );

    MIDIMultiTrack *multitrack;
    int cur_track;

    int the_format;
    int num_tracks;
    int division;

};

}


#endif
