
#ifndef JDKSMIDI_FILESHOW_H
#define JDKSMIDI_FILESHOW_H

#include "jdksmidi/fileread.h"

namespace jdksmidi
{

class  MIDIFileShow : public MIDIFileEvents
{
public:
    MIDIFileShow ( FILE *out_, bool sqspecific_as_text_ = false );
    virtual ~MIDIFileShow();

protected:

    virtual void show_time ( MIDIClockTime time );

    virtual void mf_error ( const char * );

    virtual void mf_starttrack ( int trk );
    virtual void mf_endtrack ( int trk );
    virtual void mf_header ( int, int, int );


//
// The possible events in a MIDI Files
//

    virtual void mf_system_mode ( const MIDITimedMessage &msg );
    virtual void mf_note_on ( const MIDITimedMessage &msg );
    virtual void mf_note_off ( const MIDITimedMessage &msg );
    virtual void mf_poly_after ( const MIDITimedMessage &msg );
    virtual void mf_bender ( const MIDITimedMessage &msg );
    virtual void mf_program ( const MIDITimedMessage &msg );
    virtual void mf_chan_after ( const MIDITimedMessage &msg );
    virtual void mf_control ( const MIDITimedMessage &msg );

    virtual bool mf_metamisc ( MIDIClockTime time, int type, int len, unsigned char *data );
    virtual bool mf_timesig ( MIDIClockTime time, int, int, int, int );
    virtual bool mf_tempo ( MIDIClockTime time, unsigned char a, unsigned char b, unsigned char c );
    virtual bool mf_keysig ( MIDIClockTime time, int, int );
    virtual bool mf_sqspecific ( MIDIClockTime time, int, unsigned char * );
    virtual bool mf_text ( MIDIClockTime time, int, int, unsigned char * );
    virtual bool mf_eot ( MIDIClockTime time );
    virtual bool mf_sysex ( MIDIClockTime time, int type, int len, unsigned char *s );

    FILE *out;
    int division;

private:
    bool sqspecific_as_text; // if true print META_SEQUENCER_SPECIFIC events as text string

};

}
#endif

