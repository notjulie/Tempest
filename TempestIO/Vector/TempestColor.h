
#ifndef TEMPESTCOLOR_H
#define TEMPESTCOLOR_H


class TempestColor {
public:
   TempestColor(uint8_t colorRAMValue = 0) {
      this->colorRAMValue = colorRAMValue;
   }

   uint8_t GetR(void) {
      if ((colorRAMValue & 2) == 0)
         return 255;
      else if ((colorRAMValue & 1) == 0)
         return 128;
      else
         return 0;
   }

   uint8_t GetG(void) {
      if ((colorRAMValue & 8) == 0)
         return 255;
      else
         return 0;
   }

   uint8_t GetB(void) {
      if ((colorRAMValue & 4) == 0)
         return 255;
      else
         return 0;
   }

private:
   uint8_t colorRAMValue;
};


#endif

