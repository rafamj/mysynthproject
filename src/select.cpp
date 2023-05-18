//select.cc
#include "select.h"

Select::Select(sample *s, unsigned int *inp, int n, unsigned int *sel, int ns):Device(0,0,s,n * ns,mi,1,0,0){
  so=s;
  nInputs=n;
  in=inp;
  nSelect=ns;
  select=sel;
  page=0;
}

void Select::getMessage(int n, midi_message_t *ev){
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
	          setOutput(page*nInputs+i,MAX_VALUE / 127 * ev->byte2);
		  //printf("%d output [%d][%d] %d %f \n",nInputs*nSelect,page,i,page*nInputs+i,ev->data.control.value / 127.0);
	          return;
	         }
	      }  
           } 
}

Select::~Select(){
  //delete [] in;
  //delete [] select;
}
