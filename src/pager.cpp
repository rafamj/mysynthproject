//pager.cpp
//#include <Arduino.h>
//#include <stdio.h>
#include "pager.h"

 Pager::Pager(sample **sia, sample *soa, int maxPages, unsigned int *inp, int n, unsigned int down, unsigned int up):Device(sia,n,soa,n * maxPages,mi,1,0,0){ // in[n] outs  up, down
  si=sia;
  so=soa;
  this->maxPages=maxPages;
  in=inp;
  nInputs=n;
  this->up=up;
  this->down=down;
  page=0;
  /*
	        for(int i=0; i<nInputs; i++) {
		  printf("::in[%d]=%d\n",i,in[i]);
		 }
*/
}


void Pager::getMessage(int n, midi_message_t *ev){
/*
	      SerialUSB.print("getMessage "); SerialUSB.print(ev->type);
	      SerialUSB.println();
*/
            if(ev->type==MIDI_MESSAGE_CONTROLLER){
	      //printf("controller %d %d\n",ev->data.control.param,ev->data.control.value);
	 /*
	      SerialUSB.print("controller param ");
	      SerialUSB.print(ev->data.control.param);
	      SerialUSB.print(" value ");
	      SerialUSB.println(ev->data.control.value);
	 */
	      if(ev->byte1==down && ev->byte2==0) {
	        if(page>0) page--;
		//printf("page %d\n",page);
		//SerialUSB.print("page ");
		//SerialUSB.println(page);
	      } else if(ev->byte1==up && ev->byte2==0) {
	        if(page<maxPages-1) page++;
		//printf("page %d\n",page);
		//SerialUSB.print("page ");
		//SerialUSB.println(page);
	      } else {
	        for(int i=0; i<nInputs; i++) {
		  //printf("in[%d]=%d\n",i,in[i]);
		  if(ev->byte1==in[i]) {
		    setOutput(page*nInputs+i,MAX_VALUE/127 * ev->byte2);
		    //printf("page %d nInputs %d i %d %d\n",page,nInputs,i,MAX_VALUE/127 * ev->byte2);
	/*	
		SerialUSB.print("page ");
		SerialUSB.print(page);
		SerialUSB.print(" nInputs ");SerialUSB.print(nInputs);
		SerialUSB.print(" i ");SerialUSB.print(i);
		SerialUSB.print(" value ");SerialUSB.print(ev->data.control.value);
		SerialUSB.println();
		*/

		    return;
		  }
	        }
	      }
           } 
}


Pager::~Pager() {
  //delete [] in;
}

