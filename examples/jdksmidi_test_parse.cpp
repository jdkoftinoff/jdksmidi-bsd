
#include "jdksmidi/world.h"
#include "jdksmidi/midi.h"
#include "jdksmidi/msg.h"
#include "jdksmidi/sysex.h"
#include "jdksmidi/parser.h"

using namespace jdksmidi;


void PrintSysEx ( FILE *f, MIDISystemExclusive *ex, bool normal_sysex)
{
    int l = ex->GetLength();

    if ( normal_sysex )
    {
        fprintf ( f, "Normal System-Exclusive message Len=%d", l );
    }
    else
    {
        fprintf ( f, "Authorization System-Exclusive message Len=%d", l );
    }

    for ( int i = 0; i < l; ++i )
    {
        if ( ( ( i ) % 20 ) == 0 )
        {
            fprintf ( f, "\n" );
        }

        fprintf ( f, "%02x ", ( int ) ex->GetData ( i ) );
    }

    fprintf ( f, "\n" );
    fflush ( f );
}


void PrintMsg ( FILE *f, MIDIMessage *m )
{
    int l = m->GetLength();
    fprintf ( f, "Msg : " );

    if ( l == 1 )
    {
        fprintf ( f, " %02x \t=", m->GetStatus() );
    }

    else if ( l == 2 )
    {
        fprintf ( f, " %02x %02x \t=", m->GetStatus(), m->GetByte1() );
    }

    else if ( l == 3 )
    {
        fprintf ( f, " %02x %02x %02x \t=", m->GetStatus(), m->GetByte1(), m->GetByte2() );
    }

    char buf[129];
    m->MsgToText ( buf );
    fprintf ( f, "%s\n", buf );
    fflush ( f );
}


int main ( int argc, char ** argv )
{
    fprintf ( stdout, "mdparse:\n" );
    MIDIParser p ( 32 * 1024 );
    MIDIMessage m;
    FILE *f = stdin;

    while ( !feof ( f ) )
    {
        int c = fgetc ( f );

        if ( c == EOF )
            break;

        if ( p.Parse ( ( uchar ) c, &m ) )
        {
            if ( m.IsSystemExclusive() )
            {
                PrintSysEx ( stdout, p.GetSystemExclusive(), m.IsSysExN() );
            }

            else
            {
                PrintMsg ( stdout, &m );
            }
        }
    }

    return 0;
}

