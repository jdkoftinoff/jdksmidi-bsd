
#ifndef JDKSMIDI_PARSER_H
#define JDKSMIDI_PARSER_H

#include "jdksmidi/midi.h"
#include "jdksmidi/msg.h"
#include "jdksmidi/sysex.h"

namespace jdksmidi
{

class  MIDIParser
{
public:
    MIDIParser ( ushort max_sysex_size = 384 );
    virtual  ~MIDIParser();

    void  Clear()
    {
        state = FIND_STATUS;
    }

    virtual bool Parse ( uchar b, MIDIMessage *msg );

    MIDISystemExclusive *GetSystemExclusive() const
    {
        return sysex;
    }

protected:

    //
    // The states used for parsing messages.
    //

    enum State
    {
        FIND_STATUS,  // ignore data bytes
        FIRST_OF_ONE,  // read first data byte of a one data byte msg
        FIRST_OF_TWO,  // read first data byte of two data byte msg
        SECOND_OF_TWO,  // read second data byte of two data byte msg
        FIRST_OF_ONE_NORUN, // read one byte message, do not allow
        // running status (for MTC)
        SYSEX_DATA  // read sysex data byte
    };

    MIDIMessage tmp_msg;
    MIDISystemExclusive *sysex;
    State  state;

    bool ParseSystemByte ( uchar b, MIDIMessage *msg );
    bool ParseDataByte ( uchar b, MIDIMessage *msg );
    void ParseStatusByte ( uchar b );
};


}

#endif


