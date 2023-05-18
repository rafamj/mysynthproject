//alsaFunc.cc
#include "alsaFunc.h"

snd_pcm_t *alsaOpen(const char *deviceName, snd_pcm_stream_t mode, unsigned int rate, snd_pcm_uframes_t periodSize, uint32_t periods, int channels) {
    snd_pcm_t *handle;

    snd_pcm_hw_params_t *hw_params;
    snd_pcm_sw_params_t *sw_params;
    if (snd_pcm_open (&handle, deviceName, mode, 0) < 0) { 
        fprintf (stderr, "cannot open audio device %s\n", deviceName);
        exit (1);
    }
    snd_pcm_hw_params_alloca(&hw_params);
    snd_pcm_hw_params_any(handle, hw_params);
    snd_pcm_hw_params_set_access(handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(handle, hw_params, SND_PCM_FORMAT_S16_LE);

    snd_pcm_hw_params_set_rate_near(handle, hw_params, &rate, 0);
    snd_pcm_hw_params_set_channels(handle, hw_params, channels);
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
    return handle;
}
