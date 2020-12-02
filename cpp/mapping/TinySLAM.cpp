#include "TinySLAM.h"

TinySLAM::TinySLAM() {

}

void TinySLAM::init_map() {
  int x,y,initval;
  ts_map_pixel_t* ptr;
  initval = (TS_OBSTACLE + TS_NO_OBSTACLE ) / 2 ;
  for (ptr = this->map->map, y=0;y<TS_MAP_SIZE;y++) {
    for (x=0;x<TS_MAP_SIZE;ptr++, x++) {
      *ptr = initval;
    }
  }
}
