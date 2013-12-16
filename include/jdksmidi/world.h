
#ifndef JDKSMIDI_WORLD_H
#define JDKSMIDI_WORLD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cassert>

namespace jdksmidi
{

#define ENTER(a)

#define DBG(a)
#define OSTYPE( a,b,c,d )       ((a)*0x1000000 + (b)*0x10000 + (c)*0x100 + (d) )

typedef unsigned char uchar;
typedef unsigned long ulong;
typedef unsigned int uint;
typedef unsigned short ushort;

/*! \mainpage libjdksmidi, a MIDI C++ Library

\section overview Overview

This is the Overview section. The MIDI Library allows one to parse, generate MIDI and Standard
MIDI Files easily.

\section history History



\section theory Theory of Operation

\section limitations Limitations

\section future The Future

*/

#ifdef WIN32
#pragma warning(disable: 4996) // to take away "function was declared deprecated" warnings
#pragma warning(disable: 4355) // 'this' used in base member initializer list
#endif

}

#include "jdksmidi/utils.h"

#endif

