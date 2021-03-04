#pragma once

#include <stdint.h>

class PageMapIndexer {
public:
  PageMapIndexer(uint64_t virtualAdress);
  uint64_t PDP_index;
  uint64_t PD_index;
  uint64_t PT_index;
  uint64_t P_index;
};
