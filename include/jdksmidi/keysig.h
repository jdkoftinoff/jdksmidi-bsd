
#ifndef JDKSMIDI_KEYSIG_H
#define JDKSMIDI_KEYSIG_H

#include "jdksmidi/midi.h"

namespace jdksmidi
{

enum MIDIAccidentalType
{
    ACCFlat = 0,
    ACCNatural,
    ACCSharp
};

class MIDIKeySignature
{
public:

    MIDIKeySignature();
    MIDIKeySignature ( const MIDIKeySignature &k );

    void Reset();


    bool IsMajor()
    {
        return major;
    }

    void SetSharpFlats ( int sf, bool maj = true )
    {
        sharp_flat = sf;
        major = maj;
        Reset();
    }

    int GetSharpFlats()
    {
        return sharp_flat;
    }

    MIDIAccidentalType GetNoteStatus ( int white_note )
    {
        return state[white_note%7];
    }


    bool ConvertMIDINote ( int in_note, int *out_note );

protected:

    bool ProcessWhiteNote ( int in_note, int *out_note );
    bool ProcessBlackNote ( int in_note, int *out_note );

    MIDIAccidentalType state[7];
    bool  use_sharps;
    int  sharp_flat;
    bool  major;

    static int sharp_list[7];
    static int flat_list[7];


};
}

#endif


