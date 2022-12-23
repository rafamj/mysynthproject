//alsaSound.h
#include <alsa/asoundlib.h>
#include "device.h"

class AlsaSound: public Device {
#define BUFSIZE 16384

  const char *deviceName;
  static short buffer[BUFSIZE];
  static void write_pcm(snd_pcm_t *handle, short *buffer, int size);
  static snd_async_handler_t *ahandler;
  public:
  snd_pcm_t *playback_handle;
  snd_pcm_t *capture_handle;
  AlsaSound(const char *devName);
  void open();
  void cycle();
  void callback(snd_pcm_t *playback_handle);
  void close();
  ~AlsaSound();
};
