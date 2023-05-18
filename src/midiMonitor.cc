//midiMonitor.cc
#include <stdio.h>
#include "midiMonitor.h"


void MidiMonitor::getMessage(int n, midi_message_t *ev){
        switch (ev->type) {
            case MIDI_MESSAGE_PITCHBEND:
                printf("channel %d, pitchbend %d\n",ev->channel,256 * ev->byte2 + ev->byte1);
                break;
            case MIDI_MESSAGE_CONTROLLER:
                printf("channel %d, control %d value %d\n",ev->channel,ev->byte1,ev->byte2);
                break;
            case MIDI_MESSAGE_NOTEON:
                  printf("channel %d, note on %d velocity %d\n", ev->channel, ev->byte1,  ev->byte2);
                  break;
            case MIDI_MESSAGE_NOTEOFF:
                  printf("channel %d, note off %d velocity %d\n", ev->channel, ev->byte1,  ev->byte2);
                  break;
	       /*
            case MIDI_MESSAGE_SYSEX:
                printf("sysex %d ",ev->data.ext.len);
                for(unsigned int i=0;i<ev->data.ext.len;i++) {
                char *p=static_cast<char *>(ev->data.ext.ptr);
                  printf("%x ",p[i]);
                }
                printf("\n");
                break;
		*/
            default:
              printf("event type %d\n",ev->type);
        }

}

