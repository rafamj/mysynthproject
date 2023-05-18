//midi2cv.cc
#include <math.h>
//#include <Arduino.h>
//#include <cstdio>
#include "midi2cv.h"

Midi2cv::Midi2cv():Device(0,0,so,3,mi,1,0,0){
  notes=0;
  for(int i=0;i<128;i++) {
    frequencies[i]= 8.176 * exp(i*log(2.0)/12.0);
    //printf("%d %d\n",i,frequencies[i]);
  }
}


void Midi2cv::getMessage(int n, midi_message_t *ev){
        switch (ev->type) {
            case MIDI_MESSAGE_NOTEON:
//printf("getMessage %d %d \n",ev->data.note.note,ev->data.note.velocity);
                if(ev->byte2!=0) {
                    //if(ev->data.note.channel) {
		    //freq=8.176 * (exp(getInput(0)*log(2.0)/12.0) + getInput(1));
                        //printf("device %p note %f\n",this,8.176 * exp(ev->byte1*log(2.0)/12.0));
                        setOutput(0, frequencies[ev->byte1]);
                        setOutput(1, MAX_VALUE / 127 * ev->byte2);
                        setOutput(2, MAX_VALUE);
			notes++;
                        //printf("note on  %d %d\n",  ev->byte1,  ev->byte2);
                     //}
                     break;
                }
            case MIDI_MESSAGE_NOTEOFF:
	           notes--;
	           if(notes==0) {
                     //setOutput(0, 0);
                     setOutput(1, 0);
                     setOutput(2, 0);
		   }
               break;
            default:break;
              //printf("event %d\n",ev->type);
        }

}

