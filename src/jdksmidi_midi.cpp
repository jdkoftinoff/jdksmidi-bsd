
#include "jdksmidi/world.h"

#include "jdksmidi/midi.h"

namespace jdksmidi
{

const int lut_msglen[16] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    3, // 0x80=note off, 3 bytes
    3, // 0x90=note on, 3 bytes
    3, // 0xA0=poly pressure, 3 bytes
    3, // 0xB0=control change, 3 bytes
    2, // 0xC0=program change, 2 bytes
    2, // 0xD0=channel pressure, 2 bytes
    3, // 0xE0=pitch bend, 3 bytes
    -1 // 0xF0=other things. may vary.
};

const int lut_sysmsglen[16] =
{
// System Common Messages
    -1, // 0xF0=Normal SysEx Events start. may vary
    2, // 0xF1=MIDI Time Code. 2 bytes
    3, // 0xF2=MIDI Song position. 3 bytes
    2, // 0xF3=MIDI Song Select. 2 bytes.
    0, // 0xF4=undefined. (Reserved)
    0, // 0xF5=undefined. (Reserved)
    1, // 0xF6=TUNE Request. 1 byte
    0, // 0xF7=Normal or Divided SysEx Events end.
// -1, // 0xF7=Divided or Authorization SysEx Events. may vary

// System Real-Time Messages
    1, // 0xF8=timing clock. 1 byte
    1, // 0xF9=proposed measure end? (Reserved)
    1, // 0xFA=start. 1 byte
    1, // 0xFB=continue. 1 byte
    1, // 0xFC=stop. 1 byte
    0, // 0xFD=undefined. (Reserved)
    1, // 0xFE=active sensing. 1 byte
//  1, // 0xFF=reset. 1 byte

    3  // 0xFF=not reset, but a META-EVENT, which is always 3 bytes
    // TODO@VRM // not valid jet? see comment to midi.h function:
    // inline int GetSystemMessageLength ( unsigned char stat )
};


const bool lut_is_white[12] =
{
//  C C#  D D#  E  F F#  G G#  A A#  B
    1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1
};



}
