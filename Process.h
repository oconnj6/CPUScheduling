#ifndef PROCESS_H
#define PROCESS_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <utility>
#include <vector>
#include <math.h>

//Process class to hold all the necessary process information.
class Process
{
  public:
    bool inWaitQueue;
    bool printedIsDone;
    bool hasArrived;
    //CONSTRUCTORS
    Process();
    Process(std::string inName);

    //PRINT
    void printName();

    bool isDone();

    void removeProcess();


    //SETTERS
    void decCount();
    void incCount();
    void setIAT(double inIAT);
    void setBurstNum(int num);
    void setBurstTimes(std::vector<std::pair<double,double> >* times);
    void addToBurstAvg(double burstTime);
    void setBurstTimes(std::vector<std::pair<double,double>* >* times);
    void setWaitDone(int num);
    void setCPUDone(int num);
    void setTimeRem(int num);

    //GETTERS
    double getIAT() const;
    int getTau();
    int getRunningBurstAvg();
    int getBurstNum();
    double getCPUTime();
    double getCPUTimeNoSped() const;
    double getIOTime();
    int getRemBursts();
    int getCount();

    std::vector<double> burstAvg;
    int getWaitDone();
    int getCPUDone();
    int getTimeRem();
    std::string getName();

  private:
    std::vector<std::pair<double,double>* >* burstTimes;
    std::string name;
    int tau;
    int timeRem;
    int waitDone;
    int CPUDone;
    int burstNum;
    bool done;
    int count;
    double initialArrivalT;
};

#endif
