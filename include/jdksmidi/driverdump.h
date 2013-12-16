
#ifndef JDKSMIDI_DRIVERDUMP_H
#define JDKSMIDI_DRIVERDUMP_H

#include "jdksmidi/driver.h"

namespace jdksmidi
{

///
/// This class inherits from the pure virtual MIDIDriver and it is designed for debugging purposes.
/// It doesn't send messages to MIDI ports, but prints them to a FILE object as they come, so you
/// can examine what is exactly sent the driver
///

class MIDIDriverDump : public MIDIDriver
{

public:

    /// The constructor
    MIDIDriverDump ( int queue_size, FILE *outfile );

    /// The destructor
    virtual ~MIDIDriverDump();

    /// Prints the MIDI message _msg_ to the file
    virtual bool HardwareMsgOut ( const MIDITimedBigMessage &msg );

    /// Callback function
    virtual void TimeTick ( unsigned long sys_time );

protected:

    FILE *f;            ///< The output file
};

}

#endif
