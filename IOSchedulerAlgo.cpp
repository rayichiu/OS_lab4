#include "IOSchedulerAlgo.h"

using namespace std;

/***********/
// FIFO
/***********/
void FIFOIOScheduler::AddProcess(int process){
  active_queue_.push_back(process);
}

int FIFOIOScheduler::GetNextProcess(){
  if (active_queue_.size() > 0){
    int process = active_queue_.front();
    active_queue_.pop_front();
    return process;
  } else{
    return -1;
  }
}

/***********/
// SSTF
/***********/
void SSTFIOScheduler::AddProcess(int process){
  active_queue_.push_back(process);
}

int SSTFIOScheduler::GetNextProcess(){
  if (active_queue_.size() > 0){
    int process = active_queue_.front();
    int min_shift = INT_MAX;
    deque<int>::iterator process_exit;
    for (deque<int>::iterator it = active_queue_.begin(); it != active_queue_.end(); it++){         
      int curr_shift = abs(disk_head_ - io_request_vector_[*it].disk_track);
      if (curr_shift < min_shift){
        min_shift = curr_shift;
        process = *it;
        process_exit = it;
      }
    }
    active_queue_.erase(process_exit);
    return process;
  } else{
    return -1;
  }
}

/***********/
// LOOK (S)
/***********/
void LOOKIOScheduler::AddProcess(int process){
  active_queue_.push_back(process);
}

int LOOKIOScheduler::GetNextProcess(){
  // empty active queue
  if (active_queue_.size() == 0){
    return -1;
  }

  // active queue non empty
  int process = -1;
  int min_shift = INT_MAX;
  deque<int>::iterator process_exit;
  for (deque<int>::iterator it = active_queue_.begin(); it != active_queue_.end(); it++){         
    int curr_shift = (io_request_vector_[*it].disk_track - disk_head_) * direction_;
    if (curr_shift >= 0 && curr_shift < min_shift){
      min_shift = curr_shift;
      process = *it;
      process_exit = it;
    }
  }
  // change direction if all the track are not at the moving direction
  if (process == -1) {
    direction_ = -direction_;
    return LOOKIOScheduler::GetNextProcess();
  }
  active_queue_.erase(process_exit);
  return process;
}

/***********/
// CLOOK (c)
/***********/
void CLOOKIOScheduler::AddProcess(int process){
  active_queue_.push_back(process);
}

int CLOOKIOScheduler::GetNextProcess(){
  // empty active queue
  if (active_queue_.size() == 0){
    return -1;
  }

  // active queue non empty
  int process = -1;
  int min_shift = INT_MAX;     
  deque<int>::iterator process_exit;
  // for changing to the front
  int min_posi_process = -1;
  int min_position = INT_MAX;
  deque<int>::iterator min_posi_process_exit;
  for (deque<int>::iterator it = active_queue_.begin(); it != active_queue_.end(); it++){         
    int curr_shift = io_request_vector_[*it].disk_track - disk_head_;
    if (curr_shift >= 0 && curr_shift < min_shift){
      min_shift = curr_shift;
      process = *it;
      process_exit = it;
    }
    if (io_request_vector_[*it].disk_track < min_position){
      min_position = io_request_vector_[*it].disk_track;
      min_posi_process = *it;
      min_posi_process_exit = it;
    }
  }
  
  if (process == -1) {
    active_queue_.erase(min_posi_process_exit);
    return min_posi_process;
  }
  active_queue_.erase(process_exit);
  return process;
}

/***********/
// FLOOK (f)
/***********/

void FLOOKIOScheduler::AddProcess(int process){
  add_queue_.push_back(process);
}

int FLOOKIOScheduler::GetNextProcess(){
  // empty active queue && add queue
  if (active_queue_.size() == 0 && add_queue_.size() == 0){
    return -1;
  }
  // switch active queue <-> add queue
  if (active_queue_.size() == 0){
    swap(active_queue_, add_queue_);
  }

  // active queue non empty
  int process = -1;
  int min_shift = INT_MAX;     
  deque<int>::iterator process_exit;
  for (deque<int>::iterator it = active_queue_.begin(); it != active_queue_.end(); it++){         
    int curr_shift = (io_request_vector_[*it].disk_track - disk_head_) * direction_;
    if (curr_shift >= 0 && curr_shift < min_shift){
      min_shift = curr_shift;
      process = *it;
      process_exit = it;
    }
  }
  // change direction if all the track are not at the moving direction
  if (process == -1) {
    direction_ = -direction_;
    return FLOOKIOScheduler::GetNextProcess();
  }
  active_queue_.erase(process_exit);
  return process;
}



