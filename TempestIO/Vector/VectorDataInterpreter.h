
#ifndef VECTORDATAINTERPRETER_H
#define VECTORDATAINTERPRETER_H

#include <functional>
#include <map>
#include "VectorData.h"

#ifdef _WIN32
   #pragma warning(push)
   #pragma warning(disable : 4820)	// padding in structures
#endif

class VectorGenerator
{
public:
   virtual void Center(void) = 0;
   virtual void LDraw(int x, int y, int intensity) = 0;
   virtual void Scale(int binaryScale, int linearScale) = 0;
   virtual void SDraw(int x, int y, int intensity) = 0;
   virtual void Stat(int colorIndex, int intensity, int color) = 0;
};

class VectorDataInterpreter
{
public:
	VectorDataInterpreter(void);
	virtual ~VectorDataInterpreter(void);

   void Interpret(VectorGenerator *generator);
   void SetVectorData(const VectorData &_vectorData) { this->vectorData = _vectorData; }
	void WriteVectorRAM(uint16_t address, uint8_t value) { vectorData.WriteVectorRAM(address, value); }
   void WriteColorRAM(uint16_t address, uint8_t value) { vectorData.WriteColorRAM(address, value); }
   uint8_t ReadColorRAM(uint16_t address) { return vectorData.ReadColorRAM(address); }
   void RegisterHook(uint16_t address, std::function<uint16_t(uint16_t)> hook);

   // primitives callable by hooks
   void Char(char c);
   uint16_t GetCharSubroutineAddress(char c);
   void Printf(const char *format, ...);

protected:
   void InterpretAt(uint16_t pc);
   uint16_t SingleStep(uint16_t pc);

protected:
	VectorData vectorData;
	bool isHalt = false;
   VectorGenerator *vectorGenerator = nullptr;
   bool hookFlags[16 * 1024];
   std::map<uint16_t, std::function<uint16_t(uint16_t)> > hooks;
};


#ifdef _WIN32
   #pragma warning(pop)
#endif

#endif
