#include "Process.h"
#include <numeric>
//CONSTRUCTORS
Process::Process() {
  inWaitQueue = false;
  printedIsDone = false;
  hasArrived = false;
  tau = 0;
  timeRem = 0;
  waitDone = 0;
  CPUDone = 0;
  burstNum = 0;
  done = false;
  count = 0;
  name = "A";
}

Process::Process(std::string inName) {
  inWaitQueue = false;
  printedIsDone = false;
  hasArrived = false;
  tau = 0;
  timeRem = 0;
  waitDone = 0;
  CPUDone = 0;
  burstNum = 0;
  done = false;
  count = 0;
  name = inName;
}

void Process::removeProcess() {
  for (int i = 0;i<burstNum;i++) {
    delete (*burstTimes)[i];
  }
  delete burstTimes;
}

//PRINT
void Process::printName() {
  std::cout << name << std::endl;
}

bool Process::isDone() {
  return done;
}

//SETTERS
void Process::setIAT(double inIAT) {
  initialArrivalT = inIAT;
}

void Process::setBurstNum(int num) {
  burstNum = num;
}

void Process::setBurstTimes(std::vector<std::pair<double,double>* >* times) {
  burstTimes = times;
}

//GETTERS

int Process::getRunningBurstAvg() {
  if (burstAvg.size() == 1 || burstAvg.size() == 0) {
    tau = burstAvg[0];
    return tau;
  }
  else {
    tau = ceil((tau + burstAvg[burstAvg.size() - 1]) / 2);
    return tau;
  }
}

int Process::getTau() {
  return tau;
}

void Process::addToBurstAvg(double burstTime) {
  burstAvg.push_back(burstTime);
}

void Process::setTimeRem(int num) {
  timeRem = num;
}

//GETTERS
double Process::getIAT() const {
  return initialArrivalT;
}

int Process::getBurstNum() {
  return burstNum;
}

std::string Process::getName() {
  return name;
}

void Process::setWaitDone(int num) {
  waitDone = num;
}

void Process::setCPUDone(int num) {
  CPUDone = num;
}

double Process::getCPUTime() {
  double temp = (*burstTimes)[count]->first;
  if (count == burstNum-1)
    done = true;
  count++;
  return temp;
}

double Process::getCPUTimeNoSped() const {
  return (*burstTimes)[count]->first;
}

void Process::decCount() {
  count--;
}

void Process::incCount() {
  count++;
}
double Process::getIOTime() {
  double temp = (*burstTimes)[count-1]->second;
  //count++;
  return temp;
}

int Process::getRemBursts() {
  return burstNum - count;
}

int Process::getCount() {
  return count;
}

int Process::getWaitDone() {
  return waitDone;
}

int Process::getCPUDone() {
  return CPUDone;
}

int Process::getTimeRem() {
  return timeRem;
}
