//SAMD21Midi.cc
#include "SAMD21Midi.h"

#define LED_BUILTIN 25

Uart SAMD21Midi::Serial2(&sercom1, 11, 10, SERCOM_RX_PAD_0, UART_TX_PAD_2);


SAMD21Midi::SAMD21Midi(int ch):Device(0,0,0,0,mi,1,mo,1){ 
  channel=ch;
  //open();
}

void SERCOM1_Handler() {
  SAMD21Midi::Serial2.IrqHandler();
}


void SAMD21Midi::open(){
  //initIO(0,0,1,1,this);
  pinMode(LED_BUILTIN, OUTPUT);

  //SerialUSB.begin(9600);
    //while (!SerialUSB) ;

  digitalWrite(LED_BUILTIN,HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN,LOW);
  //SerialUSB.println("begin2");
  
  Serial2.begin(31250);
  pinPeripheral(10, PIO_SERCOM);
  pinPeripheral(11, PIO_SERCOM);
  
  //SerialUSB.println("begin3");
}

void SAMD21Midi::getMessage(int n, midi_message_t *mm){
/*
  snd_seq_event_t ev;
  ev.type=mm->type;
  switch(mm->type) {
    case MIDI_MESSAGE_NOTEON:
    case MIDI_MESSAGE_NOTEOFF: 
      ev.data.note.channel=mm->data.note.channel;
      ev.data.note.note=mm->data.note.note;
      ev.data.note.velocity=mm->data.note.velocity;
      break;
    case MIDI_MESSAGE_CONTROLLER: 
      ev.data.control.channel=mm->data.control.channel;
      ev.data.control.param=mm->data.control.param;
      ev.data.control.value=mm->data.control.value;
      break;
  }
  snd_seq_ev_set_source(&ev, portOut); ///port??
  snd_seq_ev_set_subs(&ev);
  snd_seq_ev_set_direct(&ev);
  snd_seq_event_output(handle,&ev);
  snd_seq_drain_output(handle);
  */
  //drain??
}

void SAMD21Midi::callback(){
   while (Serial2.available()>2) {
      midi_message_t mm;
      byte commandByte = Serial2.read();//read first byte
      byte channel= commandByte & 0xf;
      commandByte = commandByte & 0xf0;
      if (commandByte == 0x90){//if note on message
  //SerialUSB.println("note on ");
        mm.channel = channel;
        mm.byte1=Serial2.read();
        mm.byte2=Serial2.read();
        mm.type=mm.byte1?MIDI_MESSAGE_NOTEON:MIDI_MESSAGE_NOTEOFF;
	if(mm.byte2) {
        digitalWrite(LED_BUILTIN,HIGH);
	} else {
        digitalWrite(LED_BUILTIN,LOW);
	}
        //note=8.176 * exp(noteByte*log(2.0)/12.0);
      } else if (commandByte == 0x80){ //note off
  //SerialUSB.println("note off ");
        mm.channel = channel;
        mm.type=MIDI_MESSAGE_NOTEOFF;
        mm.byte1=Serial2.read();
        mm.byte2=Serial2.read();
        //note=1;
        digitalWrite(LED_BUILTIN,LOW);
      } else if (commandByte == 0xa0){ //polyphonic aftertouch
  //SerialUSB.println("aftertouch ");
        mm.channel = channel;
        mm.type=MIDI_MESSAGE_AFTERTOUCH;
        mm.byte1=Serial2.read();
        mm.byte2=Serial2.read();
	continue;
      } else if (commandByte == 0xb0){ //controller change
  //SerialUSB.print("controller change ");

        mm.channel = channel;
        mm.type=MIDI_MESSAGE_CONTROLLER;
        mm.byte1=Serial2.read();
        mm.byte2=Serial2.read();
  //SerialUSB.println(mm.byte1);	
      } else if (commandByte == 0xe0){ //pitch bend
  //SerialUSB.println("pitch bend ");
        mm.channel = channel;
        mm.type=MIDI_MESSAGE_PITCHBEND;
        mm.byte1=Serial2.read();
        mm.byte2=Serial2.read();
      } else {
  //SerialUSB.print("type ");
  //SerialUSB.println(commandByte,HEX);
        mm.type=MIDI_MESSAGE_UNKNOWN;
        mm.byte1=Serial2.read();
        mm.byte2=Serial2.read();
	continue;
      }
      callOutput(0,&mm);    
    
  } 
}


void SAMD21Midi::close(){
}

SAMD21Midi::~SAMD21Midi(){
  close();
}
