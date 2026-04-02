#pragma once
#include <Arduino.h>
#define MAX_ARP_SIZE 128
#define ARPEGGIO_SHAPE_COUNT 7


enum ArpState
{
    ARP_STATE_OFF=0,
    ARP_STATE_PLAY=1,
    ARP_STATE_HOLD=2,
};

class Arpeggio
{
    typedef void (*StartNoteCallback)(uint8_t, uint8_t);
    typedef void (*StopNoteCallback)();
private:
    uint8_t arpBuffer[MAX_ARP_SIZE];
    int arpCount = 0;
    int arpIndex = 0;
    ArpState arpState = ARP_STATE_PLAY; 
    int arpShape = 0;
    uint8_t keyBuffer[128];
    int currentNote = -1;
    StartNoteCallback startNote;
    StopNoteCallback stopNote;
public:
    Arpeggio();
    void setCallbacks(StartNoteCallback startNoteCallback, StopNoteCallback stopNoteCallback);
    void shapeCycle();
    void setState(ArpState state);
    void clearAll();
    void playNext();
    void createArpBuffer();
    void addNote(byte note, byte velocity);
    void removeNote(byte note);
};

