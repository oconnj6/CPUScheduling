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
    //CONSTRUCTORS
    Process();
    Process(std::string inName);

    Process operator=(Process p);

    //PRINT
    void printName();

    bool isDone();

    //SETTERS
    void setIAT(double inIAT);
    void setBurstNum(int num);
    void setBurstTimes(std::vector<std::pair<double,double> >* times);
    void setWaitDone(int num);
    void setCPUDone(int num);

    //GETTERS
    double getIAT() const;
    int getBurstNum();
    double getCPUTime();
    double getCPUTimeNoSped(int index) const;
    double getIOTime();
    int getRemBursts();
    int getCount();
    int getWaitDone();
    int getCPUDone();
    std::string getName();

  private:
    std::vector<std::pair<double,double> > burstTimes;
    std::string name;
    int waitDone = 0;
    int CPUDone = 0;
    int burstNum;
    bool done = false;
    int count = 0;
    double initialArrivalT;
};

#endif
