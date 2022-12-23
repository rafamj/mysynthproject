//pager.cc
#include "pager.h"

Pager::Pager(int maxPages, unsigned int *inp, int n, unsigned int down, unsigned int up):Device(n,n * maxPages,1,0){ // in[n] outs  up, down
  this->maxPages=maxPages;
  in=new unsigned int[n];
  for(int i=0;i<n;i++) {
    in[i]=inp[i];
  }
  nInputs=n;
  this->up=up;
  this->down=down;
  page=0;
}


void Pager::getMessage(int n, snd_seq_event_t *ev){
            if(ev->type== SND_SEQ_EVENT_CONTROLLER){
	      if(ev->data.control.param==down && ev->data.control.value==0) {
	        if(page>0) page--;
		printf("page %d\n",page);
	      } else if(ev->data.control.param==up && ev->data.control.value==0) {
	        if(page<maxPages-1) page++;
		printf("page %d\n",page);
	      } else {
	        for(int i=0; i<nInputs; i++) {
		  if(ev->data.control.param==in[i]) {
		    setOutput(page*nInputs+i,ev->data.control.value / 127.0);
		    //printf("page %d nInputs %d i %d %f\n",page,nInputs,i,ev->data.control.value / 127.0);
		    return;
		  }
	        }
	      }
           } 
}

Pager::~Pager() {
  delete [] in;
}
