#ifndef PROCESS_H
#define PROCESS_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <utility>
#include <vector>

//Process class to hold all the necessary process information.
class Process
{
  public:
    bool inWaitQueue = false;
    bool printedIsDone = false;
    bool hasArrived = false;
    //CONSTRUCTORS
    Process();
    Process(std::string inName);

    //PRINT
    void printName();

    bool isDone();

    void removeProcess();


    //SETTERS
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
    int timeRem = 0;
    int waitDone = 0;
    int CPUDone = 0;
    int burstNum = 0;
    bool done = false;
    int count = 0;
    double initialArrivalT;
};

#endif
