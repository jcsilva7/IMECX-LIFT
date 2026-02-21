#ifndef DATA_H
#define DATA_H

#include <RF24.h>

const uint64_t pipeOut = 0xF0F0F0F0C3LL;

struct Signal {
  uint16_t pitch;
  uint16_t roll;
  char throttle;
} __attribute__((packed));

rf24_pa_dbm_e CURRENT_RANGE = RF24_PA_MAX;

rf24_datarate_e CURRENT_DATA_RATE = RF24_250KBPS;

uint8_t CURRENT_CHANNEL = 108;

#endif
