//device.cc
//#include <cstdio>
//#include <Arduino.h>
#include "device.h"

int Device::rate=SAMPLE_FREQ;
/*
void MArray::insert(int nd, Device *d){
  //SerialUSB.print("insert ");
  //SerialUSB.println(n);
  array[n].n=nd;
  array[n].d=d;
  n++;
  //SerialUSB.println(n);
}
*/

Device::Device(int ni, int no, int nmi, int nmo) {
  inputs=ni?new sample*[ni]:0;
  outputs=no?new sample[no]:0;
  minputs=nmi?new MidiInput[nmi]:0;
  moutputs=nmo?new MidiOutput[nmo]:0;
  for(int i=0;i<no;i++) {
    outputs[i]=0;
  }
  for(int i=0;i<nmo;i++) {
     moutputs[i].d=0;
  }
  initIO(ni,no,nmi,nmo);

  /*
  for(int i=0;i<nmo;i++) {
    moutputs[i].n=0; /////no es necesario
  }
  */
}

void Device::initIO(int ni, int no, int nmi, int nmo) {
  for(int i=0;i<ni;i++) {
    inputs[i]=0;
  }
  for(int i=0;i<no;i++) {
    outputs[i]=0;
  }
  for(int i=0;i<nmi;i++) {
    minputs[i].next=0;
  }
  for(int i=0;i<nmo;i++) {
    moutputs[i].d=0;
  }
}

Device::Device(sample **inp, int ni, sample *oup, int no, MidiInput *minp, int nmi, MidiOutput *moup, int nmo) {
//SerialUSB.begin(9600);
//SerialUSB.println("constructor");
 inputs=inp;
 outputs=oup;
 minputs=minp;
 moutputs=moup;
  for(int i=0;i<ni;i++) {
    inputs[i]=0;
  }
  for(int i=0;i<no;i++) {
    outputs[i]=0;
  }
  for(int i=0;i<nmi;i++) {
    minputs[i].next=0;
  }
  for(int i=0;i<nmo;i++) {
    moutputs[i].d=0;
  }
}

void Device::connect(int input, Device *d, int output) {
  //printf("connect %d to %d\n",input,output);
  inputs[input]=&d->outputs[output];
}

//connect  d->output to this->input
void Device::connectM(int input, Device *d, int output) {
  //printf("connectM %p.input[%d] => %p.output[%d]\n",this,input,d,output); 
  //MidiMessage *mm=new MidiMessage();
     //moutputs[output].n=0;/////////
     Device *p=d->moutputs[output].d;
     int n=d->moutputs[output].n;
     d->moutputs[output].d=this;
     d->moutputs[output].n=input;
     minputs[input].next=p;
     minputs[input].next_n=n;
  //moutputs[output].insert(input,d);
}

void Device::callOutput(int o, midi_message_t *ev) {
     Device *d=moutputs[o].d;
     int n=moutputs[o].n;
     //printf("callOutput\n");
     while(d) {
       //printf("callOutput %d %p\n",o,d);
       d->getMessage(n,ev);
       int next_n=d->minputs[n].next_n;
       d=d->minputs[n].next;
       n=next_n;
     }
     //printf("\n");
/*
     for(int i=0;i<moutputs[o].n;i++) { 
      //Device *d=moutputs[o]->array[i]->d;
      //int n=moutputs[o]->array[i]->n;
      Device *d=moutputs[o].array[i].d;
      int n=moutputs[o].array[i].n;
      //printf("callOutput o %d i %d n %d d %p\n",o,i,n,d);
      if(d) {
           d->getMessage(n,ev);
      }
    }
 */
}


Device::~Device() {
 // delete []inputs;
 // delete []outputs;
}


