//switchSum.cc
#include "switchSum.h"

#define SHIFT 4

SwitchSum::SwitchSum():Device(si,128,so,1,mi,1,0,0){ // in[128] out[128] $in
  for(int i=0; i<128; i++) {
    note[i]=0;
    setOutput(i,0);
  }
}


void SwitchSum::getMessage(int n, midi_message_t *ev){
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

void SwitchSum::cycle() {
    sample out=0;
	for(int i=24; i<120; i++) {
	  if(note[i]) {
	      int nt=i-24;
	      int octave=7-nt/12;
	      int n=nt%12;
	      int pos=octave+n*8;
	      //printf("i %d pos %d value %d\n",i,pos,getInput(pos));
#if SAMPLE_T==0
             out += getInput(pos) / (1<<SHIFT);
#else
	      out += getInput(pos)>>SHIFT;
#endif
	  }
	}
     setOutput(0,out);
}
