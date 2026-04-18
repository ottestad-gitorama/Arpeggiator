#include "midi.h"



byte midiCom;
byte midiPar1;
byte midiPar2;
byte midiRxCount=0;
void checkMidi(){
  if (Serial.available()){
    byte data=Serial.read();
    if (midiRxCount==2) {
      midiPar2=data; 
      parseMidi(midiCom, midiPar1, midiPar2);
      midiRxCount=0;
    } 
    if (midiRxCount==1) {
      midiPar1=data;
      midiRxCount=2;
      if ((midiCom&240) == 192) { // program change only use two bytes
        parseMidi(midiCom, midiPar1, 0); 
        midiRxCount=0;       
      }
    }
    if (data>=128) {// new command
      midiCom=data;
      midiRxCount=1;
    }
  }
}

void parseMidi(byte com, byte par1, byte par2){
  Serial.print(com);
  Serial.print(" : ");
  Serial.print(par1);
  Serial.print(" : ");
  Serial.println(par2);
  byte channel = com % 16;
  byte command = com / 16;
  // command 8=note off 9=note on
  if (command==9) {
    noteOn(channel, par1, par2);
  }
  if (command==8) {
    noteOff(channel, par1, par2);
  }
  if (command==0x0A) {
    controlChange(channel, par1, par2);
  }
}

void noteOn(byte channel, byte note, byte velocity){
  arp.addNote(note, velocity);
}

void noteOff(byte channel, byte note, byte velocity){
  arp.removeNote(note);
}

void controlChange(byte channel, byte controlNo, byte value){
  switch (controlNo){
    case 50:
      setVoltage(DAC1, 1, 1, map(value, 0, 127, 0, 4000));  // Top jack
    break;
    case 51:
      setVoltage(DAC2, 1, 1, map(value, 0, 127, 0, 4000));   // Second jack from top
    break;
    case 52:
      setVoltage(DAC2, 0, 1, map(value, 0, 127, 0, 4000)); // Third jack from top
    break;   
  }
}