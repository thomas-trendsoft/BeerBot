#include <iostream>
#include <stdint.h>
#include <math.h>
#include "TinySLAM.h"

#define SWAP(x,y) (x ^= y ^= x ^= y)

// default constructor
TinySLAM::TinySLAM() {

}

// update map data
void TinySLAM::map_update(ts_scan_t* scan,ts_position_t* pos,int quality) {
  double c,s,q;
  double x2p,y2p;
  int    i,x1,y1,x2,y2,xp,yp,value;
  double add,dist;

  // translate factors
  c = cos(pos->theta);
  s = sin(pos->theta);

  // start position
  x1 = (int)floor(pos->x * TS_MAP_SCALE + 0.5);
  y1 = (int)floor(pos->y * TS_MAP_SCALE + 0.5);

  // rotate scan to robot position
  for (i=0;i!=scan->nb_points;i++) {

    // delta
    x2p = scan->x[i] - pos->x;
    y2p = scan->y[i] - pos->y;

    // target point
    xp = (int)floor(scan->x[i] * TS_MAP_SCALE + 0.5);
    yp = (int)floor(scan->y[i] * TS_MAP_SCALE + 0.5);

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
// try to understand
// - x1 / y1 = roboter position / laser start position
// - x2 / y2 = laser virtual end position
// - xp / yp = real measure point
// - value = measure value
// - alpha = quality
//
void TinySLAM::map_laser_ray(int x1,int y1,int x2,int y2,int xp, int yp, int value, int alpha) {
  int x2c,y2c,dx,dy,dxc,dyc,error,errorv,derrorv,x;
  int incv,sincv,incerrorv,incptrx,incptry,pixval,horiz,diag;

  ts_map_pixel_t* ptr;

  // check position out of map
  if (x1 < 0 || x1 >= TS_MAP_SIZE || y1 < 0 || y1 >= TS_MAP_SIZE)
    return;

  x2c = x2; y2c = y2;

  // clipping
  if (x2c < 0) {
    if (x2c == x1) return;
    y2c += (y2c - y1) * (-x2c) / (x2c - x1);
    x2c  = 0;
  }
  if (x2c >= TS_MAP_SIZE) {
    if (x1 == x2c) return;
    y2c += (y2c - y1) * (TS_MAP_SIZE - 1 - x2c) / (x2c - x1);
    x2c = TS_MAP_SIZE - 1;
  }
  if (y2c < 0) {
    if (y1 == y2c) return;
    x2c += (x1 - x2c) * (-y2c) / (y1 - y2c);
    y2c = 0;
  }
  if (y2c >= TS_MAP_SIZE) {
    if (y1 == y2c) return;
    x2c += (x1 - x2c) * (TS_MAP_SIZE - 1 - y2c) / (y1 - y2c);
    y2c = TS_MAP_SIZE - 1;
  }

  // delta vals
  dx = abs(x2 - x1); dy = abs(y2 - y1);
  dxc = abs(x2c - x1); dyc = abs(y2c - y1);

  // check increment directions
  incptrx = (x2 > x1) ? 1 : -1;
  incptry = (y2 > y1) ? TS_MAP_SIZE : -TS_MAP_SIZE;
  sincv   = (value > TS_NO_OBSTACLE) ? 1 : -1;

  // calc err values
  if (dx > dy) {
    derrorv = abs(xp - x2);
  } else {
    SWAP(dx,dy);SWAP(dxc,dyc);SWAP(incptrx,incptry);
    derrorv = abs(yp - y2);
  }

  error     = 2 * dyc - dxc;
  horiz     = 2 * dyc;
  diag      = 2 * (dyc - dxc);
  errorv    = (derrorv / 2);
  incv      = (value - TS_NO_OBSTACLE) / derrorv;
  incerrorv = value - TS_NO_OBSTACLE - derrorv * incv;
  ptr       = this->map.map + y1 * TS_MAP_SIZE + x1;
  pixval    = TS_NO_OBSTACLE;

  // create map update
  for (x = 0; x <= dxc ;x++, ptr += incptrx) {

    if (x > dx - 2 * derrorv) {

      if (x <= dx - derrorv) {
        pixval += incv;
        errorv += incerrorv;

        if (errorv > derrorv) {
          pixval += sincv;
          errorv -= derrorv;
        }

      } else {
        pixval -= incv;
        errorv -= incerrorv;

        if (errorv < 0) {
          pixval -= sincv;
          errorv += derrorv;
        }

      }  // x < (dx - derrorv)

    } // x > (dx - 2 * derrorv)

  } // for x

  // update map data
  *ptr = ((256 - alpha) * (*ptr) + alpha * pixval) >> 8;
  if (error > 0) {
    ptr += incptry;
    error += diag;
  } else {
    error += horiz;
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

//  c = cos(pos->theta * M_PI / 180.0);
//  s = sin(pos->theta * M_PI / 180.0);
  c = cos(pos->theta);
  s = sin(pos->theta);

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
