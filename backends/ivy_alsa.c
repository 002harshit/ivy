#include "ivy.h"

#include <alsa/asoundlib.h>

audio_device_t audio_open(unsigned int channels, unsigned int sample_rate, float latency_secs)
{
	audio_device_t audio = {0};
	unsigned int latency = latency_secs * 1000.0 * 1000.0;
	if (snd_pcm_open((snd_pcm_t**)&audio.native, "default", SND_PCM_STREAM_PLAYBACK, 0)) {
		FATAL("Cannot open audio device");
	}
	if (snd_pcm_set_params(audio.native, SND_PCM_FORMAT_U8, SND_PCM_ACCESS_RW_INTERLEAVED, channels,
						   sample_rate, 1, latency)) {
		FATAL("Cannot set audio device params");
	}
	return audio;
}

int audio_avail_frames(audio_device_t *audio)
{
	int n = snd_pcm_avail(audio->native);
	if (n < 0) {
		snd_pcm_recover(audio->native, n, 0);
	}
	return n;
}

void audio_write_buffer(audio_device_t *audio, float *buf, size_t buf_size)
{
	int r = snd_pcm_writei(audio->native, buf, buf_size);
	if (r < 0) {
		snd_pcm_recover(audio->native, r, 0);
	}
}

void audio_close(audio_device_t *audio)
{
	snd_pcm_close(audio->native);
}
