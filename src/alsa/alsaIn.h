//alsaIn.h
#include <alsa/asoundlib.h>
#include "../device.h"

class AlsaIn: public Device {
#define BUFSIZE 16384

  const char *deviceName;
  short buffer[BUFSIZE];
  int len;
  int index;
  //static void write_pcm(snd_pcm_t *handle, short *buffer, int size);
  public:
  snd_pcm_t *handle;
  AlsaIn(const char *devName);
  void open();
  void cycle();
  //void callback();
  ~AlsaIn();
};
