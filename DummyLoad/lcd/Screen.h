#ifndef SCREEN_H
#define SCREEN_H


#include "Arduino.h"
#include <LiquidCrystal.h>
extern LiquidCrystal lcd;

class AbstractValue {
  public:
    virtual ~AbstractValue() = 0;
    virtual void up(void);
    virtual void down(void);
    virtual void advanceCursor(void);
    virtual void show(void);
    virtual void showCursor(bool editing);
    
    int posX;
    int posY;
    int nbDigits;
    int DPpos; //nb of significant digits
    bool editable = false;
    int cursorPos = 0;
};

template <class T>
class Value : public AbstractValue {
  public:
    virtual ~Value() {}
    Value(T* v, int x, int y, int digits);
    Value(T* v, int x, int y, int digits, T min, T max);
    void up(void);
    void down(void);
    void advanceCursor(void);
    void show(void);
    void showCursor(bool editing);
    T* val;
    T dispVal;
    T minVal;
    T maxVal;

};


class Screen {
  public:
    Screen(const char* line1, const char* line2, AbstractValue *value0, AbstractValue *value1, AbstractValue *value2, AbstractValue *value3);
    void show(void);
    void enter(void);
    void next(void);
    void prev(void);
    void back(void);
    AbstractValue *values[4];
    bool valueEditing = false;

  private:
    int getNextValue(int dir);
    const char* txt1;
    const char* txt2;
    int selectedValue = -1;
};


#endif
