//split.cc
#include "split.h"

Split::Split(int n):Device(0,0,1,2){
  note=n;
}


void Split::getMessage(int n, midi_message_t *ev){
        switch (ev->type) {
            case MIDI_MESSAGE_NOTEON:
            case MIDI_MESSAGE_NOTEOFF:
	      if(ev->byte1<note) {
	        callOutput(0,ev);
	      } else {
	        callOutput(1,ev);
	      }
               break;
            default:
	      callOutput(0,ev);
	      callOutput(1,ev);
        }

}

