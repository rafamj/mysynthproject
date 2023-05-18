// deviceSet.h
#include "device.h"

#define MAX_DEVICE 1000

class DeviceSet {
  Device *alm[MAX_DEVICE];
  int number;
  public:
  DeviceSet() { number=0;}
  bool contains(Device *d) {
          for(int i=0;i<number;i++) {
	    if(d==alm[i]) return true;
	  }
	  return false;
  }
  void insert(Device *d) {
    alm[number++]=d;
//if(number>=MAX_DEVICE) printf("ERROR\n");
  }
  void cycle() {
    for(int i=0;i<number;i++) {
      alm[i]->cycle();
    }
  }
};
