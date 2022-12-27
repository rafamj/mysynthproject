//chch.cc
#include "chch.h"

Chch::Chch(int ch):Device(0,0,1,1){
  channel=ch-1;
}


void Chch::getMessage(int n, snd_seq_event_t *ev){
        switch (ev->type) {
            case SND_SEQ_EVENT_NOTEON:
            case SND_SEQ_EVENT_NOTEOFF:
	          ev->data.note.channel=channel;
	          callOutput(0,ev);
        }

}

