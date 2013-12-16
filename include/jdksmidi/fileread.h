
#ifndef JDKSMIDI_FILEREAD_H
#define JDKSMIDI_FILEREAD_H

#include "jdksmidi/midi.h"
#include "jdksmidi/msg.h"
#include "jdksmidi/sysex.h"
#include "jdksmidi/file.h"

namespace jdksmidi
{

class MIDIFileReadStream;
class MIDIFileReadStreamFile;
class MIDIFileEvents;
class MIDIFileRead;

/// This class is used internally for reading MIDI files. It is pure virtual and implements a stream of *char*
/// to be read from a MIDI file
class MIDIFileReadStream
{
public:
    MIDIFileReadStream()
    {
    }

    virtual ~MIDIFileReadStream()
    {
    }

    /// To be overriden: rewinds the read pointer
    virtual void Rewind() = 0;

    /// To be overriden: reads a *char* (returning it into an *int*
    virtual int ReadChar() = 0;
};

/// This class is used internally for reading MIDI files. It inherits from MIDIFileWriteStreamFile and writes
/// a stream of *char* to a FILE C object,
class MIDIFileReadStreamFile : public MIDIFileReadStream
{
public:
    /// In this constructor you must specify the filename.\ The constructor tries to open the FILE, you
    /// should call IsValid() for checking if it was successful
    explicit MIDIFileReadStreamFile ( const char *fname )
    {
        f = fopen ( fname, "rb" );
    }

#ifdef WIN32
    explicit MIDIFileReadStreamFile ( const wchar_t *fname )
    {
        f = _wfopen ( fname, L"rb" );
    }
#endif

    /// In this constructor you must specify and already open FILE object
    explicit MIDIFileReadStreamFile ( FILE *f_ ) : f ( f_ )
    {
    }

    /// The destructor closes the file
    virtual ~MIDIFileReadStreamFile()
    {
        if ( f ) fclose ( f );
    }

    virtual void Rewind()
    {
        if ( f ) rewind ( f );
    }

    /// Returns *true* if the FILE was opened
    bool IsValid()
    {
        return f != 0;
    }

    virtual int ReadChar()
    {
        int r = -1;

        if ( f && !feof ( f ) && !ferror ( f ) )
        {
            r = fgetc ( f );
        }

        return r;
    }


private:
    FILE *f;
};

/// This class is used internally for reading MIDI files
class MIDIFileEvents : protected MIDIFile
{
public:
    MIDIFileEvents()
    {
    }

    virtual ~MIDIFileEvents()
    {
    }


//
// The possible events in a MIDI Files
//

    virtual void mf_system_mode ( const MIDITimedMessage &msg );
    virtual void mf_note_on ( const MIDITimedMessage &msg );
    virtual void mf_note_off ( const  MIDITimedMessage &msg );
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
    virtual void UpdateTime ( MIDIClockTime delta_time );
    virtual bool MetaEvent ( MIDIClockTime time, int type, int len, unsigned char *buf );
    virtual bool ChanMessage ( const MIDITimedMessage &msg);
    virtual void SortEventsOrder() {}

};

/// This class inherits from MIDIFile and converts a stream of *char* read from a MIDIFileReadStream
/// object into MIDI data
class MIDIFileRead : protected MIDIFile
{
public:
    /// In the constructor you must specify the MIDIFileReadStream.\ The stream must be alreafy opem
    MIDIFileRead (
        MIDIFileReadStream *input_stream_,
        MIDIFileEvents *event_handler_,
        unsigned long max_msg_len = 8192
    );

    /// The destructor doesn't destroy or close the MIDIFileWriteStream
    virtual ~MIDIFileRead();

    /// Returns false if not enough number of tracks or events in any track
    virtual bool Parse();

    /// \name Utility functions
    //@{
    int ReadNumTracks();

    int GetFormat() const
    {
        return header_format;
    }
    int GetNumTracks() const
    {
        return header_ntrks;
    }
    // call it after Parse(): return true if file contain event(s) with running status
    bool UsedRunningStatus() const
    {
        return used_running_status;
    }
    // return header_division = clock per beat value for range 1...32767
    int GetDivision() const
    {
        return header_division;
    }
    //@}

protected:
    virtual int ReadHeader();
    virtual void mf_error ( const char * );

    MIDIClockTime cur_time;
    int skip_init;
    unsigned long to_be_read;
    int cur_track;
    int abort_parse;

    unsigned char *the_msg;
    int max_msg_len;
    int act_msg_len; // actual msg length

private:
    unsigned long ReadVariableNum();
    unsigned long Read32Bit();
    int Read16Bit();

    void ReadTrack();

    void MsgAdd ( int );
    void MsgInit();

    int EGetC();

    int ReadMT ( unsigned long, int );

    bool FormChanMessage ( unsigned char st, unsigned char b1, unsigned char b2 );
    // reset data for multiple parse
    void Reset();

    bool used_running_status;

    int header_format;
    int header_ntrks;
    int header_division;

    MIDIFileReadStream *input_stream;
    MIDIFileEvents *event_handler;
};
}

#endif

