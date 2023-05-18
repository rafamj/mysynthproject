//alsaIn.cc
#include "alsaFunc.h"
#include "alsaIn.h"

//short AlsaIn::buffer[BUFSIZE];
//int AlsaIn::len=0;
//int AlsaIn::index=0;
//snd_async_handler_t *AlsaIn::ahandler;

AlsaIn::AlsaIn(const char *devName):Device(0,2){ //left right
  deviceName=devName;
  len=index=0;
  open();
}

void AlsaIn::cycle() {

  //static int tick=0;
  //if(++tick%1000==0) {tick=0;printf ("cycle\n");}
  //short buffer[2];
  
  if(len<=index) {
  snd_pcm_sframes_t size=4;
  //size  = snd_pcm_avail_update(handle);
  //printf("avail %ld\n",avail);


  int err = snd_pcm_readi(handle, buffer, size);
  //printf("p1\n");
        if (err < 0) {
            printf("Read error: %s\n", snd_strerror(err));
            exit(EXIT_FAILURE);
        }
        if (err != size) {
            printf("Read error: read %i expected %i\n", err, len);
            exit(EXIT_FAILURE);
        }
  index=0;
  len=size*2;
  }
 
	outputs[0]=((sample)buffer[index++])/32767.0;
	outputs[1]=((sample)buffer[index++])/32767.0;

}

void AlsaIn::open(){
    handle=alsaOpen(deviceName,SND_PCM_STREAM_CAPTURE, Device::rate,512,4,2);
    /*
    unsigned int rate=Device::rate;
   snd_pcm_uframes_t periodSize=512;
    uint32_t periods = 4;

    snd_pcm_hw_params_t *hw_params;
    snd_pcm_sw_params_t *sw_params;
    if (snd_pcm_open (&handle, deviceName, SND_PCM_STREAM_CAPTURE, 0) < 0) { //deviceName???
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
    snd_pcm_hw_params_set_period_size(handle, hw_params, periodSize, 0);

    //snd_pcm_hw_params_set_period_size(handle, hw_params, 64, 0);
    snd_pcm_hw_params(handle, hw_params);

//snd_pcm_uframes_t bufferSize;
//snd_pcm_hw_params_get_buffer_size( hw_params, &bufferSize );
//printf("buffer %ld\n",bufferSize);

    snd_pcm_sw_params_alloca(&sw_params);
    snd_pcm_sw_params_current(handle, sw_params);
    snd_pcm_sw_params_set_avail_min(handle, sw_params, 512);
    snd_pcm_sw_params(handle, sw_params);
    
    //snd_pcm_prepare(handle);
    //snd_pcm_start(handle);
    */
}
/*
void AlsaIn::callback(){
    snd_pcm_sframes_t avail;
    avail = snd_pcm_avail_update(handle);
    if(avail<0) {
        printf("Avail error: %ld\n", avail);
    }
  printf("reading %ld\n",avail);
  int err = snd_pcm_readi(handle, buffer, avail);
  printf("end reading\n");
        if (err < 0) {
            printf("Read error: %s\n", snd_strerror(err));
            exit(EXIT_FAILURE);
        }
        if (err != avail) {
            printf("Write error: written %i expected %i\n", err, len);
            exit(EXIT_FAILURE);
        }
    index=0;
    len=avail*2;
}
*/
AlsaIn::~AlsaIn(){
  snd_pcm_close (handle);
}
