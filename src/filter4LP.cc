//filter4LP.cc
#include <math.h>
#include "filter4LP.h"

Filter4LP::Filter4LP():Device(3,1) { //in cutoff reso out
}

void Filter4LP::calculate(sample omega, sample g) {
 k=(4.0*g-3.0)/(g+1.0);
p=1.0-0.25*k;p*=p;

// LP:
a=1.0/(tan(0.5*omega)*(1.0+p));
p=1.0+a;
q=1.0-a;

a0=1.0/(k+p*p*p*p);
a1=4.0*(k+p*p*p*q);
a2=6.0*(k+p*p*q*q);
a3=4.0*(k+p*q*q*q);
a4=    (k+q*q*q*q);
p=a0*(k+1.0);

coef[0]=p;
coef[1]=4.0*p;
coef[2]=6.0*p;
coef[3]=4.0*p;
coef[4]=p;
coef[5]=-a1*a0;
coef[6]=-a2*a0;
coef[7]=-a3*a0;
coef[8]=-a4*a0; 
}

void Filter4LP::cycle() {
  sample in=getInput(0);
  sample out;
  if(ticks++==10) {
    ticks=0;
    sample cutoff=getInput(1);
    sample reso=getInput(2);
    calculate(cutoff,reso);
  }

out=coef[0]*in+d[0];
d[0]=coef[1]*in+coef[5]*out+d[1];
d[1]=coef[2]*in+coef[6]*out+d[2];
d[2]=coef[3]*in+coef[7]*out+d[3];
d[3]=coef[4]*in+coef[8]*out;
  setOutput(0,out);
}

