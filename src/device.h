//device.h
#ifndef DEVICE_H
#define DEVICE_H
#include <cstdio>
//#include <vector>
//#include <alsa/asoundlib.h>

#define SAMPLE_FREQ 8000
//#define SAMPLE_FREQ 44100 
//#define SAMPLE_FREQ 48000 
#define SAMPLE_T 0 

#if SAMPLE_T == 0 //double

typedef double sample;
#define MAX_VALUE 1.0
#define CONVERT16(x) x*32767
#define NORMALIZE(x) x

#elif SAMPLE_T == 1 //16 bits

typedef short int sample;
#define MAX_VALUE 32767
#define CONVERT16(x) x
#define NORMALIZE(x) (x>>8)

#elif SAMPLE_T == 2 //32 bits

typedef int sample;
//#define MAX_VALUE 2147483647
#define MAX_VALUE 2147483647
#define CONVERT16(x) (x>>16)
#define NORMALIZE(x) (x>>16)


#endif

class Device;

const unsigned char MIDI_MESSAGE_NOTEON=0x90;
const unsigned char MIDI_MESSAGE_NOTEOFF=0x80;
const unsigned char MIDI_MESSAGE_AFTERTOUCH=0xa0;
const unsigned char MIDI_MESSAGE_CONTROLLER=0xb0;
const unsigned char MIDI_MESSAGE_PITCHBEND=0xe0;
//const unsigned char MIDI_MESSAGE_SYSEX=130;
const unsigned char MIDI_MESSAGE_UNKNOWN=255;

typedef struct midi_message {
  unsigned char type;
  unsigned char channel;
  unsigned char byte1;
  unsigned char byte2;
} midi_message_t;
/*
class MidiMessage {
  public:
  int n;
  Device *d;
};

class MArray {
  public:
  int n;
  MidiMessage array[10];
  MArray():n(0) {}
  void insert(int n, Device *d);
};
*/

class MidiInput {
  public:
  int next_n;
  Device *next;
};

class MidiOutput {
  public:
  int n;
  Device *d;
};

class Device {
  public:////
  static int rate;
  sample **inputs;
  sample *outputs;
  //MArray *moutputs;
  MidiInput *minputs;
  MidiOutput *moutputs;
  Device(int ni, int no, int nmi=0, int nmo=0);
  Device(sample **inp, int ni, sample *oup, int no, MidiInput *minp, int nmi, MidiOutput *moup, int nmo);
  void initIO(int ni, int no, int nmi, int nmo);
  void connect(int input, Device *d, int output);
  void connectM(int input, Device *d, int output);
  void open(){};
  virtual void cycle(){}; //not 0 because some devices don't cycle
  virtual void getMessage(int n, midi_message_t *ev){};
  sample getInput(int n) { return *inputs[n];};
  void setOutput(int n,sample v) {outputs[n]=v;};
  void callOutput(int i, midi_message_t *m);
  void close(){};
  ~Device();
};
#endif
