//switch.cc
#include "switch.h"

Switch::Switch():Device(si,128,so,128,mi,1,0,0){ // in[128] out[128] $in
  for(int i=0; i<128; i++) {
    note[i]=0;
    setOutput(i,0);
  }
}


void Switch::getMessage(int n, midi_message_t *ev){
        if(ev->type==MIDI_MESSAGE_NOTEON || ev->type==MIDI_MESSAGE_NOTEOFF) {
	  if(ev->byte1>=24 && ev->byte1<=119) {
	      if(ev->type==MIDI_MESSAGE_NOTEON) {
	        note[ev->byte1]=1; 
	      } else {
	        note[ev->byte1]=0; 
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
	      //printf("i %d pos %d value %d\n",i,pos,getInput(pos));
	    setOutput(i,getInput(pos));
	  } else {
	    setOutput(i,0);
	  }
	}
}
