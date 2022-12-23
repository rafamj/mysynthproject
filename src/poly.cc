//poly.cc
#include "poly.h"

Poly::Poly(int n):Device(0,129,1,n){ // pitchbend control[128] midi in midi out[n]
  max_notes=n;
  note_active= new bool[n];
  notes=new unsigned int[n];
}

void Poly::getMessage(int n, snd_seq_event_t *ev){
        switch (ev->type) {
            case SND_SEQ_EVENT_PITCHBEND: //channel
                  setOutput(0,ev->data.control.value / 8192.0);
                  //printf("pitchbend\n");
                break;
            case SND_SEQ_EVENT_CONTROLLER://channel
                setOutput(1+ev->data.control.param,ev->data.control.value / 127.0);
                {
                //int param=ev->data.control.param;
                //int value=ev->data.control.value;

                //printf("controller %d %d\n",param,value);
                }
                break;
            case SND_SEQ_EVENT_SYSEX: //no channel
                //printf("sysex %d ",ev->data.ext.len);
                //for(unsigned int i=0;i<ev->data.ext.len;i++) {
                //char *p=static_cast<char *>(ev->data.ext.ptr);
                  //printf("%x ",p[i]);
                //}
                //printf("\n");
                break;

            case SND_SEQ_EVENT_NOTEON:
                if(ev->data.note.velocity!=0) {
                  for(int i=0; i<max_notes; i++) {
                    if(!note_active[i]) {
                        callOutput(i,ev);
                        note_active[i]=true;
                        notes[i]=ev->data.note.note;
                        break;
                    }
                  }
                  break;
                }
            case SND_SEQ_EVENT_NOTEOFF:
                for(int i=0; i<max_notes; i++) {
                    if (note_active[i] && (notes[i]==ev->data.note.note)) {
                        note_active[i]=false;
                        callOutput(i,ev);
                    }
                }
                break;
        }
}

Poly::~Poly(){
  delete [] note_active;
  delete [] notes;
}
