#include "io.h"


// setVoltage -- Set DAC voltage output
// dacpin: chip select pin for DAC.  Note and velocity on DAC1, pitch bend and CC on DAC2
// channel: 0 (A) or 1 (B).  Note and pitch bend on 0, velocity and CC on 2.
// gain: 0 = 1X, 1 = 2X.  
// mV: integer 0 to 4095.  If gain is 1X, mV is in units of half mV (i.e., 0 to 2048 mV).
// If gain is 2X, mV is in units of mV

void setVoltage(int dacpin, bool channel, bool gain, unsigned int mV)
{
  unsigned int command = channel ? 0x9000 : 0x1000;

  command |= gain ? 0x0000 : 0x2000;
  command |= (mV & 0x0FFF);
  
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
  digitalWrite(dacpin,LOW);
  SPI.transfer(command>>8);
  SPI.transfer(command&0xFF);
  digitalWrite(dacpin,HIGH);
  SPI.endTransaction();
}

int mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}


unsigned long trigTimer = 0;
bool retrigGate = true;
#define CV_TRANSPOSE -12
#define NOTE_SF 47.069f // This value can be tuned if CV output isn't exactly 1V/octave
// Rescale 88 notes to 4096 mV:
//    noteMsg = 0 -> 0 mV 
//    noteMsg = 87 -> 4096 mV
// DAC output will be (4095/87) = 47.069 mV per note, and 564.9655 mV per octive
// Note that DAC output will need to be amplified by 1.77X for the standard 1V/octave 
void trigNote(byte note, byte velocity) {
  if (retrigGate){
    digitalWrite(GATE, LOW);
    delay(10);
  }
  digitalWrite(GATE, HIGH);
  digitalWrite(TRIG, HIGH);
  trigTimer = millis();
  unsigned int mV = (unsigned int) ((float) (note+CV_TRANSPOSE) * NOTE_SF + 0.5); 
  setVoltage(DAC1, 0, 1, mV);  // DAC1, channel 0, gain = 2X
}

void stopNote(){
  digitalWrite(GATE, LOW);
}

void updateNote(){
  if ((trigTimer > 0) && (millis() - trigTimer > 20)) { 
    digitalWrite(TRIG,LOW); // Set trigger low after 20 msec 
    trigTimer = 0;  
  }  
}

