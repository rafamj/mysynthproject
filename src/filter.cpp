//filter.cc
#include <math.h>
#include "filter.h"

Filter::Filter():Device(si,3,so,1,0,0,0,0) { //in cutoff reso out
y1=y2=y3=y4=oldx=oldy1=oldy2=oldy3=0;
}

void Filter::cycle() {
  sample input=getInput(0);
  sample cutoff=getInput(1);
  sample reso=getInput(2);

  sample f = (cutoff+cutoff) / rate; //[0 - 1]
  sample p=f*(1.8f-0.8f*f);
  sample k=p+p-1.f;

  sample t=(1.f-p)*1.386249f;
  sample t2=12.f+t*t;
  sample r = reso*(t2+6.f*t)/(t2-6.f*t);

sample x = input - r*y4;

y1=x*p + oldx*p - k*y1;
y2=y1*p+oldy1*p - k*y2;
y3=y2*p+oldy2*p - k*y3;
y4=y3*p+oldy3*p - k*y4;

y4-=(y4*y4*y4)/6.f;

oldx = x;
oldy1 = y1;
oldy2 = y2;
oldy3 = y3;

  setOutput(0,y4);
}

