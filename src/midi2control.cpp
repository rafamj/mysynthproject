//midi2control.cc
#include "midi2control.h"


void Midi2Control::getMessage(int n, midi_message_t *ev){
        if (ev->type==MIDI_MESSAGE_CONTROLLER) {
                setOutput(ev->byte2,ev->byte2 / 127.0);
		//printf("control %d = %f\n",ev->data.control.param,ev->data.control.value / 127.0);
         }
}
