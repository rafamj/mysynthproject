//device.cc
#include <cstdio>
#include "device.h"

int Device::rate=44100;
//int Device::rate=48000;

void MArray::insert(MidiMessage *m){
  MidiMessage **ar=new MidiMessage*[n+1];
  for(int i=0;i<n;i++) {
    ar[i]=array[i];
  }
  delete []array;
  ar[n]=m;
  array=ar;
  n++;
}


Device::Device(int ni, int no, int nmi, int nmo) {
  inputs=new sample*[ni];
  outputs= new sample[no];
  moutputs= new MArray*[nmo];
  for(int i=0;i<no;i++) {
    outputs[i]=0;
  }
  for(int i=0;i<nmo;i++) {
    moutputs[i]=new MArray();
  }
}

void Device::connect(int input, Device *d, int output) {
  //printf("connect %d to %d\n",input,output);
  inputs[input]=&d->outputs[output];
}

void Device::connectM(int input, Device *d, int output) {
  MidiMessage *mm=new MidiMessage();
  mm->n=input;
  mm->d=d;
  moutputs[output]->insert(mm);
}

void Device::callOutput(int o, snd_seq_event_t *ev) {
     for(int i=0;i<moutputs[o]->n;i++) { 
      Device *d=moutputs[o]->array[i]->d;
      int n=moutputs[o]->array[i]->n;
      //printf("callOutput o %d i %d n %d d %p\n",o,i,n,d);
      if(d) {
           d->getMessage(n,ev);
      }
    }
}


Device::~Device() {
  delete []inputs;
  delete []outputs;
}


