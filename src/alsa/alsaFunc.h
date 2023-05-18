//alsaFunc.h
#include <alsa/asoundlib.h>
snd_pcm_t *alsaOpen(const char *deviceName, snd_pcm_stream_t mode,unsigned int rate, snd_pcm_uframes_t periodSize, uint32_t periods, int channels);
