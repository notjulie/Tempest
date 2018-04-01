SQLite third-party library, and my generic C++ wrapper around it.  sqlite3.c is the amalgamated source for SQLite, and sqlite.c
is a simple wrapper that includes it and tidies up some compiler specific things that sqlite's extensive ifdef's don't quite
account for.  SQLRR contains the source for my wrapper.

The reason for adding SQLite in the first place was that I wanted to be able to store different sets of high scores for
Tempest for different configurations... a high score when you get an extra ship every 10,000 points means something different
from a high score for 20,000 points per extra ship.  And once you see one possible use for a relational database you know
you're likely to find more...
