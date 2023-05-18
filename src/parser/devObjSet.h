// devObjSet.h
#include "table.h"

#define MAX_DEVICE 1000 

class DevObjSet {
  DevObj *alm[MAX_DEVICE];
  int number;
  public:
  DevObjSet() { number=0;}
  bool contains(DevObj *d) {
          for(int i=0;i<number;i++) {
	    if(d==alm[i]) return true;
	  }
	  return false;
  }
  void insert(DevObj *d) {
    if(!contains(d)) {
      alm[number++]=d;
    }
  }
  int size(){return number;}
  DevObj *getElement(int i){return alm[i];}
};
