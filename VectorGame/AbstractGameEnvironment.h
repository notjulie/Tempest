// ====================================================================
// Vector game emulation project
//    Author: Randy Rasmussen
//    Copyright: none... do what you will
//    Warranties: none... do what you will at your own risk
//
// File summary:
//    This is a general class for passing info from the game's owner to
//    the game implementation.  The goal is to be as general as possible
//    so that any old vector game can use this without having to get into
//    a big old mess of subclassing.
// ====================================================================


#ifndef ABSTRACTTEMPESTENVIRONMENT_H
#define ABSTRACTTEMPESTENVIRONMENT_H

#include <stdint.h>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include "GameResourceID.h"

class CommandLine;

/// <summary>
/// class AbstractGameEnvironment contains information and resources that games and
/// game owners can share.  It is marked as final; it has all kinds of capability
/// to implement target specific behaviors without inheritence, so it is prefered
/// to keep this class's usage simple.
/// <summary>
class AbstractGameEnvironment final
{
public:
   /// <summary>
   /// footprint of a command handler; game implementers can assign command handlers
   /// to particular text commands so that external callers have a generic way of
   /// commanding the game; very useful for diagnostics and configuration
   /// </summary>
   typedef std::function<std::string(const CommandLine &)> CommandHandler;

private:
   class AbstractResource {
   public:
      virtual ~AbstractResource(void) {}
   };
   template <typename T> class Resource : public AbstractResource {
   public:
      Resource(T value) { this->value = value; }
      virtual ~Resource(void) {}
      T value;
   };

public:
   /// <summary>
   /// assigns the given handler as the handler for a given command
   /// </summary>
	void RegisterCommand(const std::string &name, CommandHandler handler);

   /// <summary>
   /// Registers a resource; for example, Tempest uses this to allow the caller to
   /// pass in a pointer to an optional object that loads and saves high scores.
   /// </summary>
   template <typename T, typename U> void RegisterResource(const GameResourceID<T> &id, U value) {
      resources[id.GetName()].reset(new Resource<T>(static_cast<T>(value)));
   }

   /// <summary>
   /// Executes a command; the command is parsed and passed to the appropriately
   /// registered command handler
   /// </summary>
   std::string ExecuteCommand(const CommandLine &command);

   /// <summary>
   /// Gets a resource previously registered via RegisterResource
   /// </summary>
   template <typename T> T GetResource(const GameResourceID<T> &id) const {
      auto i = resources.find(id.GetName());
      if (i == resources.end())
         return T();

      Resource<T> *r = dynamic_cast<Resource<T> *>((*i).second.get());
      if (r == nullptr)
         throw std::runtime_error("GetResource: invalid type");

      return r->value;
   }

private:
   typedef std::map<std::string, CommandHandler> CommandMap;
   typedef std::map<std::string, std::unique_ptr<AbstractResource>> ResourceMap;

private:
   CommandMap commands;
   ResourceMap resources;
};

#endif
