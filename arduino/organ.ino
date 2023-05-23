#include <I2S.h>
#include "wiring_private.h"

#define LED_BUILTIN 25
#define OCTAVES 8
#define MAX_VALUE 2147483647
#define SAMPLING_FREQ 16000
#define BRIGHTNESS_CONTROL 10

/*
Organ for SAMD21 with PCM5102A DAC

    PCM5102A   SAMD21
    Vcc        5V
    GND        GND
    FLT
    DMP
    SCL        GND
    BCK        D1
    DIN        D9
    LCK        D0
    FMT        GND
    XMT        3.3 V

    MIDI IN  D11
*/


int out1,out2;
int brightness;

Uart Serial2(&sercom1, 11, 10, SERCOM_RX_PAD_0, UART_TX_PAD_2);

struct osc_t {
  int ticks;
  int rem; //these two values define the frequency of the osc
  int d;
  int delta; 
  int phase;
  int freq;
  int out[OCTAVES];
  bool on[OCTAVES];
};
//frequency * 1000
//int notes[12]={4186009, 4434922, 4698636, 4978032, 5274041, 5587652, 5919911, 6271927, 6644875, 7040000, 7458620, 7902133};
int notes[12]={4186, 4434, 4698, 4978, 5274, 5587, 5919, 6271, 6644, 7040, 7458, 7902};

osc_t osc[12];

void SERCOM1_Handler() {
  Serial2.IrqHandler();
}


void openMidi(){
  Serial2.begin(31250);
  pinPeripheral(10, PIO_SERCOM);
  pinPeripheral(11, PIO_SERCOM);
}

void readMidi(){
   while (Serial2.available()>2) {
      byte commandByte = Serial2.read();//read first byte
      byte channel= commandByte & 0xf;
      commandByte = commandByte & 0xf0;
      if (commandByte == 0x90){//if note on message
        byte note=Serial2.read();
        int nt=note-24;
        int octave=7-nt/12;
        int n=nt%12;
	if(octave>=0 && octave<OCTAVES) {
          osc[n].on[octave]=true;
	}
        Serial2.read(); //volume
        digitalWrite(LED_BUILTIN,LOW);
      } else if (commandByte == 0x80){ //note off
  //SerialUSB.println("note off ");
        byte note=Serial2.read();
        int nt=note-24;
        int octave=7-nt/12;
        int n=nt%12;
	if(octave>=0 && octave<OCTAVES) {
          osc[n].on[octave]=false;
	}
        Serial2.read();
        digitalWrite(LED_BUILTIN,HIGH);
      } else if (commandByte == 0xa0){ //polyphonic aftertouch
        Serial2.read();
        Serial2.read();
        continue;
      } else if (commandByte == 0xb0){ //controller change
        byte byte1=Serial2.read();
        byte byte2=Serial2.read();
	if(byte1==BRIGHTNESS_CONTROL) {
	  brightness=byte2;
	}
      } else if (commandByte == 0xe0){ //pitch bend
        Serial2.read();
        Serial2.read();
      } else {
        Serial2.read();
        Serial2.read();
        continue;
      }
  }
}

void openPCM(){
   if (!I2S.begin(I2S_PHILIPS_MODE, SAMPLING_FREQ, 16)) {
    digitalWrite(LED_BUILTIN,LOW);
    while (1); // do nothing
  }
}

void writePCM(){
    int wr=I2S.availableForWrite();
    short int buffer[2048]; 
    int index=0;
    while(index*2<wr) {
      generateSound();
      buffer[index++]=out1>>16;
      buffer[index++]=out2>>16;
    }
    I2S.write(buffer,wr);
}


void initOsc() {
  for(int note=0;note<12;note++) {
    osc[note].freq=notes[note];
    osc[note].ticks=SAMPLING_FREQ/notes[note]; ////
    osc[note].rem=SAMPLING_FREQ%notes[note];
    osc[note].d=0;
    osc[note].delta=0;
    osc[note].phase=0;
    for(int i=0;i<OCTAVES;i++) {
      osc[note].out[i]=MAX_VALUE;
      osc[note].on[i]=false;
    }
  }
}

void generateSound() {
  out1=out2=0;
  for(int note=0;note<12;note++) {
    osc[note].phase++;
    if(osc[note].phase>=osc[note].ticks+osc[note].delta) {
      osc[note].d+=osc[note].rem;
      if(osc[note].d>osc[note].freq) {
         osc[note].delta=1;
         osc[note].d-=osc[note].freq;
      } else {
        osc[note].delta=0;
      }
      osc[note].phase=0;
      osc[note].out[0]*=-1;
      for(int i=1; i<OCTAVES; i++) {
        if(osc[note].out[i-1]==-MAX_VALUE) {
          osc[note].out[i]*=-1;
        } else {
          break;
        }
      }
    }
    for(int i=0; i<OCTAVES; i++) {
      if(osc[note].on[i]) {
        int out=osc[note].out[i]>>1;
        if(i>0) out += brightness * (osc[note].out[i-1]>>8);
        int nt=i*12+note;
        out1 += (out>>10) * (95 -nt);
        out2 += (out>>10) * nt;
      }
    }
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);
  brightness=0;
  openMidi();
  openPCM();
  initOsc();
}

void loop() {
  readMidi();
  writePCM();
}
