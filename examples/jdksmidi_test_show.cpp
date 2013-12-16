
#include "jdksmidi/world.h"
#include "jdksmidi/fileread.h"
#include "jdksmidi/fileshow.h"
using namespace jdksmidi;

#include <iostream>
using namespace std;

int main ( int argc, char **argv )
{
    if ( argc > 1 )
    {
        const char *infile_name = argv[1];

        MIDIFileReadStreamFile rs ( infile_name );
        if ( !rs.IsValid() )
        {
            cerr << "\nError opening file " << infile_name << endl;
            return -1;
        }

        // if true print META_SEQUENCER_SPECIFIC events as text string
        bool sqspecific_as_text = true;

        MIDIFileShow shower ( stdout, sqspecific_as_text );
        MIDIFileRead reader ( &rs, &shower );

        if ( !reader.Parse() )
        {
            cerr << "\nError parse file " << infile_name << endl;
            return -1;
        }
    }
    else
    {
        cerr << "usage:\n\tjdkmidi_test_show INFILE.mid\n";
        return -1;
    }

    return 0;
}
