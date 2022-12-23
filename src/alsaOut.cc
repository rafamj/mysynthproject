//alsaSound.cc
#include "alsaOut.h"
#include "deviceSet.h"

extern DeviceSet *devices;

//short AlsaOut::buffer[BUFSIZE];
//snd_async_handler_t *AlsaOut::ahandler;

AlsaOut::AlsaOut(const char *devName):Device(2,0){ //left right 
  deviceName=devName;
  open();
}

void AlsaOut::open(){
    unsigned int rate=Device::rate;
    snd_pcm_uframes_t periodSize=256;
    uint32_t periods = 4;
    snd_pcm_hw_params_t *hw_params;
    snd_pcm_sw_params_t *sw_params;

    if (snd_pcm_open (&handle, deviceName, SND_PCM_STREAM_PLAYBACK, 0) < 0) {
        fprintf (stderr, "cannot open audio device %s\n", deviceName);
        exit (1);
    }
    snd_pcm_hw_params_alloca(&hw_params);
    snd_pcm_hw_params_any(handle, hw_params);
    snd_pcm_hw_params_set_access(handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(handle, hw_params, SND_PCM_FORMAT_S16_LE);
   
   snd_pcm_hw_params_set_rate_near(handle, hw_params, &rate, 0);
    snd_pcm_hw_params_set_channels(handle, hw_params, 2);
    snd_pcm_hw_params_set_periods(handle, hw_params, periods, 0);
    snd_pcm_hw_params_set_period_size_near(handle, hw_params, &periodSize, 0);
    
    printf("rate %d\n",rate);
    printf("period size %ld\n",periodSize);
    snd_pcm_hw_params(handle, hw_params);
    snd_pcm_sw_params_alloca(&sw_params);
    snd_pcm_sw_params_current(handle, sw_params);
    snd_pcm_sw_params_set_avail_min(handle, sw_params, 512);
    snd_pcm_sw_params(handle, sw_params);

    snd_pcm_uframes_t buffer_size;
    snd_pcm_hw_params_get_buffer_size(hw_params,&buffer_size);
    printf("buffer size %lu\n",buffer_size);
    //snd_pcm_prepare(handle);
    //write_pcm(handle, buffer,4);
}

void AlsaOut::callback(){
    snd_pcm_sframes_t avail;
    avail = snd_pcm_avail_update(handle);
    if(avail<0) {
        printf("Avail error: %ld\n", avail);
    }
    int current=0;
    while(current<avail*2){
      //printf("callback1\n");
      //buffer[current++]=getInput(0)*16*256;
      buffer[current++]=getInput(0)*32767;
      //printf("callback2\n");
      //buffer[current++]=getInput(1)*16*256;
      buffer[current++]=getInput(1)*32767;
      //printf("callback3\n");
      devices->cycle();
      //printf("callback4\n");
      //printf("%d\n",buffer[current-2]);
      //endCycle(0);
      //endCycle(1);
      //printf("%d current %d %d \n",current, buffer[current-2],buffer[current-1]);
    }
    //snd_pcm_prepare(handle);
      int err = snd_pcm_writei(handle, buffer, avail);
        if (err ==-32 ) {
          snd_pcm_prepare(handle);
	  //snd_pcm_recover(handle,err,0);
        } else if (err < 0) {
            printf("Write error: %d %s\n", err, snd_strerror(err));
            exit(EXIT_FAILURE);
        } else if (err != avail) {
            printf("Write error: written %i expected %li\n", err, avail);
            exit(EXIT_FAILURE);
        }
}

void AlsaOut::close(){
  snd_pcm_drain(handle);
  snd_pcm_close (handle);
}

AlsaOut::~AlsaOut(){
  close();
}
