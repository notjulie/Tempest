
#ifndef LOGICSIGNAL_H
#define LOGICSIGNAL_H


// =============================================================
// ============ LogicOutput ====================================
// =============================================================

class LogicOutput 
{
public:
   bool Get(void) const { return value; }
   void OnValueChanged(const std::function<void(bool)> &callback) { changeCallbacks.push_back(callback); }
   void OnFallingEdge(const std::function<void()> &callback) { fallingEdgeCallbacks.push_back(callback); }
   void OnRisingEdge(const std::function<void()> &callback) { risingEdgeCallbacks.push_back(callback); }

   operator bool() const { return value; }

protected:
   virtual void SetOutputValue(bool value);

private:
   bool value = false;
   std::vector<std::function<void(bool)>> changeCallbacks;
   std::vector<std::function<void()>> fallingEdgeCallbacks;
   std::vector<std::function<void()>> risingEdgeCallbacks;
};



// =============================================================
// ============= LogicInput ====================================
// =============================================================

class LogicInput
{
public:
   void SetSource(LogicOutput &source);
   virtual void Set(bool value) = 0;
};



// =============================================================
// ============ LogicSignal ====================================
// =============================================================

class LogicSignal : public LogicInput, public LogicOutput
{
public:
   void Set(bool value);
};


#endif
