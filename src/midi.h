#pragma once
#include <Arduino.h>
#include "arpeggio.h"

extern Arpeggio arp;
void checkMidi();
void parseMidi(byte com, byte par1, byte par2);
void noteOn(byte channel, byte note, byte velocity);
void noteOff(byte channel, byte note, byte velocity);

