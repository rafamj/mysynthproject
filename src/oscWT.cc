//oscWT.cc
#include <limits.h>
#include "oscWT.h"

//2048 muestras por ciclo
OscWT::OscWT(unsigned char *data, size_t n, int type):Device(3,1){ //note pitchbend table out
  sample max=0,min=0;
  switch(type) {
    case 0: {
              n=n/4; //float
              wav=new sample[n]; 
	      float *d=(float *)data;
              for(size_t i=0;i<n;i++) {
                wav[i]=d[i];
	        if(wav[i]>max) max=wav[i];
	        if(wav[i]<min) min=wav[i];
              }
	    }
            break; 
    case 1: {
              n=n/4; //int
              wav=new sample[n]; 
	      int *d=(int *)data;
              for(size_t i=0;i<n;i++) {
                wav[i]=(sample)d[i]/INT_MAX;
	        if(wav[i]>max) max=wav[i];
	        if(wav[i]<min) min=wav[i];
              }
	    }
            break;
    case 2: {
              n=n/2; //short
              wav=new sample[n]; 
	      short *d=(short *)data;
              for(size_t i=0;i<n;i++) {
                wav[i]=(sample)d[i]/SHRT_MAX;
	        if(wav[i]>max) max=wav[i];
	        if(wav[i]<min) min=wav[i];
              }
	    }
            break; 
  }
  phase=0;
  spc=2048/2;
  nw=n/(spc*2);
  printf("nw %d max %f min %f\n",nw,max,min);
} 

void OscWT::cycle() {
  int zero;
  sample z;
  sample freq=getInput(0) + getInput(1);
  if(freq<=0) return;
  int n,nn;
  sample table=getInput(2);
  int w=(nw-1)*table; //the last table will be accesed with w=nw-2 + delta
  int ww=(w+1)%nw; //don't need % nw
  sample deltaw=(nw-1)*table-w;

        phase++;
        zero=rate /freq;
	z=rate /freq;
        if(phase>2*z-delta) {
	      delta += z-zero;
	      if(delta>1) delta -=1;
         phase=0;
       }
  n=phase * freq * spc / rate;
  sample deltan=phase * freq * spc / rate -n;
  nn=(n+1)%2048;
  static int ticks=0;
  if(ticks++==100000 || deltan<0 || deltan>1 || deltaw<0 || deltaw>1) {
    ticks=0;
    printf("error deltan %f deltaw %f nw %d table %f w %d\n",deltan,deltaw,nw,table,w);
    printf(" deltan %f phase %d freq %f spc %d rate %d n %d\n",deltan,phase,freq,spc,rate,n);
  }
  if(n>=2048) {
    printf("error n %d phase %d freq %f\n",n,phase,freq);
  }
  sample out1=wav[2*spc*w+nn]*deltan+wav[2*spc*w+n]*(1-deltan);
  sample out2=wav[2*spc*ww+nn]*deltan+wav[2*spc*ww+n]*(1-deltan);
  setOutput(0,out2*deltaw+out1*(1-deltaw));
}

OscWT::~OscWT(){
  delete [] wav;
}

