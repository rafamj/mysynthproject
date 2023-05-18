//alsaSeq.cc
#include <alsa/asoundlib.h>
#include "alsaSeq.h"

AlsaSeq::AlsaSeq():Device(0,0,0,0,mi,1,mo,1){ //in out
  channel=-1;
  deviceName="default";
}

AlsaSeq::AlsaSeq(int ch, const char *devName):Device(0,0,0,0,mi,1,mo,1){ //in out
  channel=ch;
  deviceName=devName;
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
    //printf("in %d out %d\n",portIn,portOut);
  
   //capture keyboard
   connect(20,0);
}

void AlsaSeq::getMessage(int n, midi_message_t *mm){
  snd_seq_event_t ev;
  //ev.type=mm->type;
  switch(mm->type) {
    case MIDI_MESSAGE_NOTEON:
      ev.type=SND_SEQ_EVENT_NOTEON;
      ev.data.note.channel=mm->channel;
      ev.data.note.note=mm->byte1;
      ev.data.note.velocity=mm->byte2;
      break;
    case MIDI_MESSAGE_NOTEOFF: 
      ev.type=SND_SEQ_EVENT_NOTEOFF;
      ev.data.note.channel=mm->channel;
      ev.data.note.note=mm->byte1;
      ev.data.note.velocity=mm->byte2;
      break;
    case MIDI_MESSAGE_CONTROLLER: 
      ev.type=SND_SEQ_EVENT_CONTROLLER;
      ev.data.note.channel=mm->channel;
      ev.data.control.param=mm->byte1;
      ev.data.control.value=mm->byte2;
      break;
  }
  snd_seq_ev_set_source(&ev, portOut); ///port??
  snd_seq_ev_set_subs(&ev);
  snd_seq_ev_set_direct(&ev);
  snd_seq_event_output(handle,&ev);
  snd_seq_drain_output(handle);
  //drain??
}

void AlsaSeq::callback(){
    snd_seq_event_t *ev;
    midi_message_t mm;
    do {
        snd_seq_event_input(handle, &ev);
        switch(ev->type) {
          case SND_SEQ_EVENT_NOTEON:
            mm.type=MIDI_MESSAGE_NOTEON;
            mm.channel=ev->data.note.channel;
            mm.byte1=ev->data.note.note;
            mm.byte2=ev->data.note.velocity;
	    break;
          case SND_SEQ_EVENT_NOTEOFF: 
            mm.type=MIDI_MESSAGE_NOTEOFF;
            mm.channel=ev->data.note.channel;
            mm.byte1=ev->data.note.note;
            mm.byte2=ev->data.note.velocity;
            break;
      case SND_SEQ_EVENT_CONTROLLER: 
            mm.type=MIDI_MESSAGE_CONTROLLER;
        mm.channel=ev->data.control.channel;
        mm.byte1=ev->data.control.param;
        mm.byte2=ev->data.control.value;
            break;
      case SND_SEQ_EVENT_KEYPRESS: 
            mm.type=MIDI_MESSAGE_AFTERTOUCH;
        mm.channel=ev->data.control.channel;
        mm.byte1=ev->data.control.param;
        mm.byte2=ev->data.control.value;
            break;
      case SND_SEQ_EVENT_PITCHBEND: 
            mm.type=MIDI_MESSAGE_PITCHBEND;
        mm.channel=ev->data.control.channel;
        mm.byte1=ev->data.control.param;
        mm.byte2=ev->data.control.value;
        break;
      }
	callOutput(0,&mm);
        snd_seq_free_event(ev);
    } while (snd_seq_event_input_pending(handle, 0) > 0);

}


void AlsaSeq::close(){
  snd_seq_close (handle);
}

AlsaSeq::~AlsaSeq(){
  close();
}
