// ====================================================================
// Vector game emulation project
//    Author: Randy Rasmussen
//    Copyright: none... do what you will
//    Warranties: none... do what you will at your own risk
//
// File summary:
//    Contains the implementation of template class GameResourceID
// ====================================================================

#ifndef GAMERESOURCEID_H
#define GAMERESOURCEID_H

/// <summary>
/// This represents a named resource in general; inour context it is used by the
/// AbstractGameEnvironment class to allow callers to add information to the
/// environment that the game can make use of.  Implementing this as a template
/// allows us the ability to declare resource ID's that declare the type of the
/// resource.  This makes for a good, solid, type-safe implementation that's
/// also generic and versatile.
/// </summary>
template <typename T> class GameResourceID {
public:
   /// <summary>
   /// Constructor
   /// </summary>
   GameResourceID(const std::string &name) {
      this->name = name;
   }

   /// <summary>
   /// Name accessor
   /// </summary>
   std::string GetName(void) const {
      return name;
   }

private:
   std::string name;
};

#endif
