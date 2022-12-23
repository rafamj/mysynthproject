//midi2cv.cc
#include <math.h>
#include "midi2cv.h"

Midi2cv::Midi2cv():Device(0,3,1,0){
  notes=0;
}


void Midi2cv::getMessage(int n, snd_seq_event_t *ev){
        switch (ev->type) {
            case SND_SEQ_EVENT_NOTEON:
                if(ev->data.note.velocity!=0) {
                    //if(ev->data.note.channel) {
		    //freq=8.176 * (exp(getInput(0)*log(2.0)/12.0) + getInput(1));

                        setOutput(0, 8.176 * exp(ev->data.note.note*log(2.0)/12.0));
                        setOutput(1, ev->data.note.velocity/127.0);
                        setOutput(2, 1);
			notes++;
                        //printf("note on  %d %d\n",  ev->data.note.note,  ev->data.note.velocity);
                     //}
                     break;
                }
            case SND_SEQ_EVENT_NOTEOFF:
	           notes--;
	           if(notes==0) {
                     setOutput(0, 0);
                     //setOutput(1, 0);
                     setOutput(2, 0);
		   }
               break;
            default:break;
              //printf("event %d\n",ev->type);
        }

}

