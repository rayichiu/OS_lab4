#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <deque>
#include <limits.h>

#include "IORequest.h"
#include "IOScheduler.h"
#include "IOSchedulerAlgo.h"

using namespace std;
/***********************************/
// Global Variables
/***********************************/
// input data
string algo;

// simulation
int current_time = 0;
int count_remaining_process = 0;
int io_idx_on_req_vector = 0;
int disk_head = 0;
int count_disk_move = 0;
int direction = 1;

// IO Request vector
vector<IORequest> io_request_vector;
// store the row number of io_request_vector
deque<int> active_queue;
// only for FLOOK
deque<int> add_queue;
// the ioprocess index of io_request_vector
int curr_io_request = -1;
int next_io_request;

// statistics
double total_turnaround = 0; 
double total_waittime = 0;
int max_waittime = 0;

/**************************************/
// process cmd
/**************************************/
void ProcessCmd(int argc, char* argv[]){
  int o;
  while ((o = getopt(argc, argv, "s:")) != -1){
    switch (o) {
      case 's':
        algo = optarg;
        break;
      case '?':
        printf("error optopt: %c\n", optopt);
        printf("error opterr: %d\n", opterr);
        break;
      default:
        exit(EXIT_FAILURE);
        break;
    }
  }
}

int main (int argc, char** argv){
  setbuf(stdout, NULL);
  string filename = argv[argc - 1];
  ProcessCmd(argc, argv);  

  fstream file;
  file.open(filename, ios::in);
  string tmp_line;

  // create io_request vector
  while (getline(file, tmp_line)){
    if (tmp_line[0] == '#') continue;

    istringstream iss(tmp_line);
    int time, track;
    iss >> time >> track;
    IORequest iorequest;
    iorequest.arrival_time = time;
    iorequest.disk_track = track;
    count_remaining_process++;
    io_request_vector.push_back(iorequest);
  }
  file.close();


  /*********************/
  // construct scheduler
  /*********************/
  IOScheduler* The_IO_Scheduler;
  switch (algo[0]){
    case 'i':
      The_IO_Scheduler = new FIFOIOScheduler(active_queue);
      break;
    case 'j':
      The_IO_Scheduler = new SSTFIOScheduler(active_queue, io_request_vector, disk_head);   
      break;
    case 's':
      The_IO_Scheduler = new LOOKIOScheduler(active_queue, io_request_vector, disk_head, direction);   
      break;
    case 'c':
      The_IO_Scheduler = new CLOOKIOScheduler(active_queue, io_request_vector, disk_head);   
      break;
    case 'f':
      The_IO_Scheduler = new FLOOKIOScheduler(active_queue, add_queue, io_request_vector, disk_head, direction);   
      break;
    default:
      break;
  }

  /*********************/
  // simulation
  /*********************/
  while (true){
    // if a new I/O arrived to the system at this current time
    // -> add request to IO-queue
    if(current_time == io_request_vector[io_idx_on_req_vector].arrival_time){     
      The_IO_Scheduler->AddProcess(io_idx_on_req_vector);
      io_idx_on_req_vector++;
    }

    // if an IO is active and completed at this time
    // -> Compute relevant info and store in IO request for final summary 
    IORequest* io = &io_request_vector[curr_io_request];
    if (curr_io_request != -1 && io->disk_track == disk_head){ 
      io->disk_end_time = current_time;

      // update statistics only when io process exit
      total_turnaround += io->disk_end_time - io->arrival_time;
      int waittime = io->disk_start_time - io->arrival_time;
      total_waittime += waittime;
      if (waittime > max_waittime){
        max_waittime = waittime;
      }

      curr_io_request = -1;
      count_remaining_process--;     
    }
    
    // if no IO request active now
    if (curr_io_request == -1){ 
      next_io_request = The_IO_Scheduler->GetNextProcess();
      
      //  if all IO from input file processed 
      //  -> exit simulation
      if (count_remaining_process == 0){
        break;
      }
      // if requests are pending
      // -> Fetch the next request from IO-queue and start the new IO.
      if (next_io_request != -1){
        curr_io_request = next_io_request;
        io_request_vector[curr_io_request].disk_start_time = current_time;
      }      
    }

    // if an IO is active
    // -> Move the head by one unit in the direction its going (to simulate seek)
    if (curr_io_request != -1){
      int target_track = io_request_vector[curr_io_request].disk_track;
      if (target_track == disk_head){
        // no need to increase current time -> no disk move 
        current_time--;
      } else {
        disk_head += ( target_track > disk_head ) ? 1 : -1;
        count_disk_move++;
      }
    }
    current_time++;
  }

  /**********************************/
  // print statistics
  /***********************************/
  for (int i = 0; i < io_request_vector.size(); i++){
    IORequest* io = &io_request_vector[i];
    printf("%5d: %5d %5d %5d\n", i,
        io->arrival_time, io->disk_start_time, io->disk_end_time);
  }  

  double avg_turnaround = total_turnaround / io_request_vector.size();
  double avg_waittime = total_waittime / io_request_vector.size();
  printf("SUM: %d %d %.2lf %.2lf %d\n", current_time, count_disk_move,
      avg_turnaround, avg_waittime, max_waittime);
}
