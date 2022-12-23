//select.cc
#include "midiSelect.h"

MidiSelect::MidiSelect(unsigned int *inp, int n, unsigned int *sel, int ns, unsigned int *trans):Device(0,0,1,1){
  nInputs=n;
  in = new unsigned int[n];
  for(int i=0;i<nInputs;i++) {
    in[i]=inp[i];
  }
  nSelect=ns;
  select=new unsigned int[ns];
  for(int i=0;i<nInputs;i++) {
    select[i]=sel[i];
  }
  translate=new unsigned int[n*ns];
  for(int i=0;i<nInputs*nSelect;i++) {
    translate[i]=trans[i];
  }
  page=0;
}

void MidiSelect::getMessage(int n, snd_seq_event_t *ev){
            if(ev->type== SND_SEQ_EVENT_CONTROLLER){
	      for(int i=0;i<nSelect;i++) {
	        if(select[i]==ev->data.control.param && ev->data.control.value==0) {
		  page=i;
		  printf("page %d \n",page);
		  return;
		}
	      }
	      for(int i=0; i<nInputs; i++) {
	        if(ev->data.control.param==in[i]) {
		  ev->data.control.param =translate[page*nInputs+i];
		  callOutput(0,ev);
		  //printf("translating %d to %d\n",in[i],translate[page*nInputs+i]);
	          return;
	         }
	      }  
           } 
}

MidiSelect::~MidiSelect(){
  delete [] in;
  delete [] select;
  delete [] translate;
}

