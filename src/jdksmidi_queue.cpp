
#include "jdksmidi/world.h"
#include "jdksmidi/queue.h"

namespace jdksmidi
{

MIDIQueue::MIDIQueue ( int num_msgs )
    :
    buf ( new MIDITimedBigMessage[ num_msgs ] ),
    bufsize ( num_msgs ),
    next_in ( 0 ),
    next_out ( 0 )
{
}


MIDIQueue::~MIDIQueue()
{
    jdks_safe_delete_array( buf );
}

void MIDIQueue::Clear()
{
    next_in = 0;
    next_out = 0;
}

bool MIDIQueue::CanPut() const
{
    return next_out != ( ( next_in + 1 ) % bufsize );
}

bool MIDIQueue::CanGet() const
{
    return next_in != next_out;
}



}
