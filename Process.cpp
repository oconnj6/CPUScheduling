#include "Process.h"

//CONSTRUCTORS
Process::Process() {
  name = "A";
}

Process::Process(std::string inName) {
  name = inName;
}

Process Process::operator=(Process p) {
  std::string name = p.getName();
  Process newP = Process(name);
  newP.burstNum = p.getBurstNum();
  newP.initialArrivalT = p.getIAT();
  newP.setBurstTimes(&p.burstTimes);

  return newP;
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

void Process::setBurstTimes(std::vector<std::pair<double,double> >* times) {
  burstTimes = *times;
}

//GETTERS
double Process::getIAT() {
  return initialArrivalT;
}

int Process::getBurstNum() {
  return burstNum;
}

std::string Process::getName() {
  return name;
}

double Process::getCPUTime() {
  double temp = burstTimes[count].first;
  if (count == burstNum-1)
    done = true;
  count++;
  return temp;
}

double Process::getIOTime() {
  double temp = burstTimes[count-1].second;
  //count++;
  return temp;
}

int Process::getRemBursts() {
  return burstNum - count;
}

int Process::getCount() {
  return count;
}
