#ifndef IORequest_H
#define IORequest_H

/***********************************/
// IO Request
/***********************************/
struct IORequest{
  int arrival_time;
  int disk_track;
  int disk_start_time;
  int disk_end_time;
};

#endif  // IORequest_H