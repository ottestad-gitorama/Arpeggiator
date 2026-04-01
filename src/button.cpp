

#include "button.h"

Button::Button(boolean _activeState) {
  Button(-1, _activeState, 0);
}


Button::Button(int _pin, boolean _activeState, int _mode) {
  activeState = _activeState;
  pin = _pin;
  mode = _mode;
}

void Button::begin(){
  if (pin >= 0) {
    pinMode(pin, mode);
  }
}

void Button::cancelClick() {
  singleClicking = false;
}


void Button::poll() {
  boolean state = digitalRead(pin);
  poll(state);
}

void Button::poll(bool state) {
  pushed = false;
  released = false;
  singleClick = false;
  doubleClick = false;
  uint32_tPress = false;
  secondPush = false;
  repeat = false;
  if (debouncing) {
    if (millis() - debounceStartTime > debounceTime) {
      debouncing = false;
    }
  } else {
    off = false;
    on = false;
    // down & up
    if (!activeState) { state = !state; }
    if (state) {
      on = true;
    } else {
      off = true;
    }
    // pushed
    if (on && !lastOn) {
      pushed = true;
      debouncing = true;
      debounceStartTime = millis();
    }
    // released
    if (!on && lastOn) {
      released = true;
      debouncing = true;
      debounceStartTime = millis();
    }
    // doubleClick
    if (pushed && singleClickReleasing) {
      doubleClicking = true;
      singleClickReleasing = false;
      secondPush = true;
    }
    if (released && doubleClicking) {
      doubleClicking = false;
      doubleClick = true;
    }
    // singleClick
    if (pushed && !singleClicking && !doubleClicking) {
      doubleClicking = false;
      startClickTime = millis();
      singleClicking = true;
    }
    if (released && singleClicking) {
      singleClickReleasing = true;
      singleClicking = false;
    }
    if (singleClickReleasing) {
      if (millis() - startClickTime > doubleClickTime) {
        singleClickReleasing = false;
        singleClick = true;
      }
    }
  }
  // uint32_tpress
  if (on && (millis() - startClickTime > uint32_tPressTime)) {
    if (!uint32_tPressing) {
      uint32_tPressing = true;
      uint32_tPress = true;
      singleClicking = false;
      doubleClicking = false;
    }
  }
  if (off) {
    uint32_tPressing = false;
  }
  // repeat
  if (pushed) {
    repeatTime = millis();
    repeat = true;
  }
  if (on && (millis() - repeatTime > repeatInterval)) {
    repeatTime = millis();
    repeat = true;
  }
  lastOn = on;
}
