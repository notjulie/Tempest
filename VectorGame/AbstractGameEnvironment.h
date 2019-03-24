
#ifndef ABSTRACTTEMPESTENVIRONMENT_H
#define ABSTRACTTEMPESTENVIRONMENT_H

#include <stdint.h>
#include <any>
#include <functional>
#include <map>
#include <string>
#include "GameResourceID.h"

class CommandLine;

class AbstractGameEnvironment
{
public:
   typedef std::function<std::string(const CommandLine &)> CommandHandler;

public:
   virtual ~AbstractGameEnvironment(void) {}

	void RegisterCommand(const std::string &name, CommandHandler handler);
   template <typename T, typename U> void RegisterResource(const GameResourceID<T> &id, U value) {
      resources[id.GetName()] = static_cast<T>(value);
   }

   std::string ExecuteCommand(const CommandLine &command);
   template <typename T> T GetResource(const GameResourceID<T> &id) const {
      auto i = resources.find(id.GetName());
      if (i == resources.end())
         return T();

      // make a non-const copy that we can return
      std::any a = (*i).second;
      return std::any_cast<T>(a);
   }

private:
   typedef std::map<std::string, CommandHandler> CommandMap;
   typedef std::map<std::string, std::any> ValueMap;

private:
   CommandMap commands;
   ValueMap resources;
};

#endif
