
#ifndef JDKSMIDI_TICK_H
#define JDKSMIDI_TICK_H

namespace jdksmidi
{

///
/// This is a pure virtual class designed to implement callback functions called in a separate thread at each tick
/// of a timer.
///

class MIDITick
{
public:
    MIDITick()
    {
    }

    virtual ~MIDITick();

    /// The callback function
    virtual void TimeTick ( unsigned long sys_time ) = 0;
};

}

#endif
