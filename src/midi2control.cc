//midi2control.cc
#include "midi2control.h"


void Midi2Control::getMessage(int n, snd_seq_event_t *ev){
        if (ev->type==SND_SEQ_EVENT_CONTROLLER) {
                setOutput(ev->data.control.param,ev->data.control.value / 127.0);
		//printf("control %d = %f\n",ev->data.control.param,ev->data.control.value / 127.0);
         }
}
