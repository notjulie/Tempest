
#ifndef STDAFX_H
#define STDAFX_H


#ifdef _WIN32
   // disable some warnings that including the standard libraries cause later on
   #pragma warning(disable : 4514)	// unreferenced inlines
   #pragma warning(disable : 4710)	// function not inlined

   // and these are some that I just don't like
   #pragma warning(disable : 4571)  // informational... SEH exceptions not caught by catch (...)
   #pragma warning(disable : 4711)  // informational... function being inlined

   // temporarily disable some warnings that get reported while loading system headers
   #pragma warning(push)
   #pragma warning(disable : 4265)	// no virtual destructor
   #pragma warning(disable : 4350)	// non-const reference thing
   #pragma warning(disable : 4625)	// can't generate copy constructor
   #pragma warning(disable : 4626)	// can't generate assignment operator
   #pragma warning(disable : 4820)	// padding in structures
#endif

// old school C
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// C++
#include <iomanip>
#include <map>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

// restore warning state
#ifdef _WIN32
   #pragma warning(pop)
#endif

#endif
