// ====================================================================
// Tempest emulation project
//    Author: Randy Rasmussen
//    Copyright: none... do what you will
//    Warranties: none... do what you will at your own risk
//
// File summary:
//    Contains definitions of vector objects to be displayed on the
//    screen.  Of course vector games rely on lines, lines and more
//    lines, but I allow for extensions, because of course I do.
// ====================================================================


#ifndef DISPLAYVECTOR_H
#define DISPLAYVECTOR_H


#ifdef _WIN32
   #pragma warning(push)
   #pragma warning(disable : 4820)	// padding in structures
#endif

struct LineVector {
   int16_t startX;
   int16_t startY;
   int16_t endX;
   int16_t endY;
   uint8_t r, g, b;
};

struct DisplayVector
{
   enum {
      Line,
      Menu
   } type = Line;

   union {
      LineVector line;
   };
};

#ifdef _WIN32
   #pragma warning(pop)
#endif

#endif
