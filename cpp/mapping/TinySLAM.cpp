#include <stdint.h>
#include <math.h>
#include "TinySLAM.h"

TinySLAM::TinySLAM() {

}

// update map data
void TinySLAM::map_update(ts_scan_t* scan,ts_position_t* pos,int quality) {
  double c,s,q;
  double x2p,y2p;
  int    i,x1,y1,x2,y2,xp,yp,value;
  double add,dist;

  // translate factors
  c = cos(pos->theta * M_PI / 180.0);
  s = sin(pos->theta * M_PI / 180.0);
  // start position
  x1 = (int)floor(pos->x * TS_MAP_SCALE + 0.5);
  y1 = (int)floor(pos->y * TS_MAP_SCALE + 0.5);

  // rotate scan to robot position
  for (i=0;i!=scan->nb_points;i++) {
    // delta pos
    x2p = c * scan->x[i] - s * scan->y[i];
    y2p = s * scan->x[i] + c * scan->y[i];
    // scan point
    xp = (int)floor((pos->x + x2p) * TS_MAP_SCALE + 0.5);
    yp = (int)floor((pos->y + y2p) * TS_MAP_SCALE + 0.5);
    // calc distance
    dist = sqrt(x2p * x2p + y2p * y2p);
    add  = TS_HOLE_WIDTH / 2 / dist;
    x2p *= TS_MAP_SCALE * (1 + add);
    y2p *= TS_MAP_SCALE * (1 + add);
    x2 = (int)floor(pos->x * TS_MAP_SCALE + x2p + 0.5);
    y2 = (int)floor(pos->y * TS_MAP_SCALE + y2p + 0.5);
    // check value of scanned position
    if (scan->value[i] == TS_NO_OBSTACLE) {
      q = quality / 2;
      value = TS_NO_OBSTACLE;
    } else {
      q = quality;
      value = TS_OBSTACLE;
    }
    this->map_laser_ray(x1,y1,x2,y2,xp,yp,value,q);
  }
}

// update scan point to map
void TinySLAM::map_laser_ray(int x1,int y1,int x2,int y2,int xp, int yp, int value, int alpha) {
  int x2c,y2c,dx,dy,dxc,dyc,error,errorv,derrorv,x;
  int incv,sincv,incerrorv,incptrx,incptry,pixval,horiz,diag;

  ts_map_pixel_t* ptr;

  x2c = x2; y2c = y2;

  // clipping
  if (x2c < 0) {
    if (x2c == x1) return;
    y2c += (y2c - y1) * (-x2c) / (x2c - x1);
    x2c  = 0;
  }
  if (x2c > TS_MAP_SIZE) {
    if (x1 == x2c) return;
    y2c += (y2c - y1) * (TS_MAP_SIZE - 1 - x2c) / (x2c - x1);
    x2c = TS_MAP_SIZE - 1;
  }
}

void TinySLAM::init_map() {
  int x,y,initval;
  ts_map_pixel_t* ptr;
  initval = (TS_OBSTACLE + TS_NO_OBSTACLE ) / 2 ;
  for (ptr = this->map.map, y=0;y<TS_MAP_SIZE;y++) {
    for (x=0;x<TS_MAP_SIZE;ptr++, x++) {
      *ptr = initval;
    }
  }
}

int TinySLAM::distance_scan_to_map(ts_scan_t* scan,ts_position_t* pos) {
  double  c,s;
  int     i,x,y,nb_points = 0;
  int64_t sum;

  c = cos(pos->theta * M_PI / 180.0);
  s = sin(pos->theta * M_PI / 180.0);
  // translate and rotate scan to map and position
  for (i=0,sum=0;i!=scan->nb_points;i++) {
    if (scan->value[i] != TS_NO_OBSTACLE) {
      // translate to map position
      x = (int)floor((pos->x + c * scan->x[i] - s * scan->y[i]) * TS_MAP_SCALE + 0.5);
      y = (int)floor((pos->y + s * scan->x[i] - c * scan->y[i]) * TS_MAP_SCALE + 0.5);
      // check inside map
      if (x >= 0 && x < TS_MAP_SIZE && y >= 0 && y < TS_MAP_SIZE) {
        sum += this->map.map[y * TS_MAP_SIZE + x];
        nb_points++;
      }
    }
  } // sum obstacle position values

  if (nb_points)
    sum = sum * 1024 / nb_points;
  else
    sum = 2000000000;

  return (int) sum;
}
