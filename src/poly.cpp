//poly.cc
#include "poly.h"

Poly::Poly(int n,bool *na, unsigned int *no):Device(0,0,so,129,mi,1,mo,n){ // pitchbend control[128] midi in midi out[n]
  max_notes=n;
  note_active=na;
  notes=no;
  for(int i=0;i<max_notes;i++) note_active[i]=false;
}

void Poly::getMessage(int n, midi_message_t *ev){
        switch (ev->type) {
            case MIDI_MESSAGE_PITCHBEND: //channel
                  setOutput(0,MAX_VALUE * (ev->byte2 * 256 + ev->byte1) / 8192);
                  //printf("pitchbend\n");
                break;
            case MIDI_MESSAGE_CONTROLLER://channel
                setOutput(1+ev->byte1,MAX_VALUE /127 * ev->byte2 );
                {
                //int param=ev->data.control.param;
                //int value=ev->data.control.value;

                //printf("controller %d %d\n",param,value);
                }
                break;
/*		
            case MIDI_MESSAGE_SYSEX: //no channel
                //printf("sysex %d ",ev->data.ext.len);
                //for(unsigned int i=0;i<ev->data.ext.len;i++) {
                //char *p=static_cast<char *>(ev->data.ext.ptr);
                  //printf("%x ",p[i]);
                //}
                //printf("\n");
                break;
*/
            case MIDI_MESSAGE_NOTEON:
                if(ev->byte2!=0) {
                  for(int i=0; i<max_notes; i++) {
                    if(!note_active[i]) {
		        //printf("%d on\n",i);
                        callOutput(i,ev);
                        note_active[i]=true;
                        notes[i]=ev->byte1;
                        break;
                    }
                  }
                  break;
                }
            case MIDI_MESSAGE_NOTEOFF:
                for(int i=0; i<max_notes; i++) {
                    if (note_active[i] && (notes[i]==ev->byte1)) {
		        //printf("%d off\n",i);
                        note_active[i]=false;
                        callOutput(i,ev);
                    }
                }
                break;
        }
}
/*
Poly::~Poly(){
  delete [] note_active;
  delete [] notes;
}
*/
