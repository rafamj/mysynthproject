//alsaSound.cc
#include "alsaSound.h"
#include "deviceSet.h"

extern DeviceSet *devices;

short AlsaSound::buffer[BUFSIZE];
snd_async_handler_t *AlsaSound::ahandler;

AlsaSound::AlsaSound(const char *devName):Device(2,2){ //left right (to dev)  left right (from dev)
//AlsaSound::AlsaSound(const char *devName):Device(2,0){ //left right
  deviceName=devName;
}

void AlsaSound::cycle() {
  short buffer[2];
  const int len=1;
  int err = snd_pcm_readi(capture_handle, buffer, len);
        if (err < 0) {
            printf("Read error: %s\n", snd_strerror(err));
            exit(EXIT_FAILURE);
        }
        if (err != len) {
            printf("Write error: written %i expected %i\n", err, len);
            exit(EXIT_FAILURE);
        }
	outputs[0]=buffer[0]/(16*256);
	outputs[1]=buffer[1]/(16*256);
}

void AlsaSound::write_pcm(snd_pcm_t *handle, short *buffer, int size) {
      //printf("writing %d\n",size);
      int err = snd_pcm_writei(handle, buffer, size);
        if (err < 0) {
            printf("Write error: %s\n", snd_strerror(err));
            exit(EXIT_FAILURE);
        }
        if (err != size) {
            printf("Write error: written %i expected %i\n", err, size);
            exit(EXIT_FAILURE);
        }
        //printf("written\n");
}


void AlsaSound::open(){
    unsigned int rate=Device::rate;
{
    snd_pcm_hw_params_t *hw_params;
    snd_pcm_sw_params_t *sw_params;

    if (snd_pcm_open (&playback_handle, deviceName, SND_PCM_STREAM_PLAYBACK, 0) < 0) {
        fprintf (stderr, "cannot open audio device %s\n", deviceName);
        exit (1);
    }
    snd_pcm_hw_params_alloca(&hw_params);
    snd_pcm_hw_params_any(playback_handle, hw_params);
    snd_pcm_hw_params_set_access(playback_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(playback_handle, hw_params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_rate_near(playback_handle, hw_params, &rate, 0);
    snd_pcm_hw_params_set_channels(playback_handle, hw_params, 2);
    snd_pcm_hw_params_set_periods(playback_handle, hw_params, 2, 0);
    snd_pcm_hw_params_set_period_size(playback_handle, hw_params, 1024, 0);
    snd_pcm_hw_params(playback_handle, hw_params);
    snd_pcm_sw_params_alloca(&sw_params);
    snd_pcm_sw_params_current(playback_handle, sw_params);
    snd_pcm_sw_params_set_avail_min(playback_handle, sw_params, 2048);
    snd_pcm_sw_params(playback_handle, sw_params);
    //snd_pcm_prepare(playback_handle);
}
{
    snd_pcm_hw_params_t *hw_params;
    snd_pcm_sw_params_t *sw_params;
    if (snd_pcm_open (&capture_handle, deviceName, SND_PCM_STREAM_CAPTURE, 0) < 0) { //deviceName???
        fprintf (stderr, "cannot open audio device %s\n", deviceName);
        exit (1);
    }
    snd_pcm_hw_params_alloca(&hw_params);
    snd_pcm_hw_params_any(capture_handle, hw_params);
    snd_pcm_hw_params_set_access(capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(capture_handle, hw_params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_rate_near(capture_handle, hw_params, &rate, 0);
    snd_pcm_hw_params_set_channels(capture_handle, hw_params, 2);
    snd_pcm_hw_params_set_periods(capture_handle, hw_params, 2, 0);
    snd_pcm_hw_params_set_period_size(capture_handle, hw_params, 1024, 0);
    snd_pcm_hw_params(capture_handle, hw_params);
    snd_pcm_sw_params_alloca(&sw_params);
    snd_pcm_sw_params_current(capture_handle, sw_params);
    snd_pcm_sw_params_set_avail_min(capture_handle, sw_params, 2);
    snd_pcm_sw_params(capture_handle, sw_params);
    
    //snd_pcm_prepare(capture_handle);
  }
}

void AlsaSound::callback(snd_pcm_t *handle){
    snd_pcm_sframes_t avail;
    avail = snd_pcm_avail_update(handle);
    if(avail<0) {
        printf("Avail error: %ld\n", avail);
    }
    int current=0;
    while(current<avail*2){
      //printf("callback1\n");
      buffer[current++]=getInput(0)*16*256;
      //printf("callback2\n");
      buffer[current++]=getInput(1)*16*256;
      //printf("callback3\n");
      devices->cycle();
      //printf("callback4\n");
      //printf("%d\n",buffer[current-2]);
      //endCycle(0);
      //endCycle(1);
      //printf("%d current %d %d \n",current, buffer[current-2],buffer[current-1]);
    }

    write_pcm(handle, buffer,avail);
}

void AlsaSound::close(){
  snd_pcm_drain(playback_handle);
  snd_pcm_close (playback_handle);
}

AlsaSound::~AlsaSound(){
}
