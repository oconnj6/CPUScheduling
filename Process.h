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

    //GETTERS
    double getIAT();
    int getBurstNum();
    double getCPUTime();
    double getIOTime();
    std::string getName();

  private:
    std::vector<std::pair<double,double> > burstTimes;
    std::string name;
    int burstNum;
    bool done = false;
    int count = 0;
    double initialArrivalT;
};

#endif
