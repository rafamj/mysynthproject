//split.cc
#include "split.h"

Split::Split(int n):Device(0,0,1,2){
  note=n;
}


void Split::getMessage(int n, snd_seq_event_t *ev){
        switch (ev->type) {
            case SND_SEQ_EVENT_NOTEON:
            case SND_SEQ_EVENT_NOTEOFF:
	      if(ev->data.note.note<note) {
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

