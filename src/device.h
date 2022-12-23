//device.h
#ifndef DEVICE_H
#define DEVICE_H
//#include <cstdio>
//#include <vector>
#include <alsa/asoundlib.h>

//typedef double sample;
typedef float sample;
#define MAX_VALUE 1.0

//typedef int sample
//#define MAX_VALUE 30000
//
class Device;


class MidiMessage {
  public:
  int n;
  Device *d;
};

class MArray {
  public:
  int n;
  MidiMessage **array;
  MArray() {n=0;array=0;}
  void insert(MidiMessage *m);
};



class Device {
  public:////
  static int rate;
  sample **inputs;
  sample *outputs;
  MArray **moutputs;
  Device(int ni, int no, int nmi=0, int nmo=0);
  void connect(int input, Device *d, int output);
  void connectM(int input, Device *d, int output);
  void open(){};
  virtual void cycle(){}; //not 0 because some devices don't cycle
  virtual void getMessage(int n, snd_seq_event_t *ev){};
  sample getInput(int n) { return *inputs[n];};
  void setOutput(int n,sample v) {outputs[n]=v;};
  void callOutput(int i, snd_seq_event_t *m);
  void close(){};
  ~Device();
};
#endif
