#include <math.h>
#include <alsa/asoundlib.h>
#include "alsa/alsaSeq.h"
#include "alsa/alsaOut.h"
#include "alsa/alsaIn.h"
#include "deviceSet.h"
#include "parser/parser.h"

int CHANNEL= -1; 

  DeviceSet *devices=new DeviceSet;
  AlsaOut *asound;
  AlsaIn *aIn;
  AlsaSeq *aseq;


void cycle() {
  devices->cycle();
}
extern int async_loop(AlsaSeq *aseq, AlsaOut *aOut, AlsaIn *aIn,void (*cycle)());

/*
static int async_loop(AlsaSeq *aseq, AlsaOut *aOut, AlsaIn *aIn){
    int seq_nfds = 0;
    int nfdsOut=0;
    //int nfdsIn=0;
   //printf("async_loop %p %p %p\n",aseq,aOut,aIn); 
    if(aseq) {
      seq_nfds = snd_seq_poll_descriptors_count(aseq->handle, POLLIN);
    }
    if(aOut) {
      nfdsOut=snd_pcm_poll_descriptors_count (aOut->handle);
    }
    //struct pollfd *pfds = (struct pollfd *)alloca(sizeof(struct pollfd) * (seq_nfds + nfdsOut + nfdsIn));
    struct pollfd *pfds = (struct pollfd *)alloca(sizeof(struct pollfd) * (seq_nfds + nfdsOut));
    if(aseq) {
    snd_seq_poll_descriptors(aseq->handle, pfds, seq_nfds, POLLIN);
    }
    if(aOut) {
    snd_pcm_poll_descriptors (aOut->handle, pfds+seq_nfds, nfdsOut);
    }

    while (1) {
        //if (poll (pfds, seq_nfds + nfdsOut+nfdsIn, 1000) > 0) {
        if (poll (pfds, seq_nfds + nfdsOut, 1000) > 0) {
	    //printf("p1 %d %d %d \n",seq_nfds,nfdsOut,nfdsIn);
            for (int l1 = 0; l1 < seq_nfds; l1++) {
               if (pfds[l1].revents > 0) aseq->callback();
            }
            for (int l1 = seq_nfds; l1 < seq_nfds + nfdsOut; l1++) {
                if (pfds[l1].revents > 0) {
                    aOut->callback(); 
		    cycle();
                }
            }
        }
    }
}
*/

void readFile(char *name,int writeCode) {
  Parser p;

  p.initFile(name);
  p.parse("end",writeCode);
  p.buildDeviceSet("asound",false); /////////
  //exit(0);
  asound=p.asound;
  aIn=p.aIn;
  aseq=p.aseq;
  p.getDevices(devices);
  p.printProgram();
}
 
bool readArguments(int argc, char *argv[]) {
  int n=1;
  int writeCode=0;

  while(n<argc) {
    if(strcmp(argv[n],"-c")==0) {
      writeCode=2; //SAMD21
    } else if(strcmp(argv[n],"-p")==0) {
      writeCode=3; //PC
    } else {
      readFile(argv[n],writeCode);
      return writeCode;
    }
    n++;
  }
  return true;
}

int main(int argc, char *argv[]) {
  int wrc=readArguments(argc,argv); 
  if(aseq) aseq->open();
  if(asound) asound->open();
  if(aIn) aIn->open();
  if(!wrc) {
    async_loop(aseq,asound,aIn,cycle);
  }
  return 0;
}

