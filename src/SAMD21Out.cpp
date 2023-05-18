//SAMD21Out.cc
#include <I2S.h>
#include "SAMD21Out.h"



SAMD21Out::SAMD21Out():Device(si,2,0,0,0,0,0,0){ //left right 
}

void SAMD21Out::open(){
   if (!I2S.begin(I2S_PHILIPS_MODE, Device::rate, 16)) {
     SerialUSB.begin(9600);
     while (!SerialUSB) ;
    SerialUSB.println("Failed to initialize I2S!");
    while (1); // do nothing
  }
 
}

void SAMD21Out::callback(void(*cycl)()){
    int wr=I2S.availableForWrite();
    short buffer[2048];
    int index=0;
    //SerialUSB.println(wr);
    while(index*2<wr) {
      cycl();
      buffer[index++]=NORMALIZE(getInput(0));
      buffer[index++]=NORMALIZE(getInput(1));
    }
      //I2S.write((short)(*inputs[0]*32512));
      //I2S.write((short)(*inputs[1]*32512));
    I2S.write(buffer,wr);
      //I2S.write((short)(*inputs[0]*32512));
      //I2S.write((short)(*inputs[1]*32512));
      //short sample=*inputs[0]*32512;
      //sample=*inputs[1]*32512;
      //I2S.write(sample);
}

void SAMD21Out::close(){
}

SAMD21Out::~SAMD21Out(){
  close();
}
