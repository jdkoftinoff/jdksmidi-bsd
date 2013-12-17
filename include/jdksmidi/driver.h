
#ifndef JDKSMIDI_DRIVER_H
#define JDKSMIDI_DRIVER_H

#include "jdksmidi/msg.h"
#include "jdksmidi/sysex.h"
#include "jdksmidi/matrix.h"
#include "jdksmidi/process.h"
#include "jdksmidi/queue.h"
#include "jdksmidi/tick.h"

namespace jdksmidi
{

///
/// This is the base class for objects which communicate with the hardware MIDI ports.
/// It is a pure virtual because for effective I/O we need OS details. Currently only two specialized
/// classes are implemented: MIDIDriverWin32 (for MS Windows) and MIDIDriverDump (a dummy driver that only writes
/// its I/O to the screen).
/// The driver is capable of MIDI in and out by mean of two MIDIQueue buffers, and can process incoming and
/// outcoming messages with optional MIDIProcessor (for transposing, muting, etc.). It inherits from pure
/// virtual MIDITick, i.e. a class with a callback method TimeTick() to be called at every timer tick. You can
/// also set a function for further processing.
/// This class is used by the MIDIManager together with a MIDISequencer for playback; AdvancedSequencer uses
/// it too. See the example files.
///

class MIDIDriver : public MIDITick
{

public:

    /// The constructor. It allocates memory for the MIDI queues, and set to NULL the pointers to in, thru and out
    /// processors
    /// \param queue_size the length in bits of the in and out MIDIQueue
    MIDIDriver ( int queue_size );

    /// The destructor frees the memory allocated for the MIDIQueue.\ Eventual MIDIProcessor pointer are not owned.
    virtual ~MIDIDriver();

public: // abstracts
    /// Opens the MIDI in port _id_
    virtual bool OpenMIDIInPort ( int id ) = 0;

    /// Opens the MIDI out port _id_
    virtual bool OpenMIDIOutPort ( int id ) = 0;

    /// Closes the open MIDI in port
    virtual void CloseMIDIInPort() = 0;

    /// Closes the open MIDI out port
    virtual void CloseMIDIOutPort() = 0;

    /// Resets open MIDI out port
    virtual void ResetMIDIOut() = 0;

    /// Start the hardware timer for playing MIDI. Default time resolution is 1 ms
    virtual bool StartTimer ( int resolution_ms ) = 0;

    /// Stops the hardware timer
    virtual void StopTimer() = 0;

	/// Use system's sleep method
	virtual void Sleep(unsigned int ms) = 0;

public:
    /// Empties the in and out MIDIQueue and the MIDIMatrix
    virtual void Reset();

    /// Returns the midi in queue
    MIDIQueue * InputQueue()
    {
        return &in_queue;
    }

    const MIDIQueue * InputQueue() const
    {
        return &in_queue;
    }

    /// Returns the midi out queue
    MIDIQueue * OutputQueue()
    {
        return &out_queue;
    }

    const MIDIQueue * OutputQueue() const
    {
        return &out_queue;
    }

    /// Returns true if the output queue is not full
    bool CanOutputMessage() const
    {
        return out_queue.CanPut();
    }

    /// Processes the message _nsg_ with the out processor and the midi matrix and then puts it
    /// in the out_queue. The message is effectively sent to the MIDI hardware by the TimeTick() callback
    void OutputMessage ( MIDITimedBigMessage &msg )
    {
        if ( ( out_proc && out_proc->Process ( &msg ) ) || !out_proc )
        {
            out_matrix.Process ( msg );
            out_queue.Put ( msg );
        }
    }

    /// Enables the MIDI thru (incoming MIDI messages are echoed to the MIDI out)
    void SetThruEnable ( bool f )
    {
        thru_enable = f;
    }

    /// Returns *true* if MIDI thru is enabled
    bool GetThruEnable() const
    {
        return thru_enable;
    }

    /// Sets the thru MIDIprocessor
    void SetThruProcessor ( MIDIProcessor *proc )
    {
        thru_proc = proc;
    }

    /// Sets the out MIDIProcessor
    void SetOutProcessor ( MIDIProcessor *proc )
    {
        out_proc = proc;
    }

    /// Sets the in MIDIProcessor
    void SetInProcessor ( MIDIProcessor *proc )
    {
        in_proc = proc;
    }

    /// Sets an additional user procedure that can process MIDI data. You should create your own class,
    /// inherited from MIDITick, implementing the pure virtual TimeTick(). Then call this with a pointer
    /// to your instance and your TimeTick() will be called by the driver TimeTick() ( MIDIDriver also inherits
    /// from MIDITick ), allowing you to process incoming and outcoming data.
    void SetTickProc ( MIDITick *tick )
    {
        tick_proc = tick;
    }

    /// Sends a MIDI ALL NOTES OFF message on selected midi chanel
    void AllNotesOff ( int chan );

    /// Sends a MIDI ALL NOTES OFF message on all midi channels
    void AllNotesOff();

    /// This should be called (by an OS originated callback or by TimeTick() function) every time a MIDI message
    /// comes to the system (and you want get it).
    /// Your function should parse raw bytes, arrange them into a MIDITimedBigMessage and call this,
    /// which processes the message with the in MIDIProcessor, manages the MIDI thru and at last put it
    /// into the in queue
    /// \see TimeTick()
    virtual bool HardwareMsgIn ( MIDITimedBigMessage &msg );

    /// This function must be overriden by a subclass and it is responsible to sending the MIDI message
    /// _msg_ to the hardware ports.
    virtual bool HardwareMsgOut ( const MIDITimedBigMessage &msg ) = 0;

    /// This is the TimeTick (callback) procedure inherited from MIDITick.
    /// It currently runs the additional tick procedure (see SetTickProc()), then gets messages from the out queue
    /// and calls HardwareMsgOut() for them. You can use the tick procedure (or override this function) if you
    /// need to poll MIDI in hardware: your function should poll the hardware, parse the bytes, form a message,
    /// and give it to HardwareMsgIn()
    /// \note This class gives no callback mechanism: it must be implemented in subclasses accordingly OS details
    virtual void TimeTick ( unsigned long sys_time );


protected:

    MIDIQueue in_queue;             ///< The in queue
    MIDIQueue out_queue;            ///< The out queue

    MIDIProcessor *in_proc;         ///< The in processor
    MIDIProcessor *out_proc;        ///< The out processor
    MIDIProcessor *thru_proc;       ///< The thru processor

    bool thru_enable;               ///< *true* if thru is enabled

    MIDITick *tick_proc;            ///< An additional TimeTick object: see SetTickProc()

    MIDIMatrix out_matrix;          ///< Keeps track of notes on going to MIDI out
};

// TODO: (note to Jeff by NC) the out_matrix has no functions to get it, and it is currently unused by
// other classes. If we don't want to eliminate it, we could introduce an enable/disable processing
// feature to speed up

}

#endif
