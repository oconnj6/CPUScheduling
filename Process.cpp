#include "Process.h"
#include <numeric>
//CONSTRUCTORS
Process::Process() {
  name = "A";
}

Process::Process(std::string inName) {
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
  return std::accumulate(burstAvg.begin(), burstAvg.end(), 0.0) / burstAvg.size();
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
