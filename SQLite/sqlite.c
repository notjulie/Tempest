//
//  sqlite.c
//  iTempest
//
//  Created by Randy Rasmussen on 3/31/18.
//  Copyright © 2018 Randy Rasmussen. All rights reserved.
//

// MIN and MAX get multiply defined and the compiler doesn't seem to honor the
// prepreocessor directives that prevent it.  sqlite.c has its own definitions
// for them, so make sure include files that we need to include and then undefine
// MIN and MAX.
#include <sys/param.h>

// undefine things that sqlite3 has its own definitions for
#undef MIN
#undef MAX

// SQLite uses the comma operator a lot... suppress warnings about it
#pragma GCC diagnostic ignored "-Wcomma"

// and it's not perfectly anal about its integer conversions
#pragma GCC diagnostic ignored "-Wconversion"

// this causes problems sometimes on the RPi Eclipse build... disable for
// now until I find a have a reason to want it
#define HAVE_MREMAP 0

// include sqlite
#include "sqlite3.c"
