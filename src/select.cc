//select.cc
#include "select.h"

Select::Select(unsigned int *inp, int n, unsigned int *sel, int ns):Device(0,n * ns,1,0){
  nInputs=n;
  in=new unsigned int[n];
  for(int i=0;i<nInputs;i++) {
    in[i]=inp[i];
  }
  nSelect=ns;
  select =new unsigned int[ns];
  for(int i=0;i<nSelect;i++) {
    select[i]=sel[i];
  }
  page=0;
}

void Select::getMessage(int n, snd_seq_event_t *ev){
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
	          setOutput(page*nInputs+i,ev->data.control.value / 127.0);
		  //printf("%d output [%d][%d] %d %f \n",nInputs*nSelect,page,i,page*nInputs+i,ev->data.control.value / 127.0);
	          return;
	         }
	      }  
           } 
}

Select::~Select(){
  delete [] in;
  delete [] select;
}
