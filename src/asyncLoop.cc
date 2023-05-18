#include <alsa/asoundlib.h>
#include "alsa/alsaSeq.h"
#include "alsa/alsaOut.h"
#include "alsa/alsaIn.h"


//extern void cycle();

int async_loop(AlsaSeq *aseq, AlsaOut *aOut, AlsaIn *aIn, void(*cycle)()){
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
    /*
    if(aIn) {
      nfdsIn=snd_pcm_poll_descriptors_count (aIn->handle);
    }
    */
    //struct pollfd *pfds = (struct pollfd *)alloca(sizeof(struct pollfd) * (seq_nfds + nfdsOut + nfdsIn));
    struct pollfd *pfds = (struct pollfd *)alloca(sizeof(struct pollfd) * (seq_nfds + nfdsOut));
    if(aseq) {
    snd_seq_poll_descriptors(aseq->handle, pfds, seq_nfds, POLLIN);
    }
    if(aOut) {
    snd_pcm_poll_descriptors (aOut->handle, pfds+seq_nfds, nfdsOut);
    }
    /*
    if(aIn) {
    snd_pcm_poll_descriptors (aIn->handle, pfds+seq_nfds+nfdsOut, nfdsIn);
    }
    */

    while (1) {
        //if (poll (pfds, seq_nfds + nfdsOut+nfdsIn, 1000) > 0) {
        if (poll (pfds, seq_nfds + nfdsOut, 1000) > 0) {
            //printf("p1 %d %d %d \n",seq_nfds,nfdsOut,nfdsIn);
            for (int l1 = 0; l1 < seq_nfds; l1++) {
               if (pfds[l1].revents > 0) aseq->callback();
            }
            for (int l1 = seq_nfds; l1 < seq_nfds + nfdsOut; l1++) {
                if (pfds[l1].revents > 0) {
                    aOut->callback(cycle);
                }
            }
            /*
            for (int l1 = seq_nfds  + nfdsOut; l1 < seq_nfds + nfdsOut+nfdsIn; l1++) {
                if (pfds[l1].revents > 0) {
                    aIn->callback();
                }
            }
            */
        }
    }
}

