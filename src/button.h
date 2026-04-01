#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button
{
  public:
    int doubleClickTime = 300;
    int uint32_tPressTime = 1000;
    int repeatInterval = 500;
    int debounceTime = 50;
    boolean off;
    boolean on;
    boolean pushed;
    boolean released;
    boolean singleClick;
    boolean secondPush; // the second push of a double click
    boolean doubleClick;
    boolean uint32_tPress;
    boolean repeat;
    Button(boolean _activeState); // init button with no physical input
    Button(int _pin, boolean _activeState, int _mode);
    void begin();
    void poll();
    void poll(bool state); // poll button with no physical input
    void cancelClick(); // cancel a click before making
    boolean activeState;
  private:    
    int pin;
    int mode;
    boolean lastOn;
    boolean debouncing;
    boolean singleClicking;
    boolean singleClickReleasing;
    boolean doubleClicking;
    boolean uint32_tPressing;
    uint32_t startClickTime;
    uint32_t debounceStartTime;
    uint32_t repeatTime;  
};

#endif
