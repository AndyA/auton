/* sounder.c */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define ALSA_PCM_NEW_HW_PARAMS_API
#include <alsa/asoundlib.h>

#include "eventio.h"
#include "synth.h"
#include "util.h"

#define RATE      48000
#define CHANNELS  2

#define SENSORS   3

static synth sy[SENSORS];
static pthread_mutex_t sy_mutex = PTHREAD_MUTEX_INITIALIZER;

static void *reader_worker(void *arg) {
  while (1) {
    struct event ev;
    ssize_t rc;
    uint8_t sn;
    uint16_t vv;

    rc = evio_read(0, &ev);
    if (rc < 0) die("I/O error: %ld", rc);
    if (rc == 0) break;

    if (ev.type != QT_RANGE) continue;
    sn = ev.d.rf.c;
    vv = ev.d.rf.r;
    if (sn < SENSORS) {
      if (vv < 200) {
        synth_set_amplitude(&sy[sn], 0);
      }
      else {
        synth_set_amplitude(&sy[sn], 0xffff);
        synth_set_frequency(&sy[sn], vv * 3);
      }
    }
  }
  return NULL;
}

static void synth_init(void) {
  int s;
  for (s = 0; s < SENSORS; s++) {
    synth_set_rate(&sy[s], RATE);
    synth_set_frequency(&sy[s], 440);
  }
}

static snd_pcm_t *sounder_init(snd_pcm_uframes_t *frames) {
  int rc, dir;
  unsigned int val;
  snd_pcm_t *snd;
  snd_pcm_hw_params_t *params;

  if ((rc = snd_pcm_open(&snd, "default", SND_PCM_STREAM_PLAYBACK, 0)) < 0)
    die("Unable to open PCM device: %s", snd_strerror(rc));

  snd_pcm_hw_params_alloca(&params);
  snd_pcm_hw_params_any(snd, params);

  snd_pcm_hw_params_set_access(snd, params, SND_PCM_ACCESS_RW_INTERLEAVED);
  snd_pcm_hw_params_set_format(snd, params, SND_PCM_FORMAT_S16_LE);
  snd_pcm_hw_params_set_channels(snd, params, CHANNELS);

  val = RATE;
  snd_pcm_hw_params_set_rate_near(snd, params, &val, &dir);

  *frames = 1024;
  snd_pcm_hw_params_set_period_size_near(snd, params, frames, &dir);

  if ((rc = snd_pcm_hw_params(snd, params)) < 0)
    die("Can't set hardware parameters: %s", snd_strerror(rc));

  snd_pcm_hw_params_get_period_size(params, frames, &dir);

  return snd;
}

static void *sounder_worker(void *arg) {
  snd_pcm_uframes_t frames;
  int i, j, rc;
  snd_pcm_t *snd;
  int16_t *buffer;

  synth_init();
  snd = sounder_init(&frames);
  buffer = alloc(frames * sizeof(int16_t) * CHANNELS);

  while (1) {
    int16_t *bp = buffer;
    for (i = 0; i < frames; i++) {
      int32_t left = 0, right = 0, scale = 0;
      pthread_mutex_lock(&sy_mutex);
      for (j = 0; j < SENSORS; j++) {
        int32_t sample = synth_sin(&sy[j]);
        left += (j + 1) * sample;
        right += (SENSORS - j) * sample;
        scale += j + 1;
      }
      pthread_mutex_unlock(&sy_mutex);
      *bp++ = (int16_t)(left / scale);
      *bp++ = (int16_t)(right / scale);
    }

    if (rc = snd_pcm_writei(snd, buffer, frames), rc < 0)
      die("Playback error: %s", snd_strerror(rc));
  }

  snd_pcm_drain(snd);
  snd_pcm_close(snd);
  free(buffer);
  return NULL;
}

int main() {
  pthread_t sounder, reader;
  void *rv;

  if (pthread_create(&sounder, NULL, sounder_worker, NULL) < 0) {
    die("Thread creation failed: %s", strerror(errno));
  }
  if (pthread_create(&reader, NULL, reader_worker, NULL) < 0) {
    die("Thread creation failed: %s", strerror(errno));
  }

  pthread_join(reader, &rv);
  pthread_join(sounder, &rv);
  pthread_exit(NULL);

  return 0;
}

/* vim:ts=2:sw=2:sts=2:et:ft=c
 */
