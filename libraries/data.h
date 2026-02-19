#ifndef DATA_H
#define DATA_H

const uint64_t pipeOut = 0xF0F0F0F0C3LL;

struct Signal {
  uint16_t pitch;
  uint16_t roll;
  char throttle;
} __attribute__((packed));


#endif
