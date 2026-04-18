#include "arpeggio.h"


Arpeggio::Arpeggio(){

}

void Arpeggio::setCallbacks(StartNoteCallback startNoteCallback, StopNoteCallback stopNoteCallback){
  startNote = startNoteCallback;
  stopNote = stopNoteCallback;
}


void Arpeggio::shapeCycle(){
    arpShape++;
    if (arpShape >= ARPEGGIO_SHAPE_COUNT){arpShape = 0;}
    if (arpState == ARP_STATE_HOLD){
        // TODO: How can we keep held notes while cycling?
    }
    createArpBuffer();
}   

void Arpeggio::setState(ArpState state){
    if (arpState != state){
        arpState = state;
        if (arpState != ARP_STATE_HOLD) {
            stopNote();
            clearAll();
        }
    }
}

void Arpeggio::clearAll(){
  for (int i=0; i<128; i++){
    keyBuffer[i] = 0;
  }
  arpCount = 0;
}

void Arpeggio::playNext(){
  if (arpState != ARP_STATE_OFF){
      // Get next note and play it
      if (arpCount > 0){
        arpIndex++;
        if (arpIndex >= arpCount) {arpIndex = 0;}
        startNote(arpBuffer[arpIndex], 127);
      }
  }
}


void Arpeggio::createArpBuffer(){
    // Creates the arp buffer from the note buffer
    // This is the place where different shapes will be made in the future.
    byte arpIndexNote = arpBuffer[arpIndex]; // Remember current note for seamless playback
    arpCount = 0;
    arpIndex = 0;
    switch (arpShape){
        case 0: // ARP UP
            for (int i=0; i<128; i++){
                if (keyBuffer[i]){
                    arpBuffer[arpCount] = i;
                    arpCount++;
                    if (i == arpIndexNote){arpIndex = i;} // Ah.. this is where I was
                }
            }
        break;
        case 1: // ARP DOWN
            for (int i=127; i>=0; i--){
                if (keyBuffer[i]){
                    arpBuffer[arpCount] = i;
                    arpCount++;
                    if (i == arpIndexNote){arpIndex = i;} // Ah.. this is where I was
                }
            }
        break;
        case 2: // ARP UP-DOWN
            for (int i=0; i<128; i++){
                if (keyBuffer[i]){
                    arpBuffer[arpCount] = i;
                    arpCount++;
                    if (i == arpIndexNote){arpIndex = i;} // Ah.. this is where I was
                }
            }
            arpCount--; // do not repeat last note
            for (int i=127; i>=0; i--){
                if (keyBuffer[i]){
                    arpBuffer[arpCount] = i;
                    arpCount++;
                }
            }
            arpCount--; // do not repeat first note either
        break;
        case 3: // ARP UP 1 OCTAVE UP
            for (int octave = 0; octave<2; octave++){
                for (int i=0; i<128; i++){
                    if (keyBuffer[i]){
                        arpBuffer[arpCount] = i+(12*octave);
                        arpCount++;
                        if (i == arpIndexNote){arpIndex = i;} // Ah.. this is where I was
                    }
                }
            }
        break;
        case 4: // ARP UP 2 OCTAVES UP
            for (int octave = 0; octave<3; octave++){
                for (int i=0; i<128; i++){
                    if (keyBuffer[i]){
                        arpBuffer[arpCount] = i+(12*octave);
                        arpCount++;
                        if (i == arpIndexNote){arpIndex = i;} // Ah.. this is where I was
                    }
                }
            }
        break;
        case 5: // ARP DOWN 1 OCTAVE DOWN
            for (int octave = 0; octave<2; octave++){
                for (int i=127; i>=0; i--){
                    if (keyBuffer[i]){
                        arpBuffer[arpCount] = i-(12*octave);
                        arpCount++;
                        if (i == arpIndexNote){arpIndex = i;} // Ah.. this is where I was
                    }
                }
            }
        break;
        case 6: // ARP DOWN 2 OCTAVES DOWN
            for (int octave = 0; octave<3; octave++){
                for (int i=127; i>=0; i--){
                    if (keyBuffer[i]){
                        arpBuffer[arpCount] = i-(12*octave);
                        arpCount++;
                        if (i == arpIndexNote){arpIndex = i;} // Ah.. this is where I was
                    }
                }
            }
        break;
    }
}

void Arpeggio::addNote(byte note, byte velocity){
    keyBuffer[note] = velocity;
    currentNote = note;
    // Emediate play if arp is off
    if (arpState == ARP_STATE_OFF){
        startNote(note, velocity);
    }
    if (arpState == ARP_STATE_PLAY){
        createArpBuffer(); 
    }
    if (arpState == ARP_STATE_HOLD){
        createArpBuffer();
    }
}

void Arpeggio::removeNote(byte note){
    keyBuffer[note] = 0;
    // Emediate stop if arp is off
    if (arpState == ARP_STATE_OFF){
        if (note == currentNote) stopNote();
    }
    if (arpState == ARP_STATE_PLAY){
        createArpBuffer();
        if (arpCount == 0) stopNote();
    }
    if (arpState == ARP_STATE_HOLD){
    }
}

