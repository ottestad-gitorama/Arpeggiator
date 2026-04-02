#pragma once
#include <Arduino.h>
#include <SPI.h>

#define ARP_HOLD 6
#define ARP_OFF 5
#define SHAPE_BTN 4
#define BPM       A0
#define GATE  2
#define TRIG  3
#define CLOCK 4
#define DAC1  8 
#define DAC2  9
#define ENABLE_MIDI A1


void setVoltage(int dacpin, bool channel, bool gain, unsigned int mV);

void trigNote(byte note, byte velocity);
void stopNote();
void updateNote();

