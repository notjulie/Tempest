
#include "stdafx.h"
#include "LogicBoard.h"


void LogicBoard::Connect(LogicOutput &output, LogicInput &input)
{
   // add the connection
   connections[&output] = &input;

   // register for notifications
   LogicInput *inputPointer = &input;
   output.OnValueChanged([this,inputPointer](bool newValue){
      // Enqueue an update... if we call it directly we are likely to end up with
      // recursive calls.  If that happens we could run into issues where the clock
      // changes, one device updates and changes the inputs on another device before
      // that device handles the clock edge.  By enqueuing updates we make sure that
      // a change in one signal propogates its way to everyone before downstream
      // updates occur.
      Update update;
      update.target = inputPointer;
      update.newValue = newValue;
      updates.push(update);
   });

   // set the initial value
   input.Set(output.Get());
}


void LogicBoard::PropogateSignals(void)
{
   // process updates until the queue is empty
   while (!updates.empty())
   {
      Update update = updates.front();
      updates.pop();

      update.target->Set(update.newValue);
   }
}

