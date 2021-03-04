#include "PageMapIndexer.h"

PageMapIndexer::PageMapIndexer(uint64_t virtualAdress) {
  virtualAdress >>= 12;
  P_index = virtualAdress & 0x1ff;
  virtualAdress >>= 9;
  PT_index = virtualAdress & 0x1ff;
  virtualAdress >>= 9;
  PD_index = virtualAdress & 0x1ff;
  virtualAdress >>= 9;
  PDP_index = virtualAdress & 0x1ff;
}
