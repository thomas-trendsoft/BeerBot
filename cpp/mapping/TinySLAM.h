#ifndef TINYSLAM_H_
#define TINYSLAM_H_

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define TS_SCAN_SIZE 8192
#define TS_MAP_SIZE  2048
#define TS_MAP_SCALE 0.1
#define TS_DISTANCE_NO_DETECTION 4000
#define TS_NO_OBSTACLE 65500
#define TS_OBSTACLE 0
#define TS_HOLE_WIDTH 600

// grid cell point
typedef unsigned short ts_map_pixel_t;

// global map
typedef struct {
  ts_map_pixel_t map[TS_MAP_SIZE * TS_MAP_SIZE];
} ts_map_t;

// scan data
typedef struct {
  double x[TS_SCAN_SIZE], y[TS_SCAN_SIZE];
  int value[TS_SCAN_SIZE];
  int nb_points;
} ts_scan_t;

typedef struct {
  double x,y;
  double theta;
} ts_position_t;

//
// try to reimplement tinyslam algorithm
// 
class TinySLAM {

  t_map_t map;

public:
  TinySLAM();

  init_map();
};

#endif
