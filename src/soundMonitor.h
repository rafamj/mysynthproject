//soundMonitor.h
#include "device.h"

class SoundMonitor: public Device {
  long int t;
  sample mean;
  sample max;
  public:
    SoundMonitor():Device(1,0){t=0;mean=max=0;} //in
    void cycle() {
            sample in=abs(getInput(0));
	    if(in>max) max=in;
	    mean= (mean*t + in)/(t+1);
	    t++;
	    if(t>=44100*5) {
	      printf("mean %f max %f\n",mean,max);
	      t=0;
	      mean=max=0;
	    }
	    }
};
