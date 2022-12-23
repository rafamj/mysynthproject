//alsaSeqMidi.cc
#include "alsaSeqMidi.h"

AlsaSeqMidi::AlsaSeqMidi(int ch):Device(0,132){ //note vol gate pitchbend control[128]
  channel=ch;
  deviceName="default";
  open();
}

void AlsaSeqMidi::open(){
    if (snd_seq_open(&seq_handle, "default", SND_SEQ_OPEN_DUPLEX, 0) < 0) {
        fprintf(stderr, "Error opening ALSA sequencer.\n");
        exit(1);
    }
    snd_seq_set_client_name(seq_handle, "miniFMsynth");
    if (snd_seq_create_simple_port(seq_handle, "listen:in",
        SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE,
        SND_SEQ_PORT_TYPE_MIDI_GENERIC) < 0) {
        fprintf(stderr, "Error creating sequencer port.\n");
        exit(1);
    }
   
//capture keyboard
    snd_seq_addr_t sender, dest;
    snd_seq_port_subscribe_t *subs;
    sender.client = 20;
    sender.port = 0;
    dest.client = 128;
    dest.port = 0;
    snd_seq_port_subscribe_alloca(&subs);
    snd_seq_port_subscribe_set_sender(subs, &sender);
    snd_seq_port_subscribe_set_dest(subs, &dest);
    snd_seq_port_subscribe_set_queue(subs, 1);
    snd_seq_port_subscribe_set_time_update(subs, 1);
    snd_seq_port_subscribe_set_time_real(subs, 1);
    snd_seq_subscribe_port(seq_handle, subs);

}

void AlsaSeqMidi::callback(){

    snd_seq_event_t *ev;

    do {
        snd_seq_event_input(seq_handle, &ev);
        switch (ev->type) {
            case SND_SEQ_EVENT_PITCHBEND:
	        if(channelOK(ev->data.control.channel)) {
		  setOutput(3,ev->data.control.value / 8192.0);
		}
                break;
            case SND_SEQ_EVENT_CONTROLLER:
	        if(channelOK(ev->data.control.channel)) {
		int param=ev->data.control.param;
		int value=ev->data.control.value;

	        setOutput(4+ev->data.control.param,ev->data.control.value / 127.0);
		printf("controller %d %d\n",param,value);
		}
		break;
            case SND_SEQ_EVENT_SYSEX:
		printf("sysex %d ",ev->data.ext.len);
		for(unsigned int i=0;i<ev->data.ext.len;i++) {
		char *p=static_cast<char *>(ev->data.ext.ptr);
		  printf("%x ",p[i]);
		}
		printf("\n");
		break;
            case SND_SEQ_EVENT_NOTEON:
                if(ev->data.note.velocity!=0) {
		    if(channelOK(ev->data.note.channel)) {
                        setOutput(0, ev->data.note.note);
			setOutput(1, ev->data.note.velocity/127.0);
                        setOutput(2, 1);
                        printf("note on  %d %d\n",  ev->data.note.note,  ev->data.note.velocity);
		     }
                     break;
                }
            case SND_SEQ_EVENT_NOTEOFF:
  	       if(channelOK(ev->data.note.channel)) {
                   setOutput(0, ev->data.note.note);
	           setOutput(1, 0);
                   setOutput(2, 1);
	       }
               break;
	    default:
	      printf("event %d\n",ev->type);
        }
        snd_seq_free_event(ev);
    } while (snd_seq_event_input_pending(seq_handle, 0) > 0);

}


void AlsaSeqMidi::close(){
  snd_seq_close (seq_handle);
}

AlsaSeqMidi::~AlsaSeqMidi(){
  close();
}
