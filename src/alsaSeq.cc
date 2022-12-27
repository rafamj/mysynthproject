//alsaSeq.cc
#include "alsaSeq.h"

AlsaSeq::AlsaSeq(int ch, const char *devName):Device(0,0,1,1){ //in out
  channel=ch;
  deviceName=devName;
  open();
}

void AlsaSeq::connect(int client, int port) {
//capture keyboard
    snd_seq_addr_t sender, dest;
    snd_seq_port_subscribe_t *subs;
    sender.client = client;
    sender.port = port;
    dest.client = snd_seq_client_id(handle);
    dest.port = portIn;
    snd_seq_port_subscribe_alloca(&subs);
    snd_seq_port_subscribe_set_sender(subs, &sender);
    snd_seq_port_subscribe_set_dest(subs, &dest);
    snd_seq_port_subscribe_set_queue(subs, 1);
    snd_seq_port_subscribe_set_time_update(subs, 1);
    snd_seq_port_subscribe_set_time_real(subs, 1);
    snd_seq_subscribe_port(handle, subs);

}

void AlsaSeq::open(){
    if (snd_seq_open(&handle, deviceName, SND_SEQ_OPEN_DUPLEX, 0) < 0) {
        fprintf(stderr, "Error opening ALSA sequencer.\n");
        exit(1);
    }
    snd_seq_set_client_name(handle, "modular");
      if ((portIn=snd_seq_create_simple_port(handle, "listen:in",
        SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE,
        SND_SEQ_PORT_TYPE_MIDI_GENERIC)) < 0) {
        fprintf(stderr, "Error creating sequencer in port.\n");
        exit(1);
      }
    if ((portOut=snd_seq_create_simple_port(handle, "write:out",
        SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ,
        SND_SEQ_PORT_TYPE_MIDI_GENERIC)) < 0) {
        fprintf(stderr, "Error creating sequencer out port.\n");
        exit(1);
    }
    printf("in %d out %d\n",portIn,portOut);
  
   //capture keyboard
   connect(20,0);
}

void AlsaSeq::getMessage(int n, snd_seq_event_t *ev){
  snd_seq_ev_set_source(ev, portOut); ///port??
  snd_seq_ev_set_subs(ev);
  snd_seq_ev_set_direct(ev);
  snd_seq_event_output(handle,ev);
  snd_seq_drain_output(handle);
  //drain??
}

void AlsaSeq::callback(){

    snd_seq_event_t *ev;

    do {
        snd_seq_event_input(handle, &ev);
	callOutput(0,ev);
        snd_seq_free_event(ev);
    } while (snd_seq_event_input_pending(handle, 0) > 0);

}


void AlsaSeq::close(){
  snd_seq_close (handle);
}

AlsaSeq::~AlsaSeq(){
  close();
}
