//chch.cc
#include "chch.h"

Chch::Chch(int ch):Device(0,0,1,1){
  channel=ch-1;
}


void Chch::getMessage(int n, midi_message_t *ev){
        switch (ev->type) {
            case MIDI_MESSAGE_NOTEON:
            case MIDI_MESSAGE_NOTEOFF:
	          ev->channel=channel;
	          callOutput(0,ev);
        }

}

