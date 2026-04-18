#include <Arduino.h>
#include "io.h"
#include "button.h"
#include "arpeggio.h"
#include "midi.h"


Button shapeBtn(SHAPE_BTN, false, INPUT_PULLUP);
Arpeggio arp;

unsigned long arpTime = 0;
int notesPerBeat = 4;
unsigned long arpeggioDelay = 100; // Time delay between each note. 




void setup() {
  Serial.begin(31250);
  pinMode(ARP_HOLD, INPUT_PULLUP);
  pinMode(ARP_OFF, INPUT_PULLUP);
  pinMode(GATE, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(DAC1, OUTPUT);
  pinMode(DAC2, OUTPUT);
  pinMode(ENABLE_MIDI, OUTPUT);
  digitalWrite(GATE, LOW);
  digitalWrite(TRIG, LOW);
  digitalWrite(CLOCK, LOW);
  digitalWrite(DAC1, HIGH);
  digitalWrite(DAC2, HIGH);
  digitalWrite(ENABLE_MIDI, HIGH);

  SPI.begin();
  setVoltage(DAC1, 0, 0, 1023);  
  // setVoltage(DAC1, 1, 1, 000);  // Top jack
  // setVoltage(DAC2, 0, 1, 000);  // Third jack from top
  // setVoltage(DAC2, 1, 1, 000);  // Second jack from top
  arp.setCallbacks(trigNote, stopNote);
  shapeBtn.begin();
}

void loop() {
  if (!digitalRead(ARP_HOLD)){arp.setState(ARP_STATE_HOLD);}
  else if (!digitalRead(ARP_OFF)){arp.setState(ARP_STATE_OFF);}
  else {arp.setState(ARP_STATE_PLAY);}

  shapeBtn.poll();
  if (shapeBtn.singleClick) arp.shapeCycle();
  float bpm = map(analogRead(BPM), 0, 1023, 20, 200);
  arpeggioDelay = 60000.0 / (notesPerBeat * bpm);

  checkMidi();
  updateNote();
  if (millis()-arpTime >= arpeggioDelay){
    arpTime = millis();
    arp.playNext();
  }
}

