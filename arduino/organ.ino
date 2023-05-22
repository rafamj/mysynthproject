#include <I2S.h>
#include "wiring_private.h"

#define LED_BUILTIN 25
#define OCTAVES 8
#define MAX_VALUE 2147483647
#define SAMPLING_FREQ 8000

typedef int sample;

sample out1,out2;
Uart Serial2;
struct osc_t {
  int ticks;
  int rem; //these two values define the frequency of the osc
  int d;
  int delta; 
  int phase;
  sample out[OCTAVES];
  bool on[OCTAVES];
};
//frequency * 1000
int notes[12]={4186009, 4434922, 4698636, 4978032, 5274041, 5587652, 5919911, 6271927, 6644875, 7040000, 7458620, 7902133};

osc_t osc[12];

void SERCOM1_Handler() {
  SAMD21Midi::Serial2.IrqHandler();
}


void openMidi(){
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN,HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN,LOW);

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
        note=Serial2.read();
        int nt=note-24;
        int octave=7-nt/12;
        int n=nt%12;
        osc[n].on[octave]=true;
        Serial2.read(); //volume
        digitalWrite(LED_BUILTIN,HIGH);
      } else if (commandByte == 0x80){ //note off
  //SerialUSB.println("note off ");
        note=Serial2.read();
        int nt=note-24;
        int octave=7-nt/12;
        int n=nt%12;
        osc[n].on[octave]=false;
        Serial2.read();
        digitalWrite(LED_BUILTIN,LOW);
      } else if (commandByte == 0xa0){ //polyphonic aftertouch
        Serial2.read();
        Serial2.read();
        continue;
      } else if (commandByte == 0xb0){ //controller change
        Serial2.read();
        Serial2.read();
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
   if (!I2S.begin(I2S_PHILIPS_MODE, Device::rate, 16)) {
     SerialUSB.begin(9600);
     while (!SerialUSB) ;
    SerialUSB.println("Failed to initialize I2S!");
    while (1); // do nothing
  }
}

void writePCM(){
    int wr=I2S.availableForWrite();
    int buffer[2048]; //every 32 bit int have 2 16 bit samples (left and right)
    int index=0;
    while(index<wr) {
      int s=generateSound();
      buffer[index++]=s;
    }
    I2S.write(buffer,wr);
}


void initOsc() {
  for(int note=0;note<12;note++) {
    osc[note].ticks=(notes[note]/1000)/SAMPLING_FREQ; ////
    osc[note].d=(notes[note]/1000)%SAMPLING_FREQ;
    osc[note].rem=osc[note].d;
    osc[note].delta=0;
    osc[note].phase=0;
    for(int i=0;i<OCTAVES;i++) {
      osc[note].out[i]=MAX_VALUE;
      osc[note].on[i]=false;
    }
  }
}

int generateSound() {
  out1=out2=0;
  for(int note=0;note<12;note++) {
    osc[note].phase++;
    if(osc[note].phase>=osc[note].ticks+osc[note].delta) {
      osc[note].d+=osc[note].rem;
      if(osc[note].d>SAMPLING_FREQ) {
         osc[note].delta=1;
	 osc[note].d-=SAMPLING_FREQ;
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
    for(int i=1; i<OCTAVES; i++) {
      if(osc[note].on[i]) {
        out1 += osc[note].out[i]>>4;
        out2 += osc[note].out[i]>>4;
      }
    }
  }
}

void setup() {
  openMidi();
  openPCM();
  initOsc();
}

void loop() {
  readMidi();
  writePCM();
}
