#ifndef IOSchedulerAlgo_H
#define IOSchedulerAlgo_H

#include <deque>
#include <vector>
#include <iostream>
#include <climits>

#include "IORequest.h"
#include "IOScheduler.h"

using namespace std;

/***********/
// FIFO
/***********/
class FIFOIOScheduler: public IOScheduler{
  public:
    FIFOIOScheduler(deque<int>& active_queue)
        : active_queue_(active_queue){};         
    void AddProcess(int process);
    int GetNextProcess();
  
  private:
    deque<int>& active_queue_;
};

/***********/
// SSTF
/***********/
class SSTFIOScheduler: public IOScheduler{
  public:
    SSTFIOScheduler(deque<int>& active_queue, 
                    vector<IORequest>& io_request_vector, 
                    int& disk_head)
        : disk_head_(disk_head),
          io_request_vector_(io_request_vector),
          active_queue_(active_queue){};
    void AddProcess(int process);
    int GetNextProcess();
    
  private:
    deque<int>& active_queue_;
    vector<IORequest>& io_request_vector_;
    int& disk_head_;
};

/***********/
// LOOK (S)
/***********/
class LOOKIOScheduler: public IOScheduler{
  public:
    LOOKIOScheduler(deque<int>& active_queue, 
                    vector<IORequest>& io_request_vector, 
                    int& disk_head, int& direction)
        : disk_head_(disk_head),
          io_request_vector_(io_request_vector),
          active_queue_(active_queue),
          direction_(direction){};
    void AddProcess(int process);
    int GetNextProcess();
  
  private:
    deque<int>& active_queue_;
    vector<IORequest>& io_request_vector_;
    int& disk_head_;
    int& direction_; 
};

/***********/
// CLOOK (c)
/***********/
class CLOOKIOScheduler: public IOScheduler{
  public:
    CLOOKIOScheduler(deque<int>& active_queue, 
                    vector<IORequest>& io_request_vector, 
                    int& disk_head)
        : disk_head_(disk_head),
          io_request_vector_(io_request_vector),
          active_queue_(active_queue){};        
    void AddProcess(int process);
    int GetNextProcess();

  private:
    deque<int>& active_queue_;
    vector<IORequest>& io_request_vector_;
    int& disk_head_;
};

/***********/
// FLOOK (f)
/***********/
class FLOOKIOScheduler: public IOScheduler{
  public:
    FLOOKIOScheduler(deque<int>& active_queue, 
                    deque<int>& add_queue, 
                    vector<IORequest>& io_request_vector, 
                    int& disk_head, int& direction)
        : disk_head_(disk_head),
          io_request_vector_(io_request_vector),
          active_queue_(active_queue),
          add_queue_(add_queue),
          direction_(direction){};
    void AddProcess(int process);
    int GetNextProcess();
  
  private:
    deque<int>& active_queue_;
    deque<int>& add_queue_;
    vector<IORequest>& io_request_vector_;
    int& disk_head_;
    int& direction_; 
};


#endif  // IOSchedulerAlgo_H