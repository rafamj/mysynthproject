//select.cc
#include "midiSelect.h"

MidiSelect::MidiSelect(unsigned int *inp, int n, unsigned int *sel, int ns, unsigned int *trans, unsigned int ntrans):Device(0,0,0,0,mi,1,mo,1){
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
  for(int i=0;i<nInputs;i++) {
    for(int j=0;i<nSelect;i++) {
      translate[i]=trans[i*nInputs+j];
    }
  }
  page=0;
}

void MidiSelect::getMessage(int n, midi_message_t *ev){
            if(ev->type== MIDI_MESSAGE_CONTROLLER){
	      for(int i=0;i<nSelect;i++) {
	        if(select[i]==ev->byte1 && ev->byte2==0) {
		  page=i;
		  //printf("page %d \n",page);
		  return;
		}
	      }
	      for(int i=0; i<nInputs; i++) {
	        if(ev->byte1==in[i]) {
		  ev->byte1 =translate[page*nInputs+i];
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

