//midiMonitor.cc
#include "midiMonitor.h"


void MidiMonitor::getMessage(int n, snd_seq_event_t *ev){
        switch (ev->type) {
            case SND_SEQ_EVENT_PITCHBEND:
                printf("channel %d, pitchbend %d\n",ev->data.control.channel,ev->data.control.value);
                break;
            case SND_SEQ_EVENT_CONTROLLER:
                printf("channel %d, control %d value %d\n",ev->data.control.channel,ev->data.control.param,ev->data.control.value);
                break;
            case SND_SEQ_EVENT_NOTEON:
                  printf("channel %d, note on %d velocity %d\n", ev->data.note.channel, ev->data.note.note,  ev->data.note.velocity);
                  break;
            case SND_SEQ_EVENT_NOTEOFF:
                  printf("channel %d, note off %d velocity %d\n", ev->data.note.channel, ev->data.note.note,  ev->data.note.velocity);
                  break;
            case SND_SEQ_EVENT_SYSEX:
                printf("sysex %d ",ev->data.ext.len);
                for(unsigned int i=0;i<ev->data.ext.len;i++) {
                char *p=static_cast<char *>(ev->data.ext.ptr);
                  printf("%x ",p[i]);
                }
                printf("\n");
                break;
            default:
              printf("event type %d\n",ev->type);
        }

}

