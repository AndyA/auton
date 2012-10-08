#include <fcntl.h>
#include <math.h>
#include <getopt.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <pthread.h>
#include <errno.h>
#include "event.h"
#include "queue.h"
#include "util.h"
#include "noticeboard.h"
static uint8_t nb_i[NB_SIZE];
static uint8_t nb_o[NB_SIZE];

static char nb_sig_start[NB_SIG_LEN];
static char nb_sig_end[NB_SIG_LEN];

pthread_mutex_t nb_i_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t nb_o_mtx = PTHREAD_MUTEX_INITIALIZER;
queue_t event_queue = QUEUE_INITIALIZER;

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static const char *device = "/dev/spidev0.0";
static uint8_t mode;
static uint8_t bits = 8;
static uint32_t speed = 125000;
static uint16_t delay;
static int fd;

static void nb_flip_string(char *buf, const char *str) {
  while (*str)
    *buf++ = ~*str++;
}

static void nb_init(void) {
  nb_flip_string(nb_sig_start, NB_SIG_START);
  nb_flip_string(nb_sig_end, NB_SIG_END);
}

static void nb_transfer() {
  int ret, i;
  uint8_t tx[NB_SIG_LEN + NB_SIZE + NB_SIG_LEN];
  uint8_t rx[NB_SIG_LEN + NB_SIZE + NB_SIG_LEN];

  /* build outgoing message */
  memcpy(tx + 0, NB_SIG_START, NB_SIG_LEN);
  pthread_mutex_lock(&nb_i_mtx);
  memcpy(tx + NB_SIG_LEN, nb_i, NB_SIZE);
  pthread_mutex_unlock(&nb_i_mtx);
  memcpy(tx + NB_SIG_LEN + NB_SIZE, NB_SIG_END, NB_SIG_LEN);

  memset(rx, 0, sizeof(rx));

  struct spi_ioc_transfer tr = {
    .tx_buf = (unsigned long) tx,
    .rx_buf = (unsigned long) rx,
    .len = ARRAY_SIZE(tx),
    .delay_usecs = delay,
    .speed_hz = speed,
    .bits_per_word = bits,
  };

  ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
  if (ret < 1)
    die("can't send spi message");

  if (memcmp(rx + 1, nb_sig_start, NB_SIG_LEN) ||
      memcmp(rx + 1 + NB_SIG_LEN + NB_SIZE, nb_sig_end,
             NB_SIG_LEN - 1)) {
    printf("Warning: communication error\n");
    hexdump(rx, sizeof(rx));
    return;
  }

  pthread_mutex_lock(&nb_o_mtx);
  for (i = 0; i < NB_SIZE; i++) {
    uint8_t nv = rx[1 + NB_SIG_LEN + i];
    uint8_t ov = nb_o[i];
    if (nv != ov) {
      queue_enqueue(&event_queue, event_make(i, ov, nv));
      nb_o[i] = nv;
    }
  }
  pthread_mutex_unlock(&nb_o_mtx);
}

static void nb_check_addr(unsigned addr) {
  if (addr >= NB_SIZE)
    die("Noticeboard offset %u out of range (0..%u)", addr, NB_SIZE - 1);
}

static uint8_t nb_peek(unsigned addr) {
  uint8_t v;
  nb_check_addr(addr);
  pthread_mutex_lock(&nb_o_mtx);
  v = nb_o[addr];
  pthread_mutex_unlock(&nb_o_mtx);
  return v;
}

static void nb_poke(unsigned addr, uint8_t v) {
  nb_check_addr(addr);
  pthread_mutex_lock(&nb_i_mtx);
  nb_i[addr] = v;
  pthread_mutex_unlock(&nb_i_mtx);
}

static void *nb_worker(void *arg) {
  while (1) {
    nb_transfer();
    usleep(25000);
  }
  return NULL;
}

static void *event_worker(void *arg) {
  while (1) {
    event_t *event = queue_dequeue(&event_queue);
    nb_cb_func cb = nb_cb[event->addr];
    if (cb)
      cb(event->addr, event->ov, event->nv);
    event_release(event);
  }
  return NULL;
}

static void joy_changed(uint16_t addr, uint8_t ov, uint8_t nv) {
  int v = nv * 255 / 168;
  switch (addr) {
  case NB_O_JOY_X:
    nb_poke(NB_I_CAM_PAN, v);
    break;
  case NB_O_JOY_Y:
    nb_poke(NB_I_CAM_TILT, v);
    break;
  }
}

static void nb_changed(uint16_t addr, uint8_t ov, uint8_t nv) {
  printf("%3d: %3d -> %3d\n", addr, ov, nv);
}

int main(int argc, char *argv[]) {
  int ret = 0;
  pthread_t spi_handler, event_handler;
  void *rv;

  fd = open(device, O_RDWR);
  if (fd < 0)
    die("can't open device");

  /*
   * spi mode
   */
  ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
  if (ret == -1)
    die("can't set spi mode");

  ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
  if (ret == -1)
    die("can't get spi mode");

  /*
   * bits per word
   */
  ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
  if (ret == -1)
    die("can't set bits per word");

  ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
  if (ret == -1)
    die("can't get bits per word");

  /*
   * max speed hz
   */
  ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
  if (ret == -1)
    die("can't set max speed hz");

  ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
  if (ret == -1)
    die("can't get max speed hz");

  nb_init();

  nb_register(nb_changed, 0, NB_SIZE - 1);
  nb_register(joy_changed, NB_O_JOY_X, NB_O_JOY_Y);

  if (pthread_create(&spi_handler, NULL, nb_worker, NULL) < 0) {
    die("Thread creation failed: %s", strerror(errno));
  }

  if (pthread_create(&event_handler, NULL, event_worker, NULL) < 0) {
    die("Thread creation failed: %s", strerror(errno));
  }

  pthread_join(spi_handler, &rv);
  pthread_join(event_handler, &rv);

  close(fd);
  pthread_exit(NULL);
  return ret;
}

/* vim:ts=2:sw=2:sts=2:et:ft=c
 */
