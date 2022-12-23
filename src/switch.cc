//switch.cc
#include "switch.h"

Switch::Switch():Device(128,128,1,0){ // in[128] out[128] $in
  for(int i=0; i<128; i++) {
    note[i]=0;
    setOutput(i,0);
  }
}


void Switch::getMessage(int n, snd_seq_event_t *ev){
        if(ev->type==SND_SEQ_EVENT_NOTEON || ev->type==SND_SEQ_EVENT_NOTEOFF) {
	  if(ev->data.note.note>=24 && ev->data.note.note<=119) {
	      if(ev->type==SND_SEQ_EVENT_NOTEON) {
	        note[ev->data.note.note]=1; 
	      } else {
	        note[ev->data.note.note]=0; 
	      }
	  }
	}
}

void Switch::cycle() {
	for(int i=24; i<120; i++) {
	  if(note[i]) {
	      int nt=i-24;
	      int octave=7-nt/12;
	      int n=nt%12;
	      int pos=octave+n*8;
	    setOutput(i,getInput(pos));
	  }
	}
}
