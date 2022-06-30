#ifndef IOScheduler_H
#define IOScheduler_H

/*********************/
// IOcheduler
/*********************/
class IOScheduler {
  public:
    virtual void AddProcess(int process) = 0;
    virtual int GetNextProcess() = 0;
};

#endif  // IOScheduler_H